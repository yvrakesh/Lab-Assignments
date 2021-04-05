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
#define PORT_NO 2201
#define BUFF_LEN 4096
#define MAX_NAME_LEN 50
#define MAX_PWD_LEN 150
#define CMD_LEN 30

bool flag = 0;
char name[MAX_NAME_LEN];
int sockfd = 0;
static bool bool_flag = 1;

void str_trim_lf(char * a, int len){
	// Change last character in the string to null pointer to end the string
	for(int i=0;i<len;i++){
		if(a[i] == '\n'){
			a[i] = '\0';
			break;
		}
	}
}

void send_msg(){
	char msg[BUFF_LEN],buffer[BUFF_LEN];
	while(1){
		fflush(stdout);
		fgets(msg,BUFF_LEN,stdin);
		str_trim_lf(msg,BUFF_LEN);
		
		if(!strcmp(msg,"Quit")){
			write(sockfd,msg,strlen(msg));
			flag = 1;
			break;
		}
		else if(!strcmp(msg,"Send Mail")){
			bzero(buffer,BUFF_LEN);
			strcpy(buffer,msg);
			printf("Enter the mail to be sent\n\n");
			char total[BUFF_LEN];
			while(1){
				bzero(msg,BUFF_LEN);
				fgets(msg,BUFF_LEN,stdin);
				str_trim_lf(msg,BUFF_LEN);
				strcat(total,msg);
				strcat(total,"\n");
				if(!strcmp(msg,"."))
					break;
			}
			char mail[BUFF_LEN];
			strcpy(mail,total);
			
			char *token1, *token2,*token3,*token4,*token5,*token6,*token7,*token8;
			token1 = strtok(mail," ");
			if(strcmp(token1,"From:")){
				printf("Missing 'From: ' at the beginning of the Email\n-> ");
				bzero(msg,BUFF_LEN);
				bzero(buffer,BUFF_LEN);
				bzero(total,BUFF_LEN);
				continue;
			}
			
			token2 = strtok(NULL,"@");
			if(strcmp(token2,name)){
				printf("User mentioned in the From address is not the current user\n-> ");
				bzero(msg,BUFF_LEN);
				bzero(buffer,BUFF_LEN);
				bzero(total,BUFF_LEN);
				continue;
			}

			token3 = strtok(NULL,"\n");
			if(strcmp(token3,"localhost")){
				printf("Domain name entered is not reachable (Enter 'localhost' as domain name)\n-> ");
				bzero(msg,BUFF_LEN);
				bzero(buffer,BUFF_LEN);
				bzero(total,BUFF_LEN);
				continue;
			}
			
			token4 = strtok(NULL," ");
			if(strcmp(token4,"To:")){
				printf("Missing 'To: ' after From address\n-> ");
				bzero(msg,BUFF_LEN);
				bzero(buffer,BUFF_LEN);
				bzero(total,BUFF_LEN);
				continue;
			}
			
			token5 = strtok(NULL,"@");
			token6 = strtok(NULL,"\n");
			if(strcmp(token6,"localhost")){
				printf("Domain name entered is not reachable (Enter 'localhost' as domain name)\n-> ");
				bzero(msg,BUFF_LEN);
				bzero(buffer,BUFF_LEN);
				bzero(total,BUFF_LEN);
				continue;
			}
			
			token7 = strtok(NULL," ");
			if(strcmp(token7,"Subject:")){
				printf("Missing 'Subject: ' after To address\n-> ");
				bzero(msg,BUFF_LEN);
				bzero(buffer,BUFF_LEN);
				bzero(total,BUFF_LEN);
				continue;
			}
			
			token8 = strtok(NULL,"\n");
			if(strlen(token8) == 0){
				printf("Subject cannot be empty\n-> ");
				bzero(msg,BUFF_LEN);
				bzero(buffer,BUFF_LEN);
				bzero(total,BUFF_LEN);
				continue;
			}
			
			write(sockfd,buffer,strlen(buffer));
			sleep(0.5);
			write(sockfd,total,strlen(total));
			bzero(total,BUFF_LEN);
				
		}
		else
			write(sockfd,msg,strlen(msg));
		bzero(msg,BUFF_LEN);
		bzero(buffer,BUFF_LEN);
	}
}

