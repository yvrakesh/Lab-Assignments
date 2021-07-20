#include<stdio.h>
#include<stdlib.h>
int enqueue(int Q[],int element,int c,int A[]);
int dequeue(int Q[],int c,int A[]);
void print(int Q[],int n,FILE *fp2,int A[],int c);
int main(int argc,char * argv[]){
    FILE *fp1,*fp2;
    fp1 = fopen((char *)argv[1],"r");
    fp2 = fopen((char *)argv[2],"w");
    int c;
    fscanf(fp1,"%d",&c);
    int Q[c];
    int A[2] = {-1,-1};
    char ch;
    int element,n,i=0;
    while(1){
        ch = fgetc(fp1);
        if(ch == 'e'){
                 fscanf(fp1,"%d",&element);
                if(enqueue(Q,element,c,A)==-999)
			fprintf(fp2,"-999\n");
        }
        else if(ch == 'd')
            fprintf(fp2,"%d\n",dequeue(Q,c,A));
        else if(ch == 'p'){
            fscanf(fp1,"%d",&n);
            print(Q,n,fp2,A,c);
        }
        else if(ch == 's')
            break;
    }
}
int enqueue(int Q[],int element,int c,int A[]){
        if((A[0]==A[1]+1)||(A[0]==0 && A[1] == c-1))
            return -999;
		if(A[0] == -1)
			A[0] = 0;
                A[1] = (A[1]+1)%c;
                Q[A[1]] = element;
        return 0;
}
int dequeue(int Q[],int c,int A[]){
    if(A[0] == -1)
        return -1;
        int element = Q[A[0]];
    if(A[0]==A[1]){
        A[0] = -1;
        A[1] = -1;
    }
    else
        A[0] = (A[0]+1)%c;
    return element;
}
void print(int Q[],int n,FILE *fp2,int A[],int c){
	if(A[0]==-1)
		fprintf(fp2,"-1\n");
	else{
		if(n==0){
			for(int i=A[0];i!=A[1];i=(i+1)%c)
				fprintf(fp2,"%d ",Q[i]);
				fprintf(fp2,"%d ",Q[A[1]]);
			fprintf(fp2,"\n");}
		else if(n<c){
			int j=A[0],k;
			for(int i=0;i<n;i++){
				fprintf(fp2,"%d ",Q[j]);
				j=(j+1)%c;
			}
				fprintf(fp2,"\n");
		}
		else if(n>=c){
			for(int i=A[0];i!=A[1];i=(i+1)%c)
				fprintf(fp2,"%d ",Q[i]);
				fprintf(fp2,"%d ",Q[A[1]]);
			fprintf(fp2,"END\n");
			}
				
	}
}
