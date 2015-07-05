/*
 * =====================================================================================
 *
 *       Filename:  parser.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/30/15 13:44:47
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */


#include "parser.h"
#include <string.h>
#include "node.h"
#include "stack.h"
#include <stdlib.h>
#include <stdio.h>

extern int yylex();
extern int yyparse();
extern FILE *yyin;
extern int line;
extern int col;
extern char *yytext;
int __attribute__((weak))  level=0;
void yy_switch_to_buffer (	struct yy_buffer_state * new_buffer  );
struct yy_buffer_state *  yy_scan_string (const char *yy_str  );
void yy_delete_buffer (struct yy_buffer_state *  b  );

static char *sep = "_";

parser_stack_t *stack;
int   ex(node_type_t *p) {
	module_t *p_terminal_node;
	instace_t *instance;
	int ok_out,ok_in;
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
				case '*':
					p_terminal_node = *((module_t**)stack_get_top(stack,sizeof(module_t*)));

					link_list_add_head( &p_terminal_node->assignments ,assign_new(&p->opr_node.op[0]->node_io.left,&p->opr_node.op[0]->node_io.right) );
					link_list_deinit(&p->opr_node.op[0]->node_io.left);
					link_list_deinit(&p->opr_node.op[0]->node_io.right);


					break;
			}
		case VAR_DECLARE:
			p_terminal_node = *((module_t**)stack_get_top(stack,sizeof(module_t*)));
			switch (p->var_declare.opr)
			{
				case OUTPUT:
					for (;p->var_declare.var_names.list_len;) {
						char *name = link_list_get_head( &p->var_declare.var_names);
						// check if the name is allready defined as output of instance
						link_list_for_each(char *output1,&p_terminal_node->outputs)
						{
							if (strcmp(output1,name)==0) {
								printf("definition of %s is allready defined as output at row %d col %d\n",name,col,line);
								goto err1;
							}

						}
						link_list_for_each(instance,&p_terminal_node->instantiations)
						{
							if (strcmp(instance->var_name,name)==0) {
								printf("definition of %s is allready defined as instancation at row %d col %d\n",name,col,line);
								goto err1;
							}
							if (strcmp(instance->var_type,name)==0) {
								printf("definition of %s is allready defined as instatntiation type %d col %d\n",name,col,line);
								goto err1;
							}

						}
						//OK,  no errors
						//	node_input_t *input = node_input_new(name,p_terminal_node );
						link_list_add_head(&p_terminal_node->outputs, name );
					}
		err1:
					break;
				case INPUT:
					for (;p->var_declare.var_names.list_len;) {
						char *name = link_list_get_head( &p->var_declare.var_names);
						// check if the name is allready defined as input of instance
						link_list_for_each(char *input,&p_terminal_node->outputs)
						{
							if (strcmp(input,name)==0) {
								printf("definition of %s is allready defined as input at row %d col %d\n",name,col,line);
								goto err1;
							}

						}
						link_list_for_each(instance,&p_terminal_node->instantiations)
						{
							if (strcmp(instance->var_name,name)==0) {
								printf("definition of %s is allready defined as instancation at row %d col %d\n",name,col,line);
								goto err1;
							}
							if (strcmp(instance->var_type,name)==0) {
								printf("definition of %s is allready defined as instatntiation type %d col %d\n",name,col,line);
								goto err1;
							}		
						}

						//OK,  no errors
						//	node_input_t *input = node_input_new(name,p_terminal_node );
						link_list_add_head(&p_terminal_node->inputs, name );
					}
					break;
			}
			break;
		case INIT_LIST:
		// take instatiation list from locatl noe and join in with comlite instantiation list of module
		// example: merge AA aa2,aa2 and BB v1,b2 to list (AA,aa1),(AA,aa2),(AA,aa3),(BB,bb1),(BB,bb2)
			p_terminal_node = *( (module_t**)stack_get_top(stack,sizeof(module_t*)));
			for (;p->instantiations.instantiations.list_len;) {
				instance = link_list_get_head(&p->instantiations.instantiations);
				link_list_add_head(&p_terminal_node->instantiations,instance);	

			}
			break;
	case MODULE:
		level++;

		// initiate new module from he stack
		// todo: add function  " terminal_node_t * terminal_node_new() "
		p_terminal_node = malloc(sizeof(module_t));
		p_terminal_node->name = (char*)p->module_declare.name;
		//p_terminal_node->level = level;
		//p_terminal_node->is_init = 0;
		link_list_init(&p_terminal_node->inputs);
		link_list_init(&p_terminal_node->outputs);
		link_list_init(&p_terminal_node->instantiations);
		link_list_init(&p_terminal_node->declarations);

		// add node decalaration to its father list
		p_terminal_node->father  =  *(module_t**)stack_get_top(stack,sizeof(module_t*));
		link_list_add_head(&p_terminal_node->father->declarations,p_terminal_node);

		// push new created node to stack
		stack_push(stack,&p_terminal_node,sizeof(module_t*) );


		// implenemt module inside instrcutions
		ex(p->module_declare.node_type);

		// get the module from the stack
		p_terminal_node = *(( module_t**)stack_pop(stack,sizeof(module_t*)));


		level--;

