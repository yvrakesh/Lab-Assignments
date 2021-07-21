// Client for receiving tcp packets
// Stream socket Client

// For all basic C programs
#include<stdio.h>
#include<stdlib.h>

// For memset function to initialize to either 0 or -1 or '\0'
#include<string.h>

#include<sys/types.h>
// For socket, connect, recv, struct sockaddr
#include<sys/socket.h>

// For inet_addr
#include<arpa/inet.h>
// For struct sockaddr_in, htons
#include<netinet/in.h>
// To close a file
#include<unistd.h>

/*

struct sockaddr{
	unsigned short sa_family;
	char sa_data[14];
};

struct sockaddr_in{
	short sin_family;
	unsigned short sin_port;
	struct in_addr sin_addr;
	char sin_zero[8];
};

struct in_addr{
	unsigned long s_addr;
	// 4 byte long integer
};

// These are predefined in the header file <sys/socket.h>
*/


int main(){
	
	// This creates a socket and its file descriptor is returned
	// int socket(int domain, int type, int protocol)
	// domain is either PF_INET OR AF_INET for IPv4   PF_INET6 OR AF_INET for IPv6
	// AF -> Address Family and PF -> Protocol Family
	// type is either SOCK_STREAM OR SOCK_DGRAM other types might also exist but out of scope for this
	// protocol by default is set to 0 so that it automatically chooses proper protocol
	// Or you can set protocol by calling getprotobyname() function with parameters either tcp or udp and get its protocol number
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd == -1){
		printf("Error in creating the socket. Try again\n");
		return 0;
	}
	else if(sockfd <= 0){
		printf("Invalid memory creation for socket. Try again\n");
		return 0;
	} 
	
	// sockaddr_in is predefined in netinet/in.h and is used for IPv4 socket address 
	// Use sockaddr_in6 in case of IPv6 socket address
	struct sockaddr_in serv_addr;
	// This can also be PF_INET for IPv4. Use AF_INET6 or PF_INET for IPv6
	serv_addr.sin_family = AF_INET;
	// htons is used to host to network translation to use network byte order
	// Port number above 1024 can be used since they are generally os reserved and require privilege to use
	serv_addr.sin_port = htons(9002);
	// Set IP address to local host
	// inet_addr("0.0.0.0");
	// INADDR_ANY
	// inet_addr converts IPv4 dotted decimal notation to an integer value which is used as internet address
	// structure declaration is above shown for clear understanding of this
	serv_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
	// Set all the bits of padding field to 0
	// memset is faster but only works for 0 or -1 initializations
	memset(serv_addr.sin_zero,0,sizeof(serv_addr.sin_zero));
	

	// Client establish connection with server since this is tcp protocol connection is essential
	int con_status = connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr));
	if(con_status == -1){
		printf("The connection to the remote socket failed\n");
		return 0;
	}
	
	// connected to server
	char serv_resp[256];
	recv(sockfd,&serv_resp,sizeof(serv_resp),0);
	printf("The server sent the data '%s'\n",serv_resp);
	/*
	while(1){
		printf("Enter the text message to send to server(enter stop to exit)\n");
		char buffer[1024],ch;
		memset(buffer,'\0',sizeof(buffer));
		scanf("%[^\n]",buffer);
		scanf("%c",&ch);
		int x = send(sockfd,buffer,strlen(buffer)+1,0);
		printf("%d\n",x);
		int recv_stat = recv(sockfd,&buffer,1024,0);
		if(recv_stat == -1){
			printf("Failed to receive the message from the server within stipulated time (Server might be powered off) \n");
			break;
		}
		printf("The message received from the server is : %s\n",buffer);
		if(strcmp(buffer,"STOP")==0)
			break;
	}
	*/
	
	// close the socket
	close(sockfd);
	
	return 0;
}
