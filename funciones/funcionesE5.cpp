#include <cassert>
#include <string>
#include <iostream>
#include <climits>
#include <limits>
#include "../clases/Grafo.cpp"
#include "../clases/Cola.cpp"
#include "../clases/MinHeap.cpp"
#include "definiciones.h"
using namespace std;

struct nodoLista
{
    int pos;
    nodoLista *sig;
};

struct mision
{
    int id;
    string nombre;
    int idCiudad;
};

struct ciudad
{
    int id;
    string nombre;
};

// PRE: Recibe una lista y un valor
// POS: Inserta al inicio de la lista el nodo con ese valor
void insertarInicio(int i, nodoLista *&l)
{
    nodoLista *nuevo = new nodoLista;
    nuevo->pos = i;
    nuevo->sig = l;
    l = nuevo;
}

// PRE: Recibe una lista no vacia
// POS: Elimina el inicio y devuelve el valor que tenia
int eliminarInicioYdevolver(nodoLista *&l)
{
    int dato = l->pos;
    nodoLista *aBorrar = l;
    l = l->sig;
    delete aBorrar;
    aBorrar = NULL;
    return dato;
}

// PRE: recibe el tamano del array
// POS: inicializa un array de tamano x con todos sus valores valor intMax
int *initCostos(int x)
{
    int *ret = new int[x];
    for (int i = 1; i < x; i++)
    {
        ret[i] = INT_MAX;
    }
    return ret;
}

// PRE: recibe el tamano del array
// POS: inicializa un array de tamano x con todos sus valores valor -1
int *initVengoDe(int x)
{
    int *ret = new int[x];
    for (int i = 1; i < x; i++)
    {
        ret[i] = -1;
    }
    return ret;
}

/*--------------Algoritmo Dijkstra v2--------------*/
// PRE: recibe un grafo de Ciudades, un origen y 2 array uno de costo y uno de vengoDe
// POS: Devuelve en costo y vengoDe los costos mas baratos del origen a su indice y con vengoDe se reconstruye el camino
void dijkstra(Grafo<ciudad *> *c, int org, int *&costo, int *&vengoDe)
{
    bool *visitados = new bool[c->cantidadVertices() + 1]();
    costo[org] = 0;
    MinHeap* cp = new MinHeap(c->cantidadVertices()+1);
    cp->add(org,0);
    while(!cp->estaVacio())
    {
        int v = cp->peak();
        cp->remove();
        visitados[v] = true;
        Arista *ady = c->adyacentes(v);
        while (ady)
        {
            int w = ady->destino;
            if (!visitados[w] && costo[w] > costo[v] + ady->peso)
            {
                costo[w] = costo[v] + ady->peso;
                vengoDe[w] = v;
                cp->add(w,costo[w]);
            }
            ady = ady->sig;
        }
    }

    delete[] visitados;
}

// PRE: Recibe un grafo de misiones
// POS: Devuelve un Array con los indices como contadores de la cantidad de aristas incidentes al vertice
int *DegreeM(Grafo<mision *> *m)
{
    int *ret = new int[m->cantidadVertices() + 1]();
    for (int i = 1; i < m->cantidadVertices() + 1; i++)
    {
        Arista *ady = m->adyacentes(i);
        while (ady)
        {
            ret[ady->destino]++;
            ady = ady->sig;
        }
    }
    return ret;
}
