#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdbool.h>
#include<ctype.h>

#define BUFF_LEN 4096
#define MAX_SUB_DOMAIN 20
#define SUB_DOMAIN_LEN 50
#define MAX_NAME_SERVERS 20

int parse(char *domain,char *name_server, char nameservers[][SUB_DOMAIN_LEN]){
	char command[BUFF_LEN];
    strcpy(command,"nslookup -type=ns ");
    strcat(command,domain);
	strcat(command," ");
	strcat(command,name_server);
	printf("%s\n",command);
	const char * cmd = command;
	char result[BUFF_LEN] = "";
	FILE * out = popen(cmd, "r");
	char buffer[BUFF_LEN];
	while (fgets (buffer, 1000, out))
		strcat(result,buffer);
	pclose(out);

	char *token;
	strtok(result,"\n");
	strtok(NULL,"\n");
	strtok(NULL,"\n");
	strtok(NULL,"=");
	token = strtok(NULL,"\n");
	int i = 0;
	while(token != NULL){
		strncpy(nameservers[i++],token+1,strlen(token)-2);
		strtok(NULL,"=");
		token = strtok(NULL,"\n");
	}
	return i;
}

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
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	memset(serv_addr.sin_zero,0,sizeof(serv_addr.sin_zero));
	
	
	int flag1 = bind(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
	if(flag1 == -1){
		printf("Failed to bind to an IP and port\n");
		close(sockfd);
		return 0;
	}

	struct sockaddr_storage serv_storage;
	int addr = sizeof(serv_storage);
	while(1){
		char buffer[BUFF_LEN];
		bzero(buffer,BUFF_LEN);
		while(1)
			if(recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr *)&serv_storage,&addr) > 0)
				break;
		if(!strncmp(buffer,"quit",4)){
			printf("Requested to close the service\n");
			sendto(sockfd,buffer,strlen(buffer),0,(struct sockaddr *)&serv_storage,addr);
			break;
		}
		else{
			printf("Request from client is : %s",buffer);
			buffer[strlen(buffer)] = '\n';
			if(strncmp(buffer,"A ",2) && strncmp(buffer,"AAAA ",5) && strncmp(buffer,"NS ",3) && strncmp(buffer,"CNAME ",5)){
				char buff[] = "Illegal\n";
				sendto(sockfd,buff,strlen(buff)+1,0,(struct sockaddr *)&serv_storage,addr);
				continue;
			}
			char *token1,*token2;
			token1 = strtok(buffer," ");
			char req[10];
			strcpy(req,token1);
			token2 = strtok(NULL,"\n");
			strcpy(buffer,token2);
			char buffer_copy[BUFF_LEN];
			strcpy(buffer_copy,buffer);

			FILE *fp=fopen("cache.txt","r");
			char check[BUFF_LEN];
			strcpy(check,buffer);
			strcat(check,"-");
			strcat(check,req);
			printf("check : %s\n",check);
			bool flag = 0;
			char buff[BUFF_LEN], res[BUFF_LEN];
			bzero(res,BUFF_LEN);
			while(fp!=NULL && fgets(buff,BUFF_LEN,fp)){
				if(strncmp(buff,check,strlen(check))==0){
					while(1){
						bzero(buff,BUFF_LEN);
						fgets(buff,BUFF_LEN,fp);
						if(strncmp(buff,"---",3)==0) break;
						strcat(res,buff);
					}
					sendto(sockfd,res,strlen(res),0,(struct sockaddr *)&serv_storage,addr);
					printf("Response found in Cache\n");
					flag = 1;
					break;
				}
			}
			if(flag)
				continue;
			
			int count = 1;
			for(int i=0;i<strlen(buffer);i++)
				if(buffer[i] == '.')
					count++;
			printf("Total number of subdomains in the given domain name is %d\n",count);

			char a[count][SUB_DOMAIN_LEN];
			char *token;
			token = strtok(buffer_copy,".");
			for(int i=0;i<count;i++){
				strcpy(a[i],token);
				token = strtok(NULL,".");
			}
			
			char div_sub_domains[count][SUB_DOMAIN_LEN];
			strcpy(div_sub_domains[0],a[count-1]);

			for(int i=1;i<count;i++){
				strcpy(div_sub_domains[i],a[count-1-i]);
				strcat(div_sub_domains[i],".");
				strcat(div_sub_domains[i],div_sub_domains[i-1]);
			}

			for(int i=0;i<count;i++)
				printf("%s\n",div_sub_domains[i]);
			
			char nameservers[MAX_NAME_SERVERS][SUB_DOMAIN_LEN];
			int name_count = parse(".","",nameservers);
			for(int i=0;i<name_count;i++)
				printf("%s\n",nameservers[i]);
			for(int j=0;j<count-1;j++){
				char dup_nameservers[MAX_NAME_SERVERS][SUB_DOMAIN_LEN];
				for(int i=0;i<name_count;i++)
					strcpy(dup_nameservers[i],nameservers[i]);
				for(int i=0;i<name_count;i++)
					bzero(nameservers[i],SUB_DOMAIN_LEN);
				for(int i=0;i<name_count;i++){
					int temp = parse(div_sub_domains[j],dup_nameservers[i],nameservers);
					if(temp != 0){
						for(int i=0;i<temp;i++)
							printf("%s\n",nameservers[i]);
						name_count = temp;
						break;
					}
				}
			}

			char command[BUFF_LEN];
			for(int i=0;i<name_count;i++){
				strcpy(command,"nslookup -type=");
				strcat(command,req);
				strcat(command," ");
				strcat(command,buffer);
				strcat(command," ");
				strcat(command,nameservers[i]);
				printf("%s\n",command);
				const char * cmd = command;
				char result[BUFF_LEN] = "";
				FILE * out = popen(cmd, "r");
				char buff[BUFF_LEN];
				while (fgets (buff, 1000, out))
					strcat(result,buff);
				pclose(out);
				sendto(sockfd,result,strlen(result),0,(struct sockaddr *)&serv_storage,addr);
				
				FILE *fp=fopen("cache.txt","a");
				fprintf(fp,"%s-%s\n",buffer,req);
				fprintf(fp,"%s\n",result);
				fprintf(fp,"-------\n");
				fclose(fp);
				break;
			}
		}
	}
	
	close(sockfd);
	
	return 0;
}