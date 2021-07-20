#include<stdio.h>
#include<stdlib.h>
struct Q_Node{
    int data;
    struct Q_Node * next;
};
struct queue{
    struct Q_Node *front,*rear;
};
void print(struct queue * Q,int n,FILE *fp2);
struct Q_Node* newNode(int k);
struct queue* createQueue();
void enqueue(struct queue* Q, FILE *fp1);
struct Q_Node* dequeue(struct queue* Q);
int main(int argc,char * argv[]){
    FILE *fp1,*fp2;
    fp1 = fopen((char *)argv[1],"r");
    fp2 = fopen((char *)argv[2],"w");
    char ch;
    struct queue * Q = createQueue();
    struct Q_Node * temp;
    int n;
    while(1){
            ch = fgetc(fp1);
        if(ch == 'e')
            enqueue(Q,fp1);
        else if(ch == 'd'){
            temp = dequeue(Q);
            if(temp!=NULL)
            fprintf(fp2,"%d\n",temp->data);
            else
                fprintf(fp2,"-1\n");
        }
        else if(ch == 'p'){
            fscanf(fp1,"%d",&n);
            print(Q,n,fp2);
        }
        else if(ch == 's')
            break;
    }
}
void print(struct queue * Q,int n,FILE *fp2){
    int i=0;
    struct Q_Node *temp;
    temp = Q->front;
     if(temp == NULL)
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
                if(temp == NULL){
                    fprintf(fp2,"END\n");
                    return;
                }
                else{
                    fprintf(fp2,"%d ",temp->data);
                    temp = temp->next;
                }
            }
            fprintf(fp2,"\n");
        }
    }
}
struct Q_Node* newNode(int k)
{
    struct Q_Node* temp = (struct Q_Node*)malloc(sizeof(struct Q_Node));
    temp->data = k;
    temp->next = NULL;
    return temp;
}
struct queue* createQueue()
{
    struct queue* Q = (struct queue*)malloc(sizeof(struct queue));
    Q->front = Q->rear = NULL;
    return Q;
}
void enqueue(struct queue* Q, FILE *fp1)
{
    int k;
    fscanf(fp1,"%d",&k);
    struct Q_Node* temp = newNode(k);
    if (Q->rear == NULL) {
        Q->front = Q->rear = temp;
        return;
    }
    Q->rear->next = temp;
    Q->rear = temp;
}
struct Q_Node* dequeue(struct queue* Q)
{
    if (Q->front == NULL)
        return NULL;
    struct Q_Node* temp = Q->front;
    Q->front = Q->front->next;
    if (Q->front == NULL)
        Q->rear = NULL;
    return temp;
}
