/**
 *       @file  py_node.c
 *      @brief  interace of node class to python
 *
 * Detailed description starts here.
 *
 *     @author  Yair Gadelov (), yair.gadelov@gmail.com
 *
 *   @internal
 *     Created  02/12/14
 *    Revision  $Id: doxygen.templates,v 1.3 2010/07/06 09:20:12 mehner Exp $
 *    Compiler  gcc/g++
 *     Company  
 *   Copyright  Copyright (c) 2014, Yair Gadelov
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License as published by the Free Software Foundation.
 * =====================================================================================
 */


#include <Python.h>
#include "structmember.h"

#include "node.h"
#include "py_output.h"
//http://starship.python.net/crew/mwh/toext/node21.html
//http://developer.nokia.com/community/discussion/showthread.php/69807-Python-C-Extension-with-Callback-crashing
///http://docs.python.org/2.5/ext/callingPython.html
//http://docs.python.org/3.4/extending/newtypes.html
//http://code.activestate.com/recipes/65210-implementing-c-function-callbacks-using-python/
//

static PyObject *Node_error;


typedef struct {
	PyObject_HEAD
	PyObject *node;
	int callbacks,inputs,outputs;
	PyObject *node_manager;
	PyObject *callback_list;
} NodeObject;



/**
 * @brief   uses as cllback function to intemidate between python call to c call
 *          http://docs.python.org/2.5/ext/callingPython.html
 * @param   
 * @return  
 */
static void  node_callback(node_t *node,void *prm_in)
{
	int i;
	PyObject *data;
	void **out,*in;
	PyObject *tmp_out;

	PyObject *arglist,*callback;
	PyObject *result;

	PyGILState_STATE state; 
	state = PyGILState_Ensure();

	callback = prm_in;

	arglist = PyTuple_New(node->in.len);

	// gather argumnet list from all conected inputs
	for (i=0;i<node->in.len;i++) {
		node_get_input_value(node,i,&in);
//		if (in && (PyObject*)*(void **)in)
//		{
			Py_INCREF ((PyObject*)*(void **)in);
			PyTuple_SetItem(arglist, i, (PyObject*)*(void **)in);
			// sice this paramer is foing to use in another python function as new obhect, increase refrece to double it
//			}
//		else
//		{
//			Py_INCREF(Py_None);			
//			PyTuple_SetItem(arglist, i, Py_None);
//
//		}
	}
	// what to to if no arg list ???????????? -> todo !!!!!!!!!!!!!
	if (arglist==0)
	{
		printf("error at node_callback: no arg list . todo !!!!!!!!!!!!!!!!!");
		exit(1);
	}

	// call python callback
	result = PyEval_CallObject( callback, arglist);


	//	push python procedure output reresult to connected inputs of the current node
	if (result )	
	{
		if (!PyTuple_Check(result))
		{
			// the return python procedure has only one output argmunet
			out = node_get_output_value(node,0);
			if (out)
			{
				*out = result; 
			}

		}
		else if (PyTuple_Size(result)==node->out.len)
		{
			// the return python prcedure has returned tuple (multy variables)
			for (i=0;i<PyTuple_Size(result) ;i++) {
				out = node_get_output_value(node,i);
				// out=0: some time the node output is not connected altghou the
				// python call return vale (the python procedure has to return the 
				// exacly values as defined in to be trigged by the callback - 
				// this prevents crash assign to NULL pointer. ignore it.
				// tmp_out=0: some time there is output from the  python function
				// but no input is connected to the pushing output, 
				if (out)
				{
					tmp_out = PyTuple_GetItem(result, i);
					if (tmp_out!=NULL)
					{
						*out  = tmp_out;
						//	Py_DECREF(tmp_out);
					}

				}

			}
		}
		Py_DECREF(result);

	}
	Py_DECREF(arglist);  

	PyGILState_Release(state); 
}

