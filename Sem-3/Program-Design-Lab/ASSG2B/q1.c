#include<stdio.h>
#include<stdlib.h>
int read(FILE *p,int X[],int n);
void display(FILE *q,int X[],int n);
void inversion(FILE *q,int X[],int n);
int divide(int X[],int temp[],int f,int l);
int merge(int X[],int temp[],int f,int mid,int l);
int mergeSort(int X[],int f,int l);
int main(int argc,char * argv[])
{
	FILE *p,*q;
	int *X;
	p = fopen((char *)argv[1],"r");
	q = fopen((char *)argv[2],"w");
	char ch;
 	ch = fgetc(p);
	int n,n1,i=0;
	for(;ch!='t' && ch!=EOF;){
		switch(ch){
			case 'r':
				if(fscanf(p,"%d",&n)>0){
				X = (int *)calloc(n,sizeof(int));
				if(read(p,X,n)!=-1)
					i++;
				else
					fprintf(q,"Please enter valid array elements\n"); }
				else
					fprintf(q,"Please enter a valid array size\n");
				break;
			case 'i':
				if(i>0)
					inversion(q,X,n);
				else
					fprintf(q,"Please enter proper array elements for further proceeding\n");
				break;
			case 'd':
				if(i>0){
				if(fscanf(p,"%d",&n1)>0)
					display(q,X,n1);
				else
					fprintf(q,"Please enter valid display size\n");}
				else
					fprintf(q,"Please enter proper array elements for further proceeding\n");
				break;
		}
		ch = fgetc(p);
	}
	return 0;
}
int read(FILE *p,int X[],int n){
	int x=0;
	for(int i=0;i<n;i++){
		if(fscanf(p,"%d",&X[i])>0)
			x++;}
	if(x==n)
		return 1;
	else
		 return -1;
}
void display(FILE *q,int X[],int n){
	for(int i=0;i<n;i++)
		fprintf(q,"%d ",X[i]);
	fprintf(q,"\n");
}
void inversion(FILE *q,int X[],int n){
	fprintf(q,"%d\n",mergeSort(X,0,n-1));
}
int divide(int X[],int temp[],int f,int l){
	int r=0;
	if(f<l){
		int mid = f+(l-f)/2;
		r = divide(X,temp,f,mid);
		r += divide(X,temp,mid+1,l);
		r += merge(X,temp,f,mid,l);
	}
		return r;
}
int mergeSort(int X[],int f,int l){
	int *temp = (int *)malloc((l+1)*sizeof(int));
	return divide(X,temp,0,l);
}
int merge(int X[],int temp[],int f,int mid,int l){
	int i,j,k,r=0;
	i = f;
	j = mid+1;
	k = f;
	while(i<=mid && j<=l){
		if(X[i]<=X[j])
			temp[k++] = X[i++];
		else{
			temp[k++] = X[j++];
			r += mid+1-i;
		}
	}
	while(i<=mid)
		temp[k++] = X[i++];
	while(j<=l)
		temp[k++] = X[j++];
	for(int i=f;i<=l;i++)
		X[i] = temp[i];
	return r;
}
