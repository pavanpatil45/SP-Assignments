%{

  #include<stdio.h>
  #include "y.tab.h"
  void yyerror(char *);
  extern int yylval;
  
  
%}

ALPHA [A-Za-z]
DIGIT [0-9]

%%
while  return WHILE;
{ALPHA}({ALPHA}|{DIGIT})* return ID;
{DIGIT}+ {yylval=atoi(yytext); return NUM;}
[\n\t] yyterminate();
. return yytext[0];

%%

int yywrap(void) {
    return 1;
}
