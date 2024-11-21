#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

struct nodoLista
{
    int indice;
    nodoLista *sig;
};

typedef nodoLista *Lista;
struct jugador
{
    int valoracion;
    int salario;
    bool extranjero;
};

int calcularValoracion(int ritmo, int tiro, int pase, int regate, int defensa, int fisico, string formaFisica, string confianza)
{
    int val = ritmo + tiro + pase + regate + defensa + fisico;
    val = val / 6;
    if (confianza == "Baja")
    {
        val -= 5;
    }
    else if (confianza == "Alta")
    {
        val += 5;
    }

    if (formaFisica == "Lesionado")
    {
        val = 0;
    }
    else if (formaFisica == "Mala")
    {
        val -= 5;
    }
    else if (formaFisica == "Buena")
    {
        val += 5;
    }
    if(val > 99) return 99;
    if(val < 0) return 0;
    return val;
}

bool elObjetoEntra(int costo, int volumen, int capacidadPresupuestoActual, int capacidadExtranjerosActual)
{
    return costo <= capacidadPresupuestoActual && volumen <= capacidadExtranjerosActual;
}

// 
Lista reconstruirJugadoresEnLista(int &cantidadMejoresJugadores, int cantJugadores, int Presupuesto, int CantExtranjeros, int ***mochila1, jugador **players)
{
    Lista ret = NULL;
    int PresupuestoRestante = Presupuesto;
    int ExtranjerosRestantes = CantExtranjeros;

    cout << "Reconstruyendo lista de jugadores..." << endl;
    for (int i = cantJugadores; i > 0; i--)
    {
        cout << "Evaluando jugador " << i << " con PresupuestoRestante = " << PresupuestoRestante 
             << " y ExtranjerosRestantes = " << ExtranjerosRestantes << endl;

        // Verificar si el jugador fue seleccionado
        if (mochila1[i][PresupuestoRestante][ExtranjerosRestantes] != mochila1[i - 1][PresupuestoRestante][ExtranjerosRestantes])
        {
            cout << "Jugador " << i << " seleccionado. Salario: " << players[i]->salario 
                 << ", Extranjero: " << (players[i]->extranjero ? "Sí" : "No") 
                 << ", Valoración: " << players[i]->valoracion << endl;

            // Agregar jugador a la lista
            Lista nuevo = new nodoLista;
            nuevo->indice = i;
            nuevo->sig = ret;
            ret = nuevo;

            // Actualizar presupuesto y extranjeros restantes
            cantidadMejoresJugadores++;
            PresupuestoRestante -= players[i]->salario;

            int extranjero = players[i]->extranjero ? 1 : 0;
            ExtranjerosRestantes -= extranjero;
        }

    }
    return ret;
}


jugador **reconstruirIndices(Lista nodos, int cantidadMejoresJugadores, jugador **players)
{
    jugador **ret = new jugador *[cantidadMejoresJugadores + 1];
    for (int i = 1; i <= cantidadMejoresJugadores; i++)
    {
        ret[i] = players[nodos->indice];
        Lista aux = nodos;
        nodos = nodos->sig;
        delete aux;
    }
    return ret;
}

void liberarMochila1(int ***mochila1, int cantJugadores, int Presupuesto)
{
    for (int i = 0; i <= cantJugadores; i++)
    {
        for (int j = 0; j <= Presupuesto; j++)
        {
            delete[] mochila1[i][j];
        }
        delete[] mochila1[i];
    }
    delete[] mochila1;
}

void liberarMochila2(int **mochila2, int cantidadMejoresJugadores)
{
    for (int i = 0; i <= cantidadMejoresJugadores; i++)
    {
        delete[] mochila2[i];
    }
    delete[] mochila2;
}

void imprimirMochila2(int **mochila2, int filas, int columnas) {
    cout << "Contenido de Mochila 2:" << endl;

    for (int i = 0; i <= filas; i++) { // Iterar por las filas
        for (int j = 0; j <= columnas; j++) { // Iterar por las columnas
            cout << mochila2[i][j] << "\t"; // Imprimir cada valor con tabulación para alinear
        }
        cout << endl; // Salto de línea después de cada fila
    }
    cout << endl; // Espacio adicional al final para separar visualmente
}


