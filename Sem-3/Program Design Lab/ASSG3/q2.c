#include<stdio.h>
#include<stdlib.h>
struct Node{
    int data;
    struct Node * next;
};
void create(struct Node ** L, FILE *fp1);
void print(struct Node * L,FILE *fp2);
int find_knode(struct Node * L,int k);
struct Node * newNode(int k);
int main(int argc,char * argv[]){
    FILE *fp1,*fp2;
    fp1 = fopen((char *)argv[1],"r");
    fp2 = fopen((char *)argv[2],"w");
    struct Node * L;
    char ch;
    int k;
    while(1){
        ch = fgetc(fp1);
        if(ch == 'c')
            create(&L,fp1);
        if(ch == 'p')
            print(L,fp2);
        if(ch == 'f'){
            fscanf(fp1,"%d",&k);
            fprintf(fp2,"%d",find_knode(L,k));
        }
        if(ch == 's')
            break;
    }
}
void create(struct Node ** L,FILE *fp1){
    int num;
    fscanf(fp1,"%d",&num);
    struct Node *temp;
    temp = newNode(num);
    *L = temp;
    while(fscanf(fp1,"%d",&num)){
             struct Node* new;
        new = (struct Node *)malloc(sizeof(struct Node));
        new->data = num;
        new->next = NULL;
        temp->next = new;
        temp = temp->next;
    }
}
void print(struct Node * L,FILE *fp2){
    struct Node * temp;
    temp = L;
    if(temp!=NULL){
    while(temp!=NULL){
        fprintf(fp2,"%d ",temp->data);
        temp = temp->next;
    }
    fprintf(fp2,"\n");}
    else
        fprintf(fp2,"-1\n");
}
int find_knode(struct Node * L,int k){
    struct Node * temp;
    temp = L;
    int i=0;
    while(temp!=NULL){
        temp = temp->next;
        i++;
    }
    if(i!=0 && k<=i){
    temp = L;
    for(int j=0;j<i-k;j++)
        temp = temp->next;
    return temp->data;}
        return -1;
}
struct Node * newNode(int k){
        struct Node * newnode;
        newnode = (struct Node*)malloc(sizeof(struct Node));
        newnode->data = k;
        newnode->next = NULL;
        return newnode;
}
