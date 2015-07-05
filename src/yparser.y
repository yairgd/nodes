%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "parser.h"
#include "link_list.h"
#include "python_maker.h"

node_type_t *var_declare(int opr);
node_type_t *module_declare(char *name,node_type_t *opr);
node_type_t *opr(int opr, int nops, ...);

// stuff from lex that yacc needs to know about:
extern int yylex();
extern int yyparse();
extern FILE *yyin;
extern int line;
extern int col;
extern char *yytext;

extern int ex(node_type_t *p);
 
/* prototypes */
int ex(node_type_t *p);
node_type_t *declare();
void yyerror(char *s);

instace_t *instance=0;
link_list_t instance_list;
link_list_t string_list;
link_list_t node_list;
int node_list_len=0;

%}



%union {
	int iValue;
	double dValue;
	char *sString;
	union node_type_u *nodePtr;
};


%token <iValue> INTEGER
%token <dValue> DOUBLE
%token <sString> VARIABLE NODE_IO STRING 
%token <var_declare_node_t>INPUT OUTPUT 
%token NODE CONNECTION ASSIGN
%type  <nodePtr>     stmt stmt_list 

%%

program:
	function	{free(instance);}
	;
	
function:
	function stmt	{ex($2);  node_type_free($2);}
	| {node_list_len=0;link_list_init(&string_list);link_list_init(&node_list);link_list_init (&instance_list );instance = instance_new();}/* NULL */
	;

stmt:
	INPUT string_list ';'  {$$=var_declare(INPUT);}
	|OUTPUT string_list ';' {$$=var_declare(OUTPUT);}
	|NODE VARIABLE '{' stmt_list '}' {$$=module_declare($2,$4);}
	|node_io_list{node_list_len=node_list.list_len;} ASSIGN node_io_list ';' {$$=node_io(); }
/*	|NODE_IO '=' NODE_IO ';' {$$=opr('=',2,node_io($1),node_io($3) );}
	|VARIABLE '=' NODE_IO ';' {$$=opr('W',2,node_io1($1),node_io($3) );}
	|NODE_IO '=' VARIABLE ';' {$$=opr('Q',2,node_io($1),node_io1($3) );}*/
	|VARIABLE  init_list ';' {$$=make_init_list($1);}
	|';'	{$$=opr(';',1,0);} /* remove non neccesry ';'*/
	;



stmt_list:
	stmt   {$$=$1;}
	|stmt_list  stmt  {$$=opr(';',2,$1,$2);} /* force ';' between orders*/
	;


string_list:
	VARIABLE {link_list_add_head(&string_list,$1);  }
	| string_list ',' VARIABLE {link_list_add_head(&string_list,$3);}
	;

node_io_list:
	VARIABLE {link_list_add_head(&node_list,$1);  }
	| node_io_list '.' VARIABLE {link_list_add_head(&node_list,$3);}
	;


init_list:
	init {;link_list_add_head(&instance_list,instance);  instance = instance_new(); ;}
	|init_list ',' init {link_list_add_head(&instance_list,instance);instance = instance_new();};
	;
init:
   	VARIABLE '(' var_list ')' {;instance->node_io.name=$1;instance->var_name=$1;}
	|VARIABLE {;instance->node_io.name=$1;instance->var_name=$1 ;};
	;

var_list:
	var{};
	|var_list ',' var{};
var:
   	VARIABLE {link_list_add_head(&instance->vars,var_new_empty($1));  }
	|VARIABLE'=' DOUBLE {link_list_add_head(&instance->vars,var_new_double($1,$3));  }
	|VARIABLE'=' '"' VARIABLE '"' {link_list_add_head(&instance->vars,var_new_string($1,$4)); }
	|VARIABLE'=' INTEGER {link_list_add_head(&instance->vars,var_new_integer($1,$3));} 
	;



%%




/**
 * @created 05/05/15
 * @brief   
 * @param   name - represents the type of the instance
 * @return  
 */
node_type_t *make_init_list(char *name)
{
	int i_init,i_var;
	node_type_t *p;
	/* allocate node */
	if ((p = malloc(sizeof(instantiations_t))) == NULL)
		yyerror("out of memory at make_init_list"); 
	p->keyword_type=INIT_LIST;
	// creae instantioation list
	// example: merge AA aa2,aa2 and BB v1,b2 to list (AA,aa1),(AA,aa2),(AA,aa3),(BB,bb1),(BB,bb2)

	link_list_init(&p->instantiations.instantiations);

	for (;instance_list.list_len;)
	{
		instace_t *instance =  link_list_get_head(&instance_list);
		link_list_add_head(&p->instantiations.instantiations,instance );
		instance->var_type = name;
	}

	return p;
}


