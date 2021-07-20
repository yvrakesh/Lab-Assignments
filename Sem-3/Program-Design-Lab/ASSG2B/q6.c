#include<stdio.h>
#include<stdlib.h>
int read(FILE *p,int A[],int n);
void display(FILE *q,int A[],int n);
void kthLargest(FILE *q,int A[],int n,int k);
void quicksort(int A[],int f,int l);
int partition(int A[],int f,int l);
int main(int argc, char * argv[]){
	FILE *p,*q;
	p = fopen((char *)argv[1],"r");
	q = fopen((char *)argv[2],"w");
	char ch;
	ch = fgetc(p);
	int n,n1,k;
	int *A,i=0;
	for(;ch!='t' && ch!=EOF;){
		switch(ch){
			case 'r':
				if(fscanf(p,"%d",&n)>0 && n>0){
				A = (int *)calloc(n,sizeof(int));
				if(read(p,A,n)!=-1)
					i++;
				else
					fprintf(q,"Please enter valid array elements\n");
				}
				else
					fprintf(q,"Please enter a valid arraysize\n");
				break;
			case 's':
				if(i>0){
				if(fscanf(p,"%d",&k)>0){
					if(k>0 && k<=n)
						kthLargest(q,A,n,k);
					else
						fprintf(q,"Please enter k value in range of 1 to n\n");
				}
				else
					fprintf(q,"Please enter a proper k value\n");}
				else
					fprintf(q,"Please enter proper array elements to continue\n");
				break;
			case 'd':
				if(i>0){
				if(fscanf(p,"%d",&n1)>0 && n1>0){
				display(q,A,n1);
				}
				else
					fprintf(q,"Please a valid display size\n");
				}
				else
					fprintf(q,"Please enter proper array elements to continue\n");
				break;
		}
		ch = fgetc(p);
	}
}
int read(FILE *p,int A[],int n){
	int c=0;
	for(int i=0;i<n;i++){
		if(fscanf(p,"%d",&A[i])<0)
			c++;}
	if(c==0)
		return 1;
	else
		return -1;
}
void display(FILE *q,int A[],int n){
	for(int i=0;i<n;i++)
		fprintf(q,"%d ",A[i]);
	fprintf(q,"\n");
}
void kthLargest(FILE *q,int A[],int n,int k){
	quicksort(A,0,n-1);
	fprintf(q,"%d\n",A[n-k]);
}
void quicksort(int A[],int f,int l){
	if(f<l){
		int p = partition(A,f,l);
		quicksort(A,f,p-1);
		quicksort(A,p+1,l);
	}
}
int partition(int A[],int f,int l){
	int i=f-1,k=A[l];
	for(int j=f;j<l;j++){
		if(A[j]<k){
			i++;
			int temp = A[i];
			A[i] = A[j];
			A[j] = temp;
		}
	}
	int temp = A[i+1];
	A[i+1] = A[l];
	A[l] = temp;
	return i+1;
}
