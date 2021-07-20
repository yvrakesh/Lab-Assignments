#include<stdio.h>
#include<stdlib.h>

struct Node{
	int value;
	char ch;
	struct Node *left;
	struct Node *right;
};

int strl(char []);
struct Node *create(char c,int count);
struct Node * build_hoffmantree(char str[]);
void insert(struct Node **a,int *n,struct Node *temp1);
int resvalue(struct Node *tree,int height);
void min_heapify(struct Node **a,int n,int i);
struct Node * minheap(struct Node ** a, int *n);

int main(){
	char ch[1000];
	scanf("%[^\n]s",ch);
		char c;
		scanf("%c",&c);
		struct Node *tree=NULL;
		tree = build_hoffmantree(ch);
		printf("%d\n",resvalue(tree,0));
return 0;

}

struct Node * build_hoffmantree(char str[]){
	int l = strl(str);
	int frequency[256]={0};
	for(int i=0;i<l;i++)
		frequency[str[i]]++;
	int n = 0;
	for(int i=0;i<256;i++)
		if(frequency[i])
			n++;
	struct Node *a[n];
	int j=0;
	for(int i=0;i<256;i++)
		if(frequency[i])
			a[j++] = create(i,frequency[i]);
	for(int i=(n/2)-1;i>=0;i--)
		min_heapify(a,n,i);
	while(n!=1){
		struct Node *temp1;
		temp1 = minheap(a,&n);
		struct Node *temp2;
		temp2 = minheap(a,&n);
		struct Node *temp3;
		temp3 = create('@',(temp1->value)+(temp2->value));
		temp3->left = temp1;
		temp3->right = temp2;
		insert(a,&n,temp3);
	}
	return minheap(a,&n);
}

struct Node *create(char c,int count){
	struct Node *temp = NULL;
	temp = (struct Node *)malloc(sizeof(struct Node));
	temp->value = count;
	temp->ch = c;
	temp->left = NULL;
	temp->right = NULL;
	return temp;
}

void insert(struct Node **a,int *n,struct Node *temp1)
{
	a[(*n)] = temp1;
	(*n) = (*n)+1;
	int k=*n;
	while(a[k]->value > a[(k-1)/2]->value)
	{
		struct Node *temp;
		temp = a[k];
		a[k] = a[(k-1)/2];
		a[(k-1)/2] = temp;		
		k=(k-1)/2;
	}
}

int strl(char str[]){
	int l=0,i=0;
	while(str[i++]!='\0')
		l++;
return l;
}

int resvalue(struct Node *tree,int height)
{
	if(tree == NULL)
		return 0;
	else if(tree->left == NULL && tree->right == NULL)
		return (tree->value) * height;
	else
		return resvalue(tree->left,height+1) + resvalue(tree->right,height+1);
}

void min_heapify(struct Node **a,int n,int i)
{
	int smallest = i;
	int l = (2*i)+1;
	int r = (2*i)+2;
	if(l < n && a[l]->value < a[smallest]->value)
		smallest=l;
	if(r < n && a[r]->value < a[smallest]->value)
		smallest=r;
	if(smallest!=i)
	{

		struct Node *temp = NULL;
		temp = a[smallest];
		a[smallest] = a[i];
		a[i] = temp;

		min_heapify(a,n,smallest);
	}
}

struct Node * minheap(struct Node ** a, int *n){
	struct Node *min = a[0];
	a[0] = a[(*n)-1];
	(*n) = (*n) - 1;
	min_heapify(a,*n,0);
	return min;	 
}



