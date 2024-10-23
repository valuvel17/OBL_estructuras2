#include <cassert>
#include <string>
#include <iostream>
#include <climits>
#include <limits>
#include "../clases/Grafo.cpp"
#include "../clases/Cola.cpp"
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

void insertarInicio(int i, nodoLista *&l)
{
    nodoLista *nuevo = new nodoLista;
    nuevo->pos = i;
    nuevo->sig = l;
    l = nuevo;
}
int eliminarInicioYdevolver(nodoLista *&l)
{
    int dato = l->pos;
    nodoLista *aBorrar = l;
    l = l->sig;
    delete aBorrar;
    aBorrar = NULL;
    return dato;
}

int *initCostos(int x)
{
    int *ret = new int[x];
    for (int i = 1; i < x; i++)
    {
        ret[i] = INT_MAX;
    }
    return ret;
}

int *initVengoDe(int x)
{
    int *ret = new int[x];
    for (int i = 1; i < x; i++)
    {
        ret[i] = -1;
    }
    return ret;
}

int verticeDesconocidoDeMenorCosto(Grafo<ciudad *> *c, int org, bool *visitados, int *costo)
{
    int ret = -1;
    int peso = INT_MAX;

    for (int i = 1; i <= c->cantidadVertices(); i++)
    {
        if (!visitados[i] && costo[i] < peso)
        {
            peso = costo[i];
            ret = i;
        }
    }
    return ret; // Retornamos el vértice con menor costo no visitado
}

void dijkstra(Grafo<ciudad *> *c, int org, int *&costo, int *&vengoDe)
{
    bool *visitados = new bool[c->cantidadVertices() + 1]();
    costo[org] = 0;
    for (int i = 1; i < c->cantidadVertices() + 1; i++)
    {
        int v = verticeDesconocidoDeMenorCosto(c, org, visitados, costo);
        visitados[v] = true;
        Arista *ady = c->adyacentes(v);
        while (ady)
        {
            int w = ady->destino;
            if (!visitados[w] && costo[w] > costo[v] + ady->peso)
            {
                costo[w] = costo[v] + ady->peso;
                vengoDe[w] = v;
            }
            ady = ady->sig;
        }
    }

    delete[] visitados;
}

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



// rresolver misiones p
// indegree O(m + dep) de misiones
// encolo los degree 0
// while quedanMisiones
// hacemos dijkstra (C+E)*log C
// int indice ciudadDeMenorCosto
// int misionAsociada
// int CantCiudades
// while cantidaddeCiudadesProcesada < C (orden C)  --> (C+E)logC +C == c=e log c //lo procesamos la cantidad de ciudades maximo
//  de la cola desencolasde degree 0
// me fijo la ciudad de la mision
// si costo[ciudad de el desencolado] < costo[ciudadMenorCosto]
// enccolo misionAsocida
// misionAsociada
// ciudadMenorCosto  =  ciudad de el desencolado
// termino el while y supuestamente tengo la ciudad de menor costo guardada y la mision
// getVertice(ciudadMenorCosto) getVertice(misionAsociada)
// imprimo toda la info :)

// misiones --