static int
Node_init(NodeObject *self, PyObject *args, PyObject *kwds)
{
//	PyObject *nodes = Py_None;
	int i;
	PyObject *py_node_manager ;
	PyObject *tmp_node = NULL;
	int *tmp_int=0;
	
	int callbacks,inputs,outputs;
	
	static char *kwlist[] = {"callbacks", "inputs", "outputs","node_manager",NULL};

	if (!PyArg_ParseTupleAndKeywords(args, kwds, "|IIIO", kwlist, &self->inputs, &self->outputs,&self->callbacks,&py_node_manager))
		return -1;

		// get point to node_manager
	if (!NodeManager_Check(py_node_manager))
	{
		PyErr_Format(Node_error,"Node_init usage error: 4 argument must be of node manager type" );
		return -2;
	}

	
	PyObject *tmp_node_manager = PyObject_GetAttrString(py_node_manager, "node_manager") ;
	node_manager_t *node_manager = PyCObject_AsVoidPtr (tmp_node_manager );
	Py_XDECREF(tmp_node_manager);


	tmp_node = self->node_manager;
	Py_INCREF(py_node_manager);
	self->node_manager = py_node_manager;
	Py_XDECREF(tmp_node);

	// create callback list
	PyObject *callback_list =  PyList_New(self->callbacks);
	tmp_node = self->callback_list;
	Py_INCREF(callback_list);
	self->callback_list = callback_list;
	Py_XDECREF(tmp_node);


/*  
	tmp_int = &self->callbacks;
	Py_INCREF(&callbacks);
	self->callbacks = callbacks;
	Py_XDECREF(tmp_int);


	tmp_int = &self->inputs;
	Py_INCREF(&inputs);
	self->inputs = inputs;
	Py_XDECREF(tmp_int);


	tmp_int = &self->outputs;
	Py_INCREF(&outputs);
	self->outputs = outputs;
	Py_XDECREF(tmp_int);

*/
	// alloate place for node and pass it to python hav to be before node_init
	// othreawise, the pointer in the node are mismatch and wrong values are get (why ???????????????????????)
	node_t *node ;// = (node_t*)malloc(sizeof(node_t));	
	

	//	static int kkk=0;
	//printf("init %d\n",kkk++);
	node_init(&node,self->inputs,self->outputs,self->callbacks,node_manager); //node_random,"a");
	// initiate output
	//!!void *tmp ;
//	void **tmp;
	for (i=0;i<self->outputs;i++) {
		NODE_INIT_OUTPUT(*node,i,void**,0);
		//tmp = node_get_output(node,i);
		//!!node_get_input(node,i,&tmp);
		//!!Py_INCREF(Py_None); //
		//!!tmp = Py_None;
//		*tmp=Py_None;
	}


	// save node pointer in local python object
	PyObject *pynode = PyCObject_FromVoidPtr(node, node_deinit);
	tmp_node = self->node;
	Py_INCREF(pynode);
	self->node = pynode;
	Py_XDECREF(tmp_node);



	return 0;
}
static PyObject* Node_get_output(PyObject *self, PyObject *args)
{

	int i;
	unsigned int out_idx;
	if (!PyArg_ParseTuple(args, "I",&out_idx))
	{
		PyErr_Format(Node_error,"usage error: Node_get_output(out_idx) " );
		return NULL;
	}

	node_t *node = PyCObject_AsVoidPtr(PyObject_GetAttrString(self,"node"));
	if (out_idx>=node->out.len)
	{
		PyErr_Format(Node_error,"usage error: node output index is out of range of %d ", node->out.len);
		return NULL;
	}

	//output_t *output = *(output_t**)list_get(&node->out,out_idx);
	output_t *output  = node_get_output(node,out_idx);



#ifdef DEBUG
	printf("Node.getInput called\n");
#endif
	Py_INCREF(Py_None);
//#define STRANGE
#ifdef  STRANGE
	Output_New(output,125);
#else
	int n=453;
	extern PyTypeObject OutputObjectType ;
	OutputObject *outputObject=PyObject_NEW(OutputObject, &OutputObjectType);
	outputObject->output = PyCObject_FromVoidPtr(output,node_deinit);
	outputObject->id =PyInt_FromLong(n);
	return (PyObject*)outputObject;
#endif
}


