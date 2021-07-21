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

#define BUFFER_LEN 4096
#define MAX_NAME_LEN 50
#define IP_ADDR "127.0.0.1" // local host
#define PORT_NO 3010

bool flag = 0;
char name[MAX_NAME_LEN];
int sockfd = 0;


void str_trim_lf(char * a, int len){
	// Change last character in the string to null pointer to end the string
	for(int i=0;i<len;i++){
		if(a[i] == '\n'){
			a[i] = '\0';
			break;
		}
	}
}

void catch_ctrl_c_and_exit(int sig){
	flag = 1;
}
static bool bool_flag = 1;

void send_msg(){
	char msg[BUFFER_LEN] = {};
	char buffer[BUFFER_LEN+2*MAX_NAME_LEN] = {};
	
	while(1){
		//printf("I am in send_msg\n");
		fflush(stdout);
		
		fgets(msg,BUFFER_LEN,stdin);
		str_trim_lf(msg,BUFFER_LEN);
		
		sprintf(buffer,"%s\n",msg);
		send(sockfd,buffer,strlen(buffer),0);
		if(!strcmp(msg,"Bye") || flag)
			break;
		
		bzero(msg,BUFFER_LEN);
		
		bzero(buffer,BUFFER_LEN+MAX_NAME_LEN);
	}
	bool_flag = 0;
	catch_ctrl_c_and_exit(2);
}

void recv_msg(){
	while(bool_flag==1 && !flag){
		char msg[500];
		if(recv(sockfd, msg, sizeof(msg), 0) > 0){
			if(!strcmp(msg, "Sending the Video\n")){
				FILE *fp;
				fp = fopen("receivedVideo.mp4", "w");
				int bytes;
				int i=0;
				while(1){
					bytes = recv(sockfd, msg, sizeof(msg), 0);

					if(strcmp(msg, "file transfer finished\n")==0)
						break;

					char buf[10];
					sprintf(buf, "received");
					//confirmation
					send(sockfd, &buf, sizeof(buf), 0);
					fwrite(msg, 1, bytes, fp);
					i+=500;
					if(i%5000000==0)
						printf("%d mb received\n", i/1000000);
				}
				printf("Video file received\n");
				bzero(msg, sizeof(msg));
				fflush(stdout);
			}
			else{
				printf("%s\n", msg);
				fflush(stdout);
			}
			printf("-> ");
			bzero(msg,500);
		}
		fflush(stdout);
	}
}


int main(){

	sockfd = socket(AF_INET,SOCK_STREAM,0);
	
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
	serv_addr.sin_port = htons(PORT_NO);
	serv_addr.sin_addr.s_addr = inet_addr(IP_ADDR);
	memset(serv_addr.sin_zero,0,sizeof(serv_addr.sin_zero));
	
	int con_status = connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
	if(con_status == -1){
		printf("Failed to connect with the server\n");
		return 0;
	}
	signal(SIGINT, catch_ctrl_c_and_exit);
	
	printf("Enter your name: ");
	fgets(name,MAX_NAME_LEN,stdin);
	str_trim_lf(name,strlen(name));
	
	if (strlen(name) > MAX_NAME_LEN || strlen(name) < 2){
		printf("Name must be less than 30 and more than 2 characters.\n");
		return 0;
	}
	
	send(sockfd,name,MAX_NAME_LEN,0);
	printf("---- Welcome to the Chatroom ----\n");
	printf("You may exit from the room anytime by entering 'ctrl+c' or 'Bye'\n");
	
	pthread_t recv_msg_thread;
	int thr_recv_status = pthread_create(&recv_msg_thread,NULL,(void *)recv_msg,NULL);
	if(thr_recv_status != 0){
		printf("Received thread failed to establish\n");
		return 0;
	}
	
	pthread_t send_msg_thread;
	int thr_status = pthread_create(&send_msg_thread,NULL,(void *)send_msg,NULL);
	if(thr_status != 0){
		printf("Failed to establish the thread\n");
		return 0;
	}
	
	pthread_join(recv_msg_thread,0);
	pthread_join(send_msg_thread,0);
	
	while(1){
		if(flag){
			printf("Thank you for using the Application. Bye\n");
			break;
		}
		// to reduce cpu usage
		sleep(1);
	}
	
	close(sockfd);
	
	return 0;
}