int main()
{
    int cantJugadores;
    cin >> cantJugadores;

    // Crear array de jugadores
    jugador **players = new jugador *[cantJugadores + 1];
    for (int i = 1; i <= cantJugadores; i++)
    {
        int ritmo, tiro, pase, regate, defensa, fisico, salario;
        string formaFisica, confianza, e_string;
        bool extranjero = false;

        // Entrada de datos para cada jugador
        cin >> ritmo >> tiro >> pase >> regate >> defensa >> fisico >> formaFisica >> salario >> e_string >> confianza;
        if (e_string == "Si")
            extranjero = true;

        // Calcular la valoración del jugador
        int valoracion = calcularValoracion(ritmo, tiro, pase, regate, defensa, fisico, formaFisica, confianza);

        // Crear un objeto jugador
        jugador *player = new jugador{valoracion, salario, extranjero};
        players[i] = player;
    }

    int Presupuesto, CantExtranjeros;
    cin >> Presupuesto >> CantExtranjeros;

    // Crear mochila1: un array tridimensional para almacenar los valores
    int ***mochila1 = new int **[cantJugadores + 1];
    for (int i = 0; i <= cantJugadores; i++)
    {
        mochila1[i] = new int *[Presupuesto + 1];
        for (int j = 0; j <= Presupuesto; j++)
        {
            mochila1[i][j] = new int[CantExtranjeros + 1](); // Inicializar a 0
        }
    }

    // Llenado de mochila1 (parte del algoritmo de mochila)
    for (int jugadorActual = 1; jugadorActual <= cantJugadores; jugadorActual++)
    {
        int peso = 1;                                             // El peso de cada jugador (unidades de presupuesto)
        int costo = players[jugadorActual]->salario;              // Costo del jugador
        int volumen = players[jugadorActual]->extranjero ? 1 : 0; // Volumen (1 si es extranjero)
        // Debug: Verifica si el jugador se puede agregar al presupuesto y extranjeros disponibles
        cout << "Jugador " << jugadorActual << ": Costo = " << costo << ", Extranjeros = " << volumen << endl;

        for (int capacidadPresupuestoActual = 0; capacidadPresupuestoActual <= Presupuesto; capacidadPresupuestoActual++)
        {
            for (int capacidadExtranjerosActual = 0; capacidadExtranjerosActual <= CantExtranjeros; capacidadExtranjerosActual++)
            {
                //for 1 a 11
                // Verificar si el jugador cabe dentro de los límites de presupuesto y extranjeros
                if (!elObjetoEntra(costo, volumen, capacidadPresupuestoActual, capacidadExtranjerosActual))
                {
                    mochila1[jugadorActual][capacidadPresupuestoActual][capacidadExtranjerosActual] = mochila1[jugadorActual - 1][capacidadPresupuestoActual][capacidadExtranjerosActual];
                }
                else
                {
                    int valorUsarJugador = players[jugadorActual]->valoracion + mochila1[jugadorActual - 1][capacidadPresupuestoActual - costo][capacidadExtranjerosActual - volumen];//[4tadim]
                    int valorNoUsarJugador = mochila1[jugadorActual - 1][capacidadPresupuestoActual][capacidadExtranjerosActual];
                    mochila1[jugadorActual][capacidadPresupuestoActual][capacidadExtranjerosActual] = max(valorUsarJugador, valorNoUsarJugador);
                }
                //endFor
            }
        }
    }

//retorar /11
    // Debug: Verificar los valores de mochila1 después de la llenada
    cout << "Mochila1 llena correctamente." << endl;
    cout << mochila1[cantJugadores][Presupuesto][CantExtranjeros] << endl;

    /* int cantidadMejoresJugadores = 0;
    Lista mejoresJugadoresDeTodos_enLista = reconstruirJugadoresEnLista(cantidadMejoresJugadores, cantJugadores, Presupuesto, CantExtranjeros, mochila1, players);
    /////////////////////////////////////////////////
    cout << "Reconstruyendo lista de mejores jugadores..." << endl;
    Lista temp = mejoresJugadoresDeTodos_enLista;
    cout << "Jugadores seleccionados en mochila1:" << endl;
    while (temp != NULL)
    {
        cout << "Jugador índice: " << temp->indice << ", Salario: " << players[temp->indice]->salario
             << ", Valoración: " << players[temp->indice]->valoracion << endl;
        temp = temp->sig;
    }
    ////////////////////////////////////////
    jugador **mejoresJugadoresDeTodos = reconstruirIndices(mejoresJugadoresDeTodos_enLista, cantidadMejoresJugadores, players);

    // Crear mochila2 para optimizar el plantel
    int **mochila2 = new int *[cantidadMejoresJugadores + 1];
    for (int i = 0; i <= cantidadMejoresJugadores; i++)
    {
        mochila2[i] = new int[12](); // Inicializar a 0 (plantel de hasta 11 jugadores)
    }

    // Llenado de mochila2 (optimización del plantel de 11 jugadores)
    for (int jugadorActual = 1; jugadorActual <= cantidadMejoresJugadores; jugadorActual++)
    {
        for (int capacidadPlantel = 0; capacidadPlantel <= 11; capacidadPlantel++)
        {
            int peso = 1; // Los jugadores pesan 1
            int valor = mejoresJugadoresDeTodos[jugadorActual]->valoracion;
            
            // Verificar si el jugador cabe en el plantel
            if (peso <= capacidadPlantel)
            {
                int valorNoUsarJugador = mochila2[jugadorActual - 1][capacidadPlantel];
                int valorUsarJugador = valor + mochila2[jugadorActual - 1][capacidadPlantel - peso];
                mochila2[jugadorActual][capacidadPlantel] = max(valorUsarJugador, valorNoUsarJugador);
            }
            else
            {
                mochila2[jugadorActual][capacidadPlantel] = mochila2[jugadorActual - 1][capacidadPlantel];
            }
        }
    }

    imprimirMochila2(mochila2, cantidadMejoresJugadores, 11);

    // Debug: Imprimir resultado final de mochila2
    cout << "Resultado final en mochila2[capacidad 11]: " << mochila2[cantidadMejoresJugadores][11] << endl;

    // Resultado final: Promedio del valor de los jugadores seleccionados
    cout << mochila2[cantidadMejoresJugadores][11] / 11 << endl;
 */
    // Liberar memoria de las estructuras usadas
    liberarMochila1(mochila1, cantJugadores, Presupuesto);
    //liberarMochila2(mochila2, cantidadMejoresJugadores);

    // Liberar memoria de los jugadores
    for (int i = 1; i <= cantJugadores; i++)
    {
        delete players[i];
    }
    delete[] players;

    return 0;
}
