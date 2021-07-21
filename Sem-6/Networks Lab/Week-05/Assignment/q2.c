#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<stdbool.h>

struct Edge{
    int src,dest,weights;
};

struct Graph{
    int V,E;
    struct Edge *edges;
};

int dijkstra(int n,int routers[][n],int routers_pred[][n],struct Graph * g,int s){
    int e = g->E;
    bool flag[n];
    memset(flag,0,sizeof(flag));
    for(int i=0;i<n;i++){
        routers[s][i] = INT_MAX/3;
        routers_pred[s][i] = s;
    }
    routers[s][s] = 0;

    for(int i=0;i<n-1;i++){
        int mini = INT_MAX, min_node;
        for(int j=0;j<n;j++){
            if(flag[j] == 0 && routers[s][j] < mini){
                mini = routers[s][j];
                min_node = j;
            }
        }
        flag[min_node] = 1;
        for(int j=0;j<e;j++){
            int a = g->edges[j].src,b = g->edges[j].dest,w = g->edges[j].weights;
            if(a == min_node){
                if(flag[b] == 0 && routers[s][b] > w + routers[s][a]){
                    routers[s][b] = w + routers[s][a];
                    routers_pred[s][b] = a;
                }
            }
            else if(b == min_node){
                if(flag[a] == 0 && routers[s][a] > w + routers[s][b]){
                    routers[s][a] = w + routers[s][b];
                    routers_pred[s][a] = b;
                }
            }
        }
    }
}
void path(int n,int routers_pred[][n],int a,int b){
    if(b != a){
        path(n,routers_pred,a,routers_pred[a][b]);
        printf(" -> %d",b);
    }
    else
        printf("%d",b);
}
int main(){
    int n,m;
    scanf("%d %d",&n,&m);

    struct Graph * graph;
    graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->V = n;
    graph->E = m;
    graph->edges = malloc(m*sizeof(struct Edge));

    int a,b,w;
    for(int i=0;i<m;i++){
        scanf("%d %d %d",&a,&b,&w);
        graph->edges[i].src = a;
        graph->edges[i].dest = b;
        graph->edges[i].weights = w;
    }
    /*for(int i=0;i<m;i++)
        printf("%d %d %d\n",graph->edges[i].src,graph->edges[i].dest,graph->edges[i].weights);
    */
    int routers[n][n],routers_pred[n][n];
    for(int i=0;i<n;i++)
        dijkstra(n,routers,routers_pred,graph,i);
    /*
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++)
            printf("%d ",routers[i][j]);
        printf("\n");
    }
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++)
            printf("%d ",routers_pred[i][j]);
        printf("\n");
    }
    */
    printf("\nRouting tables using Link state algorithm\n\n");
    for(int i=0;i<n;i++){
        printf("For Router %d \n",i);
        printf("\tNode\tDistance\tPredecessor\tPath\n");
        for(int j=0;j<n;j++){
            printf("\t%-10d%-10d \t%-10d\t", j, routers[i][j], routers_pred[i][j]);
            /*printf("\t %d\t  %d\t\t    %d\t\t ", j, routers[i][j], routers_pred[i][j]);*/
            path(n,routers_pred,i,j);
            printf("\n");
        }
        printf("\n");
    }
    return 0;
}
