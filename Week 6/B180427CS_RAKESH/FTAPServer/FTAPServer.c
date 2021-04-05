// "MSG_DONTWAIT" Flag is the most important one
// For each of the recv and send function use it to avoid any issues in flow of program
// I have used it for one case but u can use it for all the recv and send functions

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<errno.h>
#include<pthread.h>
#include<signal.h>
#include<dirent.h>

#define IP_ADDR "127.0.0.1" // local host
#define PORT_NO 2021
#define MAX_CLIENTS 256
#define MAX_NAME_LEN 50
#define MAX_PWD_LEN 150
#define MAX_FILE_NAME_LEN 250
#define BUFF_LEN 4096
#define BUFF_LEN1 4000

int count = 0,uid = 10;
typedef struct{
	struct sockaddr_in addrs;
	int sockfd;
	int uid;
	char name[MAX_NAME_LEN];
	char passwd[MAX_PWD_LEN];
}client_t;

client_t *clients[MAX_CLIENTS];

pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void str_trim_lf(char * a, int len){
	for(int i=0;i<len;i++){
		if(a[i] == '\n'){
			a[i] = '\0';
			break;
		}
	}
}

void add_client(client_t * client){
	pthread_mutex_lock(&clients_mutex);
	for(int i=0;i<MAX_CLIENTS; i++){
		if(!clients[i]){
			clients[i] = client;
			break;
		}
	}
	pthread_mutex_unlock(&clients_mutex);
}

void rem_client(int uid){
	pthread_mutex_lock(&clients_mutex);
	for(int i=0;i<MAX_CLIENTS;i++){
		if(clients[i]){
			if(clients[i]->uid == uid){
				clients[i] = NULL;
				break;
			}
		}
	}
	pthread_mutex_unlock(&clients_mutex);
}

void send_msg(char *s,int uid){
	pthread_mutex_lock(&clients_mutex);
	for(int i=0;i<MAX_CLIENTS;i++){
		if(clients[i]){
			if(clients[i]->uid !=uid){
				if(write(clients[i]->sockfd,s,strlen(s)) == -1){
					printf("Failed to send to client %s\n",clients[i]->name);
					break;
				}
			}
		}
	}
	pthread_mutex_unlock(&clients_mutex);
}

