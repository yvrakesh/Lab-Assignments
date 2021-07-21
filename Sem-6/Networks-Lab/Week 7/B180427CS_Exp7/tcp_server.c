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

#define ip_addr "127.0.1.2"
#define PORT 8080
#define MAX_CLIENTS 256
#define BUFFER_SIZE 4096

// uid for providing unique id for each of the user
// Count to tell total number of active users in the room
int count = 0,uid = 10;


typedef struct{
	struct sockaddr_in addrs;
	int sockfd;
	// user id is unique for a client
	int uid;
	//char name[MAX_NAME_LEN];
}client_t;

client_t *clients[MAX_CLIENTS];

// This mutex is required to send messages to various clients
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

// Add a particular client to the online queue
void add_client(client_t * client){
	pthread_mutex_lock(&clients_mutex);
	for(int i=0;i<MAX_CLIENTS;i++){
		if(!clients[i]){
			clients[i] = client;
			break;
		}
	}
	
	pthread_mutex_unlock(&clients_mutex);

}

// Remove client from the online status
void rem_client(int uid){
	// since uid is the unique given for a client
	pthread_mutex_lock(&clients_mutex);
	
	for(int i=0;i<MAX_CLIENTS;i++){
		if(clients[i]){
			if(clients[i]->uid == uid){
				// remove the entry for that client
				clients[i] = NULL;
				break;
			}
		}
	}
	
	pthread_mutex_unlock(&clients_mutex);
}

// Handle all the communications with the clients
void * handle_client(void *arg){

	char buffer[BUFFER_SIZE] = {};
	char *res = "HTTP/1.1 200 OK\r\nServer: Apache/2.4.46 (Ubuntu)\r\nAccept-Ranges: bytes\r\nVary: Accept-Encoding\r\nContent-Length: 119\r\nKeep-Alive: timeout=5, max=100\r\nConnection: Keep-Alive\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html><html><head><title>Networks Lab Exp 7</title></head><body><h1>Welcome to Networks Lab!</h1></body></html>";
	int leave_flag = 0;
	
	count++;
	
	client_t *client = (client_t *)arg;
	
	while(1){
		int recv_status = recv(client->sockfd,&buffer,BUFFER_SIZE,0);
		if(recv_status <= 0)
			break;
		//if(buffer[5] == ' '){
		printf("New Request from Client\n\n%s",buffer);
		send(client->sockfd, res,strlen(res),0);
		printf("Response sent to the client\n\n");
		//}
		bzero(buffer, BUFFER_SIZE);
	}
	
	// Delete client from the queue and also free the thread
	close(client->sockfd);
	rem_client(client->uid);
	free(client);
	count--;
	pthread_detach(pthread_self());
	
	return NULL;
}

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
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = inet_addr(ip_addr);
	memset(serv_addr.sin_zero,0,sizeof(serv_addr.sin_zero));
	
	int option = 1;
	signal(SIGPIPE, SIG_IGN);

	if(setsockopt(sockfd, SOL_SOCKET,(SO_REUSEPORT | SO_REUSEADDR),(char*)&option,sizeof(option)) < 0){
		perror("ERROR: setsockopt failed");
    		return EXIT_FAILURE;
	}
	
	int bind_status = bind(sockfd,(struct sockaddr*) &serv_addr, sizeof(serv_addr));
	if(bind_status == -1){
		printf("Error in binding the socket to the provided ip and port number\n");
		return 0;
	}
	
	int listen_status = listen(sockfd,10);
	if(listen_status == -1){
		printf("Error in listening for connections. i.e. listen setup failure\n");
		return 0;
	}
	
	printf("--- Server Open ---\n");
	
	pthread_t tid;
	
	while(1){
		// Accept the client from the queue
		struct sockaddr_in client_addr;
		socklen_t client_len = sizeof(client_addr);
		int newfd = accept(sockfd,(struct sockaddr*)&client_addr,&client_len);
		if(newfd < 0){
			printf("Failed to create a new socket for accepted connection\n");
			continue;
		}
		
		
		// Check if group size limit reached. If then disconnect with the client by sending limit exceeded message
		if(count == MAX_CLIENTS -1 ){
			printf("Maximum Clients connected. Group size is full. Please try after sometime\n");
			close(newfd);
			continue;
		}
		
		// Client details are collected for sending messages in future
		client_t *client = (client_t *)malloc(sizeof(client_t));
		client->addrs = client_addr;
		client->sockfd = newfd;
		client->uid = uid++;
		
		// Add client to the active member queue
		add_client(client);
		
		pthread_create(&tid, NULL, &handle_client, (void *)client);
		
		sleep(1);		
	}
	return 0;
}

