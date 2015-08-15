%{
#include <stdio.h>
#include "json.h"
#include "libjson_a-yjson.h"
int line;
int col;
%}
%%
[\n]		{col=0;line++;}
[\t]+		{}
[ ]+ 		{col+=strlen(yytext);};      /* ignore whitespace */
[0-9]+		{yylval.iValue = atoi(yytext);return JS_INTEGER;};
[0-9]+\.[0-9]*	{yylval.dValue = atof(yytext);return JS_DOUBLE;};
true		{col+=strlen(yytext);yylval.bValue=1;return JS_BOOLIAN;}
false		{col+=strlen(yytext);yylval.bValue=0;return JS_BOOLIAN;}
null		{col+=strlen(yytext);return JS_NULL;} 


[()*/:{}",\[\]]	{col+=strlen(yytext);return *yytext;}


 [a-zA-Z_][a-zA-Z0-9_]*        {
                         char *p=(char *)calloc(strlen(yytext)+1,sizeof(char))    ;
                         strcpy(p,yytext);
                         yylval.sString=p;
                         col+=strlen(yytext);
                         return JS_VAR;
                   };




		 



%%


