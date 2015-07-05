///
///      @file  py_init.c
///     @brief  python Node module instantiation
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
#include "py_node.h"
#include "py_output.h"


void initNode()
{
	PyMethodDef *def;

	/* create a new module and class */
	PyObject *module = Py_InitModule("Node", NULL);

	// Make sure the GIL has been created since we need to acquire it in our
	// callback to safely call into the python application.
	if (! PyEval_ThreadsInitialized()) {
		PyEval_InitThreads();
	//	PyEval_ReleaseLock();
	} 

	Node_add_to_module(module);

	NodeManager_add_to_module(module);
	Output_add_to_module(module);



}

