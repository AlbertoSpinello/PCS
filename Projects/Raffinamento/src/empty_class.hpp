#ifndef __EMPTY_H
#define __EMPTY_H
#include "Eigen/Eigen"
#include <iostream>
#include <fstream>
#include <queue>
#include <set>
#include <vector>


using namespace std;
using namespace Eigen;
namespace ProjectLibrary
{
inline double f0(double x, double y)
{
    return pow((x-0.5),(2)) + pow((y-0.5),(2));
}

inline double modGradf0(double x, double y)
{
    double sigma = 0.25;
    double A = 1.0;

    double centerPoints[5][2] = {{0.5, 0.5}, {0.0, 0.0}, {0.0, 1.0}, {1.0, 1.0}, {1.0, 0.0}};

    double dx = 0.0;
    double dy = 0.0;

    for (int i = 0; i < 1; i++) {
        double centerX = centerPoints[i][0];
        double centerY = centerPoints[i][1];

        double exponent = -((x - centerX) * (x - centerX) + (y - centerY) * (y - centerY)) / (2 * sigma * sigma);
        double gaussian = A * exp(exponent);

        dx += -(x - centerX) * gaussian / (sigma * sigma);
        dy += -(y - centerY) * gaussian / (sigma * sigma);

        }
    return sqrt(dx*dx +dy*dy);
}
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
    inline bool operator==(const Vertex& other)
    {return id == other.id;}
    inline bool operator!=(const Vertex& other)
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

    static constexpr double geometricTol = 1.0e-12;

    inline bool operator>(const Edge& other)
    {return length > other.length;}

    inline bool operator<(const Edge& other)
    {return length < other.length;}

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
    Vertex center;
    double gradValue;
    Triangle (vector<Edge> edge, unsigned int id):
        id(id), edges(edge)
    {
        set<Vertex> tempVertS;
        tempVertS.insert(edges[0].start);
        tempVertS.insert(edges[0].finish);
        if ((edges[1].start) != (edges[0].start) &&
            (edges[1].start) != (edges[0].finish))
            tempVertS.insert(edges[1].start);
        else
            tempVertS.insert(edges[1].finish);
        vector<Vertex> tempVertV(tempVertS.begin(), tempVertS.end());  //creo il vettore a partire dal set di punti
        vertices = tempVertV;
        area = abs(vertices[0].x*(vertices[1].y-vertices[2].y) +
                   vertices[1].x*(vertices[2].y-vertices[0].y)  +
                   vertices[2].x*(vertices[0].y-vertices[1].y)) / 2;
        center.x = (vertices[0].x + vertices[1].x + vertices[2].x)/3;
        center.y = (vertices[0].y + vertices[1].y + vertices[2].y)/3;
        gradValue = modGradf0(center.x, center.y);
    }

    Triangle():
        id(0)
    {}
    Triangle(vector<Vertex>& vertices):
        area(0)
    {
        area = abs(vertices[0].x*(vertices[1].y-vertices[2].y) +
                   vertices[1].x*(vertices[2].y-vertices[0].y)  +
                   vertices[2].x*(vertices[0].y-vertices[1].y)) / 2;
        center.x = (vertices[0].x + vertices[1].x + vertices[2].x)/3;
        center.y = (vertices[0].y + vertices[1].y + vertices[2].y)/3;
        gradValue = modGradf0(center.x, center.y);
    }
    static constexpr double geometricTol = 1.0e-12;
    // INSERIRE NELLE FORMULE LA TOLLERANZA GEOMETRICA
    // quando confronto due triangoli, uno è maggiore dell'altro in base alle loro aree


//    {
//        inline bool operator>(const Triangle& other)
//        {return area > other.area;}

//        inline bool operator<(const Triangle& other)
//        {return area < other.area;}
//    }

    inline bool operator>(const Triangle& other)
    {return (0.0001*gradValue+area) > (0.0001*(other.gradValue)+other.area);}

    inline bool operator<(const Triangle& other)
    {return (0.0001*gradValue+area) < (0.0001*(other.gradValue)+other.area);}


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
    void set_grad()
    {
        center.x = (vertices[0].x + vertices[1].x + vertices[2].x)/3;
        center.y = (vertices[0].y + vertices[1].y + vertices[2].y)/3;
        gradValue = modGradf0(center.x, center.y);
    }
};

//sfrutto l'algortimo di quicksort per ordinare i triangoli in base alla loro area
void Refine(vector<Triangle>& triangles, vector<Edge>& edges, vector<Vertex>& vertices, unsigned int &n, string &test);
//importa i vertici della mesh triangolare
bool ImportCell0Ds(vector<Vertex>& vertices, unsigned int n, string& test);
//importa i lati della mesh triangolare
bool ImportCell1Ds(vector<Edge>& edges, vector<Vertex>& vertices, unsigned int n, string& test);
//importa i triangoli della mesh
bool ImportCell2Ds(vector<Triangle>& triangles, vector<Edge>& edges, vector<Vertex>& vertices, unsigned int n, string &test);
// sfrutto l'algoritmo di insertion sort per ordinare array di piccole dimensioni, oppure parzialmente ordinati
inline void insertionSort(vector<Edge> &edges)
{
    int n=edges.size();
    for (int i = 0; i < n; i++)
    {
        Edge key = edges[i];
        int j = i - 1;
        while (j >= 0 && edges[j] < key)
        {
            edges[j + 1] = edges[j];
            j = j - 1;
        }
        edges[j + 1] = key;
    }
}
// ricava il vertice opposto dato un lato e un triangolo
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
//divido triangolo in 2
void split2(vector<Triangle> &triangles, vector<Edge> &edges, vector<Vertex> &vertices, unsigned int m,
            deque<unsigned int> &tempId, unsigned int &k, bool &permissible, deque<unsigned int> &tempId1);
//divido i triangoli successivi in 3 sottotriangoli
void split3(vector<Triangle> &triangles, vector<Edge> &edges, vector<Vertex> &vertices,
            deque<unsigned int> &tempId, unsigned int &k, bool &permissible, deque<unsigned int> &tempId1);
void split2again (vector<Triangle> &triangles, vector<Edge> &edges,
                 vector<Vertex> &vertices, unsigned int &k, bool &permissible, deque<unsigned int> &tempId1);
template <typename T>
inline void Pushback(vector<T> &edges, T &edge)
{
    edges.push_back(edge);
}
template <typename T>
inline void Erase(vector<T> &triangles, unsigned int index)
{
    if (index < triangles.size()) {
        triangles.erase(triangles.begin() + index);
    }
}
inline unsigned int massimoElementoAttivo(vector<Triangle> &vettore)
{
    Triangle massimo;
    massimo.area = 0;
    for (Triangle &elemento : vettore) {
        if (elemento.active) { // Verifica se l'elemento è attivo
            if (elemento > massimo)
                massimo = elemento;
        }
    }
    return massimo.id;
}
inline Vertex set_mid(Edge &edge) {
    Vertex mid;
    mid.x = (edge.finish.x + edge.start.x) / 2;
    mid.y = (edge.finish.y + edge.start.y) / 2;
    return mid; // Assegnare id e aggiornare vettore di vertici
}
}

#endif // __EMPTY_H

