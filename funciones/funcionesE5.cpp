#include <cassert>
#include <string>
#include <iostream>
#include <climits>
#include <limits>
#include "../clases/Grafo.cpp"
#include "../clases/ColaPrioridad.cpp"
using namespace std;

struct Arista
{
    int destino;
    int peso;
    Arista *sig;
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

int verticeDesconocidoDeMenorCosto(Grafo<ciudad> *c, int org)
{
    Arista *ady = c->adyacentes(org);
    int ret = 0;
    int peso = 9000;
    while (ady)
    {
        if (ady->peso < peso)
        {
            peso = ady->peso;
            ret = ady->destino;
        }
        ady = ady->sig;
    }
    return ret;
}

void dijkstra(Grafo<ciudad> *c, int org, int *&costoRet, int *&vengoDeRet)
{
    int *costo = initCostos(c->cantidadVertices() + 1);
    int *vengoDe = initVengoDe(c->cantidadVertices() + 1);
    bool *visitados = new bool[c->cantidadVertices() + 1]();
    costo[org] = 0;
    for (int i = 1; i < c->cantidadVertices() + 1; i++)
    {
        int v = verticeDesconocidoDeMenorCosto(c, org);
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
    costoRet = costo;
    vengoDeRet = vengoDe;
    delete[] costo;
    delete[] vengoDe;
    delete[] visitados;
}

int *DegreeM(Grafo<mision> *m)
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

void resolverMisiones(Grafo<mision> *m, Grafo<ciudad> *c, int org)
{
    int cantidadMisiones = m->cantidadVertices();
    int cantidadCiudades = c->cantidadVertices();
    int *inDegreeM = DegreeM(m);

    ColaPrioridad *cp = NULL;

    for (int i = 0; i < cantidadMisiones + 1; i++)
    {
        if (inDegreeM[i] == 0)
            cp->encolar(i); // encolo el id de la mision
    }
    int contMisiones = 0;
    int estoyEn = org;
    while (contMisiones != cantidadMisiones)
    {
        int *costo = initCostos(c->cantidadVertices() + 1);
        int *vengoDe = initVengoDe(c->cantidadVertices() + 1);
        costo[estoyEn] = 0;
        dijkstra(c, estoyEn, costo, vengoDe);
        int ciudadMenorCosto = -1;
        int misionAsociada = -1;
        int ciudadesVisitadas = 0;
        while (ciudadesVisitadas != cantidadCiudades){


        }
            estoyEn;

        delete[] costo;
        delete[] vengoDe;
        contMisiones++;
    }
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
// desencolas de la cola de degree 0
// me fijo la ciudad de la mision
// si costo[ciudad de el desencolado] < costo[ciudadMenorCosto]
// enccolo misionAsocida
// misionAsociada
// ciudadMenorCosto  =  ciudad de el desencolado
// termino el while y supuestamente tengo la ciudad de menor costo guardada y la mision
// getVertice(ciudadMenorCosto) getVertice(misionAsociada)
// imprimo toda la info :)

// misiones --