
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
	serv_addr.sin_port = htons(9002);
	serv_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
	memset(serv_addr.sin_zero,0,sizeof(serv_addr.sin_zero));
	
	bind(sockfd,(struct sockaddr*) &serv_addr, sizeof(serv_addr));
	
	while(1){
		int x = listen(sockfd,7);
		if(x != 0){
			printf("Failed in Listening to the socket\n");
			return 0;
		}
		
		int newfd = accept(sockfd,NULL,NULL);
		if(newfd < 0){
			printf("Failed to create a new socket for accepted connection\n");
			return 0;
		}
		
		char serv_resp[] = "Congrats! You have reached the server.";
		send(newfd,serv_resp,sizeof(serv_resp),0);
		
		
		close(newfd);
	}	
	close(sockfd);
	return 0;
}
