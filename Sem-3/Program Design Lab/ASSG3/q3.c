#include<stdio.h>
#include<stdlib.h>
struct Node {
    int data;
    struct Node * prev;
    struct Node * next;
};
void create(struct Node ** A,FILE *fp1);
void print(struct Node * A,FILE *fp2);
int remove_duplicate(struct Node ** A);
int del_Node(struct Node ** A,struct Node *del);
int main(int argc,char * argv[]){
    FILE *fp1,*fp2;
    fp1 = fopen((char *)argv[1],"r");
    fp2 = fopen((char *)argv[2],"w");
    char ch;
    struct Node * A=NULL;
    while(1){
            ch = fgetc(fp1);
        if(ch == 'c')
            create(&A,fp1);
        else if(ch == 'p')
            print(A,fp2);
        else if(ch == 'r'){
            remove_duplicate(&A);
        }
        else if(ch == 's')
            break;
    }
}
void print(struct Node * A,FILE *fp2){
    struct Node * temp;
    temp = A;
    while(temp!=NULL){
        fprintf(fp2,"%d ",temp->data);
        temp = temp->next;
    }
    fprintf(fp2,"\n");
}
void create(struct Node ** A,FILE *fp1){
    int element;
    struct Node * new_node;
    new_node = (struct Node *)malloc(sizeof(struct Node));
    fscanf(fp1,"%d",&element);
    new_node->data = element;
    new_node->next = NULL;
        new_node->prev = NULL;
        *A = new_node;
    while(fscanf(fp1,"%d",&element)){
        struct Node * new;
        new = (struct Node *)malloc(sizeof(struct Node));
        new->data = element;
        new->prev = new_node;
        new->next = NULL;
        new_node->next = new;
        new_node = new_node->next;
    }
}
int del_Node(struct Node ** A,struct Node * del){
    if(*A == NULL || del == NULL)
        return 0;
    if(*A == del)
        *A = del->next;
    if(del->next != NULL)
        del->next->prev = del->prev;
    if(del->prev != NULL)
        del->prev->next = del->next;
    free(del);
}
int remove_duplicate(struct Node ** A){
    struct Node *temp;
    temp = *A;
    if(temp==NULL || temp->next == NULL)
        return 0;
    struct Node *p1,*p2;
    for(p1=*A;p1!=NULL;p1=p1->next){
        for(p2 = p1->next;p2!=NULL;){
            if(p1->data==p2->data){
                temp = p2->next;
                del_Node(A,p2);
                p2 = temp;
            }
            else
                p2 = p2->next;
        }
    }
}
