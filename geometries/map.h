#ifndef MAP_H
#define MAP_H

#define N_VERTICES_SLAB 150

#include "twod_vectors.h"
#include "polygon.h"

#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <string>
#include <math.h>
#include <vector>
#include <Python.h>
#include "structmember.h"  // for PyMemberDef

typedef struct {
    PyObject_HEAD
    int n_polygons;
    PyPolygon **polygons;
    int n_slabs;
    double *slabs;
    PyObject *py_slabs;
    PyObject *py_polygons;
    std::vector<std::vector<int>> slabsIndices;
} PyMap;

static PyTypeObject PyMapType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "Map"   /* tp_name */
};

static int PyMap_init(PyMap *self, PyObject *args, PyObject *kwargs) {
    static char *keywords[] = {"polygons", NULL};
    PyObject *py_polygons;
    if(!PyArg_ParseTupleAndKeywords(args, kwargs, "O!", keywords, &PyList_Type, &py_polygons)) {
        return NULL;
    }

    self->n_polygons = (int)PyList_Size(py_polygons);
    self->polygons = (PyPolygon**)malloc(sizeof(PyPolygon*) * self->n_polygons);
    self->py_polygons = py_polygons;

    for(int i = 0; i < self->n_polygons; i++) {
        self->polygons[i] = (PyPolygon*)PyList_GetItem(py_polygons, i);
    }

    std::vector<Vetor> limits;

    double absoluteLower = self->polygons[0]->vertices[0].y, absoluteUpper = self->polygons[0]->vertices[0].y;
    for(int i = 0; i < self->n_polygons; i++) {
        double lower = -1.0, upper = -1.0;
        for(int j = 0; j < self->polygons[i]->n_vertices; j++) {
            if(j == 0) {
                lower = self->polygons[i]->vertices[j].y;
                upper = self->polygons[i]->vertices[j].y;
            } else {
                lower = fmin(lower, self->polygons[i]->vertices[j].y);
                upper = fmax(upper, self->polygons[i]->vertices[j].y);
                absoluteLower = fmin(absoluteLower, lower);
                absoluteUpper = fmax(absoluteUpper, upper);
            }
        }
        limits.push_back(Vetor(lower, upper));
    }

    self->slabs = new double [N_VERTICES_SLAB];
    self->py_slabs = PyList_New(N_VERTICES_SLAB);
    self->n_slabs = N_VERTICES_SLAB;
    double increment = (absoluteUpper - absoluteLower)/self->n_slabs;
    for(int i = 0; i < N_VERTICES_SLAB; i++) {
        self->slabs[i] = absoluteLower + i * increment;
        PyList_SetItem(self->py_slabs, i, Py_BuildValue("d", self->slabs[i]));
    }

    for(int i = 0; i < self->n_slabs; i++) {
        std::vector<int> thisIndices;
        for(int j = 0; j < limits.size(); j++) {
            if((self->slabs[i] >= limits[j].x) && (self->slabs[i] <= limits[j].y)) {
                thisIndices.push_back(j);
            }
        }
        self->slabsIndices.push_back(thisIndices);
    }
    return 1;
}

static void PyMap_dealloc(PyMap * self) {
    for(int i = 0; i < self->n_polygons; i++) {
        Py_DECREF(&self->polygons[i]);
    }
    Py_DECREF(self->py_slabs);
    Py_DECREF(self->py_polygons);
    free(self->slabs);
    Py_TYPE(self)->tp_free(self);
}

/**
 * Checa se um ponto está dentro de um polígono desse mapa.
 * @param point Ponto a ser checado
 * @return -1 se o ponto não está dentro de nenhum polígono; ou o índice do polígono ao qual esse ponto
 * está inserido
 */
PyObject *PyMap_checkInside(PyMap *self, PyObject *args, PyObject *kwargs) {
    static char *keywords[] = {"point", NULL};
    PyObject *py_point;
    if(!PyArg_ParseTupleAndKeywords(args, kwargs, "O!", keywords, &PyTuple_Type, &py_point)) {
        return NULL;
    }

    Vetor point = readPoint(py_point);

    int index = -1;
    for(int i = 0; i < self->n_slabs; i++) {
        if(point.y <= self->slabs[i]) { // TODO flipped comparison
            index = i;
            break;
        }
    }
    Py_INCREF(&PyMapType);

    if(index == -1) {
        return Py_BuildValue("i", -1);
    }

    // indices has the set of indices of polygons in that region
    std::vector<int> indices = self->slabsIndices[index];
    PyObject *res;
    PyObject *other_args = PyTuple_New(1);
    PyTuple_SetItem(other_args, 0, py_point);
    PyObject *other_kwargs = PyDict_New();

    // for each polygon in that region
    for(int j = 0; j < indices.size(); j++) {
        res = PyPolygon_isInside(self->polygons[indices[j]], other_args, other_kwargs);
        if(res == Py_True) {
            return Py_BuildValue("i", indices[j]);
        }
    }
    return Py_BuildValue("i", -1);
}

static PyMethodDef PyMap_methods[] = {
    {
        "checkInside",
        (PyCFunction)PyMap_checkInside, METH_VARARGS | METH_KEYWORDS,
        "Checks whether a point is inside this map. If true, returns the index of the polygon; -1 if it is outside"
    },
//    {
//        "toConvexHull",
//        (PyCFunction)PyPolygon_toConvexHull, METH_NOARGS,
//        "Converts a concave polygon to a convex one. Idempotent on convex polygons"
//    },
    {NULL, NULL, 0, NULL}
};

//int n_polygons;
//PyPolygon **polygons;
//int n_slabs;
//double *slabs;

static PyMemberDef PyMap_members[] = {
    {"n_polygons", T_INT, offsetof(PyMap, n_polygons), 0, "number of polygons in this map"},
    {"polygons", T_OBJECT, offsetof(PyMap, py_polygons), 0, "polygons of this map"},
    {"slabs", T_OBJECT, offsetof(PyMap, py_slabs), 0, "slabs of this map"},
//    {"isThisConvex", T_BOOL, offsetof(PyPolygon, isThisConvex), 0, "whether this polygon is convex or not"},
    {NULL}  /* Sentinel */
};


#endif // MAP_H