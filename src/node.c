/*
 * =====================================================================================
 *
 *       Filename:  node.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  27/10/13 23:12:13
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */
#include "node.h"
#include "list.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>     
#include <signal.h>
#include <time.h>
#include <stddef.h>
#include <stdbool.h>
//#include <semaphore.h>  /* Semaphore */

//sem_t g_mutex;
/**
 * Created  10/11/2014
 * @brief   inititates value class
 * @param   
 * @return  none
 */
void value_init(value_t *value)
{
	value->len= 0 ;
	value->val = 0;
}


/**
 * Created  10/11/2014
 * @brief   alloaces place for new value class
 * @param   value: pointer to preallocated  value_t
 * @return  true/falase
 */
char value_new(value_t *value,int len)
{
	value->len = len;
	value->val =  (void*)malloc(len);
	memset (value->val,0,len);
	return 0;
}

/**
 * Created  10/11/2014
 * @brief   free value
 * @param   
 * @return  
 */
void value_free(value_t *value)
{
	if (value->val)
		free(value->val);
	value->val=0;
	value->len = 0;
}

/**
 * Created  10/11/2014
 * @brief   copy source value  to destanation value
 * @param   value: preallocated and initiated value_t 
 * @param   des:  preallocated and initiated value_t 
 * @return  
 */
char value_copy(value_t *value,value_t *des)
{
	assert(!(des->len!=value->len));
	des->len = value->len;
	//	des->val = (void*)malloc(value->len);
	memcpy (des->val,value->val,value->len);
	return 0;
}



/**
 * Created  10/18/2014
 * @brief   sets new value to value_t structure.
 * @param   value: preallocated value_t
 * @param   val: pointer to memory to copy to value_t
 * @return  none
 */
void value_set(value_t *value,void *val)
{
	memcpy (value->val,val,value->len);
}



void output_manage_suspend(output_t * io) {
	pthread_mutex_lock(&io->mutex);
	if (io->suspended) {
		pthread_cond_wait(&io->m_ResumeCond, &io->mutex);
	}
	pthread_mutex_unlock(&io->mutex);

}

void output_resume(output_t * io)
{

	/* The shared state 'suspended' must be updated with the mutex held. */
	pthread_mutex_lock(&io->mutex);
	io->suspended = 0;	
	pthread_cond_signal(&io->m_ResumeCond);
	pthread_mutex_unlock(&io->mutex);


}
void output_suspend(output_t * io)
{
	pthread_mutex_lock(&io->mutex);	
	io->suspended = 1;
	pthread_mutex_unlock(&io->mutex);	
}

void output_init(output_t *io)
{
	io->suspended = 1;
	io->fan_out=0;	
	pthread_attr_init(&io->attr);
	pthread_cond_init(&io->m_ResumeCond, NULL);
	pthread_mutex_init(&io->mutex, NULL);

}

void output_deinit(void *output)
{
	if (output) {
		//free( ((output_t*)output)->val);
		value_free(&((output_t*)output)->value);
		free( ((output_t*)output)->slot);
		free( ((output_t*)output));
	}
}


/**
 * @created 05/25/15
 * @brief   return new initiated output_t 
 * @param   
 * @return  
 */
output_t   *output_new() 
{
	output_t *output = (output_t*)malloc(sizeof(output_t));
	if (output==0)
		return 0;
	output->num_of_used_out = 0;
	output->node = 0;	
	output_init(output);
	return output;

}

/**
 * @created 05/25/15
 * @brief   return new initiated output_t 
 * @param   
 * @return  
 */
input_t   *input_new() 
{
	input_t *input = malloc(sizeof(input_t));
	input->output = 0;
	input->node = 0;
	if (input==0)
		return 0;
	return input;

}


void callback_init(callback_t **callback,run_t run,int id,void *prm,prm_deinit_t prm_deinit)
{
	*callback = (callback_t*)malloc(sizeof(callback_t));
	(*callback)->run = run; 
	(*callback)->prm = prm;
	(*callback)->id  = id;
	(*callback)->prm_deinit  = prm_deinit;
	(*callback)->lock=0;
}
void callback_deinit(callback_t *callback)
{
	if (callback) {
		if (callback->prm_deinit)
			callback->prm_deinit(callback->prm);
		free(callback);
	}

}


/**
 * @brief   checksif all inputs that has the same is as the callback id are ready
 * @param   callback: callback object
 * @param   io_list: list of io
 * @return  1 ok, 0 callbak not ready o use
 */