static PyObject* Node_set_input(PyObject *self, PyObject *args)
{
	int i;
	unsigned int in_idx,id;
	PyObject *pyoutput;
	if (!PyArg_ParseTuple(args, "OI",&pyoutput,&in_idx))
	{
		PyErr_Format(Node_error,"usage error: Node_set_input(output,in_idx) " );
		return NULL;
	}
	if (!Output_Check(pyoutput))
	{
		PyErr_Format(Node_error,"usage error: object of tyep output must be given");
		return NULL;
	}

	node_t *node = PyCObject_AsVoidPtr(PyObject_GetAttrString(self,"node"));
	output_t *output = PyCObject_AsVoidPtr(PyObject_GetAttrString(pyoutput,"output"));
	if (in_idx>=node->in.len)
	{
		PyErr_Format(Node_error,"usage error: node input index is out of range of %d ", node->in.len);
		return NULL;
	}

	// set node input with inital value - default none. todo: to  add option for initial value 
	 Py_INCREF(Py_None);
	 node_set_input(node,output,in_idx, /*id=0*/ 0,/*initial value*/Py_None);

	 // set node initial value
//	 void *tmp ;
//	 node_get_input(node,in_idx,&tmp);
//	 Py_INCREF(Py_None);
	// *tmp = Py_None;
	


#ifdef DEBUG
	printf("Node.setInput called\n");
#endif
	Py_INCREF(Py_None);
	return Py_None;

}

static PyObject* Node_set_callback(PyObject *selfin, PyObject *args)
{
	int i;
	PyObject *temp=0,*tmp_callback;
	PyObject *pyInList,*pyOutList,*tmp_int;
	NodeObject *self = (NodeObject*)selfin;

	//	node_t *node = PyCObject_AsVoidPtr(PyObject_GetAttrString(self,"node"));
	node_t *node = PyCObject_AsVoidPtr(self->node);


	// take the callback id as the value of it place in the list
	int callback_id = node->callbacks.len+10;

#ifdef DEBUG
	printf("Node.setCallback called\n");
#endif
	if (!PyArg_ParseTuple(args, "OOO",&temp,&pyInList,&pyOutList)) 
	{
		PyErr_Format(Node_error,"usage error: Node_set_callback(callback,affcted_input,afftcted_outputs) " );
		return NULL;

	}


	// check if pyInList is correct
	if (!PyList_Check(pyInList)) {
		PyErr_SetString(PyExc_TypeError, "pyInList: parameter must be list of integers");
		return NULL;
	}
	for (i=0;i<PyList_Size(pyInList);i++)
	{
		tmp_int = PyList_GetItem(pyInList,  i);
		if (!PyInt_Check(tmp_int))
		{
			PyErr_Format(Node_error,"usage error: inlist must include only integers");
			return NULL;
		}
//		if (!node_set_input_id(node,PyInt_AsLong(tmp_int),callback_id))
//		{
//			PyErr_Format(Node_error,"usage error: %ld is out of node input range",PyInt_AsLong(tmp_int));
//			return NULL;
//
//		}


	}

	// check if pyOutList is correct
	if (!PyList_Check(pyOutList)) {
		PyErr_SetString(PyExc_TypeError, "pyOutList: parameter must be list of integers");
		return NULL;
	}
	for (i=0;i<PyList_Size(pyOutList);i++)
	{
		tmp_int = PyList_GetItem(pyOutList,  i);
		if (!PyInt_Check(tmp_int))
		{
			PyErr_Format(Node_error,"usage error: outlist must include only integers");
			return NULL;
		}
//		if (!node_set_output_id(node,PyInt_AsLong(tmp_int),callback_id))
//		{
//			PyErr_Format(Node_error,"usage error: %ld is out of node outputs range",PyInt_AsLong(tmp_int));
//			return NULL;
//
//		}


	}


	// check if callback correct
	if (!PyCallable_Check(temp)) {
		PyErr_SetString(PyExc_TypeError, "parameter must be callable");
		return NULL;
	}


	Py_INCREF(temp);	
	node_add_callback(node,callback_id,node_callback,temp,0);


	Py_INCREF(Py_None);
	return Py_None;


}

