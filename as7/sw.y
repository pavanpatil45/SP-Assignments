%{
#include <stdio.h>
#include <stdlib.h>
#include<ctype.h>
#include<string.h>

char stack[50][50];
extern char *yytext;
char temp[20];
int top=0;
int tempcount=0;
int lablecount=0;
int numb;
char number[10];




int generate(){
  	printf("\nt%d = %s %s %s",tempcount,stack[top-3],stack[top-2],stack[top-1]);
  	top=top-3;
	strcpy(stack[top],"t");
	sprintf(temp,"%d",tempcount);
	strcat(stack[top],temp);
	top++;
	tempcount++;
}

int generate_minus(){
	printf("\nt%d = -%s",tempcount,stack[top]);
	top--;
	strcpy(stack[top],"t");
	sprintf(temp,"%d",tempcount);
	strcat(stack[top],temp);
	tempcount;
}
int generate_equal(){
	printf("\n%s = %s",stack[top-3],stack[top-1]);
 	top=top-3;
}

int fun1(){
	printf("\nif %s is not equal to %s goto L%d",stack[top-1],stack[top-2],lablecount++);
	top--;
}
int fun2(){
	printf("\ngoto L%d",lablecount);  //for break
	printf("\nL%d:",lablecount-1); 		//for default 
}

int fun3(){
	printf("\nL%d:",lablecount);
	exit(0);
}

int push(){
	strcpy(stack[top++],yytext);
}

int yyerror (char *msg){
	return fprintf (stderr, "YACC: %s\n", msg);
}

int yywrap(){
} 



%}

%token ID NUM SWITCH CASE BREAK DEFAULT
%right '='
%left '+' '-'
%left '*' '/'
%left UMINUS

%%
S 	: SWITCH '('E')''{' CASE N {fun1();}':' E ';' BREAK {fun2();}';'
			DEFAULT ':' E {fun3();}';''}'
  	;
 E	: T '=' {push();} E{generate_equal();}
  	| E '+' {push();} E{generate();}
  	| E '-' {push();} E{generate();}
  	| E '*' {push();} E{generate();}
 	| E '/' {push();} E{generate();}
  	| '(' E ')'
  	| '-'{push();} E{generate_minus();} %prec UMINUS
  	| T
  	| NUM{numb=$1;push();}
  	;
T 	: ID {push(yytext);}
  	;
N	:NUM{numb=$1;push();}
	;
%%

int main(){
	printf("\nEnter the expression: ");
	yyparse();
}