void recv_msg(){
	char msg[BUFF_LEN];
	bzero(msg,BUFF_LEN);
	while(bool_flag==1 && !flag){
		bzero(msg,BUFF_LEN);
		if(read(sockfd, msg, sizeof(msg)) > 0){
			if(!strcmp(msg,"Wrong To User"))
				printf("Username mentioned in To address is not an existing user\n");
			else
				printf("%s\n", msg);
			fflush(stdout);
			printf("-> ");
		}
		bzero(msg,BUFF_LEN);
		fflush(stdout);
	}
}

int main(int argc, char ** argv){

	int port_num = atoi(argv[1]);
	
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
	serv_addr.sin_port = htons(port_num);
	serv_addr.sin_addr.s_addr = inet_addr(IP_ADDR);
	memset(serv_addr.sin_zero,0,sizeof(serv_addr.sin_zero));
	
	int con_status = connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
	if(con_status == -1){
		printf("Failed to connect with the server\n");
		return 0;
	}
	
	char keyword[CMD_LEN],buf,msg[BUFF_LEN];
	while(1){
		fflush(stdout);
		printf("Enter your Username: ");
		fgets(name,MAX_NAME_LEN,stdin);
		str_trim_lf(name,MAX_NAME_LEN);
		write(sockfd,name,strlen(name));
		if(!strcmp(name,"Quit")){
			bzero(name,MAX_NAME_LEN);
			bzero(msg,BUFF_LEN);
			printf("Thank you for using the Application. Bye\n");
			close(sockfd);
			return 0;
		}
		bzero(msg,BUFF_LEN);
		int temp = read(sockfd,msg,sizeof(msg));
		msg[temp] = '\0';
		printf("%s\n",msg);
		if(!strcmp(msg,"Username is Valid")){
			bzero(msg,BUFF_LEN);
			break;
		}
		bzero(name,MAX_NAME_LEN);
		bzero(msg,BUFF_LEN);
	}
	
	char passwd[MAX_PWD_LEN];
	while(1){
		fflush(stdout);
		printf("Enter your Password: ");
		fgets(passwd,MAX_PWD_LEN,stdin);
		str_trim_lf(passwd,MAX_PWD_LEN);
		write(sockfd,passwd,strlen(passwd));
		if(!strcmp(passwd,"Quit")){
			printf("Thank you for using the Application. Bye\n");
			bzero(msg,BUFF_LEN);
			bzero(passwd, MAX_PWD_LEN);
			close(sockfd);
			return 0;
		}
		bzero(msg,BUFF_LEN);
		//sleep(0.5);
		int temp = read(sockfd,msg,sizeof(msg));
		msg[temp] = '\0';
		printf("%s\n",msg);
		if(!strcmp(msg,"Password is Valid\n")){
			bzero(msg,BUFF_LEN);
			break;
		}
		bzero(msg,BUFF_LEN);
		bzero(passwd, MAX_PWD_LEN);
	}	
	
	int temp = recv(sockfd,msg,sizeof(msg),0);
	msg[temp] = '\0';
	printf("%s\n",msg);
	bzero(msg,BUFF_LEN);

	printf("---- Welcome to the Chatroom ----\n");
	printf("You may exit from the room anytime by entering 'Quit'\n");
	
	sleep(0.2);
	if(recv(sockfd,msg,sizeof(msg),0) > 0){
		printf("%s",msg);
		bzero(msg,BUFF_LEN);
	}
	fflush(stdout);
	bzero(msg,BUFF_LEN);
	
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

