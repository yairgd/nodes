/*
 * =====================================================================================
 *
 *       Filename:  python_maker.c
 *
 *    Description:  this class genetars the python code. The python code is generate in two buffers and 
 *    		    after that it is been write to file one after other.
 *    		    1. first buffer contains the generator class. this class contains all the connections definition between node objectys
 *    		    2. second buffer contais the python class relates to eahc node
 *
 *        Version:  1.0
 *        Created:  05/06/15 09:50:14
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "python_maker.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *buffer1,*buffer2;


char *node_text="\
		 node_manager=Algo.Nodes.Node.NodeManager(); \n \
		 ";


char *init_text1="\
\n\n\n##{Generator \n\
# initiate node_manager with %d workers and %d nodes \n\
";






/**
 * Created  05/29/2015
 * @brief   prints node connection to python  file
 * @param   
 * @return  
 */

void print_connection(input_t *input , output_t *output)
{

}

/**
 * @created 05/18/15
 * @brief   the function retrive the pushed node 
 * @param   io : pointer to pushing node . input node that pusheh until it get to final node. both input and output node has no instantiations (final nodes)
 * @return  null = if no pushed node or pointer id success
 */
b_node_io_t * python_generator_get_pushed_node(b_node_io_t *io)
{

}

/**
 * @created 05/07/15
 * @brief   this functions genrates and defines classes pyhthon according to the nodes defined in the script file
 * 	    also, links between instance to its father
 * @param   
 * @return  
 */
void python_generator_node_definition(node_manager_t *node_manager)
{
	// create python node definition
	list_foreach(node_t ** node,&node_manager->nodes) {
		realization_t *realization = (*node)->data;
		if (realization) {
			sprintf(&buffer1[strlen(buffer1)],"		self.%s = %s(self.node_manager)\n",(*node)->name,realization->module->name); // todo:  add var list

		}
		else
		{

		}
		

		if (realization && realization->module->is_init==0) 
		{
			realization->module->is_init = 1;
			char *var_list = link_list_covert_to_string(&realization->module->inputs,",");
			sprintf(&buffer2[strlen(buffer2)],"class %s(Algo.Nodes.Node.Node):\n",realization->module->name);
			sprintf(&buffer2[strlen(buffer2)],"	def __init__(self,node_manager):\n");  // todo:  add var list
			sprintf(&buffer2[strlen(buffer2)],"		super(%s,self).__init__(%d,%d,%d,node_manager)\n",realization->module->name,(*node)->in.len,(*node)->out.len,1);				
			sprintf(&buffer2[strlen(buffer2)],"		pass\n");			
			sprintf(&buffer2[strlen(buffer2)],"	def callback(self,%s):\n",var_list);
			sprintf(&buffer2[strlen(buffer2)],"		pass\n");
			free(var_list);
		}


	}


	// create python node cnnections
	list_foreach(input_t **input,&node_manager->inputs) {
		node_t *node = (*input)->node;
		output_t *output = (*input)->output;
		
//		realization_t *realization = (*node)->data;
//		if (realization)
//			print_node( realization->module->name,*node );
		//	else
		//		print_node( (*node)->name,realization->module->name,node );


	}


}


/**
 * Created  05/30/2015
 * @brief   prints the node connections to python file
 * @param   
 * @return  
 */
void python_generator_node_conection(node_manager_t *node_manager)
{
	
}


char python_write_file(char *filename)
{

	FILE *file = fopen(filename,"w");
	fprintf(file,"%s",buffer2);
	fprintf(file,"%s",buffer1);
	fclose(file);

}

void python_create(char *filename, node_manager_t *node_manager,char *top_name)
{
	buffer1 = (char*)calloc(100000,1);
	buffer2 = (char*)calloc(100000,1);
	int n_worker=node_manager->worker_list.workers.len;
	int n_nodes=node_manager->nodes.len;

	// create generator class header 
	sprintf(buffer1,init_text1,n_worker,n_nodes);

	sprintf(&buffer2[strlen(buffer2)],"import Algo\n");

	sprintf(&buffer1[strlen(buffer1)],"class %s:\n",top_name);	

	// inittiates all terminal nodes
	sprintf(&buffer1[strlen(buffer1)],"	def __init__(self):\n"); // todo:  add var list
	sprintf(&buffer1[strlen(buffer1)],"		self.node_manager=Algo.Nodes.Node.NodeManager(%d,%d)\n",n_nodes,n_worker);

	sprintf(&buffer1[strlen(buffer1)],"		#Initiates nodes\n");		
	python_generator_node_definition(node_manager);		


	sprintf(&buffer1[strlen(buffer1)],"		#Create connections\n");		
	python_generator_node_conection(node_manager);		

	sprintf(&buffer1[strlen(buffer1)],"top = %s()\n",top_name); // todo:  add var list	
	
	// close python file
	python_write_file(filename);

	free(buffer1);
	free(buffer2);
}



