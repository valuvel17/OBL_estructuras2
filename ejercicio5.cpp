#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include "funciones/funcionesE5.cpp"
using namespace std;

int main()
{
    // Lectura de la cantidad de misiones
    int cantMisiones = 0;
    cin >> cantMisiones;
    Grafo<mision> *misiones = new Grafo<mision>(cantMisiones, true, false);

    // Procesamiento de las misiones
    for (int i = 0; i < cantMisiones; i++)
    {
        int idMision, idCiudad;
        string nombre_mision;
        cin >> idMision >> nombre_mision >> idCiudad;
        mision info;
        info.id = idMision;
        info.idCiudad = idCiudad;
        info.nombre = nombre_mision;
        misiones->setVertice(idMision, info);

        // Lectura de la lista de misiones posteriores
        int j;
        cin >> j;
        while (j != 0) // Cambiado a j != 0
        {
            misiones->agregarArista(idMision, j); // Agrega arista de la misión actual a la misión posterior
            cin >> j;
        }
    }

    // Lectura de la cantidad de ciudades y la ciudad inicial
    int C, O;
    cin >> C >> O;
    Grafo<ciudad> *ciudades = new Grafo<ciudad>(C, false, true);

    // Procesamiento de las ciudades
    for (int i = 0; i < C; i++)
    {
        int idCiudad;
        string nombre_ciudad;
        cin >> idCiudad >> nombre_ciudad;
        ciudad info;
        info.id = idCiudad;
        info.nombre = nombre_ciudad;
        ciudades->setVertice(idCiudad, info);

    }
    int e;
    cin >> e;
    // Procesar las conexiones seguras entre ciudades
    for (int j = 0; j < e; j++) // Usamos j como índice interno
    {
        int idOrigen, idDestino, costo;
        cin >> idOrigen >> idDestino >> costo;
        ciudades->agregarArista(idOrigen, idDestino, costo);
    }

    // Imprimir la ciudad inicial
    cout << "Ciudad inicial: " << O << endl;

    // Resolver las misiones a partir de la ciudad inicial
    resolverMisiones(misiones, ciudades, O);

    return 0;
}
