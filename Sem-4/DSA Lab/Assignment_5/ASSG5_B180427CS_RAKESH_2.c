#include <stdio.h>
#include <stdlib.h>

#define MAX 1000000000

struct pair
{
	int x;
	int y;
};
struct edge
{
	int to;
	int w;
};
struct vector
{
	struct edge **arr;
	int size;
	int capacity;
};
struct q_node
{
	struct edge *item;
	struct q_node *next;
};
struct q
{
	int size;
	struct q_node *front,*tail;
};

struct pq
{
	int size;
	struct pair **item;
};


struct pair * make_pair(int x,int y);
struct vector * new_node();
struct vector * push_back(struct vector* h,int to,int w);
struct edge *new_edge(int to,int w);
void apsp(struct vector **graph,int src,int n);
void dijkstra(struct vector **graph,int src,int n,int *prev,int *dist);
void MinHeapify(struct pair **arr,int i,int n);
struct pq *init_pq();
int isEmpty(struct pq * );
struct pq *push_pq(struct pq *h,struct pair *x);
struct pair *top_pq(struct pq *h);
struct pq *pop_pq(struct pq *h);
int is_empty_queue(struct q *h);
struct q_node *createNode(int to,int w);
struct q *init_q();
struct q *enqueue(struct q *h,int to,int w);
struct q *dequeue(struct q *h);


int main()
{
	int n,m;
	scanf("%d %d",&n,&m);
	
	struct vector **graph;
	graph = (struct vector *)malloc(sizeof(struct vector) * n);
	
	for(int i = 0;i<n;i++)
		graph[i] = new_node();
		
	for(int i = 0;i < m;i++){
		int u,v,w;
		scanf("%d %d %d",&u,&v,&w);
		graph[u] = push_back(graph[u],v,w);
	}
	
	int s;
	scanf("%d",&s);
	
	apsp(graph,s,n);
	
	return 0;
}								

struct vector * push_back(struct vector* h,int to,int w){
	if (h->size == h->capacity){
		h->capacity *= 2;
		h->arr = (struct edge **)realloc(h->arr,sizeof(struct edge *) * (h->capacity));
		h->arr[(h->size)++] = new_edge(to,w);
	}
	else
		h->arr[(h->size)++] = new_edge(to,w);
	return h;
}

struct edge * new_edge(int to,int w){
	struct edge *n = (struct edge *)malloc(sizeof(struct edge));
	n->to = to;
	n->w = w;
	return n;
}

struct vector * new_node(){
	struct vector* new = (struct vector *)malloc(sizeof(struct vector));
	new->arr = (struct edge **)malloc(sizeof(struct edge *) * 0);
	new->size = 0;
	new->capacity = 2;
	return new;
}

struct pair * make_pair(int x,int y){
	struct pair *n = (struct pair *)malloc(sizeof(struct pair));
	n->x = x;
	n->y = y;
	return n;
}

void MinHeapify(struct pair **arr,int i,int n){
	int l = 2*i+1;
	int r = 2*i+2;
	int lar = i;
	if (l<n && arr[l]->x < arr[lar]->x)
		lar = l;
	if(r<n && arr[r]->x < arr[lar]->x)
		lar = r;
	if (lar != i){
		struct pair *temp = arr[i];
		arr[i] = arr[lar];
		arr[lar] = temp;

		MinHeapify(arr,lar,n);
	}
}

int is_empty_queue(struct q *h){
	return (h->size == 0);
}

struct q_node *createNode(int to,int w){
	struct q_node *n = (struct q_node *)malloc(sizeof(struct q_node));
	n->item = new_edge(to,w);
	n->next = NULL;
	return n;
}
struct q *init_q(){
	struct q *n = (struct q *)malloc(sizeof(struct q));
	n->front = n->tail = NULL;
	n->size = 0;
	return n;
}
struct q *enqueue(struct q *h,int to,int w){
	struct q_node *n = createNode(to,w);
	if (is_empty_queue(h)){
		h->front = h->tail = n;
		h->size++;
		return h;
	}
	h->tail->next = n;
	h->tail = n;
	h->size++;
	return h;
}

struct q *dequeue(struct q *h){
	if (!is_empty_queue(h)){
		h->front = h->front->next;
		h->size--;
	}
	return h;
}

void apsp(struct vector **graph,int src,int n){
	int *prev,*dist;
	prev = (int *)malloc(sizeof(int) * n);
	dist = (int *)malloc(sizeof(int) * n);
	dijkstra(graph,src,n,prev,dist);
	
	for(int i = 0;i<n;i++){
	        if(dist[i] == MAX){	
            		printf("%d INF\n",i);
            		continue;
        	}
        	printf("%d %d\n",i,dist[i]);
    	}
}

struct pq *init_pq(){
	struct pq *n = (struct pq *)malloc(sizeof(struct pq));
	n->item = (struct pair **)malloc(sizeof(struct pair *) * 0);
	n->size = 0;
	return n;
}
int isEmpty(struct pq *h){
	return (h->size == 0);
}

struct pq *push_pq(struct pq *h,struct pair *x){
	h->size++;
	h->item = (struct pair **)realloc(h->item,sizeof(struct pair *) * h->size);
	h->item[h->size-1] = x;
	int i = h->size - 1;
	while(i > 0 && h->item[i]->x < h->item[i/2]->x){
		struct pair *temp = h->item[i];
		h->item[i] = h->item[i/2];
		h->item[i/2] = temp;
		i = i/2;
	}
	return h;
}

struct pair *top_pq(struct pq *h){
	if(isEmpty(h))
		return NULL;
	return (h->item[0]);
}

struct pq *pop_pq(struct pq *h){
	if(isEmpty(h))
		return h;
	h->item[0] = h->item[h->size-1];
	h->size--;
	h->item = (struct pair **)realloc(h->item,sizeof(struct pair *) * h->size);
	MinHeapify(h->item,0,h->size);
	return h;
}

void dijkstra(struct vector **graph,int src,int n,int *prev,int *dist){
	for(int i = 0;i<n;i++){
		prev[i] = -1;
		dist[i] = MAX;
	}
	dist[src] = 0;
	
	struct pq *newpq;
	newpq = init_pq();
	newpq = push_pq(newpq,make_pair(dist[src],src));
	
	while(!isEmpty(newpq)){
		int root = top_pq(newpq)->y;
		newpq = pop_pq(newpq);
		for(int i = 0;i<graph[root]->size;i++){
			if(dist[root] + graph[root]->arr[i]->w < dist[graph[root]->arr[i]->to]){
				dist[graph[root]->arr[i]->to] = dist[root] + graph[root]->arr[i]->w;
				prev[graph[root]->arr[i]->to] = root;
				newpq = push_pq(newpq,make_pair(dist[graph[root]->arr[i]->to],graph[root]->arr[i]->to));
			}
		}
	}
}
