///
///      @file  py_init.c
///     @brief  python imlementation for class output_t
///
/// Detailed description starts here.
///
///    @author  Yair Gadelov (), yair.gadelov@gmail.com
///
///  @internal
///    Created  02/10/14
///   Revision  $Id: doxygen.cpp.templates,v 1.3 2010/07/06 09:20:12 mehner Exp $
///   Compiler  gcc/g++
///    Company  
///  Copyright  Copyright (c) 2014, Yair Gadelov
///
/// This source code is released for free distribution under the terms of the
/// GNU General Public License as published by the Free Software Foundation.
///=====================================================================================
///


#include <Python.h>
#include "structmember.h"

#include "node.h"
static PyObject *NodeManager_error;



//http://starship.python.net/crew/mwh/toext/node21.html
//http://docs.python.org/2/extending/newtypes.html
//http://ref.readthedocs.org/en/latest/understanding_python/type_system/PyTypeObject.html

typedef struct {
	PyObject_HEAD
	PyObject *nodes;
	PyObject *workers;
	PyObject *node_manager;
} NodeManagerObject;

static int
NodeManager_init(NodeManagerObject *self, PyObject *args, PyObject *kwds)
{
	PyObject *nodes = Py_None;
	PyObject *workers = Py_None;
	PyObject *tmp = NULL;

	
	static char *kwlist[] = {"nodes", "workers", NULL};

	if (!PyArg_ParseTupleAndKeywords(args, kwds, "|OO", kwlist, &nodes, &workers))
		return -1;

	tmp = self->nodes;
	if (nodes == Py_None) {
		self->nodes = PyInt_FromLong(255);
	}
	else {
		Py_INCREF(nodes);
		self->nodes = nodes;
	}
	Py_XDECREF(tmp);


	tmp = self->workers;
	if (workers == Py_None) {
		self->workers =PyInt_FromLong(10);
	}
	else {
		Py_INCREF(workers);
		self->workers = workers;
	}
	Py_XDECREF(tmp);



	// create node and put it in self class pointe
	node_manager_t *node_manager ;//= (node_manager_t*)malloc(sizeof(node_manager_t));
	node_manager_init(&node_manager,PyInt_AsLong(nodes),PyInt_AsLong(workers));

	PyObject *py_node_manager= PyCObject_FromVoidPtr(node_manager, node_manager_deinit);
	tmp = self->node_manager;
//	Py_INCREF(py_node_manager);
	self->node_manager = py_node_manager;
	Py_XDECREF(tmp);

	return 0;

}

static PyMemberDef NodeManager_members[] = {
	{"nodes", T_OBJECT_EX, offsetof(NodeManagerObject, nodes), READONLY, "num of nodes"},
	{"workers", T_OBJECT_EX, offsetof(NodeManagerObject, workers), READONLY, "num of workers"},
	{"node_manager", T_OBJECT_EX, offsetof(NodeManagerObject, node_manager), READONLY, "pointer to node manager"},
	{NULL}  /* Sentinel */
};


/**
 * Created  17/02/14
 * @brief   runs nodemanger_step from python. refferece: http://www.linuxjournal.com/article/3641
 * @param   
 * @return  
 */
static PyObject *
NodeManager_step(NodeManagerObject *self, PyObject *args)
{

	int num_of_steps;
	int i_step,i_node,i_in;
	node_t *node;
	input_t *input;

	if (!PyArg_ParseTuple(args, "I", &num_of_steps))
	{
		PyErr_Format(NodeManager_error,"NodeManager_step usage error: NodeManager_step(num_of_steps ) " );
		return NULL;
	}

	node_manager_t *node_manager = PyCObject_AsVoidPtr ( self->node_manager );
//	PyEval_ReleaseLock();
	PyThreadState*  mainThreadState = PyEval_SaveThread();


	// loop to run the required number of steps
	for (i_step=0;i_step<num_of_steps;i_step++)
	{
		// do one step (simulator tick)
		node_manager_step(node_manager);

		// update inputs with the last outputs value
	//	list_foreach(input_t **input, &node_manager->inputs) {
		for (i_in = 0; i_in<list_size(&node_manager->inputs);i_in++) {

				input =   * (input_t**) list_get(&node_manager->inputs,i_in);

				// aloacte new object
				PyObject  *update_obj = Py_BuildValue("O",  (PyObject*)*(void **)( (input)->output->value.val)  );
				
				// remove current object
				Py_DECREF( (PyObject*)*(void **)( (input)->value_copy.val) );
				
				value_set (& (input)->value_copy,&update_obj);

		}

		/* 
		// update inputs with the last outputs value
		for (i_node= 0 ;i_node<node_manager->nodes.len;i_node++)
		{
		//	node = *((node_t**) list_get ( &node_manager->nodes,i_node));			
			node = node_find_by_index(node_manager,i_node);
			
			for (i_in = 0; i_in<list_size(&node->in);i_in++)
			{
			//	input = *((input_t**)list_get(&node->in,i_in));
				input = node_get_input (node,i_in);

				// aloacte new object
				PyObject  *update_obj = Py_BuildValue("O",  (PyObject*)*(void **)(input->output->value.val)  );
				
				// remove current object
				Py_DECREF( (PyObject*)*(void **)(input->value_copy.val) );
				
				value_set (&input->value_copy,&update_obj);
				//value_copy(&input->output->value,&input->value_copy);
	
			}

		}
		*/
		

	}


	PyEval_RestoreThread(mainThreadState);
	Py_INCREF(Py_None);
	return Py_None;

}


