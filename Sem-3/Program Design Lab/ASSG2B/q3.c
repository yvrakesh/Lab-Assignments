#include<stdio.h>
#include<stdlib.h>
int read1(FILE *p,int X[], int m);
int read2(FILE *p,int Y[], int n);
void checkSubset(FILE *q,int X[] , int m, int Y[],int n);
void main(int argc,char * argv[]){
	FILE *p,*q;
	p = fopen((char *)argv[1],"r");
	q = fopen((char *)argv[2],"w");
	char ch;
	ch = fgetc(p);
	int m,n,n1,i=0;
	int *X,*Y;
	for(;ch!='t' && ch!=EOF;){
		switch(ch){
			case 'r':
				if(i%2==0){
					if(fscanf(p,"%d",&m)>0 && m>0){
						X = (int *)malloc(m*sizeof(int));
						if(read1(p,X,m)!=-1)
							i++;
						else
							fprintf(q,"Please enter proper first array elememts\n");}
					else
						fprintf(q,"Please enter a valid first array size\n");}
				else{
					if(fscanf(p,"%d",&n)>0 && n>0){
					Y = (int *)malloc(m*sizeof(int));
					if(read2(p,Y,n)!=-1)
						i++;
					else
						fprintf(q,"Please enter proper second array elements\n");}
					else
						fprintf(q,"Please enter a valid second array size\n");}
				break;
			case 'c':
				if(i%2==0 && i!=0)
				checkSubset(q,X, m, Y, n);
				else
					fprintf(q,"Please enter the two arrays first to perform some operation\n");
				break;
		}
		ch = fgetc(p);
	}
}
int read1(FILE *p,int X[],int m){
	int c=0;
	for(int i=0;i<m;i++){
		if(fscanf(p,"%d",&X[i])>0)
			c++;}
	if(c!=m)
		return -1;
	else
		return 1;
}
int read2(FILE *p,int Y[],int n){
	int c=0;
	for(int i=0;i<n;i++){
		if(fscanf(p,"%d",&Y[i])>0)
			c++;}
	if(c!=n)
		return -1;
	else
		return 1;
}
void checkSubset(FILE *q,int X[],int m,int Y[],int n){
	int c=0;
	for(int i=0;i<n;i++){
		for(int j=0;j<m;j++){
			if(Y[i] == X[j])
				c++;}}
	if(c==n)
		fprintf(q,"true\n");
	else
		fprintf(q,"false\n");
}