char callback_check_active(callback_t *callback,list_t *inputs)
{
	int i;
	output_t *output ;
	input_t *input;
	for (i=0;i<inputs->len;i++)
	{
		input = *(input_t**)list_get (inputs,i);
		if (!input)
			return 0;

		output = input->output;
		//if  (!output)
		//	return 0;

		// if slot not ready and the checked input has the same id as the callback group id, return 0 (callbak not ready o use)
		//	if (output && output->id==callback->id && output->slot[ input->slot_idx]==0)
		if (output && input->id==callback->id && output->slot[ input->slot_idx]==0)
			return 0;
	}
	return 1;
}

void node_init(node_t **node_out,int num_of_input,int num_of_out,int num_of_callbacks,node_manager_t *node_manager) //run_t run,void *prm)
{
	int i;

	node_t *node ;
	node = (node_t*)malloc(sizeof(node_t));
	node_out[0] = node;

	// init in port list
	LIST_INIT(node->in,input_t* ,num_of_input);

	for (i=0;i<num_of_input;i++) 
	{
		input_t *input = malloc(sizeof(input_t));
		input->output = 0;
		list_add(&node->in,&input);
		list_add(&node_manager->inputs,&input);
		input->node = node;
	}


	// inittia output node
	LIST_INIT(node->out,output_t* ,num_of_out);

	// make each in port as suspened. it will resume by another output node 
	for (i=0;i<num_of_out;i++) 
	{
		output_t *output = (output_t*)malloc(sizeof(output_t));
		output->num_of_used_out = 0;
		output_init(output);	
		list_add(&node->out,&output);
		list_add(&node_manager->outputs,&output);
		output->node = node;		

	}


	// initiate callback list
	LIST_INIT(node->callbacks,callback_t *,num_of_callbacks);
	node->node_manager = node_manager;
	node_manager_add_node(node_manager,node);

	//pthread_mutex_init(&node->mutex, NULL);

}

/**
 * @created 05/25/15
 * @brief   this functions initiates node with predeinef inputs and outputs
 * @param   
 * @return  
 */
void node_init_by_ios(node_t **node_out,list_t *inputs, list_t *outputs,node_manager_t *node_manager) 
{
	int i;
	node_t *node ;
	node = malloc(sizeof(node_t));
	node_out[0] = node;

	int  num_of_input = list_size(inputs);
	// init in port list
	LIST_INIT(node->in,input_t* ,num_of_input);

	for (i=0;i<num_of_input;i++) 
	{
		input_t *input=*(input_t**)list_get(inputs,i);
		list_add(&node->in,&input);
		input->node = node;
	}

	int  num_of_out = list_size(outputs);

	// inittia output node
	LIST_INIT(node->out,output_t* ,num_of_out);

	// make each in port as suspened. it will resume by another output node 
	for (i=0;i<num_of_out;i++) 
	{
	
		output_t *output=*(output_t**)list_get(outputs,i);
		list_add(&node->in,&output);
		output->node = node;

	}


	// initiate callback list
	LIST_INIT(node->callbacks,callback_t *,1);
	node->node_manager = node_manager;
	node_manager_add_node(node_manager,node);

}

/**
 * @brief   return pointer to node value and reset the slot input
 * @param   node: pointer to node
 * @param   idx: index of output
 * @return  
 */
void node_get_input_value(node_t *node,int idx,void **val)
{
	assert(!(idx>=node->in.len));
	if (idx>=node->in.len)
		return ;
	pthread_mutex_lock(&node->node_manager->mutex); // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@2	
	input_t *input = *(input_t**)list_get(&node->in,idx);

	if (input && input->output)
		//	*val =  input->output->value.val;
		*val = input->value_copy.val;
	else
		*val = NULL;
	pthread_mutex_unlock(&node->node_manager->mutex);// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	// return true if tha
	//	if (input->output && input->output->slot[ input->slot_idx])
	//		return 1;
	//	return 0;

}


void *node_get_output_value(node_t *node,int idx)
{
	//assert(!(idx>=node->out.len));
	if ((idx>=node->out.len))
		return NULL;
	output_t * io = *(output_t**)list_get(&node->out,idx);
	if (io)
		return io->value.val;
	return NULL;
}

/**
 * @created 05/25/15
 * @brief   returns pointer to output_t by index
 * @param   
 * @return  
 */
output_t *node_get_output(node_t *node,int idx)
{
	//assert(!(idx>=node->out.len));
	if ((idx>=node->out.len))
		return NULL;
	output_t * io = *(output_t**)list_get(&node->out,idx);
	if (io)
		return io;
	return NULL;
}

/**
 * @created 05/25/15
 * @brief   returns pointer to input_t by index
 * @param   
 * @return  
 */
input_t *node_get_input(node_t *node,int idx)
{
	//assert(!(idx>=node->out.len));
	if ((idx>=node->in.len))
		return NULL;
	input_t * io = *(input_t**)list_get(&node->in,idx);
	if (io)
		return io;
	return NULL;
}