#ifdef DEBUG_PARSER
		printf ("create module %s %d\n-----------\n",p_terminal_node->name,level);

		printf("input %d\n------\n",p_terminal_node->inputs.list_len);
		link_list_for_each (char *input , &p_terminal_node->inputs)
			printf("%s\n",input);

		printf("output %d\n------\n",p_terminal_node->outputs.list_len);
		link_list_for_each (char *output , &p_terminal_node->outputs)
			printf("%s\n",output);

		printf("instantiations %d\n........\n",p_terminal_node->instantiations.list_len);
		link_list_for_each (instace_t *instance , &p_terminal_node->instantiations) {
			printf("%s -> %s (",instance->var_type, instance->node_io.name);
			int kk=1;
			link_list_for_each (var_t *var,&instance->vars)
			{
				if (instance->vars.list_len>kk)
					printf ("%s,", var->name);
				else
					printf ("%s", var->name);
				kk++;
			}
			printf(")\n");	
		}

#endif


		break;

	}
	return 0;
}

/**
 * @created 05/21/15
 * @brief   The functions creates full realization_t structure from top module structure. also node_t structrure are created accoring to realization
 * 	    1. node with out internal realizations creates node_t with string name based on the realization path
 * 	    2. node with internal realizatons is sperated to to amout of node_t strucrture. node_t structure is created for each input and output
 * 	    the string name is called after the path from the top root of the io.
 * @param   top_module: initiated module_t struycture theat contain the script structure (created durewing the yacc parsing)
 * @param   realization_father: initiated realization_t structure that is used as father (root father) to all the script. all child
 * 	    realizations crteated under it
 * @param   node_manager: initiated node_manager_t structure 
 * @return none
 */
