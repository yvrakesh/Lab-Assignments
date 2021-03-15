#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<stdbool.h>
#include<string.h>

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
    int rem_edge;
    scanf("%d",&rem_edge);
    for(int j=0;j<m;j++){
        if(graph->edges[j].src == rem_edge || graph->edges[j].dest == rem_edge)
            graph->edges[j].weights = INT_MAX/3;
    }
    int routers[n][n],routers_pred[n][n];
    dijkstra(n,routers,routers_pred,graph,0);
    if(routers[0][n-1] == INT_MAX/3)
        printf("-1\n");
    else
        printf("%d\n",routers[0][n-1]);
    return 0;
}