void node_deinit(void *node_in)
{
	node_t *node = node_in;
	int i,len;
	// free in list
	len = node->in.len;
	for (i=0;i<len;i++)
	{
		input_t *input = *(input_t**)list_get(&node->in,i);
		free(input);
	}
	list_deinit(&node->in);

	// free output list
	len = node->out.len;
	for (i=0;i<len;i++)
	{
		output_t *output = *(output_t**)list_get(&node->out,i);
		output_deinit(output);
	}
	list_deinit(&node->in);

	// free callback list
	len = node->callbacks.len;
	for (i=0;i<len;i++)
	{
		callback_t *callback = *(callback_t**)list_get(&node->callbacks,i);
		callback_deinit(callback);
	}
	list_deinit(&node->callbacks);


}

/**
 * @brief  connect output node to the required input in 
 * @param  out_node: the node that output comes from
 * @param  port_out: index ot output
 * @param  in_node: the nodethat input is connected to 
 * @param  port_in: index of input port
 * @param  id: the id of the input
 * @return  
 */
void node_connect(node_t *out_node,int port_out,node_t *in_node,int port_in,int id)
{
	output_t *out =  *(output_t**)list_get(&out_node->out,port_out);
	input_t *input =  *(input_t**)list_get(&in_node->in,port_in);
	input->output = out;

	value_new (&input->value_copy,out->value.len);
	//	value_copy(&out->value,&input->value_copy);

	//	out->id=id;
	input->id = id;
	// save the cuurent output fanout as the slot index, increase the output fan out 
	input->slot_idx = out->fan_out++;

}

/**
 * @brief   connect output node to the required input in 
 * @param   
 * @return  
 */
void node_set_input(node_t *node,output_t *out,int port_in, int id,void *val)
{
	input_t *input =  *(input_t**)list_get(&node->in,port_in);
	input->output = out;
	//	out->id=id;
	input->id = id;
	// save the cuurent output fanout as the slot index, increase the output fan out 
	input->slot_idx = out->fan_out++;
	//value_copy(&out->value,&input->value_copy)

	// create value_copy accorintg to the pointed output value 
	value_new (&input->value_copy,out->value.len);

	value_set (&input->value_copy,&val);

}




/**
 * Created  19/02/14
 * @brief   add new inpout to node
 * @param   node: pointer to node
 * @param   id: uses ads node id, this id is trippled with callback function of the node,  and outputs with the same id
 * @return  1: ok 0: fails (zeor option not implemented is it neccecery ???????)
 */
char node_add_input(node_t *node, int id)
{
	input_t input;
	input.output = 0;
	input.id = id;
	list_add(&node->in,&input);
	return 1;
}

/**
 * Created  15/02/14
 * @brief   adds new callback to node callbacks list
 * @param   node: node structute
 * @param   id: the callback id
 * @param   run: function pointer of callback from type run_t
 * @param   prm: paramater to callback function
 * @return  the new crated callbak pointer
 */
//#include "Python.h"
callback_t * node_add_callback(node_t *node,int id,run_t run,void *prm,prm_deinit_t prm_deinit)
{
	callback_t *callback ; 
	callback_init(&callback,run,id,prm,prm_deinit);

	//	Py_INCREF(prm);	


	//	callback.run = run; 
	//	callback.prm = prm;
	//	callback.id  = id;
	//	callback.lock=0;
	list_add(&node->callbacks,&callback);
	//	return list_get(&node->callbacks,node->callbacks.len-1);
	return callback;

}

/**
 * Created  16/02/14
 * @brief   set function pointer for required callback id.
 * @param   node: node_t type
 * @param   run_t: pointer to callback function
 * @param   prm: unque parametr for each callback
 * @param   id: the required id
 * @return  0: callback id not exist, 1: OK
 */
/*  
    char node_set_callback(node_t *node,run_t run, void *prm,int id)
    {
    callback_t *callback;
    int i;
    for (i=0;i<node->callbacks.len+1;i++)
    {
    callback = list_get(&node->callbacks,i);
    if (callback->id == id)
    {
    callback->run = run;
    callback->prm = prm;
    return 1;
    }
    }
    return 0;
    }
    */

void node_trig_output(node_t *node,int i)
{
	output_t *output;
//	callback_t *callback;
	int i_slot;
	node_manager_t *node_manager = node->node_manager;

	assert(!(i>=node->out.len));		
	output = *(output_t**)list_get(&node->out,i);

	// 1. reset all output slots
	// 2. reset the num_of_used_out to be as the number of connected inputs (saved in fan_out)
	pthread_mutex_lock(&node_manager->mutex);				
	output->num_of_used_out = output->fan_out;
	for ( i_slot = 0;i_slot< output->fan_out;i_slot++)
		output->slot[i_slot]=1;

	//	memcpy(io->val,val,io->len);	

	//	for (i=0;i<node->callbacks.len;i++)
	//	{
	//		callback = list_get (&node->callbacks,i);
	//		callback->lock = 1;
	//	}

	// make node active
	output_resume(output);	
	pthread_mutex_unlock(&node_manager->mutex);

}




