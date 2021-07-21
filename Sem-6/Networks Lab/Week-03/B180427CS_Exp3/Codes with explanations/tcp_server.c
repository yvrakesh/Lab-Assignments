// Server for transmitting tcp packets
// Stream socket Server

// For all basic C programs
#include<stdio.h>
#include<stdlib.h>

// For memset function to initialize to either 0 or -1 or '\0'
#include<string.h>

// For toupper() functionz
#include<ctype.h>

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
	
	
	// bind assigns a local protocol address to the socket i.e. a 32 bit IPv4 address with 16 bit port number
	bind(sockfd,(struct sockaddr*) &serv_addr, sizeof(serv_addr));
	
	while(1){
		// Listen on the sockets with maximum number of connection requests queue for this socket
		int x = listen(sockfd,7);
		if(x != 0){
			printf("Failed in Listening to the socket\n");
			return 0;
		}
		// Takes the connection requests from the formed queue and then call fork() thus providing a child process for each new connection giving new file descriptors
		// These file descriptor is only for that particular accepted connection only
		int newfd = accept(sockfd,NULL,NULL);
		if(newfd < 0){
			printf("Failed to create a new socket for accepted connection\n");
			return 0;
		}
		
		// Connection established
		char serv_resp[] = "Congrats! You have reached the server.";
		// Sends some default message saying connected to server
		send(newfd,serv_resp,sizeof(serv_resp),0);
		
		/*while(1){
			char buffer[1024];
			int len = recv(sockfd,buffer,1024,0);
			printf("%d\n",len);
			printf("Message received from client : %s\n",buffer);
			// This returns number of bits of data received including null character
			for(int i=0;i<len-1;i++)
				buffer[i] = toupper(buffer[i]);
			send(sockfd,buffer,sizeof(buffer),0);
			if(strcmp(buffer,"STOP")==0)
				break;
		}*/
		close(newfd);
	}	
	close(sockfd);
	return 0;
}
