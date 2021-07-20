#include<stdio.h>
#include<stdlib.h>
struct Node{
    int data;
    struct Node * next;
};
void create(struct Node ** A, FILE *fp1);
void print(struct Node *A, FILE *fp2);
void insert_middle(struct Node ** A, int new_element);
struct Node * newNode(int k);
int main(int argc,char * argv[]){
    FILE *fp1,*fp2;
    fp1 = fopen((char *)argv[1],"r");
    fp2 = fopen((char *)argv[2],"w");
    char ch;
    int new_element;
    struct Node * A = NULL;
while(1){
            ch = fgetc(fp1);
        if(ch == 'c')
            create(&A,fp1);
        else if(ch == 'p')
            print(A,fp2);
        else if(ch == 'i'){
            fscanf(fp1,"%d",&new_element);
            insert_middle(&A, new_element);
            }
            else if(ch == 's')
                break;
        }
}
void create(struct Node ** A,FILE *fp1){
    int num;
    fscanf(fp1,"%d",&num);
    struct Node *temp;
    temp = newNode(num);
    *A = temp;
    while(fscanf(fp1,"%d",&num)){
             struct Node* new;
        new = (struct Node *)malloc(sizeof(struct Node));
        new->data = num;
        new->next = NULL;
        temp->next = new;
        temp = temp->next;
    }
}
void print(struct Node * A,FILE *fp2){
    struct Node * temp;
    temp = A;
    if(temp!=NULL){
    while(temp!=NULL){
        fprintf(fp2,"%d ",temp->data);
        temp = temp->next;
    }
    fprintf(fp2,"\n");
  }
    else
        fprintf(fp2,"-1\n");
}
struct Node * newNode(int k){
        struct Node * newnode;
        newnode = (struct Node*)malloc(sizeof(struct Node));
        newnode->data = k;
        newnode->next = NULL;
        return newnode;
}
void insert_middle(struct Node ** A,int k){
    struct Node *temp,*newnode;
    if(*A == NULL)
        *A = newNode(k);
    else{
            newnode = newNode(k);
        temp = *A;
        int i=0;
    while(temp!=NULL){
        temp = temp->next;
        i++;
        }
    if(i%2==1)
        i = (i+1)/2;
    else
        i = i/2;
    temp = *A;
        while(i-->1)
            temp = temp->next;
        newnode->next = temp->next;
        temp->next = newnode;
}
}