/**
 * @created 05/26/15
 * @brief   
 * @param   
 * @return  
 */
node_t * node_find_by_name(node_manager_t *node_manager,char *name)	
{
	list_foreach (node_t **node, &node_manager->nodes) {
		if ((*node)->name && strcmp( (*node)->name,name)==0)
			return *node;
	}
	return 0;
}


/**
 * @created 05/26/15
 * @brief   
 * @param   
 * @return  
 */
node_t * node_find_by_index(node_manager_t *node_manager,int idx)	
{
	list_foreach ( node_t **node, &node_manager->nodes) {
		if (idx==0)
			return *node;
		idx--;
	}
	return 0;
}

/**
 * Created  05/23/2015
 * @brief   sets the input node name
 * @param   node: pointer to initiated node_t
 * @param   idx: the input index
 * @param   name: the pointer to the name string
 * @return  
 */
void node_set_input_name(node_t *node,int idx,char *name)
{
	input_t *input = *(input_t**)list_get(&node->in,idx);
	input->name =  malloc(strlen(name));
	sprintf(input->name,"%s",name);	
}

/**
 * Created  05/23/2015
 * @brief   sets the output node name
 * @param   node: pointer to initiated node_t
 * @param   idx: the output index
 * @param   name: the pointer to the name string
 * @return  
 */
void node_set_output_name(node_t *node,int idx,char *name)
{
	output_t *output = *(output_t**)list_get(&node->out,idx);
	output->name =  malloc(strlen(name));
	sprintf(output->name,"%s",name);	
}


/**
 * @created 05/21/15
 * @brief   locates inputs by name
 * @param   
 * @return  
 */
input_t * node_manage_find_node_by_input_name(node_manager_t *node_manager,char *name)
{
	list_foreach (node_t *node, &node_manager->nodes) {
		list_foreach(input_t *input,&node->in) {
			if (input->name && strcmp(input->name,name)==0)
				return input;
		}
	}
}




/**
 * @brief   initate node_list and active workers
 * @param   num_of_nodes: number of nodes to initiate
 * @param   num_of_workers: nuber of workers
 * @return  none
 */
void node_manager_init(node_manager_t **node_manager_in,int num_of_nodes,int num_of_workers)
{

	node_manager_t *node_manager = (node_manager_t*)malloc(sizeof(node_manager_t));
	node_manager_in[0] = node_manager;
	worker_list_init ( &node_manager->worker_list,num_of_workers);
	LIST_INIT(node_manager->nodes,node_t *,num_of_nodes);
	worker_list_start( &node_manager->worker_list);
	pthread_mutex_init(&node_manager->mutex, NULL);

	LIST_INIT (node_manager->inputs,input_t*,256);
	LIST_INIT (node_manager->outputs,output_t*,256);


}

/**
 * @brief   deinitates worker_list and node_list
 * @param   
 * @return  
 */
void node_manager_deinit(void *node_manager_in)
{
	int i,len;
	node_manager_t *node_manager = node_manager_in;
	len = node_manager->nodes.len;
	for (i=0;i<len;i++)
	{
		node_t **node = *(node_t**)list_get (&node_manager->nodes,i);
		node_deinit(*node);
	}
	list_deinit(&node_manager->nodes);


	//	worker_list_deinit ( &node_manager.worker_list);
	//	//LIST_INIT(node_manager.nodes,node_t *,num_of_nodes);
	//	worker_list_stop( &node_manager.worker_list);
	//	//pthread_mutex_init(&node_manager.mutex, NULL);

}


/**
 * @brief   allocate place for new node, copy original (usualy temporary on stack) node content to new place and stores the pointer to new node in list
 * @param   node: pointer to temporary node content
 * @return  none
 */
void node_manager_add_node( node_manager_t *node_manager,node_t *node)
{
	//	node_t *node1 = (node_t*) malloc(sizeof(node_t));
	//	memcpy (node1,node,sizeof(node_t));
	//	list_add (&node_manager.nodes,&node1);
	list_add (&node_manager->nodes,&node);


}