void * handle_client(void *arg){
	char str[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890!@#$%^&*~?";
	char buffer[BUFF_LEN], name[MAX_NAME_LEN], passwd[MAX_PWD_LEN],msg[BUFF_LEN];
	count++;
	client_t *client = (client_t *)arg;
	while(1){
		if( read(client->sockfd,name,MAX_NAME_LEN) > 0){
			if(!strcmp(name,"QUIT")){
				printf("[UID:%d] left without providing username\n",client->uid);
				close(client->sockfd);
				rem_client(client->uid);
				free(client);
				count--;
				pthread_detach(pthread_self());
				return NULL;
			}
			bool bool_flag = 0;
			str_trim_lf(name,MAX_NAME_LEN);
			printf("Authentication requested by '%s'\n",name);
			FILE * fp;
			fp = fopen("logincred.txt","r");
			if(!fp){
				printf("Unable to Open the credentials file\n");
				bzero(buffer,BUFF_LEN);
				sprintf(buffer,"404");
				write(client->sockfd,buffer,strlen(buffer));
				close(client->sockfd);
				rem_client(client->uid);
				free(client);
				count--;
				pthread_detach(pthread_self());
				return NULL;
			}
			bzero(msg,BUFF_LEN);
			while(fgets(msg,sizeof(msg),fp)!=NULL){
				char *token1;
				token1 = strtok(msg,",");
				if(!strcmp(token1,name)){
					bool_flag = 1;
					break;
				}
				bzero(msg,BUFF_LEN);
			}
			fclose(fp);
			if(!bool_flag){
				char buf[] = "301";
				write(client->sockfd,buf,strlen(buf));
				bzero(name,MAX_NAME_LEN);
				bzero(buffer,BUFF_LEN);
				continue;
			}	
			
			char buf[] = "300";
			write(client->sockfd,buf,strlen(buf));
			strcpy(client->name,name);
			bzero(name,MAX_NAME_LEN);
			bzero(buffer,BUFF_LEN);
			break;
		}
	}
	
	while(1){
		if( read(client->sockfd,passwd,MAX_PWD_LEN) > 0){
			if(!strcmp(passwd,"QUIT")){
				printf("%s[UID:%d] has left without providing password\n",client->name,client->uid);
				close(client->sockfd);
				rem_client(client->uid);
				free(client);
				count--;
				pthread_detach(pthread_self());
				return NULL;
			}
			bool bool_flag = 0;
			strcpy(name,client->name);
			str_trim_lf(passwd,MAX_PWD_LEN);
			sprintf(buffer,"%s provided their password '%s'\n",name,passwd);
			printf("%s",buffer);
			bzero(buffer,BUFF_LEN);
			
			FILE * fp;
			fp = fopen("logincred.txt","r");
			if(!fp){
				printf("Unable to Open the credentials file\n");
				bzero(buffer,BUFF_LEN);
				sprintf(buffer,"404");
				write(client->sockfd,buffer,strlen(buffer));
				close(client->sockfd);
				rem_client(client->uid);
				free(client);
				count--;
				pthread_detach(pthread_self());
				return NULL;
			}
			bzero(msg,BUFF_LEN);
			while(fgets(msg,sizeof(msg),fp)!=NULL){
				char *token1,*token2;
				token1 = strtok(msg,",");
				token2 = strtok(NULL,"\n");
				if(!strcmp(token1,name)){
					printf("The password of the person is %s\n",token2);
					if(!strcmp(token2,passwd)){
						bool_flag = 1;
						break;
					}
				}
				bzero(msg,BUFF_LEN);
			}
			fclose(fp);
			bzero(name,MAX_NAME_LEN);
			bzero(passwd,MAX_PWD_LEN);
			bzero(buffer,BUFF_LEN);
			if(!bool_flag){
				char buf[] = "310";
				write(client->sockfd,&buf,strlen(buf));
				continue;
			}
			
			char buf[] = "305";
			write(client->sockfd, &buf, strlen(buf));
			strcpy(client->passwd,passwd);
			break;
		}
	}
	
	sprintf(buffer,"200 OK\n");
	write(client->sockfd,&buffer,strlen(buffer));
	bzero(buffer,BUFF_LEN);
	
	int flag = 1;
	for(int i=0;i<MAX_CLIENTS;i++){
		if(clients[i] && clients[i]->uid != client->uid){
			sprintf(buffer,"%s[UID:%d] is on the chat\n",clients[i]->name,clients[i]->uid);
			int write_status = write(client->sockfd,buffer,strlen(buffer));
			flag = 0;
			if(write_status == -1){
				printf("Unable to write to that particular socket\n");
				break;
			}
			bzero(buffer,BUFF_LEN);
		}
	}
	
	if(flag){
		sprintf(buffer,"No one are on the chatroom presently\n");
		int write_status = write(client->sockfd,buffer,strlen(buffer));
		if(write_status == -1)
			printf("Unable to write to that particular socket\n");
		bzero(buffer,BUFF_LEN);
	}
	
	sprintf(buffer,"Your unique ID is %d\n",client->uid);
	int write_status = write(client->sockfd,buffer,strlen(buffer));
	if(write_status == -1)
		printf("Unable to write to that particular socket\n");
	bzero(buffer,BUFF_LEN);
	
	sprintf(buffer,"%s[UID:%d] has joined this chatroom",client->name,client->uid);
	printf("%s\n",buffer);
	send_msg(buffer,client->uid);
		
	while(1){
		char buffer[BUFF_LEN];
		char string[10];
		int bytes = read(client->sockfd, buffer, sizeof(buffer));
		printf("%s",buffer);
		if(strcmp(buffer, "QUIT\n")==0){
			sprintf(buffer,"%s[UID:%d] has left",client->name,client->uid);
			printf("%s\n",buffer);
			bzero(buffer,BUFF_LEN);
			send_msg(buffer,client->uid);
			break;
		}
		else if(strcmp(buffer, "ListDir\n") == 0){
			DIR *d;
			struct dirent *dir;
			d = opendir(".");
			if(d){
				bzero(buffer,BUFF_LEN);
				while((dir = readdir(d)) != NULL){
					strcat(buffer,dir->d_name);
					strcat(buffer," ");
				}
				write(client->sockfd,buffer,strlen(buffer));
				closedir(d);
			}
		}
			
		else if(strcmp(buffer, "GetFile\n")==0){
			char msgbuf[BUFF_LEN];
			sprintf(msgbuf, "Sending the file\n");
			msgbuf[strlen(msgbuf)] = '\0';
			write(client->sockfd, msgbuf, strlen(msgbuf)+1);
			
			bzero(msgbuf,BUFF_LEN);
			bzero(buffer,BUFF_LEN);
			while(1){
				if(read(client->sockfd,buffer,sizeof(buffer))>0)
					break;
			}
			
			for(int i=1;i<50000000;i++)
				continue;
			write(client->sockfd,buffer,strlen(buffer));
			str_trim_lf(buffer,BUFF_LEN);
			printf("Opening %s\n",buffer);
			
			FILE *fp;
			fp = fopen(buffer, "r");
			if(!fp){
				printf("Unable to Open the credentials file\n");
				close(client->sockfd);
				rem_client(client->uid);
				free(client);
				count--;
				pthread_detach(pthread_self());
				return NULL;
			}
			char msg[500];
			char buf[10];
			int i=0;
			int bytes;
			printf("Initiating File Transfer\n");
			while((bytes = fread(msg, 1, 500, fp)) >= 1){
				write(client->sockfd, &msg, bytes);
				read(client->sockfd,&buf,10);
				i+=500;
				if(i%5000000==0)
					printf("%d MB data sent\n", i/1000000);
			}

			sprintf(msg, "File Transfer finished\n");
			write(client->sockfd, msg, strlen(msg));
			printf("%s", msg);
		}
		else if(strcmp(buffer,"StoreFile\n") == 0){
			char buffer[MAX_FILE_NAME_LEN];
			bzero(buffer,MAX_FILE_NAME_LEN);
			while(1)
				if(read(client->sockfd,buffer,sizeof(buffer))>0)
					break;
			str_trim_lf(buffer,MAX_FILE_NAME_LEN);
			printf("Uploading Filename is %s\n",buffer);
			FILE *fp;
			fp = fopen(buffer, "w");
			int bytes,i=0;
			char msg1[500],buf[10];
			while(1){
				bzero(msg1,500);
				bzero(buf,10);
				bytes = read(client->sockfd, msg1, sizeof(msg1));
				if(bytes == 0){
					bzero(msg1,500);
					continue;
				}
				if(!strcmp(msg1, "File Transfer finished\n")){
					bzero(msg1,500);
					break;
				}
				sprintf(buf,"Received\n");
				write(client->sockfd,&buf,strlen(buf));
				fwrite(msg1, 1, bytes, fp);
				i += bytes;
				if(i%5000000==0)
					printf("%d MB data received\n", i/1000000);
				bzero(msg1,500);
			}
			fclose(fp);
			printf("File '%s' Received\n",buffer);
			bzero(buffer,MAX_FILE_NAME_LEN);
			bzero(msg1, 500);
			bzero(msg,BUFF_LEN1);
		}
		else if(strcmp(buffer, "CreateFile\n") == 0){
			bzero(buffer,BUFF_LEN);
			char msg[BUFF_LEN1];
			bzero(msg,BUFF_LEN1);
			while(1){
				if(read(client->sockfd,msg,sizeof(msg))>0)
					break;
			}
			str_trim_lf(msg,BUFF_LEN1);
			FILE *fp;
			fp = fopen(msg,"w");
			printf("Created file named '%s'\n",msg);
			bzero(buffer,BUFF_LEN);
			sprintf(buffer,"Created the file '%s' successfully",msg);
			write(client->sockfd,buffer,strlen(buffer));
			fclose(fp);
		}
			
		else{
			for(int i=0;i<10;i++)
				string[i] = str[rand() % (strlen(str)-1)];
			write(client->sockfd, &string, sizeof(string));
		}
		bzero(buffer,BUFF_LEN);

	}
	close(client->sockfd);
	rem_client(client->uid);
	free(client);
	count--;
	pthread_detach(pthread_self());
	return NULL;
}

int main(){
	int sockfd = socket(AF_INET,SOCK_STREAM,0);	
	if(sockfd == -1){
		printf("Error in creating the socket. Try again\n");
		return 0;
	}
	else if(sockfd <= 0){
		printf("Invalid memory creation for socket. Try again\n");
		return 0;
	}
	
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT_NO);
	serv_addr.sin_addr.s_addr = inet_addr(IP_ADDR);
	memset(serv_addr.sin_zero,0,sizeof(serv_addr.sin_zero));
	
	int option = 1;
	signal(SIGPIPE, SIG_IGN);
	if(setsockopt(sockfd, SOL_SOCKET,(SO_REUSEPORT | SO_REUSEADDR),(char*)&option,sizeof(option)) < 0){
		perror("ERROR: setsockopt failed");
    		return EXIT_FAILURE;
	}
	
	int bind_status = bind(sockfd,(struct sockaddr*) &serv_addr, sizeof(serv_addr));
	if(bind_status == -1){
		printf("Error in binding the socket to the provided ip and port number\n");
		return 0;
	}
	int listen_status = listen(sockfd,10);
	if(listen_status == -1){
		printf("Error in listening for connections. i.e. listen setup failure\n");
		return 0;
	}
	
	printf("--- Chatroom Open ---\n");
	
	pthread_t tid;
	
	while(1){
		struct sockaddr_in client_addr;
		socklen_t client_len = sizeof(client_addr);
		int newfd = accept(sockfd,(struct sockaddr*)&client_addr,&client_len);
		if(newfd < 0){
			printf("Failed to create a new socket for accepted connection\n");
			continue;
		}
		
		if(count == MAX_CLIENTS -1 ){
			printf("Maximum Clients connected. Group size is full. Please try after sometime\n");
			close(newfd);
			continue;
		}
		
		client_t *client = (client_t *)malloc(sizeof(client_t));
		client->addrs = client_addr;
		client->sockfd = newfd;
		client->uid = uid++;
	
		add_client(client);
		
		pthread_create(&tid, NULL, &handle_client, (void *)client);
		
		sleep(1);		
	}
	return 0;
}
