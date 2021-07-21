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

#define MAX_CLIENTS 256
#define BUFFER_SIZE 4096
#define MAX_NAME_LEN 50

// uid for providing unique id for each of the user
// Count to tell total number of active users in the room
int count = 0,uid = 10;


typedef struct{
	struct sockaddr_in addrs;
	int sockfd;
	// user id is unique for a client
	int uid;
	char name[MAX_NAME_LEN];
}client_t;

client_t *clients[MAX_CLIENTS];

// This mutex is required to send messages to various clients
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;


void str_trim_lf(char * a, int len){
	// Change last character in the string to null pointer to end the string
	for(int i=0;i<len;i++){
		if(a[i] == '\n'){
			a[i] = '\0';
			break;
		}
	}
}

// Add a particular client to the online queue
void add_client(client_t * client){

	pthread_mutex_lock(&clients_mutex);

	char buffer[BUFFER_SIZE];
	bool flag = 1;
	int j = 1;
	for(int i=0;i<MAX_CLIENTS;i++){
		if(clients[i]){
			flag = 0;
			break;
		}
	}
	if(flag){
		sprintf(buffer,"No other Co_PIs connected\n");
		int write_status = write(client->sockfd,buffer,strlen(buffer));
		if(write_status == -1)
			printf("Unable to write to that particular socket\n");
		sprintf(buffer,"Pl. wait.\n");
		write_status = write(client->sockfd,buffer,strlen(buffer));
		if(write_status == -1)
			printf("Unable to write to that particular socket\n");
	}
	else{
	sprintf(buffer,"The Co-PIs connected are:\n");
	int write_status = write(client->sockfd,buffer,strlen(buffer));
	if(write_status == -1)
		printf("Unable to write to that particular socket\n");
		
	for(int i=0;i<MAX_CLIENTS;i++){
		if(clients[i]){
			sprintf(buffer,"  %d. %s\n",j++,clients[i]->name);
			int write_status = write(client->sockfd,buffer,strlen(buffer));
			flag = 0;
			if(write_status == -1){
				printf("Unable to write to that particular socket\n");
				break;
			}
		}
	}
	}

	
		
	for(int i=0;i<MAX_CLIENTS; i++){
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


// Sending message to all the clients except the sender
void send_msg(char *s,int uid){
	pthread_mutex_lock(&clients_mutex);
	
	for(int i=0;i<MAX_CLIENTS;i++){
		if(clients[i]){
			if(clients[i]->uid !=uid){
				int write_status = write(clients[i]->sockfd,s,strlen(s));
				if(write_status == -1){
					printf("Unable to write to that particular client socket\n");
					break;
				}
			}
		}
	}
	
	pthread_mutex_unlock(&clients_mutex);
}

// Handle all the communications with the clients
void * handle_client(void *arg){
	char buffer[2*BUFFER_SIZE];
	char name[MAX_NAME_LEN];
	int leave_flag = 0;
	
	count++;
	client_t *client = (client_t *)arg;
	
	int recv_status = recv(client->sockfd,name,MAX_NAME_LEN,0);
	
	if(strlen(name) < 2 || strlen(name) >= MAX_NAME_LEN){
		printf("Please enter the name with atleast 2 characters and atmost 50 characters\n");
		leave_flag = 1;
	}
	
	if(recv_status <= 0){
		printf("Invalid password entry\n");
		leave_flag = 1;
	}
	else{
		strcpy(client->name,name);
		sprintf(buffer,"%s connected\n",client->name);
		printf("%s",buffer);
		send_msg(buffer,client->uid);
	}
	
	bzero(buffer, 2*BUFFER_SIZE);
	
	while(1){
	
		if(leave_flag)
			break;
		fflush(stdout);
		char buff[BUFFER_SIZE] = {};
		int receive = recv(client->sockfd,buff,BUFFER_SIZE,0);
		
		if(!strcmp(buff, "Bye\n") ){
			sprintf(buffer,"[%s]: %s",client->name,buff);
			send_msg(buffer,client->uid);
			
			sprintf(buffer,"%s left the discussion\n",client->name);
			send_msg(buffer,client->uid);
			break;
		}
		else if(receive > 0){
			if(strlen(buff) > 0){
				sprintf(buffer,"[%s]: %s",client->name,buff);
				send_msg(buffer,client->uid);
				str_trim_lf(buffer,strlen(buffer));
				//printf("%s\n",buffer);
			}
		}
		else if(receive == 0 ){
			sprintf(buffer,"%s disconnected\n",client->name);
			printf("%s",buffer);
			send_msg(buffer,client->uid);
			break;
		}
		else{
			printf("-1\n");
			break;
		}		
		bzero(buffer,2*BUFFER_SIZE);
		bzero(buff,BUFFER_SIZE);
	}
	
	// Delete client from the queue and also free the thread
	printf("%s left the discussion\n",client->name);
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
	serv_addr.sin_port = htons(8182);
	serv_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
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
	
	printf("I am listening on port 8182\n");
	
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
			//print_ipaddr(client_addr);
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

