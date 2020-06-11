//
//  Matematica.cpp
//  Testes
//
//  Created by Márcio Sarroglia Pinho on 19/03/20.
//  Copyright © 2020 Márcio Sarroglia Pinho. All rights reserved.
//
#define _USE_MATH_DEFINES

#include <stdio.h>
#include <math.h>
#include <cstdlib>
#include <vector>
#include <Python.h>

int compare_doubles(const void * a, const void * b) {
  return (int)( *(double*)a - *(double*)b );
}

class Vetor {
public:
    double x, y, z;

    Vetor() {
        x = 0;
        y = 0;
        z = 0;
    }

    Vetor(double x, double y) {
        this->x = x;
        this->y = y;
        this->z = 0;
    }

    Vetor(double x, double y, double z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    /**
     * Produto interno (com sobrecarga de operador)
     * @param other Outro vetor
     * @return
     */
    double operator *(Vetor const other) {
        return this->x * other.x + this->y * other.y + this->z * other.z;
    }

    /**
     * Produto interno (método).
     * @param other Outro vetor
     * @return
     */
    double dot(Vetor const other) {
        return this->x * other.x + this->y * other.y + this->z * other.z;
    }

    /**
     * Produto vetorial.
     * @param other Outro vetor
     * @return
     */
    Vetor cross(Vetor const other) {
        double nx = this->y * other.z - this->z * other.y;
        double ny = this->z * other.x - this->x * other.z;
        double nz = this->x * other.y - this->y * other.x;

        return {nx, ny, nz};
    }

    /**
     * Multiplicação de um vetor por um escalar, com sobrecarga de operador.
     * @param n Escalar
     * @return
     */
    Vetor operator *(double n) {
        return {this->x * n, this->y * n, this->z * n};
    }

    /**
     * Adição de um vetor por um escalar, com sobrecarga de operador.
     * @param n Escalar
     * @return
     */
    Vetor operator +(Vetor const other) {
        return {this->x + other.x, this->y + other.y, this->z + other.z};
    }

    /**
     * Subtração de um vetor por um escalar, com sobrecarga de operador.
     * @param n Escalar
     * @return
     */
    Vetor operator -(Vetor const other) {
        return {this->x - other.x, this->y - other.y, this->z - other.z};
    }

    /**
     * Módulo do vetor.
     * @return O módulo do vetor atual.
     */
    double module() {
        return sqrt(x * x + y * y + z * z);
    }

    /**
     * Versor do vetor.
     * @return Um novo vetor, que é um versor do vetor atual.
     */
    Vetor versor() {
        double mod = module();
        return {x / mod, y / mod, z / mod};
    }

    /**
     * Ângulo que este vetor faz com outro vetor.
     * @param other Outro vetor
     * @return Ângulo (em graus) entre este e outro vetor
     */
    int angle(Vetor other) {
        double dotProduct = this->dot(other);
        if(dotProduct == 0) {
            return 90;
        }

        double thisModule = this->module();
        double otherModule = other.module();
        double thisAngle = acos(dotProduct/(thisModule * otherModule));
        int thisDegreeAngle = (int)((thisAngle * 180.0) / M_PI);
        return thisDegreeAngle;
    }

    /**
     * Imprime o vetor atual.
     */
    void print() {
        printf("(%0.2f, %0.2f, %0.2f)", x, y, z);
    }
};

class Reta {
public:
    Vetor p1, p2;

    Reta(Vetor a, Vetor b) {
        this->p1 = a;
        this->p2 = b;
    }

    /**
     * Dada uma reta e um ponto, verifica se o ponto está sobre o segmento de reta
     * @param ponto
     * @return 1 se o ponto estiver no segmento da reta, 0 de outra forma
     */
    int onSegment(Vetor ponto) {
        double dist1 = sqrt(pow(this->p1.x - ponto.x, 2) + pow(this->p1.y - ponto.y, 2));
        double dist2 = sqrt(pow(this->p2.x - ponto.x, 2) + pow(this->p2.y - ponto.y, 2));

        double dist3 = sqrt(pow(this->p1.x - this->p2.x, 2) + pow(this->p1.y - this->p2.y, 2));

        return (dist1 + dist2) == dist3;
    }

