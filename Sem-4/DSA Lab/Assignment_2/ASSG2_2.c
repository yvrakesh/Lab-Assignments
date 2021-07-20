#include<stdio.h>
#include<stdlib.h>

struct Node{
    int value;
    struct Node *left;
    struct Node *right;
};

int insert(struct Node ** tree,int n);
void print(struct Node *tree);
int strc(char [],char []);

int main(){
    char ch[6];
    scanf("%s",ch);
    int n;
    struct Node *tree = NULL;
    for(;;){
        if(!strc(ch,"insr")){
            scanf("%d",&n);
            insert(&tree,n);
        }
        else if(!strc(ch,"prep")){
            print(tree);
            printf("\n");
        }
        else if(!strc(ch,"stop"))
            break;
        scanf("%s",ch);
    }
    return 0;
}

int insert(struct Node ** tree,int n){
    struct Node *new;
    new = (struct Node *)malloc(sizeof(struct Node));
    new->value = n;
    new->left = NULL;
    new->right = NULL;
    if(*tree==NULL)
        *tree = new;
    else{
        struct Node *temp;
        temp = *tree;
        int flag = 0;
        while(temp!=NULL){
                if(temp->value>n){
                    if(temp->left!=NULL)
                        temp = temp->left;
                    else
                        flag = 1;
                }
                else if(temp->value<n){
                    if(temp->right!=NULL)
                        temp = temp->right;
                    else
                        flag = 2;
                }
                if(flag!=0)
                    break;
        }
        if(flag==1)
            temp->left = new;
        else
            temp->right = new;
    }
}

void print(struct Node *tree){
        struct Node *temp;
        temp = tree;
        if(temp==NULL)
            printf("()");
        else{
            printf("(");
            printf("%d",temp->value);
            print(tree->left);
            print(tree->right);
            printf(")");
        }
}

int strc(char str1[],char str2[]){
    int l1 =0,i=0;
    while(str1[i++]!='\0')
        l1++;
    if(l1==4){
        for(int i=0;i<4;i++){
            if(str1[i]!=str2[i])
                return 1;
        }
    }
    else
        return 1;

    return 0;
}