node_type_t *parm(char *s)
{
	node_type_t *p;
	/* allocate node */
	if ((p = malloc(sizeof(node_io_t))) == NULL)
		yyerror("out of memory at param"); 
	p->keyword_type=PARAM;
	p->param.name = s;
	return p;
}

node_type_t *get_string(char *s)
{
	node_type_t *p;
	/* allocate node */
	if ((p = malloc(sizeof(node_io_t))) == NULL)
		yyerror("out of memory at string"); 
	p->keyword_type=STRING1;
	p->param.name = s;
	return p;
}

node_type_t *node_io()
{
	int i;
	char *str;
	node_type_t *p;
	/* allocate node */
	if ((p = malloc(sizeof(node_io_t))) == NULL)
		yyerror("out of memory"); 

	link_list_init (&p->node_io.right );
	link_list_init (&p->node_io.left );

	// take left part before '=' : a.s.d.f = r.g.y.y (read data from list in reverse order)
	for (i=0;i<node_list_len-0;i++)
	{
		str = link_list_get_tail(&node_list);
		link_list_add_tail(&p->node_io.left,str );
	}


	// take right part after '=' (read data from list in reverse order)
	for (;node_list.list_len;)
	{
		str = link_list_get_tail(&node_list);
		link_list_add_tail(&p->node_io.right,str );
	}
	
	return opr('*',1,p );

}
#ifdef GG
node_type_t *node_io(char *s)
{
	node_type_t *p;
	/* allocate node */
	if ((p = malloc(sizeof(node_io_t))) == NULL)
		yyerror("out of memory"); 
	p->keyword_type=NODE_IO;
	p->node_io.node_name = s;
	for (;*s!='.';s++);
	*s=0;
	s++;
	p->node_io.io_name = s;
	return p;

}
node_type_t *node_io1(char *s)
{
	node_type_t *p;
	/* allocate node */
	if ((p = malloc(sizeof(node_io_t))) == NULL)
		yyerror("out of memory"); 
	p->keyword_type=NODE_IO;
	p->node_io.node_name = 0;
	p->node_io.io_name = s;
	return p;

}
#endif
node_type_t *var_declare(int opr)
{
 	node_type_t *p;
	int i;
	/* allocate node */
	if ((p = malloc(sizeof(var_declare_node_t))) == NULL)
		yyerror("out of memory"); 
	p->keyword_type=VAR_DECLARE;
	p->var_declare.opr = opr;
	link_list_init(&p->var_declare.var_names);
	
	for (i=0;string_list.list_len;i++)
	{
		char *str = link_list_get_head(&string_list);
	//	p->var_declare.names[i] = str; 
		link_list_add_head(&p->var_declare.var_names,str );
		
	}
	return p;
}

node_type_t *module_declare(char *name,node_type_t *node_type)
{
	node_type_t *p;
	/* allocate node */
	if ((p = malloc(sizeof(module_declare_t))) == NULL)
		yyerror("out of memory"); 
	
	p->module_declare.name=name;
	p->module_declare.node_type = node_type;
	p->keyword_type = MODULE;
	return p;

	
}

node_type_t *opr(int oper, int nops, ...)
{
	va_list ap;
	node_type_t *p;
	size_t size;
	int i;

	/* allocate node */
	size = sizeof(opr_node_t) + (nops - 1) * sizeof(node_type_t*);
	if ((p = malloc(size)) == NULL)
		yyerror("out of memory");
	/* copy information */
	p->keyword_type = OPR;
	p->opr_node.oper = oper;
	p->opr_node.nops = nops;
	va_start(ap, nops);
	for (i = 0; i < nops; i++)
		p->opr_node.op[i] = va_arg(ap, node_type_t*);
	va_end(ap);
	return p;
} 


void node_type_free(node_type_t *node_type)
{

}

	
void yyerror(char *s) {
	fprintf(stdout, "%s %d:%d  %s\n", s,(int)(line+0),(int)(col-strlen(yytext)+1),yytext);
}


