/*
 * =====================================================================================
 *
 *       Filename:  class.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/02/14 22:37:55
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */
//http://code.activestate.com/recipes/54352-defining-python-class-methods-in-c/
//http://docs.python.org/2/extending/extending.html
//http://stackoverflow.com/questions/17045939/how-to-access-python-class-from-c-extension
#include <Python.h>
#include "py_output.h"
#include "node.h"
#include "list.h"


//* *************************************************************************************** */
 PyObject *Node_error;

static PyObject*  NodeManager_start(PyObject* self, PyObject *args) 
{
/*  
	int num_or_nodes,num_of_workers;
	if (!PyArg_ParseTuple(args, "II", &num_or_nodes,&num_of_workers)) {
		PyErr_Format(Node_error,"usage error: Foo(num_or_nodes,num_of_workers ) " );
		return NULL;
	}

	// create node and put it in self class pointe
	node_t *node_manager = (node_manager_t*)malloc(sizeof(node_manager_t));
        PyObject *pnode_manager = PyCObject_FromVoidPtr((void*)node_manager, node_manager_deinit);
        PyObject_SetAttrString(selfobj, "__node_manager__", pnode_manager);

	node_manager_init(node_manager,num_or_nodes,num_of_workers);
#ifdef DEBUG
    printf("node_manager_start\n");	
#endif
*/
    return Py_None;
}


static PyObject*  NodeManager_stop(PyObject* self, PyObject *args) 
{
#ifdef DEBUG
    printf("node_manager_stop\n");	
#endif
}


static PyObject* Node_init(PyObject *self, PyObject *args)
{
	PyObject *selfobj,*py_node_manager;
	int i;
	unsigned int num_of_inputs,num_of_outputs,num_of_callback;
	if (!PyArg_ParseTuple(args, "OIIIO", &selfobj,&num_of_inputs,&num_of_outputs,&num_of_callback,&py_node_manager))
	{
		PyErr_Format(Node_error,"Node_init usage error: Node_init(num_of_inputs,num_of_outputs,num_of_callback,node_manager ) " );
		return NULL;
	}

	// get point to node_manager
	if (!NodeManager_Check(py_node_manager))
	{
		PyErr_Format(Node_error,"Node_init usage error: 4 argument must be of node manager type" );
		return NULL;
	}

	node_manager_t *node_manager = PyCObject_AsVoidPtr ( PyObject_GetAttrString(py_node_manager, "node_manager") );


	// create node and put it in self class pointe
	node_t *node = (node_t*)malloc(sizeof(node_t));
        PyObject *pynode = PyCObject_FromVoidPtr((void*)node, node_deinit);
        PyObject_SetAttrString(selfobj, "__node__", pynode);

	//node_t *node = PyCObject_AsVoidPtr(PyObject_GetAttrString(self,"__node__"));
	node_init(node,num_of_inputs,num_of_outputs,num_of_callback,node_manager); //node_random,"a");
	for (i=0;i<num_of_outputs;i++) {
		NODE_INIT_OUTPUT(*node,i,void*);
		void **tmp = node_get_output(node,i);
		*tmp = 0;
	}

#ifdef DEBUG
	printf("Node.init called\n");
#endif
	Py_INCREF(Py_None);
	return Py_None;
}
static PyObject* Node_repr(PyObject *self, PyObject *args)
{
	char s[256];
	PyObject *selfobj;	
	if (!PyArg_ParseTuple(args, "O", &selfobj))
	{
	//	PyErr_Format(Node_error,"usage error: Node_init(num_of_inputs,num_of_outputs,num_of_callback ) " );
		return NULL;
	}

	node_t *node = PyCObject_AsVoidPtr(PyObject_GetAttrString(selfobj,"__node__"));
	sprintf(s,"num of inputs %d\n\
num of outputs %d\n\
num of callback %d",node->in.len,node->out.len,node->callbacks.len);


#ifdef DEBUG
	printf("Node.rerp called\n");
#endif
	Py_INCREF(Py_None);
	return Py_BuildValue("s", s);

}

