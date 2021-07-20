#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

struct Node{
    int value;
    struct Node * left;
    struct Node * right;
};

struct Node * create(int a[],int *index,int min,int max,int key,int n);
void print(struct Node * );

int main(){
    int n;
    scanf("%d",&n);
    int a[n];
    for(int i=0;i<n;i++)
        scanf("%d",&a[i]);
    struct Node *tree = NULL;
    int index = 0;
    tree = create(a,&index,INT_MIN,INT_MAX,a[0],n);
    print(tree);
}

struct Node * create(int a[],int *index,int min,int max,int key,int n){
    if(*index>=n)
        return NULL;
    struct Node *temp=NULL;
    if(key>min && key<max){
        temp = (struct Node *)malloc(sizeof(struct Node));
        temp->value = key;
        temp->left = NULL;
        temp->right = NULL;
        *index = *index+1;
        if(*index<n){
            temp->left = create(a,index,min,key,a[*index],n);
            temp->right = create(a,index,key,max,a[*index],n);
        }
    }
    return temp;
}

void print(struct Node *tree){
    if(tree==NULL)
        printf("( ) ");
    else{
        printf("( ");
        printf("%d ",tree->value);
        print(tree->left);
        print(tree->right);
        printf(") ");
    }
}
