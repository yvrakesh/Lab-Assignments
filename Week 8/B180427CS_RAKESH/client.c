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
#include<signal.h>
#define IP_ADDR "127.0.0.1" // local host
#define PORT_NO 2201
#define BUFF_LEN 4096
#define MAX_NAME_LEN 50
#define MAX_PWD_LEN 150
#define CMD_LEN 30
bool flag = 0;
char name[MAX_NAME_LEN],passwd[MAX_PWD_LEN];
int sockfd1 = 0, sockfd2 = 0;
static bool bool_flag = 1;

void str_trim_lf(char * a, int len){
	for(int i=0;i<len;i++){
		if(a[i] == '\n'){
			a[i] = '\0';
			break;
		}
	}
}

void send_mail(){
	char msg[BUFF_LEN];
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
		bzero(total,BUFF_LEN);
		return ;
	}
	
	token2 = strtok(NULL,"@");
	if(strcmp(token2,name)){
		printf("User mentioned in the From address is not the current user\n-> ");
		bzero(msg,BUFF_LEN);
		bzero(total,BUFF_LEN);
		return ;
	}

	token3 = strtok(NULL,"\n");
	if(strcmp(token3,"localhost")){
		printf("Domain name entered is not reachable (Enter 'localhost' as domain name)\n-> ");
		bzero(msg,BUFF_LEN);
		bzero(total,BUFF_LEN);
		return ;
	}
	
	token4 = strtok(NULL," ");
	if(strcmp(token4,"To:")){
		printf("Missing 'To: ' after From address\n-> ");
		bzero(msg,BUFF_LEN);
		bzero(total,BUFF_LEN);
		return ;
	}
	
	token5 = strtok(NULL,"@");
	token6 = strtok(NULL,"\n");
	if(strcmp(token6,"localhost")){
		printf("Domain name entered is not reachable (Enter 'localhost' as domain name)\n-> ");
		bzero(msg,BUFF_LEN);
		bzero(total,BUFF_LEN);
		return ;
	}
	
	token7 = strtok(NULL," ");
	if(strcmp(token7,"Subject:")){
		printf("Missing 'Subject: ' after To address\n-> ");
		bzero(msg,BUFF_LEN);
		bzero(total,BUFF_LEN);
		return ;
	}
	
	token8 = strtok(NULL,"\n");
	if(strlen(token8) == 0){
		printf("Subject cannot be empty\n-> ");
		bzero(msg,BUFF_LEN);
		bzero(total,BUFF_LEN);
		return ;
	}
	printf("Captured Mail\n");
	bzero(msg,BUFF_LEN);
	sprintf(msg,"Send Mail");
	send(sockfd1,msg,strlen(msg),MSG_DONTWAIT);
	send(sockfd1,total,strlen(total),0);
	bzero(msg,BUFF_LEN);
	while(1)
		if(read(sockfd1,msg,sizeof(msg)) > 0)
			break;
	printf("%s\n",msg);
	bzero(total,BUFF_LEN);
}

int manage_mail(){
	char buf[] = "Manage Mail";
	send(sockfd2,buf,strlen(buf),MSG_DONTWAIT);
	char msg[BUFF_LEN];
	for(int i=0;i<2;i++){
		bzero(msg,BUFF_LEN);
		recv(sockfd2,msg,sizeof(msg),0);
		printf("%s\n",msg);
		printf("Enter a specific mail address: ");
		bzero(msg,BUFF_LEN);
		fgets(msg,BUFF_LEN,stdin);
		str_trim_lf(msg,BUFF_LEN);
		printf("%s is the email address entered\n",msg);
		char *token8;
		strtok(msg,"@");
		token8 = strtok(NULL,"\n");
		printf("Yes\n");
		if(strcmp(token8,"localhost")){
			printf("INVALID MAIL-FORMAT");
			continue;
		}
		printf("Sending Email Address as %s\n",msg);
		send(sockfd2,msg,strlen(msg),0);
		bzero(msg,BUFF_LEN);
		while(1)
			if(recv(sockfd2,msg,sizeof(msg),MSG_DONTWAIT) > 0)
				break;
		printf("%s\n",msg);
		if(!strcmp(msg,"NO MAILS TO SHOW")){
			bzero(msg,BUFF_LEN);
			return 0;
		}
		if(strcmp(msg,"INVALID MAIL-ID")){
			bzero(msg,BUFF_LEN);
			return 1;
		}
	}
	char buf1[] = "Quit";
	write(sockfd2,buf1,strlen(buf1));
	return 0;
}

int smtp(){
	sockfd1 = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd1 == -1){
		printf("Error in creating the socket. Try again\n");
		return 0;
	}
	else if(sockfd1 <= 0){
		printf("Invalid memory creation for socket. Try again\n");
		return 0;
	}
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(25);
	serv_addr.sin_addr.s_addr = inet_addr(IP_ADDR);
	memset(serv_addr.sin_zero,0,sizeof(serv_addr.sin_zero));
	
	int con_status = connect(sockfd1,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
	if(con_status == -1){
		printf("Failed to connect with the server\n");
		return 0;
	}
	return 1;
}

int pop(){
	sockfd2 = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd2 == -1){
		printf("Error in creating the socket. Try again\n");
		return 0;
	}
	else if(sockfd2 <= 0){
		printf("Invalid memory creation for socket. Try again\n");
		return 0;
	}
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(110);
	serv_addr.sin_addr.s_addr = inet_addr(IP_ADDR);
	memset(serv_addr.sin_zero,0,sizeof(serv_addr.sin_zero));
	
	int con_status = connect(sockfd2,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
	if(con_status == -1){
		printf("Failed to connect with the server\n");
		return 0;
	}
	return 1;
}

