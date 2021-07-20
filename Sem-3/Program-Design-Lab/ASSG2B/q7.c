#include<stdio.h>
#include<stdlib.h>
int read(FILE *p,int A[],int n);
void bucketSort(FILE *q,int A[],int n);
void display(FILE *q,int A[],int n);
void mergeSort(int B[],int f,int l);
void merge(int B[],int f,int mid,int l);
int main(int argc,char *argv[]){
	FILE *p,*q;
	p = fopen((char *)argv[1],"r");
	q = fopen((char *)argv[2],"w");
	char ch;
	ch = fgetc(p);
	int *A;
	int n,n1,i=0;
	for(;ch!='t' && ch!=EOF;){
		switch(ch){
			case 'r':
				if(fscanf(p,"%d",&n)>0 && n>0){
				if(read(p,A,n)!=-1)
					i++;
				else
					fprintf(q,"Please enter valid array elements\n");
				}
				else
					fprintf(q,"Please enter a valid element size\n");
				break;
			case 's':
				if(i>0)
					bucketSort(q,A,n);
				else
					fprintf(q,"Please enter proper array elements for further processing\n");
				break;
			case 'd':
				if(i>0){
				if(fscanf(p,"%d",&n1)>0 && n1>0)
					display(q,A,n1);
				else
					fprintf(q,"Please enter a valid display size\n");
				}
				else
					fprintf(q,"Please enter proper array details for further processes\n");
				break;
		}
		ch =fgetc(p);
	}
}
int read(FILE *p,int A[],int n){
	int c=0;
	for(int i=0;i<n;i++){
		if(fscanf(p,"%d",&A[i])>0)
			c++;}
	if(c==n)
		return 1;
	else
		return -1;
}
void bucketSort(FILE *q,int A[],int n){
	int *count;
	count = (int *)calloc(8,sizeof(int));
	int c;
	for(int i=0;i<n;i++){
		c=0;
		int temp = A[i];
		for(;temp>0;temp/=2){
			if(temp%2==1)
				c++;
		}
		count[c]++;
	}
	int B[n],t=0;
	for(int i=0;i<8;i++){
		if(count[i]!=0){
			for(int j=0;j<n;j++){
				int temp=A[j],c=0;
				for(;temp>0;temp/=2){
					if(temp%2==1)
						c++;
				}
				if(i==c)
					B[t++] = A[j];
			}
		}
	}
	for(int i=1;i<8;i++)
		count[i] += count[i-1];
	for(int i=0;i<7;i++){
		if(count[i+1] != count[i])	
		mergeSort(B,count[i],count[i+1]-1);}
	for(int i=0;i<7;i++){
		for(int j=count[i];j<count[i+1];j++)
			fprintf(q,"%d ",B[j]);
		if(count[i+1]!= count[i])
			fprintf(q,"\n");
	}
}
void display(FILE *q,int A[],int n){
	for(int i=0;i<n;i++)
		fprintf(q,"%d ",A[i]);
	fprintf(q,"\n");
}
void mergeSort(int B[],int f,int l){
	if(f<l){
	int mid = f+(l-f)/2;
	mergeSort(B,f,mid);
	mergeSort(B,mid+1,l);
	merge(B,f,mid,l);
	}
}
void merge(int B[],int f,int mid,int l){
	int n1 = mid-f+1;
	int n2 = l-mid;
	int L[n1],R[n2];
	for(int i=0;i<n1;i++)
		L[i] = B[f+i];
	for(int i=0;i<n2;i++)
		R[i] = B[mid+i+1];
	int i=0;
	int j=0;
	int k=f;
	while(i<n1 && j<n2){
		if(L[i]<=R[j]){
			B[k] = L[i];
			i++;
		}
		else{
			B[k] = R[j];
			j++;
		}
		k++;
	}
	while(i<n1){
		B[k] = L[i];
		i++;
		k++;
	}
	while(j<n2){
		B[k] = R[j];
		j++;
		k++;
	}
}	
