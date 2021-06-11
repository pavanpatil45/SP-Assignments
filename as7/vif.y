%{

#include<ctype.h>
#include<stdio.h>
#include<string.h>
int yylex(void);
void yyerror(char *);

extern char *yytext;
char st[100][10];
int top=0;
char i_[2]="0";
char temp[2]="t";

int label[20];
int lnum=0;
int ltop=0;



int push()
 {
  strcpy(st[++top],yytext);
 }

int codegen()
 {
 strcpy(temp,"t");
 strcat(temp,i_);
  printf("%s = %s %s %s\n",temp,st[top-2],st[top-1],st[top]);
  top-=2;
 strcpy(st[top],temp);
 i_[0]++;
 }

int codegen_umin()
 {
 strcpy(temp,"t");
 strcat(temp,i_);
 printf("%s = -%s\n",temp,st[top]);
 top--;
 strcpy(st[top],temp);
 i_[0]++;
 }

int codegen_assign()
 {
 printf("%s = %s\n",st[top-2],st[top]);
 top-=2;
 }


int lab1()
{
 lnum++;
 strcpy(temp,"t");
 strcat(temp,i_);
 printf("%s = not %s\n",temp,st[top]);
 printf("if %s goto L%d\n",temp,lnum);
 i_[0]++;
 label[++ltop]=lnum;
}

int lab2()
{

int x;
lnum++;
x=label[ltop--];
printf("goto L%d\n",lnum);
printf("L%d: \n",x);
label[++ltop]=lnum;
}

int lab3()
{
int y;
y=label[ltop--];
printf("L%d: \n",y);
}


void yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
    
}



%}

%token ID NUM IF THEN ELSE
%right '='
%left '+' '-'
%left '*' '/'
%left UMINUS
%%

S : IF'('E')'{lab1();}THEN E';'{lab2();}ELSE E';'{lab3();}
  
E : E '+' {push();} E{codegen();}
  | E '-' {push();} E{codegen();}
  | E '*' {push();} E{codegen();}
  | E '/' {push();} E{codegen();}
  | '(' E ')'
  | '-'{push();} E{codegen_umin();} %prec UMINUS
  | V
  | NUM{push();}
  ;
V : ID {push();}
  ;
%%

int main()
 {
 printf("Enter the expression : ");
 yyparse();
 }



