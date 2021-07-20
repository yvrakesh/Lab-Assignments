#include<stdio.h>
#include<stdlib.h>
int read(FILE *p,int X[],int n);
void kthSmallest(FILE *q,int X[],int n,int k);
void max_Heapify(int X[],int i,int n);
void display(FILE *q,int X[],int n);
int main(int argc,char *argv[]){
	FILE *p,*q;
	p = fopen((char *)argv[1],"r");
	q = fopen((char *)argv[2],"w");
	char ch;
	ch = fgetc(p);
	int n,n1,k,*X,i=0;
	for(;ch!='t' && ch!=EOF;){
		switch(ch){
			case 'r':
				if(fscanf(p,"%d",&n)>0 && n>0){
				X = (int *)calloc(n,sizeof(int));
				if(read(p,X,n)!=-1)
					i++;
				else
					fprintf(q,"Please enter valid array elements\n");
				}
				else
					fprintf(q,"Please enter valid array size\n");
				break;
			case 's':
				if(i>0){
				if(fscanf(p,"%d",&k)>0){
					if(k>0 && k<=n)
					kthSmallest(q,X,n,k);
					else
						fprintf(q,"Please enter k value in the range 1 to n\n");
				}
				  else
					fprintf(q,"Please enter valid search element\n");
				}
				else
					fprintf(q,"Please enter proper array details for further proceedance\n");
				break;
			case 'd':
				if(i>0){
				if(fscanf(p,"%d",&n1)>0 && n1>0)
					display(q,X,n1);
				else
					fprintf(q,"Please enter proper display size\n");
				}
				else
					fprintf(q,"Please enter proper array details for further proceedence\n");
				break;
		}
		ch = fgetc(p);
	}
}
int read(FILE *p,int X[],int n){
	int c=0;
	for(int i=0;i<n;i++){
		if(fscanf(p,"%d",&X[i])>0)
			c++;
	}
	if(c==n)
		return 1;
	else
		return -1;
}
void kthSmallest(FILE *q,int X[],int n,int k){
	int temp,j=0;
	for(int i=n/2;i>=0;i--)
		max_Heapify(X,i,n);
		for(int i=n-1;i>0;i--){
			if(i+1 == k){
				fprintf(q,"%d\n",X[0]);
				break;}
			temp = X[i];
			X[i] = X[0];
			X[0] = temp;
			max_Heapify(X,0,i);
		}
}
void max_Heapify(int X[],int i,int n){
	int l=2*i+1;
	int r=2*i+2;
	int lar = i;
	if(l<n && X[l]>X[lar])
		lar = l;
	if(r<n && X[r]>X[lar])
		lar = r;
	if(lar!=i){
		int temp = X[i];
		X[i] = X[lar];
		X[lar] = temp;
		max_Heapify(X,lar,n);
	}
}
void display(FILE *q,int X[],int n){
	for(int i=0;i<n;i++)
		fprintf(q,"%d ",X[i]);
	fprintf(q,"\n");
}
