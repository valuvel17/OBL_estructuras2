#include "funciones/funcionesE5.cpp"

using namespace std;

int main()
{
    int C;
    cin >> C;
    Grafo<ciudad *> *c = new Grafo<ciudad *>(C, false, true); // no dirigido ponderado
    for (int i = 0; i < C; i++)
    {
        int idCiudad;
        string nombre_sin_espacios;
        cin >> idCiudad >> nombre_sin_espacios;
        ciudad *nueva = new ciudad;
        nueva->id = idCiudad;
        nueva->nombre = nombre_sin_espacios;
        c->setVertice(idCiudad, nueva);
    }
    int S, E, T, P, cant_aristas;
    cin >> S >> E >> T >> P >> cant_aristas;

    int id_ciudad_origen, id_ciudad_destino, costo;
    for (int i = 0; i < cant_aristas; i++)
    {
        cin >> id_ciudad_origen >> id_ciudad_destino >> costo;
        c->agregarArista(id_ciudad_origen, id_ciudad_destino, costo);
    }
    /* ------------------------- Fin de lectura de datos ------------------------ */
    // hola salva estoy probando una extension para hacer comentarios esta pillaza

    return 0;
}