void node_manager_steps_loop(node_manager_t *node_manager,int num_of_steps)
{
	int i_step,i_node,i_in;
	node_t *node;
	int tot_nodes = node_manager->nodes.len;
	input_t *input;

	for (i_step=0;i_step<num_of_steps;i_step++)
	{
		node_manager_step( node_manager);
		
	 	list_foreach(input_t **input, &node_manager->inputs) {
			value_copy(&(*input)->output->value,&(*input)->value_copy);
		}
	//	for (i_in = 0;i_in<list_size(&node_manager->inputs);i_in++) {
	//		input = *((input_t**)list_get(&node_manager->inputs,i_in));
	//		value_copy(&input->output->value,&input->value_copy);
	//	}


		/*  
		// update inputs with the last utput value
		for (i_node= 0 ;i_node<tot_nodes;i_node++)
		{
			node = *((node_t**) list_get ( &node_manager->nodes,i_node));			
			for (i_in = 0; i_in<list_size(&node->in);i_in++)
			{
				input = *((input_t**)list_get(&node->in,i_in));
				value_copy(&input->output->value,&input->value_copy);

			}

		}
		*/

	}
}


void node_manager_step(node_manager_t *node_manager)
{
	int i_node;//,i_input;//,i_in;
	node_t *node;
	// input_t *input;
//	int len = node_manager->worker_list.workers.len ;
	int tot_nodes = node_manager->nodes.len;
	node_manager->worker_list.counter = 0;
	for (i_node= 0 ;i_node<tot_nodes;i_node++)
	{
		node = *((node_t**) list_get ( &node_manager->nodes,i_node));		
		worker_list_trigger ( &node_manager->worker_list, node );

	}
	char b=1;
	do {
		pthread_mutex_lock(&node_manager->worker_list.mutex);
		if (node_manager->worker_list.counter  ==node_manager->nodes.len)
			b=0;
		pthread_mutex_unlock(&node_manager->worker_list.mutex);

	} while (b);
}

/**
 * @brief   thread function to run node_manager_step
 * @param   none
 * @return  none
 */
void  * node_manager_run(  void *prm) //,worker_list_t * worker_list)
{
	node_manager_thread_prm_t *node_manager_thread_prm = prm;
	int n = node_manager_thread_prm->num_of_steps;
	node_manager_t *node_manager = node_manager_thread_prm->node_manager;
	node_manager_steps_loop(node_manager,n);
	thread_suspend(&node_manager->tid);
	
	return 0;
}


/**
 * Created  05/30/2015
 * @brief   this function remove all nodes middles nodes exampe: 1->2->3 the function remove 2 and connect 1->3 
 * @param   initiated node_manager_t *
 * @return  none
 */
void *node_manager_eleiminate_nodes(node_manager_t *node_manager) 
{

}


/**
 * @created 05/26/15
 * @brief   
 * @param   
 * @return  
 */
output_t * node_manager_find_output_by_name(node_manager_t *node_manager,char *name)
{
	list_foreach ( output_t **output, &node_manager->outputs) {
		if ((*output)->name && strcmp( (*output)->name,name)==0)
			return *output;
	}
	return 0;
}

/**
 * @created 05/26/15
 * @brief   
 * @param   
 * @return  
 */
input_t * node_manager_find_input_by_name(node_manager_t *node_manager,char *name)
{
	list_foreach ( input_t **input, &node_manager->inputs) {
		if ((*input)->name && strcmp( (*input)->name,name)==0)
			return *input;
	}
	return 0;
}





#ifdef TO_REMOVE
/**
 * @created 05/23/15
 * @brief   locates outputs by name
 * @param   
 * @return  
 */
output_t * node_manage_find_node_by_output_name(node_manager_t *node_manager,char *name)
{
	list_foreach (node_t *node, &node_manager->nodes) {
		list_foreach(output_t *output,&node->out) {
			if (output->name && strcmp(output->name,name)==0)
				return output;
		}
	}
	return 0;
}




/**
 * @brief   manage the worker activation. runs as thread in the backgraound
 * @param   none
 * @return  none
 */