int main(){
	if(smtp()==0)
		return 0;
	char keyword[CMD_LEN],msg[BUFF_LEN];
	while(1){
		printf("Enter your Username: ");
		fgets(name,MAX_NAME_LEN,stdin);
		str_trim_lf(name,MAX_NAME_LEN);
		write(sockfd1,name,strlen(name));
		if(!strcmp(name,"Quit")){
			bzero(name,MAX_NAME_LEN);
			bzero(msg,BUFF_LEN);
			printf("Thank you for using the Application. Bye\n");
			close(sockfd1);
			return 0;
		}
		bzero(msg,BUFF_LEN);
		int temp = read(sockfd1,msg,sizeof(msg));
		msg[temp] = '\0';
		printf("%s\n",msg);
		if(!strcmp(msg,"Username is Valid")){
			bzero(msg,BUFF_LEN);
			break;
		}
		bzero(name,MAX_NAME_LEN);
		bzero(msg,BUFF_LEN);
	}
	while(1){
		printf("Enter your Password: ");
		fgets(passwd,MAX_PWD_LEN,stdin);
		str_trim_lf(passwd,MAX_PWD_LEN);
		write(sockfd1,passwd,strlen(passwd));
		if(!strcmp(passwd,"Quit")){
			printf("Thank you for using the Application. Bye\n");
			bzero(msg,BUFF_LEN);
			bzero(passwd, MAX_PWD_LEN);
			close(sockfd1);
			return 0;
		}
		bzero(msg,BUFF_LEN);
		int temp = read(sockfd1,msg,sizeof(msg));
		msg[temp] = '\0';
		printf("%s\n",msg);
		if(!strcmp(msg,"Password is Valid")){
			bzero(msg,BUFF_LEN);
			break;
		}
		bzero(msg,BUFF_LEN);
		bzero(passwd, MAX_PWD_LEN);
	}
	char buf[] = "Quit";
	write(sockfd1,buf,strlen(buf));
	close(sockfd1);
	while(1){
		printf("Choose One among the following functions\n\t\tSend Mail\n\t\tManage Mail\n\t\tQuit\n");
		printf("Enter your required function: ");
		char str[20];
		fgets(str,20,stdin);
		str_trim_lf(str,20);
		if(!strcmp(str,"Quit")){
			printf("Thank you for using our application\n");
			close(sockfd1);
			close(sockfd2);
			return 0;
		}
		else if(!strcmp(str,"Send Mail")){
			if(smtp()){
				send(sockfd1,name,strlen(name),MSG_DONTWAIT);
				char msg[BUFF_LEN];
				while(1)
					if(recv(sockfd1,msg,sizeof(msg),MSG_DONTWAIT) > 0)
						break;
				printf("%s\n",msg);
				if(strcmp(msg,"Username is Valid")){
					char buf[] = "Quit";
					send(sockfd1,buf,strlen(buf),MSG_DONTWAIT);
					bzero(msg,BUFF_LEN);
					continue;
				}
				send(sockfd1,passwd,strlen(passwd),MSG_DONTWAIT);
				bzero(msg,BUFF_LEN);
				while(1)
					if(recv(sockfd1,msg,sizeof(msg),MSG_DONTWAIT) > 0)
						break;
				printf("%s\n",msg);
				if(strcmp(msg,"Password is Valid")){
					char buf[] = "Quit";
					send(sockfd2,buf,strlen(buf),MSG_DONTWAIT);
					bzero(msg,BUFF_LEN);
					continue;
				}
				send_mail();
				char buf[] = "Quit";
				write(sockfd1,buf,strlen(buf));
				bzero(msg,BUFF_LEN);
				close(sockfd1);
			}
		}
		else if(!strcmp(str,"Manage Mail")){
			if(pop()){
				send(sockfd2,name,strlen(name),MSG_DONTWAIT);
				char msg[BUFF_LEN];
				while(1)
					if(recv(sockfd2,msg,sizeof(msg),MSG_DONTWAIT) > 0)
						break;
				printf("%s\n",msg);
				if(strcmp(msg,"Username is Valid")){
					char buf[] = "Quit";
					send(sockfd2,buf,strlen(buf),MSG_DONTWAIT);
					bzero(msg,BUFF_LEN);
					continue;
				}
				send(sockfd2,passwd,strlen(passwd),MSG_DONTWAIT);
				bzero(msg,BUFF_LEN);
				while(1)
					if(recv(sockfd2,msg,sizeof(msg),MSG_DONTWAIT) > 0)
						break;
				printf("%s\n",msg);
				if(strcmp(msg,"Password is Valid")){
					char buf[] = "Quit";
					send(sockfd2,buf,strlen(buf),MSG_DONTWAIT);
					bzero(msg,BUFF_LEN);
					continue;
				}
				while(1){
					if(!manage_mail()){
						printf("Failed for two times. Hence terminating the connection\n");
						break;
					}
					char temp[20];
					fgets(temp,20,stdin);
					if(!strcmp(temp,"q\n"))
						break;	
				}
				char buf[] = "Quit";
				send(sockfd2,buf,strlen(buf),MSG_DONTWAIT);
				bzero(msg,BUFF_LEN);
				close(sockfd2);
			}
		}
		else
			printf("Please enter a correct function from the list shown\n");
	}
}

