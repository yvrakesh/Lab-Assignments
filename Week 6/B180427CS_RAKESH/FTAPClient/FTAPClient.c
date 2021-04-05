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

#define IP_ADDR "127.0.0.1" // local host
#define PORT_NO 2021
#define BUFF_LEN 4096
#define MAX_NAME_LEN 50
#define MAX_PWD_LEN 150
#define CMD_LEN 30
#define MAX_FILE_NAME_LEN 250
#define BUFF_LEN1 4000

char name[MAX_NAME_LEN];
int sockfd = 0;
bool flag = 0,glob_flag = 1;

void str_trim_lf(char * a, int len){
	for(int i=0;i<len;i++){
		if(a[i] == '\n'){
			a[i] = '\0';
			break;
		}
	}
}

void catch_ctrl_c_and_exit(int sig){
	flag = 1;
}

void send_msg(){
	char msg[BUFF_LEN1],buffer[BUFF_LEN];
	while(1){
		scanf("%s",msg);
		str_trim_lf(msg,BUFF_LEN1);
		sprintf(buffer,"%s\n",msg);
		bzero(msg,BUFF_LEN1);
		write(sockfd,buffer,strlen(buffer));
		
		if(!strcmp(buffer,"CreateFile\n")){
			bzero(buffer,BUFF_LEN);
			char ch;
			scanf("%c",&ch);
			fgets(msg,BUFF_LEN1,stdin);
			str_trim_lf(msg,BUFF_LEN1);
			sprintf(buffer,"%s\n",msg);
			sleep(0.5);
			write(sockfd,buffer,strlen(buffer));
		}
		else if(!strcmp(buffer,"GetFile\n")){
			bzero(buffer,BUFF_LEN);
			char ch;
			scanf("%c",&ch);
			fgets(msg,BUFF_LEN1,stdin);
			str_trim_lf(msg,BUFF_LEN1);
			sprintf(buffer,"%s\n",msg);
			printf("Requesting File Name is %s",buffer);
			sleep(0.5);
			write(sockfd,buffer,strlen(buffer));
		}
		else if(!strcmp(buffer,"StoreFile\n")){
			glob_flag = 0;
			bzero(buffer,BUFF_LEN);
			char ch;
			scanf("%c",&ch);
			fgets(msg,BUFF_LEN1,stdin);
			str_trim_lf(msg,BUFF_LEN1);
			sprintf(buffer,"%s",msg);
			printf("Uploading File Name is %s\n",buffer);
			FILE *fp;
			fp = fopen(buffer, "r");
			if(!fp){
				printf("Unable to Open the credentials file\n");
				continue;
			}
			write(sockfd,&buffer,strlen(buffer));
			for(int i=1;i<50000000;i++)
				continue;
			bzero(buffer,BUFF_LEN);
			char msg[500],buf[10];
			int i=0;
			int bytes;
			
			printf("Initiating File Transfer\n");
			while((bytes = fread(msg, 1, 500, fp)) >= 1){
				write(sockfd, &msg, bytes);
				read(sockfd,&buf,10);
				i+=500;
				if(i%5000000==0)
					printf("%d MB data sent\n", i/1000000);
			}

			sprintf(msg, "File Transfer finished\n");
			printf("%s-> ", msg);
			write(sockfd, msg, strlen(msg));
			
			bzero(msg,500);
			bzero(buf,10);
			glob_flag = 1;
		}
		if(!strcmp(buffer,"QUIT\n") || flag){
			bzero(msg,BUFF_LEN1);
			bzero(buffer,BUFF_LEN);
			break;
		}
		bzero(msg,BUFF_LEN1);
		bzero(buffer,BUFF_LEN);
	}
	flag = 1;
	catch_ctrl_c_and_exit(2);
}

