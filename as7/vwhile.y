%{

#include<ctype.h>
#include<stdio.h>
#include<string.h>
int yylex(void);
void yyerror(char *);
char st[100][10];
int top=0;
char i_[2]="0";
char temp[2]="t";
int lnum=1;
int start=1;
extern char *yytext;


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
printf("L%d: \n",lnum++);
}

int lab2()
{
strcpy(temp,"t");
strcat(temp,i_);
printf("%s= not %s\n",temp,st[top]);
printf("if %s goto L %d \n",temp,lnum);
i_[0]++;
}

int lab3()
{
printf("goto L%d \n",start);
printf("L%d: \n",lnum);

}
 
 void yyerror(char *s) 
 {
    fprintf(stderr, "%s\n", s);
}


%}


%token ID NUM WHILE
%right '='
%left '+' '-'
%left '*' '/'
%left UMINUS
%%

 S : WHILE{lab1();}'(' E ')'{lab2();}E';'{lab3();} 
   ;
E : E '+'{push();} T{codegen();}
   | E '-'{push();} T{codegen();}
   | T
   ;
T : T '*'{push();} F{codegen();}
   | T '/'{push();} F{codegen();}
   | F
   ;
F : '(' E ')'
   | '-'{push();} F{codegen_umin();} %prec UMINUS
   | ID{push();}
   | NUM{push();}
   ;
%%



int main()
 {
 printf("Enter the expression : ");
 yyparse();
 }
 












/*root@localhost /]# lex vwhile.l
[root@localhost /]# yacc -d vwhile.y
[root@localhost /]# gcc lex.yy.c y.tab.c
[root@localhost /]# ./a.out
Enter the expression : while(2+3) x+1; 
L1: 
t0 = 2 + 3
t1= not t0
if t1 goto L 0 
syntax error
[root@localhost /]# ./a.out
Enter the expression : while (x+2) c+1;
L1: 
syntax error
[root@localhost /]# ./a.out
Enter the expression : while(a+1)x+2;
L1: 
t0 = a + 1
t1= not t0
if t1 goto L 0 
t2 = x + 2
goto L1 
L0: 
[root@localhost /]# 

*/