void NodeManagde_dealloc(NodeManagerObject *self)
{

	Py_XDECREF(self->nodes);
	Py_XDECREF(self->workers);
	Py_XDECREF(self->node_manager);
       self->ob_type->tp_free((PyObject*)self);
}

static PyMethodDef NodeManager_methods[] = {
	{"step", (PyCFunction)NodeManager_step, METH_VARARGS,"Runs node manger for required number of steps "},
	{NULL}
};

static char NodeManager_doc[] = 
	"boo.";

static PyTypeObject NodeManagerObjectType = {
	PyObject_HEAD_INIT(NULL)
	0,				/* ob_size           */
	"name.NodeManager",			/* tp_name           */
	sizeof(NodeManagerObject),		/* tp_basicsize      */
	0,				/* tp_itemsize       */
	(destructor)NodeManagde_dealloc,				/* tp_dealloc        */
	0,				/* tp_print          */
	0,				/* tp_getattr        */
	0,				/* tp_setattr        */
	0,				/* tp_compare        */
	0,				/* tp_repr           */
	0,				/* tp_as_number      */
	0,				/* tp_as_sequence    */
	0,				/* tp_as_mapping     */
	0,				/* tp_hash           */
	0,				/* tp_call           */
	0,				/* tp_str            */
	0,				/* tp_getattro       */
	0,				/* tp_setattro       */
	0,				/* tp_as_buffer      */
	Py_TPFLAGS_DEFAULT,		/* tp_flags          */
	NodeManager_doc,			/* tp_doc            */
	0,				/* tp_traverse       */
	0,				/* tp_clear          */
	0,				/* tp_richcompare    */
	0,				/* tp_weaklistoffset */
	0,				/* tp_iter           */
	0,				/* tp_iternext       */
	NodeManager_methods,	     		/* tp_methods        */
	NodeManager_members,			/* tp_members        */
	0,				/* tp_getset         */
	0,				/* tp_base           */
	0,				/* tp_dict           */
	0,				/* tp_descr_get      */
	0,				/* tp_descr_set      */
	0,				/* tp_dictoffset     */
	(initproc)NodeManager_init,		/* tp_init           */
};


static PyObject* NodeManager_New(NodeManagerObject *self, PyObject *args)
{

	NodeManagerObject *object = NULL;
	object = PyObject_NEW(NodeManagerObject, &NodeManagerObjectType);
	return (PyObject *)object;
}


void NodeManager_add_to_module(PyObject *module)
{
	NodeManagerObjectType.tp_new = PyType_GenericNew;
	if (PyType_Ready(&NodeManagerObjectType) < 0)
		return;

//	NodeManagerObjectType.tp_del = NodeManager_del;


	Py_INCREF(&NodeManagerObjectType);
	PyModule_AddObject(module, "NodeManager", (PyObject *)&NodeManagerObjectType);

        NodeManager_error = PyErr_NewException("NodeManager.error", NULL, NULL);
        PyModule_AddObject(module, "error", NodeManager_error);

}


#define NodeManager_Check1(v)  ((v)->ob_type == &NodeManagerObjectType)

inline char  NodeManager_Check(PyObject *v) 
{
	if (v->ob_type==&NodeManagerObjectType)
		return 1;
	return 0;
}


