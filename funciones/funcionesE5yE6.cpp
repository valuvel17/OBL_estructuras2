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


// Funciones para ejercicio 6
string imprimirCamino(nodoLista* l, Grafo<ciudad *> *c){
    string ret = "";
    nodoLista* aux = l;
    while(aux){
        ret += c->getVertice(aux->pos)->nombre;
        ret += " -> ";
        aux = aux->sig;
    }
    return ret;
}

void eliminarLista(nodoLista *&l)
{
    while (l)
    {
        eliminarInicioYdevolver(l);
    }
}
//Verificacion para grafos no dirigidos
bool existeArista(int origen, int destino, Grafo<ciudad*>* g){
    bool ret = false;
    Arista* adyO = g->adyacentes(origen);
    Arista* adyD = g->adyacentes(destino);
    while(adyO){
        if(adyO->destino == destino) ret = true;
        adyO = adyO ->sig;
    }
    while(adyD){
        if(adyD->destino == origen) ret = true;
        adyD = adyD ->sig;
    }

    return ret;
}

int calcularCosto(int org, int dest, int dest2, int final, nodoLista *&p1, nodoLista *&p2, nodoLista *&p3, Grafo<ciudad *> *c)
{
    int ret, estoyEn, voyA, vineDe;
    ret = 0;

    /* --------------------------------- Paso 1 --------------------------------- */
    estoyEn = org;
    voyA = dest;

    int *costos = initCostos(c->cantidadVertices() + 1);
    int *vengoDe = initVengoDe(c->cantidadVertices() + 1);
    costos[estoyEn] = 0;
    vengoDe[estoyEn] = estoyEn;
    dijkstra(c, estoyEn, costos, vengoDe);

    ret = costos[voyA];

    while (voyA != estoyEn)
    {
        insertarInicio(voyA, p1);
        vineDe = vengoDe[voyA];
        c->duplicarValorArista(vineDe, voyA);
        voyA = vineDe;
    }
    insertarInicio(estoyEn, p1);

    delete[] vengoDe;
    delete[] costos;
    
    /* --------------------------------- Paso 2 --------------------------------- */
    estoyEn = dest;
    voyA = dest2;

    int *vengoDe2 = initVengoDe(c->cantidadVertices() + 1);
    int *costos2 = initCostos(c->cantidadVertices() + 1);
    costos2[estoyEn] = 0;
    vengoDe2[estoyEn] = estoyEn;

    dijkstra(c, estoyEn, costos2, vengoDe2);

    ret += costos2[voyA];

    while (voyA != estoyEn)
    {
        insertarInicio(voyA, p2);
        vineDe = vengoDe2[voyA];
        c->duplicarValorArista(vineDe, voyA);
        voyA = vineDe;
    }
    insertarInicio(estoyEn, p2);

    delete[] vengoDe2;
    delete[] costos2;

    /* --------------------------------- Paso 3 --------------------------------- */
    estoyEn = dest2;
    voyA = final;

    int *vengoDe3 = initVengoDe(c->cantidadVertices() + 1);
    int *costos3 = initCostos(c->cantidadVertices() + 1);
    costos3[estoyEn] = 0;
    vengoDe3[estoyEn] = estoyEn;

    dijkstra(c, estoyEn, costos3, vengoDe3);

    ret += costos3[voyA];
    while (voyA != estoyEn)
    {
        insertarInicio(voyA, p3);
        vineDe = vengoDe3[voyA];
        c->duplicarValorArista(vineDe, voyA);
        voyA = vineDe;
    }
    insertarInicio(estoyEn, p3);

    delete[] vengoDe3;
    delete[] costos3;

    return ret;
}
