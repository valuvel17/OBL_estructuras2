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

int verticeDesconocidoDeMenorCosto(Grafo<ciudad> *c, int org, bool* visitados, int* costo)
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

void dijkstra(Grafo<ciudad> *c, int org, int *&costo, int *&vengoDe)
{
    bool *visitados = new bool[c->cantidadVertices() + 1]();
    costo[org] = 0;
    for (int i = 1; i < c->cantidadVertices() + 1; i++)
    {
        int v = verticeDesconocidoDeMenorCosto(c, org, visitados, costo);
        visitados[v] = true;
        Arista* ady = c->adyacentes(v);
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
    cout << "llega a resolver misiones" << endl;
    int cantidadMisiones = m->cantidadVertices();
    cout << "cantidadMisiones: "<< cantidadMisiones << endl;
    int cantidadCiudades = c->cantidadVertices();
        cout << "cantidadCiudades: "<< cantidadCiudades << endl;
    int *inDegreeM = DegreeM(m);
        cout << "se hizo degree "<< endl;
    Cola *C = new Cola();

    for (int i = 0; i < cantidadMisiones + 1; i++)
    {
        if (inDegreeM[i] == 0){
            cout << i << "es indegree 0" << endl;
            C->encolar(i); // encolo el id de la mision
        }
    }
    int contMisiones = 0;
    int estoyEn = org;
    while (contMisiones != cantidadMisiones)
    {
        cout << "while misiones nro :" << contMisiones << endl;
        int *costo = initCostos(c->cantidadVertices() + 1);
        int *vengoDe = initVengoDe(c->cantidadVertices() + 1);
        costo[estoyEn] = 0;
        dijkstra(c, estoyEn, costo, vengoDe);
        cout << "sehizo dijkstra" <<endl;

        int indice_mision1 = C->desencolar(); // primera mision candidata a hacer
        cout << "se desencolo" << endl;
        cout << indice_mision1 << endl;
        mision primera = m->getVertice(indice_mision1);
        cout << "se obtuvo Mision" <<endl;
        int ciudadMenorCosto = primera.idCiudad;
        int misionAsociada = primera.id;
        cout << ciudadMenorCosto << misionAsociada;
        int ultimoCosto = costo[ciudadMenorCosto];
        int ciudadesVisitadas = 1;
        cout << "ciudadesVisitadas != cantidadCiudades?" << ciudadesVisitadas << cantidadCiudades << endl;
        while (ciudadesVisitadas != cantidadCiudades && !C->esVacia()) // O c
        {
            cout << "while ciudades nro :" << ciudadesVisitadas << endl;
            int indice_mision = C->desencolar();
            mision mision_candidata = m->getVertice(indice_mision);
            int indice_ciudad = mision_candidata.idCiudad;
            if (costo[indice_ciudad] < ultimoCosto)
            {
                C->encolar(misionAsociada);
                ultimoCosto = costo[indice_ciudad];
                ciudadMenorCosto = indice_ciudad;
                misionAsociada = indice_mision;
            }
            else
            {
                C->encolar(indice_mision);
            }
            ciudadesVisitadas++;
        }

        primera = m->getVertice(misionAsociada); // esta es la mision que hay que hacer
        Arista *misionesConPrevia = m->adyacentes(misionAsociada);
        while (misionesConPrevia)
        {
            cout << "while misionesConPrevia" << endl;
            int destino = misionesConPrevia->destino;
            inDegreeM[destino]--;
            if (inDegreeM[destino] == 0)
                C->encolar(destino);
            misionesConPrevia = misionesConPrevia->sig;
        }
        // tengo que recorrer todo el vengoDe para hacer el camino desde el origen hasta la ciudad;
        nodoLista *listaImprimir = NULL;
        int voyA = ciudadMenorCosto;
        if (ciudadMenorCosto == estoyEn) insertarInicio(estoyEn, listaImprimir);
       
        while (voyA != estoyEn)
        {
            cout << "while voyA nro :" << voyA << endl;
            insertarInicio(voyA, listaImprimir);
            voyA = vengoDe[voyA];
        }

        while(listaImprimir){
            int pos = eliminarInicioYdevolver(listaImprimir);
            cout << "while listaImprimir nro :" << pos << endl;
            ciudad ciu = c->getVertice(pos);
            string nombre_ciudad = ciu.nombre;
            cout << nombre_ciudad << " -> ";
        }
        string nombre_mision = primera.nombre;
        cout << "Mision: " << nombre_mision << " - " << ciudadMenorCosto << " - Tiempo de viaje: " << ultimoCosto << endl;
        estoyEn = ciudadMenorCosto;
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