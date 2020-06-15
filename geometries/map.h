#ifndef MAP_H
#define MAP_H

#define N_VERTICES_SLAB 150

#include "twod_vectors.h"
#include "polygon.h"

#include <math.h>
#include <vector>
#include <Python.h>

//class Map {
//public:
//    int n_polygons;
//    PolygonClass **polygons;
//
//    int n_slabs;
//    double *slabs;
//    std::vector<std::vector<int>> slabsIndices;
//
//    Map(PyObject *p_polygons) {
//        this->n_polygons = (int)PyList_Size(p_polygons);
//        this->polygons = (PolygonClass**)malloc(sizeof(PolygonClass*) * this->n_polygons);
//
//        for(int i = 0; i < this->n_polygons; i++) {
//            this->polygons[i] = new PolygonClass(PyList_GetItem(p_polygons, i));
//        }
//
//        std::vector<Vetor> limits;
//
//        double absoluteLower = this->polygons[0]->vertices[0].y, absoluteUpper = this->polygons[0]->vertices[0].y;
//        for(int i = 0; i < this->n_polygons; i++) {
//            double lower = -1.0, upper = -1.0;
//            for(int j = 0; j < this->polygons[i]->n_vertices; j++) {
//                if(j == 0) {
//                    lower = this->polygons[i]->vertices[j].y;
//                    upper = this->polygons[i]->vertices[j].y;
//                } else {
//                    lower = fmin(lower, this->polygons[i]->vertices[j].y);
//                    upper = fmax(upper, this->polygons[i]->vertices[j].y);
//                    absoluteLower = fmin(absoluteLower, lower);
//                    absoluteUpper = fmax(absoluteUpper, upper);
//                }
//            }
//            limits.push_back(Vetor(lower, upper));
//        }
//
//        this->slabs = new double [N_VERTICES_SLAB];
//        this->n_slabs = N_VERTICES_SLAB;
//        double increment = (absoluteUpper - absoluteLower)/this->n_slabs;
//        for(int i = 0; i < N_VERTICES_SLAB; i++) {
//            this->slabs[i] = absoluteLower + i * increment;
//        }
//
//        for(int i = 0; i < this->n_slabs; i++) {
//            std::vector<int> thisIndices;
//            for(int j = 0; j < limits.size(); j++) {
//                if((this->slabs[i] >= limits[j].x) && (this->slabs[i] <= limits[j].y)) {
//                    thisIndices.push_back(j);
//                }
//            }
//            this->slabsIndices.push_back(thisIndices);
//        }
//    }
//
//    /**
//     * Checa se um ponto está dentro de um polígono desse mapa.
//     * @param point Ponto a ser checado
//     * @return -1 se o ponto não está dentro de nenhum polígono; ou o índice do polígono ao qual esse ponto
//     * está inserido
//     */
//    int checkInside(Vetor point) {
//        int index = -1;
//        for(int i = 0; i < this->n_slabs; i++) {
//            if(point.y >= this->slabs[i]) {
//                index = i;
//            }
//        }
//        if(index == -1) {
//            return -1;
//        }
//        std::vector<int> indices = this->slabsIndices[index];
//        int res = 0;
//        for(int j = 0; j < indices.size(); j++) {
//            if(this->polygons[indices[j]]->isThisConvex) {
//                res = isInsideConvex(this->polygons[indices[j]]->n_vertices, this->polygons[indices[j]]->vertices, point);
//            } else {
//                res = isInsideConcave(this->polygons[indices[j]]->n_vertices, this->polygons[indices[j]]->vertices, point);
//            }
//            if(res == 1) {
//                return indices[j];
//            }
//        }
//        return -1;
//    }
//
//    ~Map() {
//        for(int i = 0; i < this->n_polygons; i++) {
//            free(this->polygons[i]);
//        }
//        free(this->slabs);
//    }
//};

#endif // MAP_H