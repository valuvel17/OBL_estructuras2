#include <iostream>
#include <string>
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
    Grafo(int cantV, bool esDirigido, bool esPonderado)
    {
        vertices = new Arista *[cantV + 1]();
        cantidadV = cantV;
        dirigido = esDirigido;
        ponderado = esPonderado;
        infoVertices = new V[cantV + 1]();
    }

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

    V getVertice(int i)
    {
        return infoVertices[i];
    }

    void setVertice(int i, V info)
    {
        infoVertices[i] = info;
    }

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

    Arista *adyacentes(int vertice)
    {
        return vertices[vertice];
    }

    int cantidadVertices()
    {
        return cantidadV;
    }

};