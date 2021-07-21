#include<stdio.h>
#include<stdlib.h>


#include<string.h>

#include<sys/types.h>
#include<sys/socket.h>


#include<arpa/inet.h>
#include<netinet/in.h>


#include<unistd.h>


int main(){
	
	
	int sockfd = socket(PF_INET,SOCK_DGRAM,0);
	if(sockfd == -1){
		printf("Failed to create a socket\n");
		return 0;
	}
	else if(sockfd <= 0){
		printf("Invalid memory creation for socket. Try again\n");
		return 0;
	}
	
	
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(9003);
	serv_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
	memset(serv_addr.sin_zero,0,sizeof(serv_addr.sin_zero));
	
	struct timeval read_timeout;
	read_timeout.tv_sec = 0;
	read_timeout.tv_usec = 100;
	int k = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof(read_timeout));
	
	
	while(1){
		printf("Enter the text message to send to server(enter stop to exit)\n");
		char buffer[1024],ch;
		memset(buffer,'\0',sizeof(buffer));
		scanf("%[^\n]",buffer);
		scanf("%c",&ch);
		int x = sendto(sockfd,buffer,strlen(buffer)+1,0,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
		int recv_stat = recvfrom(sockfd,buffer,1024,0,NULL,NULL);
		if(recv_stat == -1){
			printf("Failed to receive the message from the server within stipulated time (Server might be powered off) \n");
			break;
		}
		printf("The message received from the server is : %s\n",buffer);
		if(strcmp(buffer,"STOP")==0)
			break;
	}
	return 0;
}
