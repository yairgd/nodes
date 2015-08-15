/**
 *       @file  yjson.y
 *      @brief  yacc file for jason parser
 *
 * Detailed description starts here.
 *
 *     @author  Yair Gadelov (), yair.gadelov@gmail.com
 *
 *   @internal
 *     Created  07/25/2015
 *    Revision  $Id: doxygen.templates,v 1.3 2010/07/06 09:20:12 mehner Exp $
 *    Compiler  gcc/g++
 *     Company  
 *   Copyright  Copyright (c) 2015, Yair Gadelov
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License as published by the Free Software Foundation.
 * =====================================================================================
 */


%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "json.h"



// stuff from lex that yacc needs to know about:
extern int yylex();
extern int yyparse();
extern FILE *yyin;
extern int line;
extern int col;
extern char *yytext;
void yyerror(char *s);

 
static parser_stack_t *stack_array;
static parser_stack_t *stack_object;
static json_object_t *json_object;
static json_array_t *json_array;
void set_stack_array(parser_stack_t *stack);
void set_stack_object(parser_stack_t *stack);
void set_json_object(json_object_t *jobj);
void set_json_array(json_array_t *jobj);
static json_record_t *tmp_json_record;

%}



%union {
	char bValue;
	int iValue;
	double dValue;
	char *sString;
	json_record_t *js_record;
};


%token <iValue> JS_INTEGER
%token <dValue> JS_DOUBLE
%token <sString> JS_STRING JS_VAR
%token <bValue> JS_BOOLIAN
%token <js_record> JS_RECORD 
%token JS_TRUE JS_FALSE JS_NULL JS_OBJECT JS_ARRAY
%type  <js_record>     json_record json_array_element

%%

program:
	'{' json_record_list	'}'   
	|   /* initiate top json obejcy*/
	;
	

json_record:
	'"' JS_VAR '"' ':' '"' JS_VAR '"'                 {$$=json_record_new_string($2,$6);}
	| '"' JS_VAR '"' ':' JS_INTEGER               {$$=json_record_new_integer($2,$5);}
	| '"' JS_VAR '"' ':' JS_DOUBLE                {$$=json_record_new_double($2,$5);}
	| '"' JS_VAR '"' ':' JS_BOOLIAN               {$$=json_record_new_boolian($2,$5);}
	| '"' JS_VAR '"' ':' '{' { stack_push(stack_object,json_object,sizeof(json_object_t* )); tmp_json_record = json_record_new_object($2);json_object =json_record_get_object(tmp_json_record);  } json_record_list '}'  {json_object = *(json_object_t**)stack_pop(stack_object,sizeof(json_object_t* ));$$=tmp_json_record;}
	| '"' JS_VAR '"' ':' '[' { stack_push(stack_array,json_array,sizeof(json_array_t* ));tmp_json_record = json_record_new_array($2);json_array = json_record_get_array( tmp_json_record); } json_array']'      {json_array=*(json_array_t**)stack_pop(stack_array,sizeof(json_array_t* ));$$=tmp_json_record;};
	;


json_record_list:
	json_record {json_object_add_record ( json_object,$1);}
	|json_record_list ',' json_record  {json_object_add_record ( json_object,$3);}
	;


json_array_element:
	'"' JS_VAR '"'   {$$=json_record_new_string("",$2);}
	|JS_INTEGER   {$$=json_record_new_integer("",$1);}
	|JS_DOUBLE  {$$=json_record_new_double("",$1);}
	|  '{' { stack_push(stack_object,json_object,sizeof(json_object_t* )); tmp_json_record = json_record_new_object("");json_object = json_record_get_object(tmp_json_record); } json_record_list '}'  {json_object = *(json_object_t**)stack_pop(stack_object,sizeof(json_object_t* ));$$=tmp_json_record;}
	| '[' { stack_push(stack_array,json_array,sizeof(json_array_t* ));json_array = json_record_get_array( tmp_json_record);  } json_array']'      {json_array=*(json_array_t**)stack_pop(stack_array,sizeof(json_array_t* ));$$=tmp_json_record;};
	;

json_array:
	json_array_element  {json_array_add_record ( json_array,$1);}
	|json_array ',' json_array_element  {json_array_add_record ( json_array,$3);}
	;



%%


void set_stack_array(parser_stack_t *stack)
{
	stack_array = stack;
}

void set_stack_object(parser_stack_t *stack)
{
	stack_object = stack;
}

void set_json_object(json_object_t *jobj)
{
	json_object = jobj;
}

void set_json_array(json_array_t *jobj)
{
	json_array = jobj;
}

json_record_t *get_tmp_record()
{
	return tmp_json_record;
}

void yyerror(char *s) {
	fprintf(stdout, "%s %d:%d  %s\n", s,(int)(line+0),(int)(col-strlen(yytext)+1),yytext);
}



