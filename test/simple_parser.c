/*
 * =====================================================================================
 *
 *       Filename:  test1.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/29/15 15:44:00
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include "parser.h"
#include "node.h"

extern int yylex();
extern int yyparse();
extern FILE *yyin;
extern int line;
extern int col;
extern char *yytext;
int level=0;

int __wrap_ex(node_type_t *p) {
	if (!p) return 0;
	int i;
	switch (p->keyword_type)
	{
		case OPR:
			switch (p->opr_node.oper)
			{	
				case ';':
					for (i=0;p->opr_node.op[i];i++) {
						ex(p->opr_node.op[i]);
						}
					break;
//				case '=':
//					printf("%s.%s=%s.%s\n",p->opr_node.op[0]->node_io.node_name,p->opr_node.op[0]->node_io.io_name,
//							       p->opr_node.op[1]->node_io.node_name,p->opr_node.op[1]->node_io.io_name);

					break;

				break;
			}
			break;
		case VAR_DECLARE:
			switch (p->var_declare.opr)
			{
				case INPUT:
					printf("input %d\n------\n",level);
					for (;p->var_declare.var_names.list_len;)
						printf("%s\n",link_list_get_head(&p->var_declare.var_names));
					break;
				case OUTPUT:
					printf("outputs %d\n------\n",level);
					for (;p->var_declare.var_names.list_len;)
						printf("%s\n",link_list_get_head (&p->var_declare.var_names));
					break;
			}
			break;
		case MODULE:
			level++;		
			printf ("create module %s %d\n",p->module_declare.name,level);
			ex(p->module_declare.node_type);
			level--;
			break;
	}
}

int main() {
	// open a file handle to a particular file:
	FILE *myfile = fopen("node.ls", "r");
	// make sure it is valid:
	if (!myfile) {
		printf( "I can't open a.snazzle.file!");
		return -1;
	}
	// set lex to read from it instead of defaulting to STDIN:
	yyin = myfile;
	
	// parse through the input until there is no more:
	do {
		yyparse();
	} while (!feof(yyin));
	printf("stop at line %d\n",line);
	return 0;
}

