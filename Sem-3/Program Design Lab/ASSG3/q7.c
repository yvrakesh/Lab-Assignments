#include<stdio.h>
#include<stdlib.h>
int push(int stk[],int top,int element,int c);
int pop(int stk[],int top);
void print(int stk[],int n,int top,FILE *q);
int main(int argc,char * argv[]){
	FILE *p,*q;
	p = fopen((char *)argv[1],"r");
	q = fopen((char *)argv[2],"w");
	int c;
	fscanf(p,"%d",&c);
	int stk[c],top,element,n;
	top = -1;
	char ch;
	ch = fgetc(p);
	while(1){
			if(ch =='p'){
				fscanf(p,"%d",&element);
				if(push(stk,++top,element,c)==-1){
					fprintf(q,"999\n");
					--top;}
			}
			else if(ch =='t'){
                    int c;
                    c = pop(stk,--top);
                if(c!=-1)
                    fprintf(q,"%d\n",c);
				else{
                    fprintf(q,"-1\n");
                    ++top;
				}
			}
			else if(ch == 'd'){
				fscanf(p,"%d",&n);
				print(stk,n,top,q);
			}
			else if(ch == 's')
                break;
		ch = fgetc(p);
	}
return 0;
}
int push(int stk[],int top,int element,int c){
	if(top<c){
	stk[top] = element;
	}
	else
		return -1;
	return 0;
}
int pop(int stk[],int top){
	if(top<-1)
	return -1;
	else
		return stk[top+1];
}
void print(int stk[],int n,int top,FILE *q){
	if(n==0){
		for(int i=0;i<=top;i++)
			fprintf(q,"%d ",stk[top-i]);
			fprintf(q,"\n");
	}
	if(n>0 & top>=0){
	       if(n>top){
		for(int i=0;i<=top;i++)
			fprintf(q,"%d ",stk[top-i]);
		fprintf(q,"END\n");
		}
	       else {
		       for(int i=0;i<n;i++)
			       fprintf(q,"%d ",stk[n-i]);
			       fprintf(q,"\n");
	       }
	}
	if(top < 0)
		fprintf(q,"-1\n");
}