void module_create_instance(module_t *top_module,realization_t  *realization_father,node_manager_t *node_manager)
{
	int i;
	module_t *tmp_module;
	realization_t *realization;
	link_list_for_each (instace_t *instace , &top_module->instantiations) {
		// locate the module declaration of the current instance
		tmp_module = module_is_valid_instance(top_module,instace);

		// if valid module declaration 
		if (tmp_module && tmp_module!=top_module) {
			// create new instance and add it to realization insatce list
			realization = realization_new(tmp_module, realization_father,instace->var_name );
			char *name= node_io_get_full_name (realization,sep);

			link_list_add_head (&realization_father->realizations,realization);

			// declare current node if no internal in isie, if so go to recrvive call on the compose node
			if (tmp_module->instantiations.list_len) 
				module_create_instance(tmp_module,realization,node_manager);


			// if there are no sub modules define in the current one,
			// define node for this terminal module
			if (tmp_module->instantiations.list_len==0) {
				printf("%s\n",name);
				// create new node
				node_t  *node;
				node_init(&node,tmp_module->inputs.list_len,tmp_module->outputs.list_len,1,node_manager);
				node->name  = name;
				node->data = realization;



				// set input name for each of the the node io's
				i=0;
				link_list_for_each(char *input,&tmp_module->inputs) {
					char *str =  malloc(strlen(name)+strlen(input)+3);
					sprintf(str,"%s%s%s",name,sep,input);
					node_set_input_name (node,i,str);
					free(str);
					i++;
				}

				// set output name for each of the the node io's
				i=0;
				link_list_for_each(char *output,&tmp_module->outputs) {
					char *str =  malloc(strlen(name)+strlen(output)+3);
					sprintf(str,"%s%s%s",name,sep,output);
					node_set_output_name (node,i,str);
					free(str);
					i++;
				}
				//#endif

			}

			// make io's as separate node with sigle input and single output
			//if (tmp_module->instantiations.list_len) 
			else
			{

				link_list_for_each(char *input, &tmp_module->inputs) {
					node_t  *node;
					node_init(&node,1,1,1,node_manager);
					char *str =  malloc(strlen(name)+strlen(input)+3);
					sprintf(str,"%s%s%s",name,sep,input);	
					node->name = str;
					node->data = 0;
					//	node_set_input_name (node,0,str);
					//	free(str);


				}
				link_list_for_each(char *output, &tmp_module->outputs) {
					node_t  *node;
					node_init(&node,1,1,1,node_manager);
					char *str =  malloc(strlen(name)+strlen(output)+3);
					sprintf(str,"%s%s%s",name,sep,output);	
					node->name = str;
					node->data = 0;
					//	node_set_output_name (node,0,str);
					//	free(str);
				}



			}


		}

		else
		{
err:			printf("error declaratin %s at line %d and row %d type  %s not exist \n",instace->var_name,line,col,instace->var_type );

		}



	}

}

/**
 * @created 05/21/15
 * @brief   this function create full path name of assiment inside module
 * @param   
 * @return  
 */
char * realization_get_full_name (realization_t  *realization,char *path) {
	link_list_t tokens;
	link_list_init(&tokens);
	char *full_name;
	char *name;
	realization_t *tmp = realization;
	//	if (tmp->father) {
	do {
		if (tmp->name) 
			link_list_add_head(&tokens,tmp->name);		
		tmp = tmp->father;
	} while (tmp);

	name = link_list_covert_to_string (&tokens,sep);
	//	}
	//	else
	//		name =0;

	if (name)
	{
		full_name = malloc(strlen(name) + strlen(path)+5);
		sprintf(full_name,"%s%s%s",name,sep,path);
		free(name);
	}
	else {
		full_name = malloc( strlen(path)+5);
		sprintf(full_name,"%s",path)	;	
	}

	link_list_deinit(&tokens);

	return full_name;	

	//realization_t  *realization
}

/**
 * @created 05/21/15
 * @brief   creare assiments of the module
 * @param   
 * @return  
 */
void realization_create_instance(realization_t  *realization,node_manager_t *node_manager) {

	char *up,*full_name_left,*full_name_right ;
	module_t *module = realization->module;


	link_list_for_each (assign_t *assign, &realization->module->assignments) {

		full_name_left = realization_get_full_name(realization,assign->left);
		node_t *left_node =  node_find_by_name (node_manager,full_name_left);



		full_name_right = realization_get_full_name(realization,assign->right);
		node_t *right_node =  node_find_by_name (node_manager,full_name_right);

		output_t *output_left = node_manager_find_output_by_name(node_manager,full_name_left);
		input_t  *input_right = node_manager_find_input_by_name(node_manager,full_name_right);

	//	node_t *node = node_find
	//	node->data = realization;


		if (left_node && right_node && !output_left && !input_right) {
#ifdef DEBUG_PARSER
			printf("1111. %s.OUT --> %s.IN\n",left_node->name,right_node->name);
#endif
			input_t *input = node_get_input(right_node,0);
			output_t *output = node_get_output(left_node,0);
			input->output = output;
		} else

			if (!left_node && right_node && output_left && !input_right) {
#ifdef DEBUG_PARSER
				printf("2222. %s --> %s.IN\n",output_left->name,right_node->name);
#endif
				input_t *input = node_get_input(right_node,0);
				input->output = output_left;

			} else
				if (left_node && !right_node && !output_left && input_right) {
#ifdef DEBUG_PARSER
					printf("3333. %s.OUT --> %s\n",left_node->name,input_right->name);
#endif
					output_t *output = node_get_output(left_node,0);
					input_right->output = output;
				} else 
					if ( output_left &&  input_right && !left_node && !right_node) {
#ifdef DEBUG_PARSER
						printf("4444. %s --> %s\n",output_left->name,input_right->name);
#endif
						input_right->output = output_left;
					}  else {
						printf("5555. error ");
						if (left_node) 
							printf("%s  .",left_node->name);
						if (right_node) 
							printf("%s  .",right_node->name);
						if (output_left) 
							printf("%s  .",output_left->name);
						if (input_right) 
							printf("%s  ",input_right->name);
						printf("\n");




					}


	}

	link_list_for_each (realization_t *tmp_realization , &realization->realizations) {
		if (tmp_realization->realizations.list_len)
			realization_create_instance(tmp_realization,node_manager);
	}		



}


