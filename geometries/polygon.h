#ifndef POLYGON_H
#define POLYGON_H

#include <math.h>
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <string>
#include "twod_vectors.h"
#include <Python.h>
#include "structmember.h"  // for PyMemberDef

/**
 * Verifica se um polígono é côncavo ou convexo.
 * @param num_vert Número de vértices do polígono
 * @param points Pontos do polígono
 * @return 1 se o polígono for convexo, 0 se for côncavo
 */
bool isConvex(int n_points, Vetor *points) {
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
            return false;
        }
    }
    return true;
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

//    Vetor bbCenter;
//    double bbHeight;
//    double bbWidth;

} PyPolygon;

static PyTypeObject PyPolygonType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "Polygon"   /* tp_name */
};

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
        PyList_SetItem(self->py_vertices, j, Py_BuildValue("(ddd)", x, y, z));
    }
    self->n_vertices = num_vert;
    self->isThisConvex = isConvex(self->n_vertices, self->vertices);

    return 1;
}

static void PyPolygon_dealloc(PyPolygon * self) {
    free(self->vertices);
    Py_TYPE(self)->tp_free(self);
}

static Vetor readPoint(PyObject *py_point) {
    double xp = (double)PyFloat_AsDouble(PyTuple_GetItem(py_point, 0));
    double yp = (double)PyFloat_AsDouble(PyTuple_GetItem(py_point, 1));
    double zp = 0;
    int tuple_size = (int)PyTuple_Size(py_point);
    if(tuple_size > 2) {
        zp = (double)PyFloat_AsDouble(PyTuple_GetItem(py_point, 2));
    }
    Vetor point(xp, yp, zp);
    return point;
}

static PyObject *PyPolygon_isInside(PyPolygon *self, PyObject *args, PyObject *kwargs) {
    PyObject *py_point;

    static char *keywords[] = {"point", NULL};
    if(!PyArg_ParseTupleAndKeywords(args, kwargs, "O!", keywords, &PyTuple_Type, &py_point)) {
        return NULL;
    }

    Vetor point = readPoint(py_point);

    int isInside;
    if(self->isThisConvex) {
        isInside = isInsideConvex(self->n_vertices, self->vertices, point);
    } else {
        isInside = isInsideConcave(self->n_vertices, self->vertices, point);
    }
    if(isInside) {
        return Py_True;
    }
    return Py_False;
}

/**
 * Implementa o algoritmo gift wrapping para calcular o convex hull.
 * Começa pelo ponto mais abaixo, e procura os pontos subsequentes que possuem o menor ângulo com este vetor.
 */
static PyPolygon *PyPolygon_toConvexHull(PyPolygon *self) {
    int lowestY = 2147483647;  // max int value
    int startIndex = -1;
    for(int i = 0; i < self->n_vertices; i++) {
        if(self->vertices[i].y < lowestY) {
            lowestY = i;
            startIndex = i;
        }
    }
    int n_newPoints = 1;
    Vetor *new_points = (Vetor*)malloc(sizeof(Vetor) * self->n_vertices);
    new_points[0] = self->vertices[startIndex];

    int curIndex = startIndex;
    int lastIndex = curIndex;

    do {
        int minAngle = 359;
        int maxIndex = -1;
        Vetor thisPoint = self->vertices[curIndex];
        Vetor thisFarRight(100, 0);

        for(int j = 0; j < self->n_vertices; j++) {
            if(j == curIndex) {
                continue;
            }
            Vetor thatPoint = self->vertices[j];
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
        new_points[n_newPoints] = self->vertices[curIndex];
        n_newPoints += 1;
    } while((curIndex != startIndex) && (n_newPoints < self->n_vertices));

    n_newPoints -= 1;  // last point will be the same as first

//    PyPolygon *new_polygon = new PyPolygon;
    PyPolygon *new_polygon = PyObject_New(PyPolygon, &PyPolygonType);
    new_polygon->n_vertices = n_newPoints;
    new_polygon->vertices = new_points;
    new_polygon->isThisConvex = true;  // it has to be

    new_polygon->py_vertices = PyList_New(n_newPoints);

    for(int i = 0; i < n_newPoints; i++) {
        PyList_SetItem(
            new_polygon->py_vertices, i,
            Py_BuildValue("(ddd)", new_points[i].x, new_points[i].y, new_points[i].z)
        );
    }
    // TODO some seg fault problem here
//    return self;
    return new_polygon;
}


PyObject *PyPolygon_str(PyPolygon *self) {
    std::string res("[");

    for(int i = 0; i < self->n_vertices; i++) {
        char buffer[50];
        sprintf_s(buffer, "(%.1f, %.1f)", self->vertices[i].x, self->vertices[i].y);
        res += std::string(buffer);
        if(i < (self->n_vertices - 1)) {
            res += std::string(" ");
        }
    }
    return PyUnicode_FromFormat("%s]", res.c_str());
}

static PyMethodDef polygon_instance_methods[] = {
    {
        "isInside",
        (PyCFunction)PyPolygon_isInside, METH_VARARGS | METH_KEYWORDS,
        "Checks whether a point is inside this polygon"
    },
    {
        "toConvexHull",
        (PyCFunction)PyPolygon_toConvexHull, METH_NOARGS,
        "Converts a concave polygon to a convex one. Idempotent on convex polygons"
    },
    {NULL, NULL, 0, NULL}
};

static PyMemberDef PyPolygon_members[] = {
    {"n_vertices", T_INT, offsetof(PyPolygon, n_vertices), 0, "number of vertices of this polygon"},
    {"vertices", T_OBJECT, offsetof(PyPolygon, py_vertices), 0, "vertices of this polygon"},
    {"isThisConvex", T_BOOL, offsetof(PyPolygon, isThisConvex), 0, "whether this polygon is convex or not"},
    {NULL}  /* Sentinel */
};


#endif // POLYGON