void node_manager_step(node_manager_t *node_manager,int num_of_steps)
{
	int i,j,k;
	output_t *out;
	node_t *node;
	node_t *first_node = *((node_t **) list_get(&node_manager->nodes,0));

	if (node_manager->nodes.len==0)
		return ;

	if (1)
	{
		for (i=0;i<node_manager->nodes.len;i++)
		{
			node = *((node_t **) list_get(&node_manager->nodes,i));
			for (j=0;j<node->callbacks.len;j++)
			{
				callback_t *callback = *((callback_t**)list_get(&node->callbacks,j));
				callback_release_inputs(callback,node);
			}
		}
	}
//	char first_round = 1;

	while (num_of_steps>0)
	{
bb:
	//	usleep(1);

		// take the node at the top of the list and remove it. the folowing code will check if this node is vbalid to

		#ifdef PYTHON
	 if(PyErr_CheckSignals() == -1) {
		 sleep(5);
           return;
       }
#endif
		// be run, if not it will return to the top of the list at aa lable.
		while (node_manager->nodes.len==0);

		pthread_mutex_lock(&node_manager->mutex);		
		node = *((node_t **) list_get(&node_manager->nodes,0));
		list_remove(&node_manager->nodes,0);
		pthread_mutex_unlock(&node_manager->mutex);


		
//		if  (first_round) 
//			goto hh;
		callback_t *callback;
		int bad_in=0;
		int bad_out=0;
		for (k=0;k<node->callbacks.len;k++)
		{
			callback	 = *((callback_t**)list_get(&node->callbacks,k));


			// check if node is non relavat any more. in this case all connected input were used the corrent node output state
			for (j=0;j<node->out.len;j++)
			{		
				out = list_get(&node->out,j);
				if (out->num_of_used_out!=0 && out->id==callback->id)
				{
					bad_out++;
					//goto aa;


				}
			}

			// check if the output for the node are either:
			// 1. ready (not suspend by the node that produing the data)
			// 2. allready used by the current node
			char b=0;		
			for(i=0;i<node->in.len;i++)
			{

				// take pointer to node
				input_t *input =*(input_t**)list_get(&node->in,i);
				if (callback->id == input->id)
				{

					// if node connected input, skip the input and continue
					if (input->output==0)
						continue;

					if (input->output->suspended==0  &&  input->output->slot[input->slot_idx]==1)
					{
						b=1;
						break;
					}
				}
			}

			if (node->in.len && b==0) 
			 //	goto aa;
			 	bad_in++;
		}

		// ceck if the is at least one callback relate to node that has non used inpus or it output not used by 
		// all connected nodes
		if (bad_in==node->callbacks.len || bad_out==node->callbacks.len)
			goto aa;



		// check for full nodes run round
  	        if (first_node==node) {
			if (!num_of_steps)
				goto aa;
			num_of_steps--;
		}		
		// node is ready, trigger worker with node
		worker_list_trigger ( &node_manager->worker_list, node );
		goto bb;

	
aa:
		pthread_mutex_lock(&node_manager->mutex);		
		list_add(&node_manager->nodes,&node);
		pthread_mutex_unlock(&node_manager->mutex);
	//	k++;
//		if (0 && k==node_manager.nodes.len && k<len) {
//			thread_suspend(&node_manager.tid);
//			printf("%d\n",k);
		//	if (k==8) {
		//		usleep(100);
		//		thread_resume(&node_manager.tid);
		//	}

			//printf("!!!!!!!!!!!!!!!!!!!\n");
//			k=0;
//			usleep(1);
//		}

	}

}
#endif


/**
 * @brief   initiates worker and node manager running
 * @param   none
 * @return  none
 */
void node_manager_start(node_manager_t *node_manager,void *prm)
{
	node_manager->run = 1;
	thread_start(&node_manager->tid,node_manager_run, prm) ;
	thread_resume(&node_manager->tid);
}


void * worker_run (void *prm)
{
	worker_t *worker = prm;
	int i;

//	pthread_mutex_init(&worker->mutex, NULL);
	
	while (1)
	{

//	pthread_mutex_lock(&worker->tid.mutex);
//		pthread_cond_wait(&worker->tid.m_ResumeCond, &worker->tid.mutex);
//	pthread_mutex_unlock(&worker->tid.mutex);
	//	printf ("before trigger  workdr DDDDDDDDDDDDDDDDDDDDdddd111 %d\n",worker);

		// wait untill worker is triggred
		thread_manage_suspend(&worker->tid);
	//	printf ("after trigger  workdr XXXXXXXXXXXXXXXXXXXXXXXXXXXX %d\n",worker);

//		worker->is_active  = 1;
		//	
		node_t *node = worker->node;
	//pthread_mutex_lock(&node->mutex);


	//	if (node==0) {
			//	thread_suspend(&worker->tid);
			//	continue;
	//	}
			node_manager_t *node_manager = node->node_manager;



		// run the node function	
		for (i=0;i<node->callbacks.len;i++)
		{
			callback_t *callback = *((callback_t**)list_get(&node->callbacks,i));
		//	if (callback_check_active(callback,&node->in) && callback->lock) {
				callback->run(node,callback->prm);
			//	callback_release_inputs(callback,node);
			//	callback->lock=0;
		//	}

		}
		

		// return the corrent runnjing node to node_list
//		pthread_mutex_lock(&node_manager->mutex);
//		list_add(&node_manager->nodes,&worker->node);
//		pthread_mutex_unlock(&node_manager->mutex);

		// stop thread

	//	thread_suspend(&worker->tid);


		// add current worker to worker list
	//	pthread_mutex_lock(&node_manager->worker_list.mutex);
	//	printf ("add workdr!!!!!!!!!!!!!111\n");
	
	//	worker_list_add_worker(&node_manager->worker_list,worker);
	//	worker->node=0;

//		sem_post(&g_mutex);      
	//	pthread_mutex_unlock(&node_manager->worker_list.mutex);


pthread_mutex_lock(&node_manager->worker_list.mutex);

			node_manager->worker_list.counter++;
pthread_mutex_unlock(&node_manager->worker_list.mutex);


		thread_suspend(&worker->tid);

//		worker->is_active  = 0;

//	thread_resume(&node_manager.tid);


	}
}







