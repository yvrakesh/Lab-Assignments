
#include<stdio.h>
#include<stdlib.h>

#include<string.h>

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
	

	int con_status = connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr));
	if(con_status == -1){
		printf("The connection to the remote socket failed\n");
		return 0;
	}
	
	char serv_resp[256];
	recv(sockfd,&serv_resp,sizeof(serv_resp),0);
	printf("The server sent the data '%s'\n",serv_resp);
	
	close(sockfd);
	
	return 0;
}
