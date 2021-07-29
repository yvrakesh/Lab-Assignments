#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<limits.h>

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

int Prim(struct Graph * g);
int minkey(int key[],bool set[],int V);

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
	graph->E=e;
	Prim(graph);
}

int minkey(int key[],bool set[],int V)
{
	int min = INT_MAX,index;
	for(int v=0;v<V;v++)
	{
		if(set[v]==false && key[v]<min)
		{
			min = key[v];
			index = v;
		}
	}
	return index;
}

int Prim(struct Graph * g)
{
	int v=g->V,e=0;
	struct Edge edges[g->E],result[v];
	for(int i=0;i<v;i++)
	{
		struct Node * p = g->list[i];
		while(p)
		{
			edges[e].src = i;
			edges[e].dest = p->vertex;
			int wgt;
			scanf("%d",&wgt);
			edges[e].wgt = wgt;
			e++;
			p=p->next;
		}
	}
	e = g->E;
	bool set[v];
	int key[v];
	for(int i=0;i<v;i++)
	{
		set[i] = false;
		key[i] = INT_MAX;
	}
	key[0] = 0;
	for(int i=0;i<v-1;i++)
	{
		int u = minkey(key,set,v);
		set[u] = true;
		for(int j=0;j<e;j++)
		{
			if(edges[j].src == u && set[edges[j].dest]==false && key[edges[j].dest] > edges[j].wgt)
				key[edges[j].dest] = edges[j].wgt;
		}
	}
	int sum = 0;
	for(int i=0;i<v;i++)
		sum += key[i];
	printf("%d\n",sum);
				
}
