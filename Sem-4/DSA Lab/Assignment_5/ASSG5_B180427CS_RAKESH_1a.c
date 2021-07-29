#include<stdio.h>
#include<stdlib.h>

struct Node{
	int vertex;
	struct Node *next;
};
struct Graph{
	int V,E;
	struct Node ** list;
};
struct Edge{
	int src,dest,wgt;
};
struct Subset{
	int par,rank;
};

int search(struct Subset subsets[],int );
void Union(struct Subset subsets[],int ,int );
void sort(struct Edge edges[],int );
int Kruskal(struct Graph * );


int main()
{
	int v;
	scanf("%d",&v);
	
	struct Graph * graph = (struct Graph *)malloc(sizeof(struct Graph));	
	graph->V = v;
	graph->list = malloc(v*sizeof(struct Node));
	
	for(int i=0;i<v;i++)
		graph->list[i] = NULL;
	int e=0;
	for(int i=0;i<v;i++)
	{
		char c;
		int d;
		struct Node * p;
		do{
			scanf("%d%c",&d,&c);
			struct Node * new = (struct Node *)malloc(sizeof(struct Node));
			new->next = NULL;
			new->vertex = d;
			if(graph->list[i] == NULL)
			{
				graph->list[i] = new;
				p = graph->list[i];
			}
			else
			{
				p->next = new;
				p = p->next;
			}
			e++;
		}while(c != '\n');
	}
	graph->E=e/2;
	Kruskal(graph);
}

void Union(struct Subset subsets[],int x,int y)	{
	int x_rep = search(subsets,x) , y_rep = search(subsets,y);
	if(subsets[x_rep].rank < subsets[y_rep].rank)
		subsets[x_rep].par = y_rep;
	else if(subsets[x_rep].rank > subsets[y_rep].rank)
		subsets[y_rep].par = x_rep;
	else{
		subsets[y_rep].par = x_rep;
		subsets[x_rep].rank++;
	}
}

int search(struct Subset subsets[],int i){
	if(subsets[i].par!=i)
		subsets[i].par = search(subsets,subsets[i].par);
	return subsets[i].par;
}

void sort(struct Edge edges[],int e){
	int min=0,index=0;
	for(int j=0;j<e-1;j++){
		index = j;
		min = edges[j].wgt;
		for(int k=j+1;k<e;k++){
			if(min > edges[k].wgt){
				index = k;
				min = edges[k].wgt;
			}
		}
		int s,d,w;
		s = edges[j].src;
		d = edges[j].dest;
		w = edges[j].wgt;
		edges[j].src = edges[index].src;
		edges[j].dest = edges[index].dest;
		edges[j].wgt = edges[index].wgt;
		edges[index].src = s;
		edges[index].dest = d;
		edges[index].wgt = w;
	}
}

int Kruskal(struct Graph * g){
	int v=g->V;
	int e=0;
	int flag=1;
	struct Edge edges[g->E],result[v];
	
	for(int i=0;i<v;i++){
		struct Node * p = g->list[i];
		while(p){
			int wgt;
			scanf("%d",&wgt);
			for(int j=0;j<e;j++)
				if(edges[j].src==p->vertex&&edges[j].dest==i){
					flag=0;
					break;
				}
			if(flag){
				edges[e].src = i;
				edges[e].dest = p->vertex;
				edges[e].wgt = wgt;
				e++;
				p=p->next;
			}
			else{	
				flag = 1;
				p = p->next;
			}
		}
	}
	sort(edges,g->E);
	struct Subset subsets[v];
	for(int i=0;i<v;i++){
		subsets[i].par = i;
		subsets[i].rank = 0;
	}
	e = 0;
	int i = 0,sum=0;
	while(e < v-1 && i < g->E){
		int x = search(subsets,edges[i].src) , y = search(subsets,edges[i].dest);
		if(x!=y){
			result[e].src =  edges[i].src;
			result[e].dest = edges[i].dest;
			result[e].wgt = edges[i].wgt;
			sum += result[e].wgt;
			Union(subsets,x,y);
			e++;
			i++;
		}
		else
			i++;
	}
	printf("%d\n",sum);
}
