#include<stdio.h>
#include<stdlib.h>

struct Node {
    int value;
    struct Node *left;
    struct Node *right;
};

void create(struct Node ** );
void print(struct Node *);
void mirror(struct Node **);

int main(){
    struct Node *tree = NULL;
    create(&tree);
    //print(tree);
    mirror(&tree);
    print(tree);
    printf("\n");
}

void create(struct Node ** tree){
    char ch;
    scanf("%c",&ch);
    while(ch==' ' || ch == ')')
        scanf("%c",&ch);
    int n;
    if(scanf("%d",&n)==1){
            struct Node *temp;
            temp = (struct Node *)malloc(sizeof(struct Node));
            temp->value = n;
            temp->left = NULL;
            temp->right = NULL;
            create(&(temp->left));
            create(&(temp->right));
            *tree = temp;
        }
        else
            *tree = NULL;
}

void mirror(struct Node ** tree){
    if(*tree!=NULL){
    struct Node *temp;
    temp = (*tree)->left;
    (*tree)->left = (*tree)->right;
    (*tree)->right = temp;
    mirror(&((*tree)->left));
    mirror(&((*tree)->right));
    }
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
