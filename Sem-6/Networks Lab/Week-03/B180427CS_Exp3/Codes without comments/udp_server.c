#include<stdio.h>
#include<stdlib.h>

#include<string.h>

#include<ctype.h>

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
	
	
	int flag = bind(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
	if(flag == -1){
		printf("Failed to bind to an IP and port\n");
		close(sockfd);
		return 0;
	}

	struct sockaddr_storage serv_storage;
	int addr = sizeof(serv_storage);
	while(1){
		char buffer[1024];
		int len = recvfrom(sockfd,buffer,1024,0,(struct sockaddr *)&serv_storage,&addr);
		printf("Message received from client : %s\n",buffer);
		for(int i=0;i<len-1;i++)
			buffer[i] = toupper(buffer[i]);
		sendto(sockfd,buffer,len,0,(struct sockaddr *)&serv_storage,addr);
	}
	
	close(sockfd);
	
	return 0;
}
