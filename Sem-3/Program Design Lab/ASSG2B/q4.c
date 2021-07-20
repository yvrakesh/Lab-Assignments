#include<stdio.h>
#include<stdlib.h>
void read(FILE *p,char A[],int n);
void heapSort(FILE *q,char A[],int n);
void max_Heapify(char A[],int i,int n);
void display(FILE *q,char A[],int n);
void main(int argc,char * argv[]){
	FILE *p,*q;
	p = fopen((char *)argv[1],"r");
	q = fopen((char *)argv[2],"w");
	char *A;
	char ch;
	ch = fgetc(p);
	int n,n1,i;
	for(;ch!='t' && ch!=EOF;){
	switch(ch){
		case 'r':
			if(fscanf(p,"%d",&n)>0 && n>0){
				A = (char *)calloc(n,sizeof(char));
				read(p,A,n);
				i++;	}
			else
				fprintf(q,"Please enter the valid array size\n");
			break;
		case 's':
			if(i>0)
				heapSort(q,A,n);
			else
				fprintf(q,"Please enter a valid character array before further processing\n");
			break;
		case 'd':
			if(i>0){
			if(fscanf(p,"%d",&n1)>0 && n1>0)
				display(q,A,n1);
			else
				fprintf(q,"Please enter a valid display size\n");}
			else
				fprintf(q,"Please enter a valid character array before further proceeding\n");
			break;
	}
	ch = fgetc(p);
	}
}
void read(FILE *p,char A[],int n){
	char ch;
	fscanf(p,"%c",&ch);
	for(int i=0;i<n;i++)
		fscanf(p,"%c ",&A[i]);
}
void heapSort(FILE *q,char A[],int n){
	char temp;
	for(int i=n/2;i>=0;i--)
		max_Heapify(A,i,n);
	for(int i=n-1;i>0;i--){
		temp = A[i];
		A[i] = A[0];
		A[0] = temp;
		max_Heapify(A,0,i);
	}
	for(int i=0;i<n;i++)
		fprintf(q,"%c ",A[i]);
	fprintf(q,"\n");
}
void max_Heapify(char A[],int i,int n){
	int l = 2*i+1;
	int r = 2*i+2;
	int lar = i;
	if(l<n && A[l]>A[lar])
		lar = l;
	if(r<n && A[r]>A[lar])
		lar = r;
	if(lar!=i){
		char temp;
		temp = A[i];
		A[i] = A[lar];
		A[lar] = temp;
		max_Heapify(A,lar,n);
	}
}
void display(FILE *q,char A[],int n){
	for(int i=0;i<n;i++)
		fprintf(q,"%c ",A[i]);
	fprintf(q,"\n");
}
