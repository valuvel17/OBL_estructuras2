#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include "clases/Grafo.cpp"
#include "funciones/funcionesE5.cpp"
using namespace std;

int main()
{
    int cantMisiones = 0;
    cin >> cantMisiones;
    Grafo<mision> *misiones = new Grafo<mision>(cantMisiones, true, false);

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
        int j = 0;
        cin >> j;
        while (j = !0)
        {

            misiones->agregarArista(i, j);
            cin >> j;
        }
    }

    int C, O;
    cin >> C >> O;
    Grafo<ciudad> *ciudades = new Grafo<ciudad>(C, false, true);
    int idCiudad, idOrigen, IdDestino, costo;
    string nombre_ciudad;
    int e;
    for (int i = 0; i < C; i++)
    {

        cin >> idCiudad >> nombre_ciudad >> e;
        ciudad info;
        info.id = idCiudad;
        info.nombre = nombre_ciudad;
        ciudades->setVertice(idCiudad, info);
        for (int i = 0; i < e; i++)
        {
            cin >> idOrigen >> IdDestino >> costo;
            ciudades->agregarArista(idOrigen, IdDestino, costo);
        }
    }

    resolverMisiones(misiones, ciudades, O);
}