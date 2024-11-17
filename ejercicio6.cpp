#include <string>
#include "funciones/funcionesE5yE6.cpp"

using namespace std;

int main()
{
    int C;
    cin >> C;
    Grafo<ciudad *> *c = new Grafo<ciudad *>(C, false, true); //  no dirigido y ponderado
    Grafo<ciudad *> *c2 = new Grafo<ciudad *>(C, false, true); // no dirigido y ponderado
    for (int i = 0; i < C; i++)
    {
        int idCiudad;
        string nombre_sin_espacios;
        cin >> idCiudad >> nombre_sin_espacios;
        ciudad *nueva = new ciudad;
        nueva->id = idCiudad;
        nueva->nombre = nombre_sin_espacios;
        c->setVertice(idCiudad, nueva);
        c2->setVertice(idCiudad, nueva);
    }
    int S, E, T, P, cant_aristas;
    cin >> S >> E >> T >> P >> cant_aristas;

    int id_ciudad_origen, id_ciudad_destino, costo;
    for (int i = 0; i < cant_aristas; i++)
    {
        cin >> id_ciudad_origen >> id_ciudad_destino >> costo;
        // if(!existeArista(id_ciudad_origen, id_ciudad_destino, c)){ //preguntar que onda con los in 
            c->agregarArista(id_ciudad_origen, id_ciudad_destino, costo);
            c2->agregarArista(id_ciudad_origen, id_ciudad_destino, costo);
        // } 
    }

    /* ------------------------ Terminamos de leer datos ------------------------ */
    /* -------------------------------- Solucion -------------------------------- */

    nodoLista *paso1E = NULL;
    nodoLista *paso2E = NULL;
    nodoLista *paso3E = NULL;
    nodoLista *paso1T = NULL;
    nodoLista *paso2T = NULL;
    nodoLista *paso3T = NULL;
    int costoE = calcularCosto(S, E, T, P, paso1E, paso2E, paso3E, c);
    int costoT = calcularCosto(S, T, E, P, paso1T, paso2T, paso3T, c2);

    if (costoE <= costoT)
    {
        cout << "BE11, la mejor ruta es Desactivar la Entidad, buscar equipo y punto de extraccion con un costo de " << costoE << endl;
        cout << "La otra opcion tiene un costo de " << costoT << endl;
        cout << "Paso 1: " << imprimirCamino(paso1E, c) << "Desactivar la Entidad" << endl;
        cout << "Paso 2: " << imprimirCamino(paso2E, c) << "Buscar equipo" << endl;
        cout << "Paso 3: " << imprimirCamino(paso3E, c) << "Ir a Punto de extraccion" << endl;
    }
    else
    {
        cout << "BE11, la mejor ruta es buscar equipo, Desactivar la Entidad y punto de extraccion con un costo de " << costoT << endl;
        cout << "La otra opcion tiene un costo de " << costoE << endl;
        cout << "Paso 1: " << imprimirCamino(paso1T, c2) << "Buscar equipo" << endl;
        cout << "Paso 2: " << imprimirCamino(paso2T, c2) << "Desactivar la Entidad" << endl;
        cout << "Paso 3: " << imprimirCamino(paso3T, c2) << "Ir a Punto de extraccion"  << endl;
    }

    eliminarLista(paso1E);
    eliminarLista(paso2E);
    eliminarLista(paso3E);
    eliminarLista(paso1T);
    eliminarLista(paso2T);
    eliminarLista(paso3T);

    delete paso1E;
    delete paso2E;
    delete paso3E;
    delete paso1T;
    delete paso2T;
    delete paso3T;
    return 0;
}


// pseudocodigo

// Dijstra para el origen
// funcion calcular costo yendo por entidad VengoDe o lista que digga los que visito
// funcion calcular costo yendo por grupo
//  el que tenga menor costo
//  imprimo su camino

// En la funcion calcularCosto yendo primero a destino (ya sea entidad o equipo):
//  veo el camino mas barato hasta el destino en el vector costos de dijkstra
//  costo = costos[destino];
//  recorro vengoDe actualizando costos de las aristas que visite para llegar al destino al doble de su valor, en el grafo. Y ademas
//  .. hay que guardar los visitados en una lista (capaz que inserte al final asi queda ordenadi)
//  haces dijstra de el destino hacia el otro lugar a visitar
//  costo+= costosDestino[destino2];
//  recorres todo vengoDe de el destino duplicando los costos de las aristas para llegar al segundo destino, en el grafo.
//  ademas insertas en la lista todas las ciudades del camino
//  costo+=
//  haces dijkstra del destino 2 a el punto de escape
//  guardas todos los