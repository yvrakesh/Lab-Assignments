#include<stdio.h>
#include<stdlib.h>
struct Node{
    int data;
    struct Node * next;
};
void push(struct Node ** stk,int element);
struct Node * pop(struct Node ** stk);
void print(struct Node * stk,int n,FILE *fp2);
struct Node * newNode(int k);
int main(int argc,char * argv[]){
    FILE *fp1,*fp2;
    fp1 = fopen((char *)argv[1],"r");
    fp2 = fopen((char *)argv[2],"w");
    struct Node * stk = NULL;
    char ch;
    int n;
    while(1){
        ch = fgetc(fp1);
        if(ch == 'p'){
            fscanf(fp1,"%d",&n);
            push(&stk,n);
        }
        else if(ch == 't'){
            struct Node *temp;
            temp = pop(&stk);
                if(temp!=NULL)
                fprintf(fp2,"%d\n",temp->data);
                else
                    fprintf(fp2,"-1\n");
        }
        else if(ch == 'd'){
            fscanf(fp1,"%d",&n);
            print(stk,n,fp2);
        }
        else if(ch == 's')
            break;
    }
}
void push(struct Node ** stk,int element){
            struct Node * temp1;
            temp1 = (struct Node *)malloc(sizeof(struct Node));
            temp1->data = element;
            temp1->next = *stk;
            *stk = temp1;
}
struct Node * pop(struct Node ** stk){
        struct Node * temp;
            temp = *stk;
        if(temp==NULL)
            return NULL;
        else{
                struct Node * t;
                t = temp;
            temp = temp->next;
                *stk = temp;
            return t;
        }
}
void print(struct Node * stk,int n,FILE *fp2){
     struct Node * temp;
        temp = stk;
    if(stk==NULL)
        fprintf(fp2,"-1\n");
    else{
    if(n==0){
        while(temp!=NULL){
            fprintf(fp2,"%d ",temp->data);
            temp = temp->next;
        }
        fprintf(fp2,"\n");
    }
    else if(n>0){
                for(int i=0;i<n;i++){
                    if(temp==NULL){
                        fprintf(fp2,"END\n");
                        return;
                    }
                    fprintf(fp2,"%d ",temp->data);
                    temp = temp->next;
            }
            fprintf(fp2,"\n");
    }
}
}
