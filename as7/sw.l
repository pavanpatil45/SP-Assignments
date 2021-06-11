%{
#include "y.tab.h"
extern int yylval;
%}
VAR [A-Za-z]
DIGIT [0-9]
%%
[ \t\n]    
switch			{return SWITCH;}
case				{return CASE;}
break				{return BREAK;}
default			{return DEFAULT;}
{VAR}({VAR}|{DIGIT})*	{return ID;}
{DIGIT}+             	{return NUM;}
.                 	{return yytext[0];}
%%