void recv_msg(){
	char msg[BUFF_LEN1],buffer[MAX_FILE_NAME_LEN];
	while(!flag){
		bzero(msg,BUFF_LEN1);
		bzero(buffer,MAX_FILE_NAME_LEN);
		if(glob_flag){
			if(recv(sockfd,msg,sizeof(msg),MSG_DONTWAIT)> 0){
				if(!strcmp(msg,"Sending the file\n")){
					while(1)
						if(read(sockfd,buffer,sizeof(buffer))>0)
							break;
					printf("The file which is transferring is %s",buffer);
					str_trim_lf(buffer,MAX_FILE_NAME_LEN);
					FILE *fp;
					fp = fopen(buffer, "w");
					int bytes,i=0;
					char msg1[500],buf[10];
					while(1){
						bzero(msg1,500);
						bzero(buf,10);
						bytes = read(sockfd, msg1, sizeof(msg1));
						if(bytes == 0){
							bzero(msg1,500);
							continue;
						}
						if(!strcmp(msg1, "File Transfer finished\n")){
							bzero(msg1,500);
							break;
						}
						sprintf(buf,"Received\n");
						write(sockfd,&buf,strlen(buf));
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
				else
					printf("%s\n", msg);
				printf("-> ");
			}
		}
		fflush(stdout);
	}
}
int main(){

	sockfd = socket(AF_INET,SOCK_STREAM,0);
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
	
	int con_status = connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
	if(con_status == -1){
		printf("Failed to connect with the server\n");
		return 0;
	}
	signal(SIGINT, catch_ctrl_c_and_exit);
	
	char cmd_msg[CMD_LEN];
	while(1){
		printf("Enter 'START' to establish the connection\n-> ");
		fgets(cmd_msg,CMD_LEN,stdin);
		str_trim_lf(cmd_msg,CMD_LEN);
		if(!strcmp(cmd_msg,"START"))
			break;
		else if(!strcmp(cmd_msg,"QUIT")){
			write(sockfd,"QUIT",4);
			return 0;
		}
	}
	char keyword[CMD_LEN],buf,name[MAX_NAME_LEN],name1[MAX_NAME_LEN-1],msg[BUFF_LEN1];
	while(1){
		printf("Enter 'USERN' followed by username\n-> ");
		scanf("%s",keyword);
		scanf("%c",&buf);
		
		if(!strcmp(keyword,"USERN")){
			fgets(name1,MAX_NAME_LEN-1,stdin);
			str_trim_lf(name1,strlen(name));
			sprintf(name,"%s\n",name1);
			write(sockfd,name,strlen(name));
			read(sockfd,msg,3);
			printf("%s\n",msg);
			if(!strcmp(msg,"404")){
				printf("Some Unavoidable issue from server side. Please try after sometime\n");
				close(sockfd);
				return 0;
			}
			if(strcmp(msg,"301"))
				break;
		}
		else if(!strcmp(keyword,"QUIT")){
			write(sockfd,"QUIT",4);
			return 0;
		}
		else
			printf("505\n");
		bzero(name,MAX_NAME_LEN);
		bzero(keyword,CMD_LEN);
		bzero(msg,BUFF_LEN1);
	}
	char passwd[MAX_PWD_LEN],passwd1[MAX_PWD_LEN-1];
	while(1){
		printf("Enter 'PASSWD' followed by password\n-> ");
		scanf("%s",keyword);
		scanf("%c",&buf);
		if(!strcmp(keyword,"PASSWD")){
			fgets(passwd1,MAX_PWD_LEN-1,stdin);
			str_trim_lf(passwd1,strlen(passwd1));
			sprintf(passwd,"%s\n",passwd1);
			write(sockfd,passwd,strlen(passwd));
			while(1){
				if(read(sockfd,msg,3) == 3)
					break;
				bzero(msg,BUFF_LEN1);
			}
			printf("%s\n",msg);
			if(!strcmp(msg,"404")){
				printf("Some Unavoidable issue from server side. Please try after sometime\n");
				close(sockfd);
				return 0;
			}
			if(strcmp(msg,"310"))
				break;
		}
		else if(!strcmp(keyword,"QUIT")){
			write(sockfd,"QUIT",4);
			return 0;
		}
		else
			printf("505\n");
		bzero(passwd, MAX_PWD_LEN);
		bzero(keyword,CMD_LEN);
		bzero(msg,BUFF_LEN1);
	}
	
	read(sockfd,msg,7);
	printf("%s",msg);
	bzero(msg,BUFF_LEN1);

	printf("---- Welcome to the Chatroom ----\n");
	printf("You may exit from the room anytime by entering 'ctrl+c' or 'Bye'\n");
	
	while(1){
		if(read(sockfd,msg,sizeof(msg)) > 0){
			printf("%s",msg);
			bzero(msg,BUFF_LEN1);
		}
		break;
	}
	
	printf("-> ");
	
	pthread_t recv_msg_thread;
	int thr_recv_status = pthread_create(&recv_msg_thread,NULL,(void *)recv_msg,NULL);
	if(thr_recv_status != 0){
		printf("Received thread failed to establish\n");
		return 0;
	}
	
	pthread_t send_msg_thread;
	int thr_status = pthread_create(&send_msg_thread,NULL,(void *)send_msg,NULL);
	if(thr_status != 0){
		printf("Failed to establish the thread\n");
		return 0;
	}
	
	pthread_join(recv_msg_thread,0);
	pthread_join(send_msg_thread,0);
	
	
	
	while(1){
		if(flag){
			printf("Thank you for using the Application. Bye\n");
			break;
		}
		sleep(1);
	}
	
	close(sockfd);
	
	return 0;
}

