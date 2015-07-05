#ifndef PYTHON_MAKER_H
#define PYTHON_MAKER_H
#include "parser.h"
#include "node.h"


char python_write_file(char *filename);

char *python_generator_class_header(node_manager_t *node_manager,char *name); 
//char  * python_generator_hyrarchic_name(terminal_node_t *top_node);
void python_create(char *filename, node_manager_t *node_manager,char *top_name);



void python_generator_node_definition(node_manager_t *node_manager);







#endif
