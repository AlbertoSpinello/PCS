#ifndef __EMPTY_H
#define __EMPTY_H
#include "Eigen/Eigen"
#include <iostream>
#include <fstream>
#include <queue>
#include <vector>


using namespace std;
using namespace Eigen;
namespace ProjectLibrary
{
// il vertice è formato da due punti nel piano: x e y
struct Vertex
{
    double x;
    double y;
    unsigned int id;

    Vertex(double x, double y, unsigned int id):
        x(x), y(y), id(id)
    {}
    Vertex():
        x(0), y(0), id(0)
    {}

    inline bool operator>(const Vertex& other) const
    {return id > other.id;}
    inline bool operator<(const Vertex& other) const
    {return id < other.id;}
    inline bool operator==(const Vertex& other) const
    {return id == other.id;}
    inline bool operator!=(const Vertex& other) const
    {return !(id == other.id);}

};
// il lato è formato da puntatori a due vertici
class Triangle; //forward declaration
struct Edge
{
    bool active = true;
    Vertex start;
    Vertex finish ;
    vector<unsigned int> adjTriangles;
    unsigned int id;
    double length;
    Edge(Vertex start, Vertex finish, unsigned int id):
        start(start), finish(finish), id(id)
    {
        length = sqrt(pow(finish.y-start.y, 2) + pow(finish.x-start.x, 2));
    }
    Edge():
        id(0)
    {}
    void set_length()
    {
        length = sqrt(pow(finish.y-start.y, 2) + pow(finish.x-start.x, 2));
    }

    inline bool operator>(const Edge& other)
    {return length > other.length;}

    inline bool operator<(const Edge& other)
    {return length  < other.length;}

    inline bool operator==(const Edge& other)
    {return id == other.id;}

    inline bool operator!= (const Edge& other)
    {return !(id == other.id);}

};
// il triangolo è formato da area, id, lati e vertici
class Triangle
{
private:
public:
    bool active = true;
    double area;
    unsigned int id;
    vector<Edge> edges;
    vector<Vertex> vertices;
    Triangle (vector<Edge> edge, unsigned int id):
        id(id), edges(edge)
    {
        vertices.push_back(edges[0].start);
        vertices.push_back(edges[0].finish);
        if (edges[1].start != edges[0].start && edges[1].start != edges[0].finish)
            vertices.push_back(edges[1].start);
        else
            vertices.push_back(edges[1].finish);
        area = abs(vertices[0].x*(vertices[1].y-vertices[2].y) +
                   vertices[1].x*(vertices[2].y-vertices[0].y)  +
                   vertices[2].x*(vertices[0].y-vertices[1].y)) / 2;
    }

    Triangle():
        id(0)
    {}

    // quando confronto due triangoli, uno è maggiore dell'altro in base alle loro aree
    inline bool operator>(const Triangle& other)
    {return area  > other.area;}

    inline bool operator<(const Triangle& other)
    {return area < other.area;}

    inline bool operator==(const Triangle& other)
    {return id == other.id;}

    inline bool operator!=(const Triangle& other)
    {return !(id == other.id);}

    void set_area()
    {
        area = abs(vertices[0].x*(vertices[1].y-vertices[2].y) +
                   vertices[1].x*(vertices[2].y-vertices[0].y)  +
                   vertices[2].x*(vertices[0].y-vertices[1].y)) / 2;
    }
};

//sfrutto l'algortimo di quicksort per ordinare i triangoli in base alla loro area
void Refine(vector<Triangle>& triangles, vector<Edge>& edges, vector<Vertex>& vertices, unsigned int &n);
//importa i vertici della mesh triangolare
bool ImportCell0Ds(vector<Vertex>& vertices, unsigned int n, string& test);
//importa i lati della mesh triangolare
bool ImportCell1Ds(vector<Edge>& edges, vector<Vertex>& vertices, unsigned int n, string& test);
//importa i triangoli della mesh
bool ImportCell2Ds(vector<Triangle>& triangles, vector<Edge>& edges, vector<Vertex>& vertices, unsigned int n, string &test);
//divido triangolo in 2
void split2(vector<Triangle> &triangles, vector<Edge> &edges, vector<Vertex> &vertices, unsigned int m,
            deque<unsigned int> &tempId, unsigned int &k, bool &permissible, deque<unsigned int> &tempId1);
//divido i triangoli successivi in 3 sottotriangoli
void split3(vector<Triangle> &triangles, vector<Edge> &edges, vector<Vertex> &vertices,
            deque<unsigned int> &tempId, unsigned int &k, bool &permissible, deque<unsigned int> &tempId1);
void split2again (vector<Triangle> &triangles, vector<Edge> &edges,vector<Vertex> &vertices,
                 unsigned int &k, bool &permissible, deque<unsigned int> &tempId, deque<unsigned int> &tempId1);

template <typename T>
inline void Pushback(vector<T> &edges, T &edge)
{
    edges.push_back(edge);
}

// sfrutto l'algortimo di quicksort per ordinare i triangoli in base alla loro area

inline void insertionSort(vector<Edge> &edge)
{
    unsigned int n= edge.size();
    for (int i = 0; i < n; i++)
    {
        Edge key = edge[i];
        int j = i - 1;
        while (j >= 0 && edge[j] < key)
        {
            edge[j + 1] = edge[j];
            j = j - 1;
        }
        edge[j + 1] = key;
    }
}
template <typename T>
inline void Erase(vector<T> &triangles, unsigned int index)
{
    if (index < triangles.size())
    {
        triangles.erase(triangles.begin() + index);
    }
}

inline unsigned int massimoElementoAttivo(vector<Triangle> &vettore)
{
    Triangle massimo;
    massimo.area = 0;
    for (Triangle &elemento : vettore)
    {
        if (elemento.active)
        { // Verifica se l'elemento è attivo
            if (elemento > massimo)
                massimo = elemento;
        }
    }
    return massimo.id;
}
inline Vertex getOppositeVertex(Triangle &triangle, Edge &edge)
{
    for (unsigned int i = 0; i < 3; i++)
    {
        if (triangle.vertices[i] != (edge.start) &&
            triangle.vertices[i] != (edge.finish))
            return triangle.vertices[i];
    }
    throw(runtime_error("Something went wrong getting the opposite vertex"));
}
// Prendo il punto medio di un lato
inline Vertex set_mid(Edge edge)
{
    Vertex mid;
    mid.x = (edge.finish.x + edge.start.x) / 2;
    mid.y = (edge.finish.y + edge.start.y) / 2;
    return mid; // Assegnare id e aggiornare vettore di vertici
}
}

#endif // __EMPTY_H