    /**
     * Computa se duas retas se intersectam.
     * @param other Outra reta
     * @return 1 se as retas se intersectam, 0 de outra forma
     */
    int intersects(Reta other) {
        int o1 = (int)this->angle(other.p1);
        int o2 = (int)this->angle(other.p2);
        int o3 = (int)other.angle(this->p1);
        int o4 = (int)other.angle(this->p2);

        // caso geral
        if(o1 != o2 && o3 != o4) {
            return 1;
        }
        if(o1 == 0 && this->onSegment(other.p1)) {
            return 1;
        }
        if(o2 == 0 && this->onSegment(other.p2)) {
            return 1;
        }
        if(o3 == 0 && other.onSegment(this->p1)) {
            return 1;
        }
        if(o4 == 0 && other.onSegment(this->p2)) {
            return 1;
        }
        return 0;
    }

    /**
     * Orientação entre ponto e reta: 0 se forem colineares, 1 se estiverem em sentido horário, 2 anti-horário
     * @param ponto
     * @return
     */
    double angle(Vetor ponto) {
        int val = (int)((this->p2.y - this->p1.y) * (ponto.x - this->p2.x) - (this->p2.x - this->p1.x) * (ponto.y - this->p2.y));

        // ponto e reta são colineares
        if (val == 0) {
            return 0;
        }
        return (val > 0)? 1: 2; // horário ou anti-horário
    }
};


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

class Polygon {
public:
    int n_vertices;
    Vetor *vertices;
    bool isThisConvex;

    Polygon(int n_vertices, Vetor *vertices) {
        this->vertices = (Vetor*)malloc(sizeof(Vetor) * n_vertices);
        for(int i = 0; i < n_vertices; i++) {
            this->vertices[i] = vertices[i];
        }
        this->n_vertices = n_vertices;
        this->isThisConvex = (bool)isConvex(this->n_vertices, this->vertices);
    }

    ~Polygon() {
        free(this->vertices);
    }
};

class Map {
public:
    int n_polygons;
    Polygon *polygons;

    int n_slabs;
    double *slabs;
    std::vector<std::vector<int>> slabsIndices;

    Map(int n_polygons, Polygon *polygons) {
        this->n_polygons = n_polygons;
        this->polygons = (Polygon*)malloc(sizeof(Polygon) * this->n_polygons);
        int n_vertices = 0;

        for(int i = 0; i < this->n_polygons; i++) {
            this->polygons[i] = polygons[i];
            n_vertices += this->polygons[i].n_vertices;
        }

        std::vector<Vetor> limits;
        this->slabs = (double*)malloc(sizeof(double) * n_vertices);
        this->n_slabs = 0;
        for(int i = 0; i < this->n_polygons; i++) {
            Vetor current(0, 0);
            for(int j = 0; j < this->polygons[i].n_vertices; j++) {
                this->slabs[this->n_slabs] = this->polygons[i].vertices[j].y;
                current.x = fmin(current.x, this->polygons[i].vertices[j].y);
                current.y = fmax(current.y, this->polygons[i].vertices[j].y);
                this->n_slabs += 1;
            }
            limits.push_back(current);
        }
        qsort(this->slabs, this->n_slabs, sizeof(double), compare_doubles);
        for(int i = 0; i < this->n_slabs; i++) {
            std::vector<int> thisIndices;
            for(int j = 0; j < limits.size(); j++) {
                if((this->slabs[i] >= limits[j].x) && (this->slabs[i] <= limits[j].y)) {
                    thisIndices.push_back(j);
                }
            }
            this->slabsIndices.push_back(thisIndices);
        }
    }

