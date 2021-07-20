#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

struct Node{
    int value;
    struct Node *left;
    struct Node *right;
};

int largestsubbst(struct Node *,int *,int * ,int *,int *);
int maxSubBST(struct Node *);
void create(struct Node **);
void print(struct Node *);

int main(){
    struct Node *tree = NULL;
    create(&tree);
    printf("%d\n",maxSubBST(tree));
}

int maxSubBST(struct Node *tree){
    int min = INT_MAX;
    int max = INT_MIN;
    int max_size = 0;
    int is_bst = 0;
    largestsubbst(tree,&min,&max,&max_size,&is_bst);
    return max_size;
}

int largestsubbst(struct Node *tree,int *mini,int *maxi,int *max_size,int *is_bst){
    if(tree==NULL){
        *is_bst=1;
        return 0;
    }
    int min = INT_MAX;
    int l_flag = 0,r_flag = 0;
    *maxi = INT_MIN;
    int left_side,right_side;
    left_side = largestsubbst(tree->left,mini,maxi,max_size,is_bst);
    if(*is_bst==1 && tree->value>*maxi)
        l_flag = 1;
    min = *mini;
    *mini = INT_MAX;
    right_side = largestsubbst(tree->right,mini,maxi,max_size,is_bst);
    if(*is_bst==1 && tree->value<*mini)
        r_flag = 1;
    if(min<*mini)
        *mini = min;
    if(tree->value<*mini)
        *mini = tree->value;
    if(tree->value>*maxi)
        *maxi = tree->value;
    if(l_flag && r_flag){
        if(left_side+right_side+1>*max_size)
            *max_size = left_side+right_side+1;
        return left_side+right_side+1;
    }
    else{
        *is_bst =0;
        return 0;
    }
}

void create(struct Node ** tree){
    char ch;
    int n;
    scanf("%c",&ch);
    while(ch==' ' || ch == ')')
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
