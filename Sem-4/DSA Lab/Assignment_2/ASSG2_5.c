#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

struct Node{
    int value;
    struct Node *left;
    struct Node *right;
};

void create(struct Node **);
int height(struct Node *,int *);
int longest_path(struct Node *);

int main(){
    struct Node *tree = NULL;
    create(&tree);
    printf("%d\n",longest_path(tree)-1);
}

void create(struct Node ** tree){
    int n;
    char ch;
    scanf("%c",&ch);
    while(ch ==' ' || ch == ')')
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

/*int height(struct Node *tree){
    if(tree == NULL)
        return 0;
    int left = height(tree->left);
    int right = height(tree->right);
    if(left<right)
        return right+1;
    else
        return left+1;
}
int longest_path(struct Node *tree){
    if(tree==NULL)
        return 0;
    int left_height = height(tree->left);
    int right_height = height(tree->right);
    int left_path = longest_path(tree->left);
    int right_path = longest_path(tree->right);
    int res1 = left_height+right_height+1;
    int res2 = left_path>right_path?left_path:right_path;
    return res1>res2?res1:res2;
}
*/

int height(struct Node * tree, int *ans){
    if(tree==NULL)
        return 0;
    int left = height(tree->left,ans);
    int right = height(tree->right,ans);
    if((*ans)<1+left+right)
        (*ans) = 1+left+right;
    return 1+(left>right?left:right);
}

int longest_path(struct Node *tree){
    if(tree==NULL)
        return 0;
    int ans = INT_MIN;
    int h = height(tree,&ans);
    printf("%d ",h-1);
    return ans;
}
