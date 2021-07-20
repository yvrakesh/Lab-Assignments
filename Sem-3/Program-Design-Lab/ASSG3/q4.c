#include<stdio.h>
#include<stdlib.h>
struct Node{
    int data;
    struct Node * next;
};
void create(struct Node ** L, FILE *fp1);
void print(struct Node * L,FILE *fp2);
void swap_pairwise(struct Node ** L);
struct Node * newNode(int k);
int main(int argc,char * argv[]){
    FILE *fp1,*fp2;
    fp1 = fopen((char *)argv[1],"r");
    fp2 = fopen((char *)argv[2],"w");
    char ch;
    struct Node * L = NULL;
    while(1){
        ch = fgetc(fp1);
        if(ch == 'c')
            create(&L,fp1);
        else if(ch == 'p')
            print(L,fp2);
        else if(ch == 'w')
            swap_pairwise(&L);
        else if(ch == 's')
            break;
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
void swap_pairwise(struct Node ** L){
     struct Node * first = *L;
     struct Node * second = *L;
     for(;second!=NULL && second->next!=NULL;){
        second = second->next;
       /* int *a,*b;
        a = &first->data;
        b = &second->data;
        int swap;
        swap = *a;
        *a = *b;
        *b = swap;*/
        int swap;
        swap = first->data;
        first->data = second->data;
        second->data = swap;
        first = first->next->next;
        second = second->next;
     }
}
struct Node * newNode(int k){
        struct Node * newnode;
        newnode = (struct Node*)malloc(sizeof(struct Node));
        newnode->data = k;
        newnode->next = NULL;
        return newnode;
}

