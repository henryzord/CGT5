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

/**
 * Implementa o algoritmo gift wrapping para calcular o convex hull.
 * Começa pelo ponto mais abaixo, e procura os pontos subsequentes que possuem o menor ângulo com este vetor.
 */
PyObject *convexHullCore(int n_points, Vetor *points) {
    int lowestY = 2147483647;  // max int value
    int startIndex = -1;
    for(int i = 0; i < n_points; i++) {
        if(points[i].y < lowestY) {
            lowestY = i;
            startIndex = i;
        }
    }
    int n_newPoints = 1;
    Vetor *new_points = (Vetor*)malloc(sizeof(Vetor) * n_points);
    new_points[0] = points[startIndex];

    int curIndex = startIndex;
    int lastIndex = curIndex;

    do {
        int minAngle = 359;
        int maxIndex = -1;
        Vetor thisPoint = points[curIndex];
        Vetor thisFarRight(100, 0);

        for(int j = 0; j < n_points; j++) {
            if(j == curIndex) {
                continue;
            }
            Vetor thatPoint = points[j];
            Vetor thatTransPoint(thatPoint.x - thisPoint.x, thatPoint.y - thisPoint.y);

            int angle = thisFarRight.angle(thatTransPoint);
            if(thatTransPoint.y < 0) {
                angle += 180;
            }

            if(angle < minAngle) {
                minAngle = angle;
                maxIndex = j;
            }
        }
        lastIndex = curIndex;
        curIndex = maxIndex;
        new_points[n_newPoints] = points[curIndex];
        n_newPoints += 1;
    } while((curIndex != startIndex) && (n_newPoints < n_points));

    n_newPoints -= 1;  // last point will be the same as first

    PyObject *new_polygon = PyList_New(n_newPoints);
    for(int i = 0; i < n_newPoints; i++) {
        PyObject *thisPoint = PyTuple_Pack(2, Py_BuildValue("d", new_points[i].x), Py_BuildValue("d", new_points[i].y));
        PyList_SetItem(new_polygon, i, thisPoint);
    }
    free(new_points);
//    free(checked);
    return new_polygon;
}


static PyObject *convexHull(PyObject *self, PyObject *args) {
    PyObject *p_pontos;

    if (!PyArg_ParseTuple(
        args, "O!",
        &PyList_Type, &p_pontos
    )) {
        return NULL;
    }

    int num_vert = (int)PyList_Size(p_pontos);

    Vetor *pontos = (Vetor*)malloc(sizeof(Vetor) * num_vert);
    for(int i = 0; i < num_vert; i++) {
        PyObject *item = PyList_GetItem(p_pontos, i);

        double x = (double)PyFloat_AsDouble(PyTuple_GetItem(item, 0));
        double y = (double)PyFloat_AsDouble(PyTuple_GetItem(item, 1));
        int tuple_size = (int)PyTuple_Size(item);
        double z = 0;
        if(tuple_size > 2) {
            z = (double)PyFloat_AsDouble(PyTuple_GetItem(item, 2));
        }
        pontos[i] = Vetor(x, y, z);
    }

    PyObject *novosPontos = convexHullCore(num_vert, pontos);
    free(pontos);
    return novosPontos;
}


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
        "convexHull", convexHull, METH_VARARGS,
        "Transforms a concave polygon into a convex polygon"
    }, {
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
    PyPolygonType.tp_methods = polygon_instance_methods;
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