    /**
     * Checa se um ponto está dentro de um polígono desse mapa.
     * @param point Ponto a ser checado
     * @return NULL se o ponto não está dentro de nenhum polígono; ou um ponteiro para o
     * polígono ao qual esse ponto está inserido
     */
    Polygon *checkInside(Vetor point) {
        int index = -1;
        for(int i = 0; i < this->n_slabs; i++) {
            if(point.y >= this->slabs[i]) {
                index = i;
            }
        }
        if(index == -1) {
            return NULL;
        }
        std::vector<int> indices = this->slabsIndices[index];
        int res = 0;
        for(int j = 0; j < indices.size(); j++) {
            if(this->polygons[indices[j]].isThisConvex) {
                res = isInsideConvex(this->polygons[indices[j]].n_vertices, this->polygons[indices[j]].vertices, point);
            } else {
                res = isInsideConcave(this->polygons[indices[j]].n_vertices, this->polygons[indices[j]].vertices, point);
            }
            if(res == 1) {
                return &this->polygons[indices[j]];
            }
        }
        return NULL;
    }

    ~Map() {
        free(polygons);
        free(this->slabs);
    }
};

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


static PyObject *commonCheckCode(PyObject *self, PyObject *args, bool concave) {
    PyObject *p_pontos, *p_ponto;

    if (!PyArg_ParseTuple(
        args, "O!O!",
        &PyList_Type, &p_pontos,
        &PyTuple_Type, &p_ponto
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

    double xp = (double)PyFloat_AsDouble(PyTuple_GetItem(p_ponto, 0));
    double yp = (double)PyFloat_AsDouble(PyTuple_GetItem(p_ponto, 1));
    double zp = 0;
    int tuple_size = (int)PyTuple_Size(p_ponto);
    if(tuple_size > 2) {
        zp = (double)PyFloat_AsDouble(PyTuple_GetItem(p_ponto, 2));
    }
    Vetor ponto(xp, yp, zp);

    int isInside;
    if(concave) {
        isInside = isInsideConcave(num_vert, &pontos[0], ponto);
    } else {
        isInside = isInsideConvex(num_vert, &pontos[0], ponto);
    }
    free(pontos);
    return Py_BuildValue("i", isInside);
}

static PyObject* isInsideConcavePolygon(PyObject *self, PyObject *args) {
    return commonCheckCode(self, args, true);
}

static PyObject* isInsideConvexPolygon(PyObject *self, PyObject *args) {
    return commonCheckCode(self, args, false);
}

// Method definition object for this extension, these argumens mean:
// ml_name: The name of the method
// ml_meth: Function pointer to the method implementation
// ml_flags: Flags indicating special features of this method, such as
//          accepting arguments, accepting keyword arguments, being a
//          class method, or being a static method of a class.
// ml_doc:  Contents of this method's docstring
static PyMethodDef module_methods[] = {{
    "isInsideConcavePolygon", isInsideConcavePolygon, METH_VARARGS,
    "Checks whether a point is inside a concave polygon"
    }, {
    "isInsideConvexPolygon", isInsideConvexPolygon, METH_VARARGS,
    "Checks whether a point is inside a convex polygon"
    }, {
    "convexHull", convexHull, METH_VARARGS,
    "Transforms a concave polygon into a convex polygon"
    },
    {NULL, NULL, 0, NULL}
};

// Module definition
// The arguments of this structure tell Python what to call your extension,
// what it's methods are and where to look for it's method definitions
static struct PyModuleDef definition = {
    PyModuleDef_HEAD_INIT,
    "polygons",
    "A Python module to compute whether points are inside or outside polygons.",
    -1,
    module_methods
};

// Module initialization
// Python calls this function when importing your extension. It is important
// that this function is named PyInit_[[your_module_name]] exactly, and matches
// the name keyword argument in setup.py's setup() call.
PyMODINIT_FUNC PyInit_polygons(void) {
    Py_Initialize();
    return PyModule_Create(&definition);
}