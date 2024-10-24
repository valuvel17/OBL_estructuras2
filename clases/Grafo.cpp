#include <iostream>
#include <string>
#include "../funciones/definiciones.h"

using namespace std;

template <class V>

class Grafo
{
private:
    Arista **vertices;
    int cantidadV;
    bool dirigido;
    bool ponderado;
    V *infoVertices;

public:
    // Constructor
    Grafo(int cantV, bool esDirigido, bool esPonderado)
    {
        vertices = new Arista *[cantV + 1]();
        cantidadV = cantV;
        dirigido = esDirigido;
        ponderado = esPonderado;
        infoVertices = new V[cantV + 1];
    }

    // Destructor
    ~Grafo()
    {
        for (int i = 1; i <= cantidadV; ++i)
        {
            Arista *actual = vertices[i];
            while (actual != nullptr)
            {
                Arista *temp = actual;
                actual = actual->sig;
                delete temp;
            }
        }
        delete[] vertices;
        delete[] infoVertices;
    }

    // PRE: recibe un indice
    // POS: devuelve el nodo del grafo asociado a ese indice
    V getVertice(int i)
    {
        return infoVertices[i];
    }

    // PRE: recibe un indice y una info
    // POS: setea el nodo en el indice recibido y le asigna la informacion 
    void setVertice(int i, V info)
    {
        infoVertices[i] = info;
    }

    // PRE: recibe un origen un destino y un peso (opcional)
    // POS: agrega la arista con esa informacion a la lista de aristas del origen
    void agregarArista(int origen, int destino, int peso = 1)
    {
        Arista *nuevaArista = new Arista();
        nuevaArista->destino = destino;
        nuevaArista->peso = peso;
        nuevaArista->sig = vertices[origen];
        vertices[origen] = nuevaArista;
        if (!dirigido)
        {
            Arista *inversa = new Arista();
            inversa->destino = origen;
            inversa->peso = peso;
            inversa->sig = vertices[destino];
            vertices[destino] = inversa;
        }
    }

    // PRE: 
    // POS: impirme el grafo
    void imprimir()
    {
        cout << endl
             << "Lista de adyacencia:" << endl;
        for (int i = 1; i <= cantidadV; i++)
        {
            cout << i << ": ";
            Arista *actual = adyacentes(i);
            while (actual->sig != NULL)
            {
                cout << actual->destino << " ";
                if (ponderado)
                {
                    cout << "(" << actual->peso << ") ";
                }
                if (actual->sig->sig)
                {
                    cout << "-> ";
                }
                actual = actual->sig;
            }
            cout << endl;
        }
    }

    // PRE: recibe un vertice
    // POS: devuelve una lista con los adyacentes de ese veritce
    Arista *adyacentes(int vertice)
    {
        return vertices[vertice];
    }

    // PRE: 
    // POS: retorna la cantidad de vertices del grafo
    int cantidadVertices()
    {
        return cantidadV;
    }
};