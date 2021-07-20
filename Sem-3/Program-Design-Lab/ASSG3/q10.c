#include<stdio.h>
#include<stdlib.h>
struct Node{
   char data;
   struct Node * next;
};
void push(struct Node ** stk,char element);
char pop(struct Node ** stk);
int Is_Matching_Pair(char ch1,char ch2);
int checkParantheses(char exp[]);
int main(int argc,char * argv[]){
	FILE *fp1,*fp2;
	fp1 = fopen((char *)argv[1],"r");
	fp2 = fopen((char *)argv[2],"w");
	char ch;
	while(1){
		ch = fgetc(fp1);
		if(ch == 'e'){
			char exp[100];
			fscanf(fp1,"%s",exp);
			if(checkParantheses(exp)==1)
			   fprintf(fp2,"YES\n");
			else
			   fprintf(fp2,"NO\n");
		}
		else if(ch == 's')
			break;
	}
}
void push(struct Node ** stk,char element){
            struct Node * temp1;
            temp1 = (struct Node *)malloc(sizeof(struct Node));
            temp1->data = element;
            temp1->next = *stk;
            *stk = temp1;
}
char pop(struct Node ** stk){
        struct Node * temp;
            temp = *stk;
        if(temp==NULL)
            return 0;
        else{
                struct Node * t;
                t = temp;
            temp = temp->next;
                *stk = temp;
            return t->data;
        }
}
int Is_Matching_Pair(char ch1,char ch2){
	if(ch1 == '[' && ch2 == ']')
		return 1;
	else if(ch1 == '{' && ch2 == '}')
		return 1;
	else if(ch1 == '(' && ch2 == ')')
		return 1;
	return 0;
}
int checkParantheses(char exp[])
{
	struct Node * stk = NULL;
	int i=0;
	while (exp[i]) 
   		{ 
      			if (exp[i] == '{' || exp[i] == '(' || exp[i] == '[') 
        			push(&stk, exp[i]); 
      			if (exp[i] == '}' || exp[i] == ')' || exp[i] == ']') 
      			{ 
         			if (stk == NULL) 
           				return 0;  
         			else if ( !Is_Matching_Pair(pop(&stk), exp[i]) ) 
           				return 0; 
      			} 
      				i++; 
   		} 
   if (stk == NULL) 
     return 1;
   else
     return 0; 
	return 0;
} 
