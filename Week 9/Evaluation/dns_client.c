#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<unistd.h>

#define BUFF_LEN 4096
#define MAX_SUB_buffer 20


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
	serv_addr.sin_port = htons(1234);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(serv_addr.sin_zero,0,sizeof(serv_addr.sin_zero));
	
	struct timeval read_timeout;
	read_timeout.tv_sec = 0;
	read_timeout.tv_usec = 100;
	int k = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof(read_timeout));
	
	while(1){
		printf("Enter the command:");
        char buffer[BUFF_LEN];
        fgets(buffer,sizeof(buffer),stdin);
        int len = strlen(buffer);
        buffer[len-1] = '\0';
		int x = sendto(sockfd,buffer,strlen(buffer)+1,0,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
		bzero(buffer,BUFF_LEN);
		while(1){
			if(recvfrom(sockfd,buffer,sizeof(buffer),0,NULL,NULL) > 0)
				break;
			//sleep(1);
		}
		printf("Message received from the server is : \n%s\n",buffer);
		if(!strcmp(buffer,"quit")){
			printf("Thank you for using the application\n");
			break;
		}
		bzero(buffer,BUFF_LEN);
        printf("Else Enter quit to close the communication\n");
	}
    
    close(sockfd);

}