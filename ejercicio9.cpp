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


//PRE: recibe los atributos de un jugador
//POS: calcula la valoracion correspondiente
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
    if (val > 99)
        return 99;
    if (val < 0)
        return 0;
    return val;
}

//PRE: -
//POS: Devvuelve si es posible agregar el objeto a la mochila
bool elObjetoEntra(int costo, int volumen, int capacidadPresupuestoActual, int capacidadExtranjerosActual)
{
    return costo <= capacidadPresupuestoActual && volumen <= capacidadExtranjerosActual;
}

//PRE: recibe la mochila, la cantidad 
//POS: calcula la valoracion correspondiente
void liberarMochila1(int ****mochila1, int cantJugadores, int Presupuesto)
{
    for (int i = 0; i <= cantJugadores; i++)
    {
        for (int j = 0; j <= Presupuesto; j++)
        {
            for (int k = 0; k <=11; k++)
            {
                delete[] mochila1[i][j][k];
            }
            
            delete[] mochila1[i][j];
        }
        delete[] mochila1[i];
    }
    delete[] mochila1;
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
    int ****mochila1 = new int ***[cantJugadores + 1];
    for (int i = 0; i <= cantJugadores; i++)
    {
        mochila1[i] = new int **[Presupuesto + 1];
        for (int j = 0; j <= Presupuesto; j++)
        {
            mochila1[i][j] = new int*[CantExtranjeros + 1]; // Inicializar a 0
            for(int k = 0; k <= CantExtranjeros; k++){
                mochila1[i][j][k] = new int[12]();
            }
        }
    }

    // Llenado de mochila
    for (int jugadorActual = 1; jugadorActual <= cantJugadores; jugadorActual++)
    {
        int peso = 1;                                             // El peso de cada jugador (unidades de presupuesto)
        int costo = players[jugadorActual]->salario;              // Costo del jugador
        int volumen = players[jugadorActual]->extranjero ? 1 : 0; // Volumen (1 si es extranjero)
    

        for (int capacidadPresupuestoActual = 0; capacidadPresupuestoActual <= Presupuesto; capacidadPresupuestoActual++)
        {
            for (int capacidadExtranjerosActual = 0; capacidadExtranjerosActual <= CantExtranjeros; capacidadExtranjerosActual++)
            {
                for (int i = 1; i <= 11; i++)
                {
                    // Verificar si el jugador cabe dentro de los límites de presupuesto y extranjeros
                    if (!elObjetoEntra(costo, volumen, capacidadPresupuestoActual, capacidadExtranjerosActual)){
                        mochila1[jugadorActual][capacidadPresupuestoActual][capacidadExtranjerosActual][i] = mochila1[jugadorActual - 1][capacidadPresupuestoActual][capacidadExtranjerosActual][i];
                    }
                    else
                    {
                        int valorUsarJugador = players[jugadorActual]->valoracion + mochila1[jugadorActual - 1][capacidadPresupuestoActual - costo][capacidadExtranjerosActual - volumen][i-1]; //[4tadim]
                        int valorNoUsarJugador = mochila1[jugadorActual - 1][capacidadPresupuestoActual][capacidadExtranjerosActual][i];
                        mochila1[jugadorActual][capacidadPresupuestoActual][capacidadExtranjerosActual][i] = max(valorUsarJugador, valorNoUsarJugador);
                    }
                }
            }
        }
    }

    //out
    cout << (mochila1[cantJugadores][Presupuesto][CantExtranjeros][11])/11 << endl;

    
    // Liberar memoria de las estructuras usadas
    liberarMochila1(mochila1, cantJugadores, Presupuesto);

    // Liberar memoria de los jugadores
    for (int i = 1; i <= cantJugadores; i++)
    {
        delete players[i];
    }
    delete[] players;

    return 0;
}
