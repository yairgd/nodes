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
#include "py_output.h"

#include "node.h"
static PyObject *Output_error;



//http://starship.python.net/crew/mwh/toext/node21.html

static int
Output_init(OutputObject *self, PyObject *args, PyObject *kwds)
{
	PyObject *id = Py_None;
	PyObject *tmp = NULL;
	
	static char *kwlist[] = { "id", NULL};

	if (!PyArg_ParseTupleAndKeywords(args, kwds, "|O", kwlist, &id))
		return -1;

	tmp = self->id;
	if (id == Py_None) {
		self->id = PyInt_FromLong(0);
	}
	else {
		Py_INCREF(id);
		self->id = id;
	}
	Py_XDECREF(tmp);


	// create node and put it in self class pointe
	output_t **output = (output_t**)malloc(sizeof(output_t*));
	*output = 0;
	tmp = self->output;
	Py_INCREF(output);
	self->output = PyCObject_FromVoidPtr(output, output_deinit);
	Py_XDECREF(tmp);
	return 0;
}

static PyMemberDef Output_members[] = {
	{"id", T_OBJECT_EX, offsetof(OutputObject, id), READONLY, "num of nodes"},
	{"output", T_OBJECT_EX, offsetof(OutputObject, output), READONLY, "output c object pointer"},
	{NULL}  /* Sentinel */
};



#ifdef EE
static PyObject *
Output_step(OutputObject *self, PyObject *args)
{

	int num_of_steps;
	if (!PyArg_ParseTuple(args, "I", &num_of_steps))
	{
		PyErr_Format(Output_error,"Output_step usage error: Output_step(num_of_steps ) " );
		return NULL;
	}

	node_manager_t *node_manager = PyCObject_AsVoidPtr ( self->node_manager );
	node_manager_step(node_manager, num_of_steps);
	return Py_None;
}
#endif


static PyMethodDef Output_methods[] = {
//	{"step", (PyCFunction)Output_step, METH_VARARGS,"Runs node manger for required number of steps "},
	{NULL}
};

static char Output_doc[] = 
	"boo.";

 PyTypeObject OutputObjectType = {
	PyObject_HEAD_INIT(NULL)
	0,				/* ob_size           */
	"name.Output",			/* tp_name           */
	sizeof(OutputObject),		/* tp_basicsize      */
	0,				/* tp_itemsize       */
	0,				/* tp_dealloc        */
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
	Output_doc,			/* tp_doc            */
	0,				/* tp_traverse       */
	0,				/* tp_clear          */
	0,				/* tp_richcompare    */
	0,				/* tp_weaklistoffset */
	0,				/* tp_iter           */
	0,				/* tp_iternext       */
	Output_methods,	     		/* tp_methods        */
	Output_members,			/* tp_members        */
	0,				/* tp_getset         */
	0,				/* tp_base           */
	0,				/* tp_dict           */
	0,				/* tp_descr_get      */
	0,				/* tp_descr_set      */
	0,				/* tp_dictoffset     */
	(initproc)Output_init,		/* tp_init           */
};


 PyObject * Output_New(output_t *output,int n)
{

	OutputObject *outputObject=PyObject_NEW(OutputObject, &OutputObjectType);
	outputObject->output = PyCObject_FromVoidPtr(output,node_deinit);
	outputObject->id =PyInt_FromLong(n);
	return (PyObject*)outputObject;
}

void Output_add_to_module(PyObject *module)
{
	OutputObjectType.tp_new = PyType_GenericNew;
	if (PyType_Ready(&OutputObjectType) < 0)
		return;

	Py_INCREF(&OutputObjectType);
	PyModule_AddObject(module, "Output", (PyObject *)&OutputObjectType);

        Output_error = PyErr_NewException("Output.error", NULL, NULL);
        PyModule_AddObject(module, "error", Output_error);

}


#define Output_Check1(v)  ((v)->ob_type == &OutputObjectType)

inline char  Output_Check(PyObject *v) 
{
	if (v->ob_type==&OutputObjectType)
		return 1;
	return 0;
}

