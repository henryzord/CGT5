//
//  Matematica.cpp
//  Testes
//
//  Created by Márcio Sarroglia Pinho on 19/03/20.
//  Copyright © 2020 Márcio Sarroglia Pinho. All rights reserved.
//

#include "polygon.h"
#include "map.h"

#include <stdio.h>
#include <cstdlib>
#include <Python.h>
#include "structmember.h"  // for PyMemberDef


PyObject *slabAlgorithm(PyObject *self, PyObject *args) {
//    PyObject *p_polygons;
//    PyObject *p_points;
//
//    if (!PyArg_ParseTuple(
//        args, "O!O!",
//        &PyList_Type, &p_polygons,
//        &PyList_Type, &p_points
//    )) {
//        return NULL;
//    }
//
//    Map myMap(p_polygons);
//
//    int n_points = (int)PyList_Size(p_points);
//
//    PyObject *indices = PyList_New(n_points);
//
//    for(int i = 0; i < n_points; i++) {
//        PyObject *vertex = PyList_GetItem(p_points, i);
//        double x = (double)PyFloat_AsDouble(PyTuple_GetItem(vertex, 0));
//        double y = (double)PyFloat_AsDouble(PyTuple_GetItem(vertex, 1));
//        int tuple_size = (int)PyTuple_Size(vertex);
//        double z = 0;
//        if(tuple_size > 2) {
//            z = (double)PyFloat_AsDouble(PyTuple_GetItem(vertex, 2));
//        }
//        Vetor point = Vetor(x, y, z);
//
//        PyList_SetItem(indices, i, Py_BuildValue("i", myMap.checkInside(point)));
//    }
//    return indices;
    return Py_BuildValue("s", "not implemented yet");
}

//------------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------------//


// Method definition object for this extension, these argumens mean:
// ml_name: The name of the method
// ml_meth: Function pointer to the method implementation
// ml_flags: Flags indicating special features of this method, such as
//          accepting arguments, accepting keyword arguments, being a
//          class method, or being a static method of a class.
// ml_doc:  Contents of this method's docstring
static PyMethodDef module_methods[] = {//{
//    "isInsideConcavePolygon", isInsideConcavePolygon, METH_VARARGS,
//    "Checks whether a point is inside a concave polygon"
//    }, {
//    "isInsideConvexPolygon", isInsideConvexPolygon, METH_VARARGS,
//    "Checks whether a point is inside a convex polygon"
//    }, {
    {
        "slabAlgorithm", slabAlgorithm, METH_VARARGS,
        "Checks in which polygon a point is inside based on the Slab Algorithm"
    },
    {NULL, NULL, 0, NULL}
};

// Module definition
// The arguments of this structure tell Python what to call your extension,
// what it's methods are and where to look for it's method definitions
static struct PyModuleDef module_definition = {
    PyModuleDef_HEAD_INIT,
    "geometries",
    "A Python module to compute whether points are inside or outside geometries.",
    -1,
    module_methods
};

// Module initialization
// Python calls this function when importing your extension. It is important
// that this function is named PyInit_[[your_module_name]] exactly, and matches
// the name keyword argument in setup.py's setup() call.
PyMODINIT_FUNC PyInit_geometries(void) {
    PyObject* module_py;

    PyPolygonType.tp_new = PyType_GenericNew;
    PyPolygonType.tp_basicsize = sizeof(PyPolygon);
    PyPolygonType.tp_dealloc=(destructor) PyPolygon_dealloc;
    PyPolygonType.tp_flags=Py_TPFLAGS_DEFAULT;
    PyPolygonType.tp_doc="Polygon object";
    PyPolygonType.tp_methods=polygon_instance_methods;
    PyPolygonType.tp_members=PyPolygon_members;
    PyPolygonType.tp_str=(reprfunc)PyPolygon_str;
    PyPolygonType.tp_repr=(reprfunc)PyPolygon_str;
    PyPolygonType.tp_init=(initproc)PyPolygon_init;

    if (PyType_Ready(&PyPolygonType) < 0) {
        return NULL;
    }

    module_py = PyModule_Create(&module_definition);
    if (!module_py) {
        return NULL;
    }

    Py_INCREF(&PyPolygonType);
    PyModule_AddObject(module_py, "Polygon", (PyObject *)&PyPolygonType);
    return module_py;
}