void  worker_list_init (worker_list_t *worker_list, int n)
{
	int i;
	// signal(SIGINT, signal_callback_handler);
	worker_t *worker;

	// initiate worker list with pointers to workers
	LIST_INIT(worker_list->workers,worker_t * ,n);


	for (i=0;i<worker_list->workers.max;i++)
	{
		worker = (worker_t *)malloc(sizeof(worker_t));
		list_add(&worker_list->workers,&worker);
	}


	pthread_attr_init(&worker_list->attr);
	pthread_cond_init(&worker_list->signal, NULL);
	pthread_mutex_init(&worker_list->mutex, NULL);

//	thread_mutex_unlock(&node_manager->mutex);
}


/**
 * @brief   initates all workers thread
 * @param   worker_list: list that contains worker_t pointers
 * @return  none
 */
void  worker_list_start (worker_list_t *worker_list)
{
	static int id=0;	
	int i;
	worker_t *worker;

	for (i=0;i<worker_list->workers.len;i++)
	{
		worker = *(( worker_t**)list_get(&worker_list->workers,i));
		worker->node = 0;
		worker->id = id++;
		thread_start(&worker->tid,worker_run, worker) ;

	}
}

void  worker_list_trigger (worker_list_t *worker_list,node_t *node )
{
	int i=0;
		worker_t *worker;
	callback_t *callback;

	while (1) 
	{
	//for (i_worker = 0; i_worker<=worker_list->workers.len;i_worker++)
	//
		worker = *((worker_t**) list_get ( &worker_list->workers, i));
		if (worker->tid.suspended)
			break;
		i++;
		i%=worker_list->workers.len;

	}
	for (i=0;i<node->callbacks.len;i++)
	{
		callback = *(callback_t**)list_get (&node->callbacks,i);
		callback->lock = 1;
	}


	// trigger worker
	worker->node = node;
	//printf ("woker len %d\n",worker_list->workers.len);


	//	pthread_mutex_lock(&worker->mutex);
	
	//while (worker->tid.suspended==0);
	//
//	while (worker->is_active );
//	printf ("worker to active %d\n",worker);
		thread_resume (&worker->tid);


	//pthread_mutex_unlock(&worker_list->mutex);


}





#ifdef TO_REMOVE
/**
 * @brief   slelects the worker at the tof the list and attach it node to run
 * @param   worker_list: list of workers
 * @param   ndoe: the node that should run with worker
 * @return  
 */
void  worker_list_trigger (worker_list_t *worker_list,node_t *node )
{
	int i;
	callback_t *callback;
	worker_t *worker;

		

	pthread_mutex_lock(&worker_list->mutex);

	// uses this code instead of using: while (worker_list.workers.len==0);
	if (worker_list->workers.len==0) {
//		printf ("now mutex !!\n" );
		pthread_cond_wait(&worker_list->signal, &worker_list->mutex);
//		printf ("now mutex end!!\n");		
	}
//	printf ("workes #%d\n",worker_list->workers.len);


	
	// get first worker
	worker = *((worker_t**) list_get ( &worker_list->workers, 0));

	// remove worker

	list_remove (&worker_list->workers,0);
	printf ("woker len %d\n",worker_list->workers.len);

	for (i=0;i<node->callbacks.len;i++)
	{
		callback = *(callback_t**)list_get (&node->callbacks,i);
		callback->lock = 1;
	}


	// trigger worker
	worker->node = node;
	//printf ("woker len %d\n",worker_list->workers.len);


	//	pthread_mutex_lock(&worker->mutex);
	
	//while (worker->tid.suspended==0);
	//
//	while (worker->is_active );
	printf ("worker to active %d\n",worker);
		thread_resume (&worker->tid);


//
	
//	pthread_mutex_lock(&worker->tid.mutex);
//		pthread_cond_signal(&worker->tid.m_ResumeCond);
	
//	pthread_mutex_unlock(&worker->tid.mutex);

	//pthread_mutex_unlock(&worker->mutex);



	pthread_mutex_unlock(&worker_list->mutex);





}
#endif
void worker_list_add_worker(worker_list_t *worker_list,worker_t *worker)
{
	pthread_mutex_lock(&worker_list->mutex);
	pthread_cond_signal(&worker_list->signal);
	list_add(&worker_list->workers,&worker);

//	pthread_mutex_lock(&worker->mutex);
//		thread_suspend(&worker->tid);
//	pthread_mutex_unlock(&worker->mutex);

	pthread_mutex_unlock(&worker_list->mutex);
}


