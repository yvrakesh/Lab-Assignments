#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<stdbool.h>
void pred_calc(int n,int routers_next[][n],int a,int b){
    int path[n],i = a,count = 0;
    int next = routers_next[a][b];
    path[count] = a;
    while(next != b){
        count++;
        i = next;
        next = routers_next[i][b];
        path[count] = i;
    }
    printf("%d\t",i);
    for(int j=0;j<=count;j++)
        if(path[j] != b)
            printf("%d -> ",path[j]);
    printf("%d\n",b);
}
int main(){
    int n,m;
    scanf("%d %d",&n,&m);
    int w[n][n],routers[n][n],routers_next[n][n];
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            w[i][j] = INT_MAX/3;
            routers[i][j] = INT_MAX/3;
            routers_next[i][j] = j;
        }
    }
    for(int i=0;i<n;i++){
        w[i][i] = 0;
        routers[i][i] = 0;
        routers_next[i][i] = i;
    }
    for(int i=0;i<m;i++){
        int a,b,r;
        scanf("%d %d %d",&a,&b,&r);
        w[a][b] = w[b][a] = routers[a][b] = routers[b][a] = r;
    }
    /*
    printf("The cost matrix is \n");
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++)
            printf("%d ",w[i][j]);
        printf("\n");
    }
    printf("The router distances is \n");
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++)
            printf("%d ",routers[i][j]);
        printf("\n");
    }
    */
    bool flag;
    for(;;){
        flag = 1;
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                for(int k=0;k<n;k++){
                    if(routers[i][j] == INT_MAX/3 && w[i][k] == INT_MAX/3)
                        continue;
                    if(routers[i][j] > w[i][k]+routers[k][j]){
                        routers[i][j] = w[i][k]+routers[k][j];
                        routers_next[i][j] = k;
                        flag = 0;
                    }
                }
            }
        }
        if(flag)
            break;
    }
    /*
    printf("The router distances is \n");
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++)
            printf("%d ",routers[i][j]);
        printf("\n");
    }
    */
    printf("\nRouting tables using Direct Vector Router Algorithm\n\n");
    for(int i=0;i<n;i++){
        printf("For Router %d \n",i);
        printf("\tNode\tDistance\tNext\tPred\tPath\n");
        for(int j=0;j<n;j++){
            printf("\t%d\t%-10d\t%-10d", j, routers[i][j], routers_next[i][j]);
            pred_calc(n,routers_next,i,j);
        }
        printf("\n");
    }
    return 0;
}
