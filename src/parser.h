#ifndef PARSER_H
#define PARSER_H

#include "libnode_a-yparser.h"
#include "link_list.h"

typedef enum keyword_type_e {
	VAR_DECLARE,
	OPR,
	MODULE,
	PARAM,
	STRING1,
	INIT_LIST,
	NODE_LIST,
} keyword_type_t;
/*  input,outputs,variables decalre */
typedef  struct var_declare_node_s {
	keyword_type_t keyword_type;
	int  opr;
	link_list_t var_names;
} var_declare_node_t;






/* operators */
typedef struct opr_node_s {
		keyword_type_t keyword_type;;

	int oper; /* operator */
	int nops; /* number of operands */
	union node_type_u *op[1]; /* operands (expandable) */
} opr_node_t; 

/*  moduls declare (like in verilog) */
typedef  struct module_declare_s {
		keyword_type_t keyword_type;;

	char *name;
		union node_type_u *node_type;
} module_declare_t;


/*  moduls declare (like in verilog) */
typedef  struct node_io_s {
	keyword_type_t keyword_type;;
	link_list_t left;
	link_list_t right;

//	char *node_name,*io_name;
} node_io_t;

/* declare patameter */
typedef  struct param_s {
	keyword_type_t keyword_type;;
	char *name;
	double value;
} param_t;

/* declare string */
typedef  struct string_s {
	keyword_type_t keyword_type;;
	char *name;
} string_t;





typedef struct assign_s {
	char * left;
	char * right;
}assign_t;

assign_t *assign_new(link_list_t *left,link_list_t *right);



typedef union  val_value_u {
	int integer;
	double real;
	char *string;
} val_value_t;

//char * node_io_get_full_name(b_node_io_t *node_io);
//char * node_io_get_full_name(b_node_io_t *node_io,char *sep);
typedef struct b_node_io_s {
	char *name;

//	terminal_node_t *terminal_node;

	/*  list of connected instances. */
	struct instace_s * father_instance;

} b_node_io_t;
typedef struct instace_s {
	
	struct b_node_io_s node_io;

	// list of variables passed to python created class
	link_list_t vars;
	// var_type
	char *var_type;
	char *var_name;
	struct module_t *module;
	struct instace_s *father;


	/* holds parametrs of the next node the the current one pushes */
	//pushed_node_t pushed_node;


} instace_t;

instace_t * instance_new();

///instace_t * instance_new(char *var_type,char *var_name);




typedef struct var_s {
	char *name;
	int type;
	val_value_t value;
} var_t;


var_t *var_new_integer(char *name,int val);
var_t *var_new_string(char *name,char *string);
var_t *var_new_double(char *name,double val);
var_t *var_new_empty(char *name);
var_t *var_new_copy (var_t *var);




/* declare string */
typedef  struct instantiations_s {
	keyword_type_t keyword_type;
	link_list_t instantiations;
} instantiations_t;



typedef union node_type_u {
	keyword_type_t keyword_type;;
	var_declare_node_t var_declare;
	opr_node_t	opr_node;
	node_io_t node_io;
	module_declare_t module_declare;
	param_t param;
	string_t string;
	instantiations_t instantiations;  
} node_type_t;

node_type_t *make_init_list();
node_type_t *get_string(char *s);
node_type_t *node_io();
node_type_t *node_io1(char *s);
node_type_t *var_declare(int opr);
node_type_t *node_type_new();
void node_type_free(node_type_t *node_type);

int parse(const char *text,char *top_name,int *col_out,int *line_out,char **msg);





typedef struct module_s{
	char *name;
	int is_init;	
	struct module_s * father;
	link_list_t inputs;
	link_list_t outputs;
	link_list_t instantiations; 
	link_list_t declarations;
	link_list_t assignments;

} module_t;

typedef struct realization_s{
	module_t *module;
 	char *name;	
	struct realization_s * father;
	link_list_t realizations;


}realization_t;

realization_t *realization_new(module_t *module,realization_t *father_realization,char *name);
char * node_io_get_full_name(realization_t *realization,char *sep);
module_t * module_is_valid_instance(module_t  *module_declare, instace_t *instance);

void module_init (module_t *module_declare,char *name);

module_t * module_new ();
#endif