#define TESET
#ifdef TEST
#include <stdlib.h>
#include <stdio.h>

void  test1_run(node_t *node,void *node_name)
{
	int i;

	char *name = node_name;
	//	printf("node name:%s \n",name);
	for (i=0;i<node->out.len;i++) {
	//	if (strcmp(name,"a1")==0)
	//		printf("!!!!!!!!!!!!!!!!!!!!!!!!  ");
	//	printf ("node %s: calculating out #%d\n",name,i);
		//	usleep(rand() % 1000);
		node_trig_output(node,i);

	}
}
void  test2_run(node_t *node,void *node_name)
{
	int i;

	char *name = node_name;
	//	printf("node name:%s \n",name);
	for (i=0;i<node->out.len;i++) {
		printf ("node %s:  !!!!!!!!!!!!! calculating out #%d\n",name,i);
		//	usleep(rand() % /1000);
//		io->val
		node_trig_output(node,i);

	}
	//	idx++;
	
}



void test1_init( node_manager_t *node_manager)
{
	node_t *a111,*a112,*a11,*a12,*a13,*a14,*a1;

	
	node_init(&a111,0,2,1,node_manager);
	NODE_INIT_OUTPUT(*a111,0,double,0);
	NODE_INIT_OUTPUT(*a111,1,double,0);
	node_add_callback(a111,0, test1_run,"a111",0);
//	node_manager_add_node(node_manager, a111);

	
	node_init(&a112,0,2,1,node_manager);
	NODE_INIT_OUTPUT(*a112,0,double,0);
	NODE_INIT_OUTPUT(*a112,1,double,0);
	node_add_callback(a112,0, test1_run,"a112",0);
//	node_manager_add_node(node_manager, a112);




	node_init(&a11,1,1,1,node_manager);
	NODE_INIT_OUTPUT(*a11,0,double,0);
	node_add_callback(a11,0, test1_run,"a11",0);
//	node_manager_add_node(node_manager, a11);



	node_init(&a12,1,1,1,node_manager);
	NODE_INIT_OUTPUT(*a12,0,double,0);
	node_add_callback(a12,0, test1_run,"a12",0);
//	node_manager_add_node(node_manager, a12);


	node_init(&a13,1,1,1,node_manager);
	NODE_INIT_OUTPUT(*a13,0,double,0);
	node_add_callback(a13,0, test1_run,"a13",0);
//	node_manager_add_node(node_manager, a13);


	node_init(&a14,1,1,1,node_manager);
	NODE_INIT_OUTPUT(*a14,0,double,0);
	node_add_callback(a14,0, test1_run,"a14",0);

//	node_manager_add_node(node_manager, a14);



	node_init(&a1,4,1,1,node_manager);
	NODE_INIT_OUTPUT(*a1,0,double,0);
	node_add_callback(a1,0, test1_run,"a1",0);
//	node_manager_add_node(node_manager, a1);

	node_connect(a111,0,a11,0,1);
	node_connect(a111,1,a12,0,1);
	node_connect(a112,0,a13,0,1);
	node_connect(a112,1,a14,0,1);
	node_connect(a11,0,a1,0,1);
	node_connect(a12,0,a1,1,1);
	node_connect(a13,0,a1,2,1);
	node_connect(a14,0,a1,3,1);
}
#include <time.h>       /* time */


int g_r1,g_r2,g_r3,g_r4;
/* gcc node.c thread.c list.c -o node -g -lpthread -DTEST */
void  random(node_t *node,void *node_name)
{
	int i = (int*)
	iSecret = rand() % 1000+ 1;

		
}

void test2_init( node_manager_t *node_manager)
{
	node_t *r1,*r2,*r3,*r4;

	
	node_init(&a1,0,1,1,node_manager);
	NODE_INIT_OUTPUT(*a1,0,double,0);
	node_add_callback(a111,0, test1_run,"a1",0);
//	node_manager_add_node(node_manager, a111);

}
int main()
{
int i;
//	worker_list_init (5);
//	worker_list_start();

	node_manager_t *node_manager;
srand (time(NULL));
	node_manager_init(&node_manager,255,8); //test1_init);
	test1_init(node_manager);
	node_manager_step(node_manager,200000);
	sleep(1);
//	node_manager_start(node_manager);

//	while(1);
//	time (&g_start);
	 //g_start = clock();
//		for (i=0;i<200000000;i++){
//	node_manager_run(&node_manager); //,&worker_list);
	//		getchar();
//		}
//	printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1\n");
//	usleep(1000000);
	return 0;

}
#endif
