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
#define SIZE 2048
#define MAX_NAME_SERVERS 20
#define QSIZE 100

typedef struct DNSPacketHeader DNSHeader;
typedef struct DNSPacketQuestion DNS_Question;
typedef struct DNSPacketAnsAddition DNS_Ans;
typedef struct DNSRecord DNS_Record;

typedef struct handleLookupArg HLArg;

struct DNSPacketHeader{
	// ID a 16 bit identifier
	char ID[2];
	// 1 bit that specifies query(0) or response(1)
	unsigned short QR :1;
	// 4 bit field that tells kind of query
	// 0 to represent a standard query
	unsigned short OPCODE :4;
	// Authritative answer 1 bit for responses
	// specifies if responding name server is authoritative
	unsigned short AA :1;	
	// Truncation to specify if msg was truncated
	unsigned short TC :1;
	// 1 to tell if recursion is required
	unsigned short RD :1;
	// To denote if recursive query support is available in the nameserver
	unsigned short RA :1;	
	// Reserved for future use
	unsigned short Z :1;
	// 4 bit field is set as part of response
	// 0 No error condition
	// 1 Format error
	// 2 Server Failure
	// 3 Name Error
	// 4 Not implemented
	// 5 Refused
	unsigned short RCODE :4;
	// Checking Disabled
	unsigned short CD :1;
	// Authenticated Date
	unsigned short AD :1;	
	// Number of entries in question section
	unsigned short QDCOUNT;
	// Number of resource records in answer section
	unsigned short ANCOUNT;
	// Number of name server resource records 
	unsigned short NSCOUNT;
	// Anumber of resource records in additional records
	unsigned short ARCOUNT;
};

// Constant sized fields of query structure
struct DNSPacketQuestion{
	char QNAME[QSIZE];
	short unsigned qsize;
	char QTYPE[2];
	char QCLASS[2];
};

struct DNSPacketAnsAddition{
	unsigned short TTL;
	unsigned short RDLENGTH;
	char RDATA[QSIZE];
};

struct DNS_RECORD{
	DNS_Question Q;
	DNS_Ans A;
	struct DNS_RECORD *next;
	struct DNS_RECORD *prev;
};


struct HandleLookupArg{
	int sock;
	char buf[SIZE];
	struct sockaddr_in clientAddr;
};

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
		if(!strncmp(token," 1",2) || !strncmp(token," 2",2) || !strncmp(token," 3",2) || !strncmp(token," 4",2))
			break;
		if(!strncmp(token," 5",2) || !strncmp(token," 6",2) || !strncmp(token," 7",2) || !strncmp(token," 8",2) || !strncmp(token," 9",2))
			break;
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
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
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
		int len = recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr *)&serv_storage,&addr);
		if(len == 0)
			continue;
		if(!strncmp(buffer,"quit",4)){
			printf("Requested to close the service\n");
			sendto(sockfd,buffer,len,0,(struct sockaddr *)&serv_storage,addr);
			break;
		}
		else{
			printf("Request from client is : %s\n",buffer);
			buffer[strlen(buffer)] = '\n';
			if(strncmp(buffer,"A ",2) && strncmp(buffer,"AAAA ",5) && strncmp(buffer,"NS ",3) && strncmp(buffer,"CNAME ",5)){
				char buff[] = "UNAUTHORISED REQUEST";
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

			for(int j=0;j<count;j++){
				char dup_nameservers[MAX_NAME_SERVERS][SUB_DOMAIN_LEN];
				for(int i=0;i<name_count;i++)
					strcpy(dup_nameservers[i],nameservers[i]);
				for(int i=0;i<name_count;i++)
					bzero(nameservers,SUB_DOMAIN_LEN);
				for(int i=0;i<name_count;i++){
					int temp = parse(div_sub_domains[j],dup_nameservers[i],nameservers);
					if(temp != 0){
						for(int i=0;i<temp;i++)
							printf("%s\n",nameservers[i]);
						name_count = temp;
						break;
					}
					bzero(nameservers[0],SUB_DOMAIN_LEN);
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