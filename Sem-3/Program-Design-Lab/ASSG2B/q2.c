#include<stdio.h>
#include<stdlib.h>
int read(FILE *p,int X[],int n);
void search(FILE *q,int X[],int n);
void display(FILE *q,int X[],int n);
void main(int argc,char * argv[]){
	FILE *p,*q;
	p = fopen((char *)argv[1],"r");
	q = fopen((char *)argv[2],"w");
	char ch;
	ch = fgetc(p);
	int n,n1,i=0;
	int *X;
	for(;ch!='t' && ch!=EOF;){
		switch(ch){
			case 'r':
				if(fscanf(p,"%d",&n)>0){
				  if(n>0){
					X = (int *)calloc(n,sizeof(int));
					if(read(p,X,n)!=-1)
					i++;
					else
					fprintf(q,"Please enter valid array elements\n");}
				  else
					  fprintf(q,"Please enter a positive array size\n");}
				else
					fprintf(q,"Please enter a valid array size\n");
				break;
			case 's':
				if(i>0)
				search(q,X,n);
				else
					fprintf(q,"Please enter the valid array elements before performing any operation\n");
				break;
			case 'd':
				if(i>0){
				   if(fscanf(p,"%d",&n1)>0){
					if(n1>0)
						display(q,X,n1);
					else
						fprintf(q,"Please enter a positive display size\n");}
				   else
					fprintf(q,"Please enter the valid display size\n");}
				else
					fprintf(q,"Please enter the valid array elements before performing any operation\n");
				break;
		}
		ch = fgetc(p);
	}
}
int read(FILE *p,int X[],int n){
	int c=0;
	for(int i=0;i<n;i++){
		if(fscanf(p,"%d",&X[i])>0)
			c++;}
	if(c!=n)
		return -1;
}
void search(FILE *q,int X[],int n){
	int cout = 0,a,b=0,h=0;
	for(int i=0;i<n;i++){
		if(X[i]>0){
			h = -1;
			break;}}

	if(h==-1){
	for(int i=0;i<n;i++){
		a = 0; 
		for(int j=0;j<n;j++){
			if(X[i]<X[j])
				a++;
		}
		if(X[i]<=a)
			b = X[i];
		if(cout<b)
			cout = b;}

		if(cout == 0)
			fprintf(q,"%d\n",-999);
		else
			fprintf(q,"%d\n",cout);

	}
	else{
		int h1=0;
		for(int i=0;i<n;i++){
			if(X[h1]>X[i])
				h1 = i;}
		fprintf(q,"%d\n",X[h1]);}
}
void display(FILE *q,int X[],int n){
	for(int i=0;i<n;i++)
		fprintf(q,"%d ",X[i]);
	fprintf(q,"\n");
}