int parse(const char *text,char *top_name,int *col_out,int *line_out,char **msg)
{
	stack_new(&stack,1024*100);
	struct yy_buffer_state * my_string_buffer = yy_scan_string(text);

	module_t *terminal_top_node;	
	int level=0;

	// initiate top level module and push it to stack
	terminal_top_node = (module_t*)malloc(sizeof(module_t));
	//<t_úX>	terminal_top_node->level = level++;
	terminal_top_node->father =0 ;
	terminal_top_node->is_init = 0;
	link_list_init(&terminal_top_node->inputs);
	link_list_init(&terminal_top_node->outputs);
	link_list_init(&terminal_top_node->instantiations);
	link_list_init(&terminal_top_node->declarations);
	terminal_top_node->name = top_name;
	stack_push(stack,&terminal_top_node,sizeof(module_t*) ); // terminal_node_t


	// initiate top level realization
	realization_t *realization =  realization_new(terminal_top_node,0,0);


	// run flex code from file
	yy_switch_to_buffer( my_string_buffer ); // switch flex to the buffer we just created
	yyparse(); 
	yy_delete_buffer(my_string_buffer );

	// initiate node manager
	node_manager_t *node_manager;
	node_manager_init(&node_manager,255,8); //test1_init);


	// create instance
	module_create_instance(terminal_top_node,realization,node_manager);
	realization_create_instance (realization,node_manager);
	node_manager_eleiminate_nodes (node_manager);
	// create python code
	python_create("res.py",node_manager,"top_res");

	// save yacc error message paramaters
	msg[0] = yytext;
	*col_out = col;
	*line_out = line;


	// pop top module from stack andfree it
	stack_pop(stack,sizeof(module_t*));
	stack_free(stack);
	return 0;
}

/**
 * Created  05/16/2015
 * @brief   finds complete node name  (from button to top)
 * @param   b_node_io_t  
 * @param  sep: pointer c=to char which is seprator (example: . _ )
 * @return  name as string
 */
char * node_io_get_full_name(realization_t *realization,char *sep)
{
	char tmp[256];
	char *name = malloc(256);
	//	char *tmp = name;
	tmp[0]=0;
	name[0]=0;
	// get names from button to top
	while (realization->name) { 
		if (realization) {
			strcat (tmp,realization->name);
		}
		if (realization->father)
			realization = realization->father;
		else
			realization = 0;
		if (node_io)
			strcat (tmp,sep);

	}
	// swap the string name

	link_list_t tokens;
	link_list_init(&tokens);
	char *pch = strtok (tmp,sep);
	while (pch != NULL)
	{
		link_list_add_head(&tokens,pch);
		pch = strtok (NULL, sep);
	}
	link_list_for_each(char *toekn,&tokens){
		strcat (name, link_list_get_head(&tokens));
		strncat (name,sep,1);	
	}
	name[strlen(name)-1]=0;

	return name;
}

/**
 * created 3-5-2015
 * @brief   copy  var to  var
 * @param   var  - source variable
 * @param   var_to_copy_to - destanation
 * @return  void
 */
