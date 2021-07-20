#include<stdio.h>
#include<stdlib.h>

struct Node {
    int value;
    struct Node * left;
    struct Node * right;
};

int strc(char [],char []);
int level(struct Node *tree,int n,int level);
void insert(struct Node **,int );
void successor(struct Node *,int );
void predecessor(struct Node *,int );
struct Node *search(struct Node *,int );
struct Node *delete(struct Node *,int );
void preorder(struct Node *);
void inorder(struct Node *);
void postorder(struct Node *);
void findMin(struct Node *);
void findMax(struct Node *);

int main(){
    char ch[6];
    scanf("%s",ch);
    int n,k;
    struct Node *tree = NULL;
    for(;;){
        if(!strc(ch,"insr")){
            scanf("%d",&n);
            insert(&tree,n);
        }
        else if(!strc(ch,"srch")){
            scanf("%d",&k);
            if(search(tree,k)!=NULL)
                printf("%d\n",level(tree,k,0));
            else
                printf("-1\n");
        }
        else if(!strc(ch,"minm"))
            findMin(tree);
        else if(!strc(ch,"maxm"))
            findMax(tree);
        else if(!strc(ch,"pred")){
            scanf("%d",&k);
            predecessor(tree,k);
        }
        else if(!strc(ch,"succ")){
            scanf("%d",&k);
            successor(tree,k);
        }
        else if(!strc(ch,"delt")){
            scanf("%d",&k);
            if(delete(tree,k)==NULL)
                printf("NULL\n");
        }
        else if(!strc(ch,"inor")){
            inorder(tree);
            printf("\n");
        }
        else if(!strc(ch,"prer")){
            preorder(tree);
            printf("\n");
        }
        else if(!strc(ch,"post")){
            postorder(tree);
            printf("\n");
        }
        else if(!strc(ch,"stop"))
            break;

        scanf("%s",ch);
    }
return 0;

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

void insert(struct Node ** tree,int n){

    struct Node *new;
    new = (struct Node *)malloc(sizeof(struct Node));
    new->value = n;
    new->left = NULL;
    new->right = NULL;
    if(*tree == NULL)
        *tree = new;
    else{
        struct Node *temp;
        temp = *tree;
        int flag = 0;
        while(1){
            if(temp->value>=n){
                if(temp->left!=NULL)
                    temp = temp->left;
                else
                    flag = 1;
            }
            else{
                if(temp->right!=NULL)
                    temp = temp->right;
                else
                    flag = 2;
            }
            if(flag)
                break;
        }
        if(flag == 1)
            temp->left = new;
        else
            temp->right = new;
    }
}

void findMin(struct Node *tree){
    if(tree!=NULL){
        while(tree->left!=NULL)
            tree = tree->left;
        printf("%d\n",tree->value);
    }
    else
        printf("NIL\n");
}

void findMax(struct Node *tree){
    if(tree!=NULL){
        while(tree->right!=NULL)
            tree = tree->right;
        printf("%d\n",tree->value);
    }
    else
        printf("NIL\n");
}

void inorder(struct Node *tree){
    if(tree!=NULL){
        inorder(tree->left);
        printf("%d ",tree->value);
        inorder(tree->right);
    }
}

void preorder(struct Node *tree){
    if(tree!=NULL){
        printf("%d ",tree->value);
        preorder(tree->left);
        preorder(tree->right);
    }
}

void postorder(struct Node *tree){
    if(tree!=NULL){
        postorder(tree->left);
        postorder(tree->right);
        printf("%d ",tree->value);
    }
}

int level(struct Node *tree,int n,int l){
    if(tree==NULL)
        return 0;
    if(tree->value==n)
        return l;
    int lowerlevel = level(tree->left,n,l+1);
    if(lowerlevel!=0)
        return lowerlevel;
    lowerlevel = level(tree->right,n,l+1);
        return lowerlevel;
}

struct Node * search(struct Node * tree,int n){
    while(tree!=NULL){
        if(tree->value>n)
            tree = tree->left;
        else if(tree->value<n)
            tree = tree->right;
        else
            break;
    }
    return tree;
}

struct Node *delete(struct Node *tree,int n){
    if(tree==NULL){
        return NULL;
    }
    if(tree->value>n)
        tree->left = delete(tree->left,n);
    else if(tree->value<n)
        tree->right = delete(tree->right,n);
    else{
        if(tree->left==NULL && tree->right==NULL)
            tree = NULL;
        else if(tree->left == NULL)
            tree = tree->right;
        else if(tree->right == NULL)
            tree = tree->left;
        else{
            struct Node *temp;
            temp = tree->right;
            while(temp->left!=NULL)
                temp = temp->left;
            tree->value = temp->value;
            tree->right = delete(tree->right,temp->value);
        }
    }
    return tree;
}

void predecessor(struct Node *tree,int n){
    struct Node *temp;
    temp = search(tree,n);
    if(temp==NULL)
        printf("NULL\n");
    else if(temp->left!=NULL){
        struct Node *temp1;
        temp1 = temp->left;
        while(temp1->right!=NULL)
            temp1 = temp1->right;
        printf("%d\n",temp1->value);
    }
    else{
        struct Node *temp1;
        temp1 = tree;
        while(temp1->left!=NULL)
            temp1 =temp1->left;
        if(temp1->value==n){
            printf("-1\n");
            return;
        }
        int x;
        temp1 = tree;
        while(temp1->value!=n){
            if(n>temp1->value){
                x = temp1->value;
                temp1 = temp1->right;
            }
            else
                temp1 = temp1->left;
        }
        printf("%d\n",x);
    }
}

void successor(struct Node *tree,int n){
    struct Node *temp;
    temp = search(tree,n);
    if(temp==NULL)
        printf("NULL\n");
    else if(temp->right!=NULL){
        struct Node *temp1;
        temp1 = temp->right;
        while(temp1->left!=NULL)
            temp1 = temp1->left;
        printf("%d\n",temp1->value);
    }
    else{
        struct Node *temp1;
        temp1 = tree;
        while(temp1->right!=NULL)
            temp1 = temp1->right;
        if(temp1->value == n)
            printf("-1\n");
        else{
            int x;
            temp1 = tree;
            while(temp1->value!=n){
                if(n<temp1->value){
                    x = temp1->value;
                    temp1 = temp1->left;
                }
                else
                    temp1 = temp1->right;
            }
            printf("%d\n",x);
        }
    }
}
