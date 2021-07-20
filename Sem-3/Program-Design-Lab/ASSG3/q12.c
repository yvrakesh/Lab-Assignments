#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct Node{
    int top;
    int size;
    int *arr;
};

struct Node *create(int size){
    struct Node *stk = (struct Node*)malloc(sizeof(struct Node));
    stk->top = -1;
    stk->size = size;
    stk->arr = (int*)malloc(stk->size * sizeof(int));
    if(!stk->arr)
        return NULL;
    return stk;
}

void push(struct Node* stk,int element){
    stk->arr[++stk->top] = element;
}

char pop(struct Node* stk){
    if(stk->top != -1)
        return stk->arr[stk->top--];
}

int evaluatePostfix(char *exp){
    struct Node *stk = create(strlen(exp));
    for(int i = 0;exp[i]; i++){
        if('0'<=exp[i] && exp[i]<='9'){
            int num = 0;
            while('0'<=exp[i] && exp[i]<='9'){
                int digit = exp[i] - '0';
                num = num*10 + digit;
                i++;
            }
            push(stk,num);
        }
        else if(exp[i]=='-' && '0'<=exp[i+1] && exp[i+1]<='9'){
            i++;
            int num = 0;
            while('0'<=exp[i] && exp[i]<='9'){
                int digit = exp[i] - '0';
                num = num*10 + digit;
                i++;
            }
            num *= -1;
            push(stk,num);
        }

        else if(exp[i]==' ')
            continue;

        else{
            int n1 = stk->arr[stk->top];
            pop(stk);
            int n2 = stk->arr[stk->top];
            pop(stk);
            if(exp[i]=='+')
                push(stk,n2+n1);
            else if(exp[i]=='-')
                push(stk,n2-n1);
            else if(exp[i]=='*')
                push(stk,n2*n1);
            else if(exp[i]=='/')
                push(stk,n2/n1);
        }
    }
    return stk->arr[stk->top];
}
int main(int argc,char * argv[])
{
    FILE* fp1 = fopen((char *)argv[1],"r");
    FILE* fp2 = fopen((char *)argv[2],"w");
    char opt,temp;
    char str[100];
    while(1){
        fscanf(fp1,"%c",&opt);
        if(opt=='e'){
            fscanf(fp1,"%c",&temp);
            fscanf(fp1,"%[^\n]s",&str);
            fprintf(fp2,"%d\n",evaluatePostfix(str));
        }
        else if(opt=='s')
            break;
    }
    return 0;
}