static PyMemberDef Node_members[] = {
	{"callbacks", T_INT, offsetof(NodeObject, callbacks), READONLY, "num of callbacks"},
	{"inputs", T_INT, offsetof(NodeObject, inputs), READONLY, "num of inputs"},
	{"outputs", T_INT, offsetof(NodeObject, outputs), READONLY, "outputs"},
	{"callback_list", T_OBJECT_EX, offsetof(NodeObject, callback_list), READONLY, "callback_list"},
	{"node", T_OBJECT_EX, offsetof(NodeObject, node), READONLY, "pointer to c node object"},
	{"node_manager", T_OBJECT_EX, offsetof(NodeObject, node_manager), READONLY, "pointer to c node_manager object"},
	{NULL}  /* Sentinel */


};


static PyObject* Node_repr(PyObject *selfin, PyObject *args)
{
	char s[256];
	NodeObject *self = (NodeObject*)selfin;
	node_t *node = PyCObject_AsVoidPtr(self->node);
	sprintf(s,"num of inputs %d\n\
num of outputs %d\n\
num of callback %d",node->in.len,node->out.len,node->callbacks.len);


#ifdef DEBUG
	printf("Node.rerp called\n");
#endif
	Py_INCREF(Py_None);
	return Py_BuildValue("s", s);

}


static PyMethodDef Node_methods[] = {
	{"setInput", Node_set_input, METH_VARARGS, "doc set input"},
	{"getOutput", Node_get_output, METH_VARARGS, "doc get output"},
	{"setCallback", Node_set_callback, METH_VARARGS, "doc set callback"},	
//	{"__repr__", Node_repr, METH_VARARGS, "doc __repr__"},	
	{NULL}
};


static char Node_doc[] = 
	"boo.";

static PyTypeObject NodeObjectType = {
	PyObject_HEAD_INIT(NULL)
	0,				/* ob_size           */
	"name.Node",			/* tp_name           */
	sizeof(NodeObject),		/* tp_basicsize      */
	0,				/* tp_itemsize       */
	0,				/* tp_dealloc        */
	0,				/* tp_print          */
	0,				/* tp_getattr        */
	0,				/* tp_setattr        */
	0,				/* tp_compare        */
	Node_repr,			/* tp_repr           */
	0,				/* tp_as_number      */
	0,				/* tp_as_sequence    */
	0,				/* tp_as_mapping     */
	0,				/* tp_hash           */
	0,				/* tp_call           */
	0,				/* tp_str            */
	0,				/* tp_getattro       */
	0,				/* tp_setattro       */
	0,				/* tp_as_buffer      */
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE|Py_TPFLAGS_HAVE_GC,		/* tp_flags          */
	Node_doc,			/* tp_doc            */
	0,				/* tp_traverse       */
	0,				/* tp_clear          */
	0,				/* tp_richcompare    */
	0,				/* tp_weaklistoffset */
	0,				/* tp_iter           */
	0,				/* tp_iternext       */
	Node_methods,	     		/* tp_methods        */
	Node_members,			/* tp_members        */
	0,				/* tp_getset         */
	0,				/* tp_base           */
	0,				/* tp_dict           */
	0,				/* tp_descr_get      */
	0,				/* tp_descr_set      */
	0,				/* tp_dictoffset     */
	(initproc)Node_init,		/* tp_init           */
};


static PyObject* Node_New()
{

	NodeObject *object = NULL;
	object = PyObject_NEW(NodeObject, &NodeObjectType);
	return (PyObject *)object;
}

void Node_add_to_module(PyObject *module)
{
	NodeObjectType.tp_new = PyType_GenericNew;
	if (PyType_Ready(&NodeObjectType) < 0)
		return;

	Py_INCREF(&NodeObjectType);
	PyModule_AddObject(module, "Node", (PyObject *)&NodeObjectType);

        Node_error = PyErr_NewException("Node.error", NULL, NULL);
        PyModule_AddObject(module, "error", Node_error);

}


#define Node_Check1(v)  ((v)->ob_type == &NodeObjectType)

inline char  Node_Check(PyObject *v) 
{
	if (v->ob_type==&NodeObjectType)
		return 1;
	return 0;
}



