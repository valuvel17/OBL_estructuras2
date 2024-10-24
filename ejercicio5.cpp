#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include "funciones/funcionesE5.cpp"

int main()
{
    int cantMisiones = 0;
    cin >> cantMisiones;
    Grafo<mision *> *m = new Grafo<mision *>(cantMisiones, true, false);

    for (int i = 0; i < cantMisiones; i++)
    {
        int idMision, idCiudad;
        string nombre_mision;
        cin >> idMision >> nombre_mision >> idCiudad;
        mision *info = new mision;
        info->id = idMision;
        info->idCiudad = idCiudad;
        info->nombre = nombre_mision;
        m->setVertice(idMision, info);

        int j;
        cin >> j;
        while (j != 0)
        {
            m->agregarArista(idMision, j); // Agrega arista de la misión actual a la misión posterior
            cin >> j;
        }
    }

    int C, O;
    cin >> C >> O;
    Grafo<ciudad *> *c = new Grafo<ciudad *>(C, false, true); //no dirigido y ponderado

    for (int i = 0; i < C; i++)
    {
        int idCiudad;
        string nombre_ciudad;
        cin >> idCiudad >> nombre_ciudad;
        ciudad *info = new ciudad;
        info->id = idCiudad;
        info->nombre = nombre_ciudad;
        c->setVertice(idCiudad, info);
    }
    int e;
    cin >> e;

    for (int j = 0; j < e; j++)
    {
        int idOrigen, idDestino, costo;
        cin >> idOrigen >> idDestino >> costo;
        c->agregarArista(idOrigen, idDestino, costo); // Agregamos aristas de ciudades
    }

/* ------------------------- Fin de lectura de datos ------------------------ */
    // Resolucion problema: 
    ciudad *origen = c->getVertice(O);
    cout << "Ciudad inicial: " << origen->nombre << endl;
    
    // guardo cantidades
    int cantidadMisiones = m->cantidadVertices();
    int cantidadCiudades = c->cantidadVertices();

    // calculo grado de entrada de las misiones
    int *inDegreeM = DegreeM(m);

    Cola *Cmisiones = new Cola();
    
    // encolo las misiones sin dependencias
    for (int i = 1; i < cantidadMisiones + 1; i++)
    {
        if (inDegreeM[i] == 0)
        {
            Cmisiones->encolar(i); // encolo el id de la mision
        }
    }
    
    int contMisiones = 0;
    int estoyEn = O;
    int costoTotalViaje = 0;

    // mientras no haya procesado todas las misiones
    while (contMisiones != cantidadMisiones) // O(M)
    {
        int *costo = initCostos(c->cantidadVertices() + 1);     // Inicializamos costos en maxInt
        int *vengoDe = initVengoDe(c->cantidadVertices() + 1);  // Incializamos vengoDe en -1 (vengoDe luego nos ayuda a reconstruir el camino)
        costo[estoyEn] = 0;                                     // El costo del origen a si mismo es de 0
        dijkstra(c, estoyEn, costo, vengoDe);                   // O((C+E) * log C)Hacemos dijkstra para obtener el camino en vengoDe a cada vertice con el menor costo y ademas guardar el costo

        int indice_mision1 = Cmisiones->desencolar();           // Agarramos una primera mision como referencia para hacer
        mision *primera = m->getVertice(indice_mision1);        // Obtenemos sus datos
        int ciudadMenorCosto = primera->idCiudad;               // La ciudad de menor costo asumimos que es la asociada a la primera mision (tambien como referencia)

        int misionAsociada = primera->id;                       // Guardamos el id asociado a la mision
        int ultimoCosto = costo[ciudadMenorCosto];              // Guardamos el costo a la ciudad de menor costo asociada a la mision

        int ciudadesVisitadas = 1;                              // Ya visitamos una ciudad

        // Busco en las misiones sin dependencias la que tenga la ciudad con menor costo de viaje
        while (ciudadesVisitadas != cantidadCiudades && !Cmisiones->esVacia()) // O(C)
        {
            // desencolo una mision 
            int indice_mision = Cmisiones->desencolar();
            mision *mision_candidata = m->getVertice(indice_mision);
            int indice_ciudad = mision_candidata->idCiudad;
            
            // Si la ciudad de la mision desencolada tiene menor costo que la anterior de menor costo actualizo los datos
            if (costo[indice_ciudad] < ultimoCosto)
            {
                Cmisiones->encolar(misionAsociada);
                ultimoCosto = costo[indice_ciudad];
                ciudadMenorCosto = indice_ciudad;
                misionAsociada = indice_mision;
            }
            else
            {
                Cmisiones->encolar(indice_mision);
            }
            ciudadesVisitadas++;
        }
        
        primera = m->getVertice(misionAsociada); // esta es la mision que hay que hacer
        Arista *misionesConPrevia = m->adyacentes(misionAsociada);

        // bajo el grado de inDegree de las misiones adyacentes a la que voy a hacer
        while (misionesConPrevia)
        {
            int destino = misionesConPrevia->destino;
            inDegreeM[destino]--;
            
            // si tienen grado 0 las encolo
            if (inDegreeM[destino] == 0)
                Cmisiones->encolar(destino);
            misionesConPrevia = misionesConPrevia->sig;
        }
        
        // tengo que recorrer todo el vengoDe para hacer el camino desde el origen hasta la ciudad de origen
        nodoLista *listaImprimir = NULL;
        int voyA = ciudadMenorCosto;
        
        // inserto en una lista la ciudades que recorro para realizar la mision esto es para imprimirlo en el orden correcto
        while (voyA != estoyEn)
        {
            insertarInicio(voyA, listaImprimir);
            voyA = vengoDe[voyA];
        }
        insertarInicio(estoyEn, listaImprimir); // inserto el origen
        
        // imprimo las ciudades
        while (listaImprimir)
        {
            int pos = eliminarInicioYdevolver(listaImprimir);
            ciudad *ciu = c->getVertice(pos);
            string nombre_ciudad = ciu->nombre;
            cout << nombre_ciudad << " -> ";
        }

        // imprimo la mision
        string nombre_mision = primera->nombre;
        ciudad *ciudadMC = c->getVertice(ciudadMenorCosto);
        cout << "Mision: " << nombre_mision << " - " << ciudadMC->nombre << " - Tiempo de viaje: " << ultimoCosto << endl;
        estoyEn = ciudadMenorCosto;
        costoTotalViaje += ultimoCosto;
        delete[] costo;
        delete[] vengoDe;
        contMisiones++;
    }
    
    cout << "Misiones ejecutadas con exito." << endl;
    cout << "Tiempo total de viaje: " << costoTotalViaje << endl;
    return 0;
    
}
