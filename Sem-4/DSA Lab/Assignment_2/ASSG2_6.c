#include<stdio.h>
#include<stdlib.h>

struct Node {
    int value;
    struct Node *left;
    struct Node *right;
};

struct Node * mod_tree(struct Node * temp);
void create(struct Node ** tree);
int value(struct Node *temp);
void print(struct Node *temp);

int main(){
    struct Node *tree = NULL;
    create(&tree);
    struct Node *temp = NULL;
    temp = mod_tree(tree);
    print(temp);
    printf("\n");
}

void create(struct Node ** tree){
    char ch;
    int n;
    scanf("%c",&ch);
    while(ch == ' ' || ch == ')')
        scanf("%c",&ch);
    if(scanf("%d",&n)==1){
        struct Node *temp = NULL;
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

struct Node * mod_tree(struct Node * temp){
    if(temp!=NULL){
        int n = value(temp);
        temp->value = n;
        mod_tree(temp->left);
        mod_tree(temp->right);
    }
    return temp;
}

int value(struct Node *temp){
    if(temp==NULL)
        return 0;
    return temp->value+value(temp->left)+value(temp->right);
}

void print(struct Node *temp){
    if(temp==NULL)
        printf("( ) ");
    else{
        printf("( ");
        printf("%d ",temp->value);
        print(temp->left);
        print(temp->right);
        printf(") ");
    }
}