void var_copy(var_t *var,var_t *var_to_copy_to)
{
	memcpy (var_to_copy_to,var,sizeof(var_t));
}


/**
 * @brief   create new var_t
 * @param   field of var_t, integer value
 * @return  pointer to var_t
 */
var_t *var_new_integer(char *name,int val)
{

	var_t *var = (var_t*) malloc(sizeof(var_t));
	var->type = INTEGER;
	var->value.integer = val;
	var->name = name;
}

/**
 * @brief   create new var_t
 * @param   field of var_t, integer value
 * @return  pointer to var_t
 */
var_t *var_new_string(char *name,char *string)
{
	var_t *var = (var_t*) malloc(sizeof(var_t));
	var->type = STRING;
	var->value.string = string;
	var->name = name;	
	return var;
}


/**
 * @brief   create new var_t
 * @param   field of var_t, integer value
 * @return  pointer to var_t
 */
var_t *var_new_double(char *name,double val)
{
	var_t *var = (var_t*) malloc(sizeof(var_t));
	var->type = DOUBLE;
	var->value.real = val;
	var->name = name;	
	return var;
}

var_t *var_new_empty(char *name)
{
	var_t *var = (var_t*) malloc(sizeof(var_t));
	var->type = -1;
	var->name = name;	
	return var;
}

var_t *var_new_copy (var_t *var_in)
{
	var_t *var = (var_t*) malloc(sizeof(var_t));
	memcpy (var,var_in,sizeof(var_t));

	return var;
}


instace_t * instance_new()
{	
	instace_t *instance;
	if ((instance = malloc(sizeof(instace_t))) == NULL)
		yyerror("out of memory at instance_new");
	//	instance->pushed_node.instance = 0;
	//	instance->pushed_node.input = 0;

	return instance;

}


/**
 * @created 05/06/15
 * @brief   scans modules declare and find if the instance is valid for it (exist in its records). also sets module declatration to instance
 * @param   module_declare - predifined module_declare_t
 * @param   instance_t - predifined instance
 * @return  1 - exist /0 not exist
 **/
module_t * module_is_valid_instance(module_t  *module_declare, instace_t *instance)
{
	module_t *tmp;

	// fnid in the module local list
	link_list_set_current_to_head(&module_declare->declarations);
	do {
		tmp = link_list_get_current_and_next(&module_declare->declarations);
		if (tmp && strcmp(tmp->name,instance->var_type)==0){
			//	instance->node_io.terminal_node = module_copy (tmp);
			// OK
			return tmp;
		}
	} while (tmp);


	// do iteration of list of modulef athers
	if (module_declare->father)
		return module_is_valid_instance(module_declare->father,instance);

	return 0;
}


void module_init (module_t *module_declare,char *name)
{
	module_declare = malloc(sizeof(module_declare_t));
	module_declare->name = name;
	//module_declare->level = level;
	module_declare->is_init = 0;
	module_declare->father = 0;
	link_list_init(&module_declare->inputs);
	link_list_init(&module_declare->outputs);
	link_list_init(&module_declare->instantiations);
	link_list_init(&module_declare->declarations);

}

module_t * module_new ()
{
	module_t *module_declare = (module_t*)malloc(sizeof(module_t));
	module_init(module_declare,0);
	return module_declare;

}

/**
 * @created 05/19/15
 * @brief   
 * @param   
 * @return  
 */
realization_t *realization_new(module_t *module,realization_t *father_realization,char *name)
{
	realization_t * realization = (realization_t*)malloc(sizeof(realization_t));
	realization -> father = father_realization;
	realization -> module = module;
	realization -> name = name;
	link_list_init (&realization->realizations);
	return realization;
}




/**
 * @created 05/19/15
 * @brief   
 * @param   
 * @return  
 */
assign_t *assign_new(link_list_t *left,link_list_t *right)
{
	assign_t *assign =(assign_t*) malloc(sizeof(assign_t));

	assign->left = link_list_covert_to_string(left,sep);
	assign->right = link_list_covert_to_string(right,sep);


	return assign;

}
