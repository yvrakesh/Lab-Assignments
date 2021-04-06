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
void * handle_client(void *arg){
	bool leave_flag = 0;
	char buffer[BUFF_LEN], name[MAX_NAME_LEN], passwd[MAX_PWD_LEN];
	count++;
	client_t *client = (client_t *)arg;
	
	while(1){
		if( recv(client->sockfd,name,MAX_NAME_LEN,MSG_DONTWAIT) > 0){
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
			bool bool_flag = 1;
			while(fgets(buffer,sizeof(buffer),fp)>0){
				char *token1;
				token1 = strtok(buffer,",");
				if(!strcmp(token1,name)){
					bool_flag = 0;
					break;
				}
				bzero(buffer,BUFF_LEN);
			}
			fclose(fp);
			bzero(buffer,BUFF_LEN);
			if(bool_flag){
				char buf[] = "Username is Invalid";
				write(client->sockfd,buf,strlen(buf));
				bzero(name,MAX_NAME_LEN);
				
				continue;
			}	
			char buf[] = "Username is Valid";
			write(client->sockfd,buf,strlen(buf));
			strcpy(client->name,name);
			bzero(name,MAX_NAME_LEN);
			break;
		}
	}
	
	while(1){
		if( recv(client->sockfd,passwd,MAX_PWD_LEN,MSG_DONTWAIT) > 0){
			if(!strcmp(passwd,"Quit")){
				printf("%s[UID:%d] has left without providing password\n",client->name,client->uid);
				close(client->sockfd);
				rem_client(client->uid);
				free(client);
				count--;
				pthread_detach(pthread_self());
				return NULL;
			}
			strcpy(name,client->name);
			sprintf(buffer,"%s provided their password '%s'\n",name,passwd);
			printf("%s",buffer);
			bzero(buffer,BUFF_LEN);
			FILE * fp;
			fp = fopen("logincred.txt","r");
			bool bool_flag = 1;
			while(fgets(buffer,sizeof(buffer),fp)){
				char *token1,*token2;
				token1 = strtok(buffer,",");
				if(!strcmp(token1,name)){
					token2 = strtok(NULL,"\n");
					if(!strcmp(token2,passwd)){
						bool_flag = 0;
						break;
					}
				}
			}
			fclose(fp);
			bzero(name,MAX_NAME_LEN);
			bzero(passwd,MAX_PWD_LEN);
			bzero(buffer,BUFF_LEN);
			if(bool_flag){
				char buf[] = "Password is Invalid";
				write(client->sockfd,buf,strlen(buf));
				continue;
			}
			char buf[] = "Password is Valid";
			write(client->sockfd,buf,strlen(buf));
			sleep(0.1);
			strcpy(client->passwd,passwd);
			break;
		}
	}	
	while(1){
		char buffer[BUFF_LEN];
		bzero(buffer,BUFF_LEN);
		if(read(client->sockfd, buffer, sizeof(buffer)) == 0)
			continue;
		printf("%s\n",buffer);
		if(strcmp(buffer, "Quit")==0){
			sprintf(buffer,"%s[UID:%d] has left",client->name,client->uid);
			printf("%s\n",buffer);
			break;
		}
		else if(strcmp(buffer,"Manage Mail") == 0){
			bzero(buffer,BUFF_LEN);
			char file_name[MAX_NAME_LEN];
			strcpy(file_name,client->name);
			strcat(file_name,"/mymailbox.mail");
			FILE *fp;
			fp = fopen(file_name,"r");
			
			char total[BUFF_LEN],total1[BUFF_LEN],total2[BUFF_LEN],msg[BUFF_LEN];
			int i=0;
			bzero(total,BUFF_LEN);
			while(fgets(msg,BUFF_LEN,fp)){
				if(!strncmp(msg,"From: ",5)){
					char *token1,*token2;
					token1 = strtok(msg," ");
					token2 = strtok(NULL,"\n");
					strcat(total,token2);
					strcat(total," ");
				}
				else if(!strncmp(msg,"Subject: ",9)){
					char *token1,*token2;
					token1 = strtok(msg," ");
					token2 = strtok(NULL,"\n");
					strcat(total1,token2);
				}
				else if(!strncmp(msg,"Received: ",10)){
					char *token1,*token2;
					token1 = strtok(msg," ");
					token2 = strtok(NULL,"\n");
					strcat(total,token2);
					strcat(total," ");
					strcat(total,total1);
					strcat(total,"\n");
					bzero(total1,BUFF_LEN);
				}
			}
			fclose(fp);
			bzero(msg,BUFF_LEN);
			char mail_id[2*MAX_NAME_LEN];
			bool mail_flag = 0;
			for(int i=0;i<2;i++){
				write(client->sockfd,total,strlen(total));
				while(1){
					if(read(client->sockfd,mail_id,sizeof(mail_id)) > 0){
						printf("%s@localhost is the requested sender mail id\n",mail_id);
						FILE * fp;
						fp = fopen("logincred.txt","r");
						bool bool_flag = 1;
						bzero(msg,BUFF_LEN);
						while(fgets(msg,sizeof(msg),fp)>0){
							char *token1;
							token1 = strtok(msg,",");
							if(!strcmp(token1,mail_id)){
								bool_flag = 0;
								break;
							}
						}
						fclose(fp);
						if(bool_flag){
							char buf[] = "INVALID MAIL-ID";
							write(client->sockfd,buf,strlen(buf));
							bzero(name,MAX_NAME_LEN);
							bzero(buf,strlen(buf));
							bzero(mail_id,2*MAX_NAME_LEN);
							break;
						}
						fp = fopen(file_name,"r");
						char total[BUFF_LEN];
						bzero(total,BUFF_LEN);
						bool flag1 = 0;
						printf("Started extracting files\n");
						while(fgets(msg,BUFF_LEN,fp)){
							if(!strncmp(msg,"From: ",5)){
								printf("%s\n",msg);
								char msg1[BUFF_LEN];
								strcpy(msg1,msg);
								char *token1,*token2;
								token1 = strtok(msg," ");
								token2 = strtok(NULL,"@");
								if(strcmp(token2,mail_id))
									continue;
								flag1 = 1;
								strcat(total,msg1);
								while(fgets(msg,BUFF_LEN,fp)){
									strcat(total,msg);
									if(!strcmp(msg,".\n"))
										break;
								}
							}
						}
						if(flag1)
							write(client->sockfd,total,strlen(total));
						else{
							char buf[] = "NO MAILS TO SHOW";
							write(client->sockfd,buf,strlen(buf));
						}
						printf("Sent to client\n");
						bzero(mail_id,2*MAX_NAME_LEN);
						mail_flag = 1;
						break;
					}
				}
				if(mail_flag)
					break;
			}
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
	int port_num = atoi(argv[1]), sockfd = socket(AF_INET,SOCK_STREAM,0);	
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
	printf("--- Pop server Opened ---\n");
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
