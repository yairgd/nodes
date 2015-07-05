#pragma once
#pragma once
typedef struct {
	PyObject_HEAD
	PyObject *id;
	PyObject *output;
} OutputObject;


static PyObject* Node_yair(PyObject *self, PyObject *args);
static PyObject* Output_repr(PyObject *self, PyObject *args);
static PyObject* Output_set_group_id(PyObject *self, PyObject *args);
	
