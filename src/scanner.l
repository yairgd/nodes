%{
#include <stdio.h>
#include "libnode_a-yparser.h"
int line;
int col;
%}
%%
[\n]		{col=0;line++;}
[\t]+		{}
[ ]+ 		{col+=strlen(yytext);};      /* ignore whitespace */
[0-9]+		{yylval.iValue = atoi(yytext);return INTEGER;};
[0-9]+\.[0-9]*	{yylval.dValue = atof(yytext);return DOUBLE;};
input		{col+=strlen(yytext);return INPUT;}
output		{col+=strlen(yytext);return OUTPUT;}
node		{col+=strlen(yytext);return NODE;} 
connection	{col+=strlen(yytext);return CONNECTION;}
\->		{col+=strlen(yytext);return ASSIGN;}



		 
[a-zA-Z_][a-zA-Z0-9_]*        {
                       char *p=(char *)calloc(strlen(yytext)+1,sizeof(char));
                       strcpy(p,yytext);
                       yylval.sString=p;
		       col+=strlen(yytext);
		       return VARIABLE;
                 };



\/\/.* {col+=strlen(yytext);} /* remark*/

[-()<>=+*/;{}.,]	{col+=strlen(yytext);return *yytext;}





[^a-zA-Z0-9] {col+=strlen(yytext);return *yytext;} /* returns all possibles combibation of illegal TOKENS*/

%%

/*
int yywrap(void) {
	return 1;
}

extern int yyparse();

int main(int argc, char *argv[]) 
{
      yyin = fopen(argv[1], "r");
      yylex();
      fclose(yyin);
}
*/
