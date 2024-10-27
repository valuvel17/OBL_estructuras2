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

    /* ------------------------ Terminamos de leer datos ------------------------ */
    /* -------------------------------- Solucion -------------------------------- */

    nodoLista *paso1E = NULL;
    nodoLista *paso2E = NULL;
    nodoLista *paso3E = NULL;
    nodoLista *paso1T = NULL;
    nodoLista *paso2T = NULL;
    nodoLista *paso3T = NULL;
    nodoLista *recorridoT = NULL;
    int costoE = calcularCosto(S, E, T, P, paso1E, paso2E, paso3E, c);
    int costoT = calcularCosto(S, T, E, P, paso1T, paso2T, paso3T, c);

    if (costoE <= costoT)
    {
        cout << "BE11, la mejor ruta es Desactivar la Entidad, buscar equipo y punto de extraccion con un costo de " << costoE << endl;
        cout << "La otra opcion tiene un costo de " << costoT << endl;
        imprimirCamino(paso1E);
        imprimirCamino(paso2E);
        imprimirCamino(paso3E);
    }
    else
    {
        cout << "BE11, la mejor ruta es Desactivar la Entidad, buscar equipo y punto de extraccion con un costo de " << costoT << endl;
        cout << "La otra opcion tiene un costo de " << costoE << endl;
        imprimirCamino(paso1T);
        imprimirCamino(paso2T);
        imprimirCamino(paso3T);
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

void eliminarLista(nodoLista *&l)
{
    while (l)
    {
        eliminarInicioYdevolver(l);
    }
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
}

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