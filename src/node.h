/*
 * =====================================================================================
 *
 *       Filename:  node.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  27/10/13 23:11:41
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#pragma once
#include "thread.h"
#include "list.h"
#include <pthread.h>
#include <semaphore.h>
/*
#define NODE_INIT_OUTPUT(node,idx,type,callbackid)\
{\
	output_t *output = list_get ( &(node).out,idx);\
	value_new (&output->value,sizeof(type));\
	output->slot = (char*)malloc(255);\
	output->id = (callbackid);\
}
*/
#define NODE_INIT_OUTPUT(node,idx,type,callbackid)\
{\
	output_t *output = *(output_t**)list_get ( &(node).out,idx);\
	value_new (&output->value,sizeof(type));\
}


/*
typedef enum yaccEnumNodeType_e {
	yaccINPUT,
	yaccOUTPUT,
	yacaNODE,
	yaccCONNECTION
}yaccEnumNodeType;

typedef struct yacc_node_declare_s {
	char *name;
	int num_inputs;
	int num_outputs;
} yacc_node_declare_t;

typedef struct yaccNodeIO_s {
	yaccEnumNodeType type;
	char *io_list[255];
	int n;
} yaccNodeIO_t;


typedef union yaccNodeType_u {
	yaccEnumNodeType type;
	yaccNodeIO_t io;
	
} yaccNodeType;


*/

typedef struct value_s {
	void *val;
	int len;
} value_t;
void value_init(value_t *value);
char value_new(value_t *value,int len);
void value_free(value_t *value);
char value_copy(value_t *src,value_t *des);
void value_set(value_t *value,void *val);






typedef struct {
	value_t value;
	char suspended;
	char fan_out;
	char num_of_used_out;
	pthread_mutex_t mutex;
	pthread_cond_t m_ResumeCond;
	pthread_attr_t attr;
	char *slot;
	int id;
	char *name;
	struct _node_t *node; // pointer to contained node
	

} output_t;



void output_init(output_t *io);
void output_manage_suspend(output_t * io) ;
void output_resume(output_t * io);
void output_suspend(output_t * io);
void output_deinit(void *io);

typedef struct {
	output_t *output;
	value_t value_copy;	
	int slot_idx;
	int id;
	char *name;
	struct _node_t *node; // pointer to contained node

	
} input_t;


typedef struct _node_t{
	char *name;
	list_t in;
	list_t out;
	list_t callbacks;
	struct _node_manager_t *node_manager;
	void *data;
//	pthread_mutex_t mutex;

} node_t;
typedef void (*run_t)(node_t*,void *);
typedef void (*prm_deinit_t)(void *);

void node_init(node_t **node,int num_of_input,int num_of_out,int num_of_callbacks, struct _node_manager_t *node_manager); //run_t run,void *prm);
void  node_run(node_t *node);
void node_deinit(void *node);
void node_connect(node_t *out_node,int port_out,node_t *in_node,int port_in,int id);
void node_set_input(node_t *node,output_t *out,int port_in, int id,void *val);
void node_trig_output(node_t *node,int i);
void *node_get_output_value(node_t *node,int idx);
void node_get_input_value(node_t *node,int idx,void **val);

void node_release_input12(node_t *node,int idx);
struct _callback_t  *  node_add_callback(node_t *node,int id,run_t run,void *prm,prm_deinit_t );
//void node_release_input(node_t *,input_t *input);
char node_set_input_id(node_t *node,int port_in, int id);
char node_set_output_id(node_t *node,int port_out, int id);
//char node_set_callback(node_t *node,run_t run, void *prm,int id);
char node_add_input(node_t *node, int id);
output_t *node_get_output(node_t *node,int idx);

	output_t *  node_manage_find_output_by_name(struct _node_manager_t  *, char *full_name_left);
			input_t   *  node_manage_find_input_by_name(struct _node_manager_t*,char *full_name_left);
			node_t *  node_find_by_name (struct _node_manager_t*,char *full_name_left);


typedef struct _callback_t {
	void *prm;
	int prm_len;
	run_t run;
	int id;
	char lock;
	prm_deinit_t prm_deinit;

}callback_t;
char callback_check_active(callback_t *callback,list_t *slot_io_list);
void callback_release_inputs(callback_t *callback,node_t *node ); 
void callback_init(callback_t **callback,run_t run,int id,void *prm,prm_deinit_t);
void callback_deinit(callback_t *callback);




typedef struct _worker_t {
	thread_t  tid;
	pthread_mutex_t mutex;
	node_t *node;
	void (*worker)(void *);	
	int id;
//	char is_active;
} worker_t;
void * worker_run (void *prm);



typedef struct _worker_list_t {
	list_t  workers;
	int idx;
	 pthread_mutex_t mutex;
	 pthread_cond_t signal;
	 pthread_attr_t attr;
	 int counter;
} worker_list_t;
void  worker_list_init (worker_list_t *worker_list, int n);
void  worker_list_start (worker_list_t *worker_list);
void  worker_list_trigger (worker_list_t *worker_list,node_t *node );
void worker_list_add_worker(worker_list_t *worker_list,worker_t *worker);



typedef struct _node_manager_t {
		list_t  inputs;
	list_t outputs;

	worker_list_t worker_list;
	list_t nodes;
	pthread_mutex_t mutex;
	thread_t tid;
	char run;

} node_manager_t;
void node_manager_init(node_manager_t **node_manager,int num_of_nodes,int num_of_workers);
void node_manager_add_node(node_manager_t *node_manager, node_t *node);
//void node_manager_step(node_manager_t *node_manager,int n);
void node_manager_start(node_manager_t *node_manager,void *prm);
void node_manager_deinit(void *);
void node_manager_steps_loop(node_manager_t *node_manager,int num_of_steps);
void node_manager_step(node_manager_t *node_manager);
node_t * node_manage_find_node_by_name(node_manager_t *node_manager,char *name);
input_t * node_manage_find_node_by_input_name(node_manager_t *node_manager,char *name);
output_t * node_manage_find_node_by_output_name(node_manager_t *node_manager,char *name);
node_t * node_find_by_index(node_manager_t *node_manager,int idx)	;
void node_init_by_ios(node_t **node_out,list_t *inputs, list_t *outputs,node_manager_t *node_manager);
void *node_manager_eleiminate_nodes(node_manager_t *node_manager); 

void node_set_input_name(node_t *node,int idx,char *name);
void node_set_output_name(node_t *node,int idx,char *name);
input_t *node_get_input(node_t *node,int idx);

void  * node_manager_run( void *); 
typedef struct _node_manager_thread_prm_t {
	node_manager_t *node_manager;
	int num_of_steps;
} node_manager_thread_prm_t;



output_t   *output_new() ;
input_t   *input_new() ;