static PyObject* Node_get_output(PyObject *self, PyObject *args)
{

	PyObject *selfobj;
	int i;
	unsigned int out_idx;
	if (!PyArg_ParseTuple(args, "OI", &selfobj,&out_idx))
	{
		PyErr_Format(Node_error,"usage error: Node_get_output(out_idx) " );
		return NULL;
	}

	node_t *node = PyCObject_AsVoidPtr(PyObject_GetAttrString(selfobj,"__node__"));
	if (out_idx>=node->out.len)
	{
		PyErr_Format(Node_error,"usage error: node output index in out of range of %d ", node->out.len);
		return NULL;
	}

	io_t *output = list_get(&node->out,out_idx);

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
	PyObject *selfobj;
	int i;
	unsigned int in_idx,id;
	PyObject *pyoutput;
	if (!PyArg_ParseTuple(args, "OOII", &selfobj,&pyoutput,&in_idx,&id))
	{
		PyErr_Format(Node_error,"usage error: Node_set_output(output,in_idx,id) " );
		return NULL;
	}
	if (!Output_Check(pyoutput))
	{
		PyErr_Format(Node_error,"usage error: object of tyep output must be given");
		return NULL;
	}

	node_t *node = PyCObject_AsVoidPtr(PyObject_GetAttrString(selfobj,"__node__"));
	io_t *output = PyCObject_AsVoidPtr(PyObject_GetAttrString(pyoutput,"__output__"));
	if (in_idx>=node->in.len)
	{
		PyErr_Format(Node_error,"usage error: node input index in out of range of %d ", node->in.len);
		return NULL;
	}

	 node_set_input(node,output,in_idx,  id);


#ifdef DEBUG
	printf("Node.setInput called\n");
#endif
	Py_INCREF(Py_None);
	return Py_None;

}
static PyObject* Node_set_callback(PyObject *self, PyObject *args)
{

	PyObject *selfobj;
	int i;
	unsigned int num_of_inputs,num_of_outputs,num_of_callback;
	if (!PyArg_ParseTuple(args, "OIII", &selfobj,&num_of_inputs,&num_of_outputs,&num_of_callback))
	{
		PyErr_Format(Node_error,"usage error: Node_init(num_of_inputs,num_of_outputs,num_of_callback ) " );
		return NULL;
	}

#ifdef DEBUG
	printf("Node.setInput called\n");
#endif
	Py_INCREF(Py_None);
	return Py_None;

}
static PyObject* Node_start(PyObject *self, PyObject *args)
{
	printf("node_start called\n");

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* Node_doSomething(PyObject *self, PyObject *args)
{
	printf("Node.doSomething called\n");

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* Node_yair(PyObject *self, PyObject *args)
{
	PyObject *selfobj;
	const char *command;
	int sts;

	if (!PyArg_ParseTuple(args, "Os", &selfobj,&command))
		return NULL;
	sts = system(command);

	return Py_BuildValue("i", sts);

}

 PyMethodDef NodeMethods[] = 
{
	{"__init__", Node_init, METH_VARARGS, "doc string"},
	{"__repr__", Node_repr, METH_VARARGS, "doc __repr__"},
	{"setInput", Node_set_input, METH_VARARGS, "doc set input"},
	{"getOutput", Node_get_output, METH_VARARGS, "doc get output"},
	{"setCallback", Node_set_callback, METH_VARARGS, "doc set callback"},
	{"doSomething", Node_doSomething, METH_VARARGS,"doc string"},
	{"yair", Node_yair, METH_VARARGS,"doc yair"},
	{NULL},
};


 PyMethodDef NodeManagerMethods[] = 
{
	{"start", NodeManager_start, METH_VARARGS,"doc start"},
	{"stop", NodeManager_stop, METH_NOARGS,"doc stop"},
	{NULL},
};


void initNode()
{
    PyMethodDef *def;

    /* create a new module and class */
    PyObject *module = Py_InitModule("Node", NodeMethods);

    //* ********************************************create Node class  ************************************ */
    /* create a class Node*/
    PyObject *moduleDict = PyModule_GetDict(module);
    PyObject *classDict = PyDict_New();
    PyObject *className = PyString_FromString("Node");
    PyObject *fooClass = PyClass_New(NULL, classDict, className);
    PyDict_SetItemString(moduleDict, "Node", fooClass);
    Py_DECREF(classDict);
    Py_DECREF(className);
    Py_DECREF(fooClass);
    
    /* add methods to class */
    for (def = NodeMethods; def->ml_name != NULL; def++) {
	PyObject *func = PyCFunction_New(def, NULL);
	PyObject *method = PyMethod_New(func, NULL, fooClass);
	PyDict_SetItemString(classDict, def->ml_name, method);
	Py_DECREF(func);
	Py_DECREF(method);
    }

  

}

