#include <stdio.h>
#include <stdlib.h>

#define INF 1000000007

struct pair
{
	int x,y,z;
};
struct vector
{
    	struct pair **arr;
    	unsigned int size;
    	unsigned int capacity;
};

int sum(int ,int );
struct pair * make_pair(int ,int ,int );
struct vector * init_vector();
struct vector * push_back(struct vector * ,struct pair * );
int neg_num_detection(struct vector ** ,int ,int );

int main()
{
	int n,m;
	scanf("%d %d",&n,&m);
    
    	struct vector **graph;
    	graph = (struct vector **)malloc(sizeof(struct vector *) * n);
    	for(int i = 0;i<n;i++)
        	graph[i] = init_vector();
        	
    	for(int i = 0;i < m;i++){
	    int u,v,w;
	    scanf("%d %d %d",&u,&v,&w);
	    graph[u] = push_back(graph[u],make_pair(u,v,w));
    	}
    	printf("%d\n",neg_num_detection(graph,0,n));
    	return 0;
}

struct pair *make_pair(int x,int y,int z){
    	struct pair *newpair = (struct pair *)malloc(sizeof(struct pair));
    	newpair->x = x;
    	newpair->y = y;
    	newpair->z = z;
    	return newpair;
}

int neg_num_detection(struct vector **graph,int src,int V){
    	int *dist = (int *)malloc(sizeof(int) * V);
    	for(int i = 0;i<V;i++)
        	dist[i] = INF;
    	dist[src] = 0;
    	for(int i = 0;i<V - 1;i++){
        	for(int j = 0;j<V;j++){
            		if(graph[j]->size){
                		for(int k = 0;k<graph[j]->size;k++){
                    			int v = graph[j]->arr[k]->y;
                    			int w = graph[j]->arr[k]->z;
                    			if(sum(dist[j],w) < dist[v])
                        			dist[v] = sum(dist[j],w);
                		}
            		}
        	}
    	}
    	for(int j = 0;j<V;j++){
        	if(graph[j]->size){
        	    for(int k = 0;k<graph[j]->size;k++){
        	        int v = graph[j]->arr[k]->y;
        	        int w = graph[j]->arr[k]->z;
        	        if(sum(dist[j],w) < dist[v])
        	            return 1;
        	    }
        	}
    	}
    return -1;
}

struct vector *init_vector(){
    	struct vector *newvector = (struct vector *)malloc(sizeof(struct vector));
	    newvector->arr = (struct pair **)malloc(sizeof(struct pair *) * 0);
	    newvector->size = 0;
	    newvector->capacity = 2;
    return newvector;
}

int sum(int a,int b){
    	if ((a != INF && a != -INF) && (b != INF && b != -INF))
        	return (a + b);
    	else if (a == INF && (b != INF && b != -INF))
        	return INF;
    	else if (a == INF && b == INF)
        	return INF;
    	else if (a == INF && b == -INF)
        	return 0;
    	else if (a == -INF && (b != INF && b != -INF))
        	return -INF;
    	else if (a == -INF && b == INF)
        	return 0;
    	else if (a == -INF && b == -INF)
        	return -INF;
    	else if (b == INF && (a != INF && a != -INF))
        	return INF;
    	else if (b == -INF && (a != INF && a != -INF))
        	return -INF;
}

struct vector * push_back(struct vector *h,struct pair *item){
    	if(h->capacity == h->size){
        	h->capacity = 2 * h->capacity;
        	h->arr = (struct pair **)realloc(h->arr,sizeof(struct pair *) * h->capacity);
        	h->arr[h->size++] = item;
    	}
    	else
        	h->arr[h->size++] = item;
    	return h;
}
