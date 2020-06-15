#ifndef MATH_PRIMITIVES_H
#define MATH_PRIMITIVES_H

#include <math.h>
#include <stdio.h>
#include <cstdlib>
#include "twod_vectors.h"
#include <Python.h>
#include "structmember.h"  // for PyMemberDef

/**
 * Verifica se um polígono é côncavo ou convexo.
 * @param num_vert Número de vértices do polígono
 * @param points Pontos do polígono
 * @return 1 se o polígono for convexo, 0 se for côncavo
 */
int isConvex(int n_points, Vetor *points) {
    int signal = 1;

    for(int i = 0; i < n_points; i++) {
        int index[3] = {
            ((i - 1) < 0) * (n_points - 1) + ((i - 1) > 0) * (i - 1),
            i,
            ((i + 1) < n_points) * (i + 1) + ((i + 1) > n_points) * 0
        };

        Vetor a = points[index[1]] - points[index[0]];
        Vetor b = points[index[1]] - points[index[2]];

        Vetor c1 = a.cross(b);
        int new_signal = c1.z >= 0? 1 : -1;
        if(i == 0) {
            signal = new_signal;
        } else if(new_signal != signal) {
            return 0;
        }
    }
    return 1;
}


/**
 * Verifica se um Vetor está dentro de um polígono côncavo.
 * @param num_vert Número de vértices do polígono
 * @param Vetors Coordenadas dos vértices
 * @param Vetor Coordenada do Vetor
 * @return 1 se o Vetor estiver dentro do polígono, 0 se não
 */
int isInsideConcave(int num_vert, Vetor *pontos, Vetor ponto) {
    int inside = 0;
    for(int i = 0; i < num_vert; ++i) {
        Vetor v1 = pontos[i];
        Vetor v2 = pontos[(i+1)%num_vert];

        int considered = (v1.y > ponto.y) != (v2.y > ponto.y);
        int in_half_plane = ponto.x < ((((v2.x - v1.x) * (ponto.y - v1.y)) / (v2.y - v1.y)) + v1.x);
        int intersect = considered && in_half_plane;
        if(intersect) inside = !inside;
    }

    return inside;
}

/**
 * Verifica se um Vetor está dentro de um polígono convexo.
 * @param num_vert Número de vértices do polígono
 * @param vetores Coordenadas dos vértices
 * @param Vetor Coordenada do Vetor
 * @return 1 se o Vetor estiver dentro do polígono, 0 se não
 */
int isInsideConvex(int num_vert, Vetor *vetores, Vetor ponto) {
    int signal = 1;

    for(int i = 0; i < num_vert; i++) {
        int index[2] = {
            ((i - 1) < 0) * (num_vert - 1) + ((i - 1) > 0) * (i - 1),
            i
        };

        Vetor a = vetores[index[1]] - vetores[index[0]];
        Vetor b = vetores[index[1]] - ponto;

        Vetor c1 = a.cross(b);
        int new_signal = c1.z >= 0? 1 : -1;
        if(i == 0) {
            signal = new_signal;
        } else if(new_signal != signal) {
            return 0;
        }
    }
    return 1;
}

typedef struct {
    PyObject_HEAD
    int n_vertices;
    Vetor *vertices;
    PyObject *py_vertices;
    bool isThisConvex;

    Vetor bbCenter;
    double bbHeight;
    double bbWidth;

} PyPolygon;

static PyTypeObject PyPolygonType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "Polygon"   /* tp_name */
};

//    PolygonClass() {
//    }
//
//    PolygonClass(int n_vertices, Vetor *vertices) {
//        this->vertices = vertices;
//        this->n_vertices = n_vertices;
//        this->isThisConvex = (bool)isConvex(this->n_vertices, this->vertices);
//    }

static int PyPolygon_init(PyPolygon *self, PyObject *args, PyObject *kwargs) {
    static char *keywords[] = {"thisPolygon", NULL};
    PyObject *thisPolygon;
    PyArg_ParseTupleAndKeywords(args, kwargs, "O!", keywords, &PyList_Type, &thisPolygon);

    int num_vert = (int)PyList_Size(thisPolygon);
    self->vertices = new Vetor [num_vert];
    self->py_vertices = PyList_New(num_vert);

    for(int j = 0; j < num_vert; j++) {
        PyObject *vertex = PyList_GetItem(thisPolygon, j);

        double x = (double)PyFloat_AsDouble(PyTuple_GetItem(vertex, 0));
        double y = (double)PyFloat_AsDouble(PyTuple_GetItem(vertex, 1));
        int tuple_size = (int)PyTuple_Size(vertex);
        double z = 0;
        if(tuple_size > 2) {
            z = (double)PyFloat_AsDouble(PyTuple_GetItem(vertex, 2));
        }
        self->vertices[j] = Vetor(x, y, z);
        PyList_SetItem(self->py_vertices, j, Py_BuildValue("(ddd)", x, y, z)); // TODO wrong, fix later
    }
    self->n_vertices = num_vert;
    self->isThisConvex = (bool)isConvex(self->n_vertices, self->vertices);

    return 1;
}

static void PyPolygon_dealloc(PyPolygon * self) {
    free(self->vertices);
    Py_TYPE(self)->tp_free(self);
}

static PyObject* Polygon__increase_meter(PyObject *self, PyObject *args, PyObject *kwargs) {
    static char *keywords[] = {"how_much", NULL};
    int how_much;
    PyArg_ParseTupleAndKeywords(args, kwargs, "i", keywords, &how_much);

    printf("increasing happy meter by %d\n", how_much);

    Py_INCREF(Py_None);
    return Py_None;
}


PyObject *PyPolygon_str(PyPolygon *self) {
    // TODO implement later
//    for(int i = 0; i < self->n_vertices; i++) {
//        printf("(%.1f, %.1f)\n", self->vertices[i].x, self->vertices[i].y);
//    }
    return Py_BuildValue("s", "not implemented yet");
}

static PyMethodDef polygon_instance_methods[] = {{
    "increase_meter",
    (PyCFunction)Polygon__increase_meter, METH_VARARGS | METH_KEYWORDS,
    "Increases the happy meter by a given value"
    },
    {NULL, NULL, 0, NULL}
};

static PyMemberDef PyPolygon_members[] = {
    // int n_vertices;
//    Vetor *vertices;
//    bool isThisConvex;
//
//    Vetor bbCenter;
//    double bbHeight;
//    double bbWidth;
    {"n_vertices", T_INT, offsetof(PyPolygon, n_vertices), 0, "number of vertices of this polygon"},
    {"vertices", T_OBJECT, offsetof(PyPolygon, py_vertices), 0, "vertices of this polygon"},
//    {"first", T_OBJECT_EX, offsetof(Noddy, first), 0, "first name"},
//    {"last", T_OBJECT_EX, offsetof(Noddy, last), 0, "last name"},
//    {"number", T_INT, offsetof(Noddy, number), 0, "noddy number"},
    {NULL}  /* Sentinel */
};


//int compare_doubles(const void * a, const void * b) {
//  return (int)( *(double*)a - *(double*)b );
//}

#endif // MATH_PRIMITIVES_H