#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
struct Node 
{ 
	int data; 
	unsigned capacity; 
	int* A; 
}; 
struct Node* createStack( unsigned capacity ) 
{ 
	struct Node* stk = (struct Node*) malloc(sizeof(struct Node)); \
	if (!stk) 
		return NULL; 
	stk->data = -1; 
	stk->capacity = capacity; 
	stk->A = (int*) malloc(stk->capacity * sizeof(int)); 
	if (!stk->A) 
		return NULL; 
	return stk; 
} 
int isEmpty(struct Node* stk) 
{ 
	return stk->data == -1 ; 
} 
char peek(struct Node* stk) 
{ 
	return stk->A[stk->data]; 
} 
char pop(struct Node* stk) 
{ 
	if (!isEmpty(stk)) 
		return stk->A[stk->data--] ; 
	return '$'; 
} 
void push(struct Node* stk, char op) 
{ 
	stk->A[++stk->data] = op; 
} 
int isOperand(char ch) 
{ 
	return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'); 
} 
int precedence(char ch) 
{ 
	switch (ch) 
	{ 
	case '+': 
	case '-': 
		return 1; 

	case '*': 
	case '/': 
		return 2; 
	} 
	return -1; 
} 
int infixToPostfix(char* exp) 
{ 
	int i, k; 
	struct Node* stk = createStack(strlen(exp)); 
	if(!stk)
		return -1 ; 

	for (i = 0, k = -1; exp[i]; ++i) 
	{ 
		if (isOperand(exp[i])) 
			exp[++k] = exp[i]; 
		else if (exp[i] == '(') 
			push(stk, exp[i]); 
		else if (exp[i] == ')') 
		{ 
			while (!isEmpty(stk) && peek(stk) != '(') 
				exp[++k] = pop(stk); 
			if (!isEmpty(stk) && peek(stk) != '(') 
				return -1; 
			else
				pop(stk); 
		} 
		else 
		{ 
			while (!isEmpty(stk) && precedence(exp[i]) <= precedence(peek(stk))) 
				exp[++k] = pop(stk); 
			push(stk, exp[i]); 
		} 

	} 
	while (!isEmpty(stk)) 
		exp[++k] = pop(stk ); 
	exp[++k] = '\0'; 
}  
int main(int argc,char *argv[]) 
{ 
	FILE *fp1,*fp2;
	fp1 = fopen((char *)argv[1],"r");
	fp2 = fopen((char *)argv[2],"w");
	char ch;
	while(1){
		ch = fgetc(fp1);
		if(ch == 'e'){
			char exp[100];
			fscanf(fp1,"%s",exp);
			infixToPostfix(exp);
			fprintf(fp2,"%s\n",exp);
		}
		else if(ch == 's')
			break;
	}
} 

