%{
	#include<stdio.h>
	#include<stdlib.h>
	int yylex(void);
	void yyerror(char *);
	int i=0,j=0,k=0,n=0;
%}

%token NUM
%left '+''-'
%left '*''/'
%left '^'
%left '('')'

%%

op : expn	{ printf("=%d\n",$1); }
;
expn : expn '+' expn	{ $$=$1+$3 ; }
     | expn '-' expn	{ $$=$1-$3 ; }
     | expn '*' expn	{ $$=$1*$3 ; }
     | expn '/' expn 	{
				if($3==0)
				{
					printf("\nDivide by 0 error\n");
					exit(0);
				}
				else	
					$$=$1/$3 ;
			 }	
     | expn '^' expn	{ /*$$=pow($1,$3)*/ 
			if($3==0)
				$$=1;
			else
			{
				n=$3;
				j=$1;
				k=j;
				for(i=1;i<n;i++)
					k=k*j;
				$$=k;
			}
			 }
     | '(' expn ')'	{ $$=$2 ; }
     | '-' expn		{ $$=-$2 ;} 
     | '+' expn		{ $$=+$2 ;}
     | NUM		{ $$=$1 ; }
;
%%

int main()
{
	printf("\nEnter expression : ");
	yyparse();
}

void yyerror(char *str)
{
	printf("%s",str);
	exit(0);
}
