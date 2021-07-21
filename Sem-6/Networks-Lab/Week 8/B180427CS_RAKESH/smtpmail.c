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
#include<time.h>
#include<ctype.h>

#define IP_ADDR "127.0.0.1" // local host
#define PORT_NO 2201
#define MAX_CLIENTS 256
#define MAX_NAME_LEN 50
#define BUFF_LEN 4096
#define MAX_PWD_LEN 150

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

void send_msg_to(char *s,char *s1){
	pthread_mutex_lock(&clients_mutex);
	for(int i=0;i<MAX_CLIENTS;i++){
		if(clients[i]){
			if(!strcmp(clients[i]->name,s)){
				char buf[BUFF_LEN];
				bzero(buf,BUFF_LEN);
				sprintf(buf,"Hey %s, You got a new mail from %s",s,s1);
				if(write(clients[i]->sockfd,buf,strlen(buf)) == -1){
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
	int n = sizeof(str) - 1, leave_flag = 0;
	char buffer[BUFF_LEN], name[MAX_NAME_LEN], passwd[MAX_PWD_LEN];
	
	count++;
	client_t *client = (client_t *)arg;
	
	while(1){
		if( read(client->sockfd,name,MAX_NAME_LEN) > 0){
			bool bool_flag = 0;
			if(!strcmp(name,"Quit")){
				printf("[UID:%d] has left without providing username\n",client->uid);
				close(client->sockfd);
				rem_client(client->uid);
				free(client);
				count--;
				pthread_detach(pthread_self());
				return NULL;
			}
			sprintf(buffer,"Authentication requested by %s\n",name);
			printf("%s",buffer);
			bzero(buffer,BUFF_LEN);
			
			FILE * fp;
			fp = fopen("logincred.txt","r");
			int i = -1;
			char ch;
			while( (ch = fgetc(fp)) != EOF){
				i++;
				if(i >= strlen(name)){
					if(ch == ','){
						bool_flag = 1;
						break;
					}
				}
				if(name[i] == ch)
					continue;
				while((ch = fgetc(fp)) != '\n');
				i = -1;
			}
			fclose(fp);
			if(!bool_flag){
				char buf[] = "Username is Invalid";
				write(client->sockfd,buf,strlen(buf));
				bzero(name,MAX_NAME_LEN);
				bzero(buffer,BUFF_LEN);
				bzero(buf,strlen(buf));
				continue;
			}	
			
			char buf[] = "Username is Valid";
			write(client->sockfd,buf,strlen(buf));
			strcpy(client->name,name);
			bzero(name,MAX_NAME_LEN);
			bzero(buffer,BUFF_LEN);
			break;
		}
	}
	
	while(1){
		if( read(client->sockfd,passwd,MAX_PWD_LEN) > 0){
			if(!strcmp(passwd,"Quit")){
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
			sprintf(buffer,"%s provided their password '%s'\n",name,passwd);
			printf("%s",buffer);
			bzero(buffer,BUFF_LEN);
			
			FILE * fp;
			fp = fopen("logincred.txt","r");
			int i = -1;
			char ch;
			while( (ch = fgetc(fp)) != EOF){
				i++;
				if(i >= strlen(name)){
					if(ch == ','){
						int j = -1;
						while( (ch = fgetc(fp)) != EOF){
							j++;
							if(j >= strlen(passwd)){
								if(ch == '\n'){
									bool_flag = 1;
									break;
								}
							}
							if(passwd[j] == ch)
								continue;
							else
								break;
						}
						break;
					}
				}
				else if(name[i] == ch)
					continue;
				while(1){
					ch = fgetc(fp);
					if(ch == '\n' || ch == EOF)
						break;
				}
				i = -1;
			}
			fclose(fp);
			bzero(name,MAX_NAME_LEN);
			bzero(passwd,MAX_PWD_LEN);
			bzero(buffer,BUFF_LEN);
			if(!bool_flag){
				char buf[] = "Password is Invalid";
				write(client->sockfd,buf,strlen(buf));
				continue;
			}
			
			char buf[] = "Password is Valid";
			write(client->sockfd,buf,strlen(buf));
			strcpy(client->passwd,passwd);
			break;
		}
	}	
	while(1){
		char buffer[BUFF_LEN], string[10];
		bzero(buffer,BUFF_LEN);
		if(recv(client->sockfd, buffer, sizeof(buffer),MSG_DONTWAIT) < 2)
			continue;
		printf("%s\n",buffer);
		if(strcmp(buffer, "Quit")==0){
			sprintf(buffer,"%s[UID:%d] has left",client->name,client->uid);
			printf("%s\n",buffer);
			send_msg(buffer,client->uid);
			break;
		}
		else if(!strcmp(buffer,"Send Mail")){
			char mail[BUFF_LEN], mail1[BUFF_LEN], total[BUFF_LEN], msg[BUFF_LEN],name1[MAX_NAME_LEN];
			char *token1, *token2, *token3, *token4, *token5, *token6, *token7, *token8, *token9;
			int bytes = recv(client->sockfd, mail, sizeof(mail), 0);
			mail[bytes] = '\0';
			strcpy(mail1,mail);
			
			token1 = strtok(mail," ");
			strcat(total,token1);
			strcat(total," ");
			
			token2 = strtok(NULL,"@");
			strcat(total,token2);
			strcat(total,"@");
			
			token3 = strtok(NULL,"\n");
			strcat(total,token3);
			strcat(total,"\n");
			
			token4 = strtok(NULL," ");
			strcat(total,token4);
			strcat(total," ");
			
			token5 = strtok(NULL,"@");
			printf("Mail received from %s to %s\n",client->name,token5);
			bzero(name1,MAX_NAME_LEN);
			strcpy(name1,token5);
			printf("%s\n",mail1);
			
			bool bool_flag = 0;
			FILE * fp;
			fp = fopen("logincred.txt","r");
			int i = -1;
			char ch;
			while( (ch = fgetc(fp)) != EOF){
				i++;
				if(i >= strlen(token5)){
					if(ch == ','){
						bool_flag = 1;
						break;
					}
				}
				if(token5[i] == ch)
					continue;
				while((ch = fgetc(fp)) != '\n');
				i = -1;
			}
			fclose(fp);
			if(!bool_flag){
				char buf[] = "Wrong To User";
				write(client->sockfd,buf,strlen(buf));
				bzero(buffer,BUFF_LEN);
				bzero(total,BUFF_LEN);
				continue;
			}
			strcat(total,token5);
			strcat(total,"@");
			
			token6 = strtok(NULL,"\n");
			strcat(total,token6);
			strcat(total,"\n");
			
			token7 = strtok(NULL," ");
			strcat(total,token7);
			strcat(total," ");
			
			token8 = strtok(NULL,"\n");
			strcat(total,token8);
			strcat(total,"\n");
			
			time_t rawtime;
		        struct tm * cur_time;
  			time(&rawtime);
  			cur_time = localtime(&rawtime);
			sprintf(msg, "Received: %d/%d/%d : %d hours : %d minutes\n",cur_time->tm_mday, cur_time->tm_mon + 1, cur_time->tm_year + 1900, cur_time->tm_hour, cur_time->tm_min);
			strcat(total,msg);
			bzero(msg,BUFF_LEN);
			while(1){
				token9 = strtok(NULL,"\n");
				strcat(total,token9);
				strcat(total,"\n");
				if(!strcmp(token9,"."))
					break;
				bzero(token9,strlen(token9));
			}
			
			printf("%s\n",token5);
			strcat(token5, "/mymailbox.mail");
			printf("Appending to %s\n",token5);
			
			FILE *fp1;
			fp1 = fopen(token5,"a");
			fwrite(total,1,strlen(total),fp1);
			fclose(fp1);
			
			printf("Mail successfully updated\n");
			char buf[] = "Mail sent successfully";
			write(client->sockfd,buf,strlen(buf));
			send_msg_to(name1,client->name);
			bzero(buffer,BUFF_LEN);
			bzero(total,BUFF_LEN);
			
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




int main(int argc, char ** argv){

	int port_num = atoi(argv[1]);
	
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
	serv_addr.sin_port = htons(port_num);
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
