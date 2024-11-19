#include <cassert>
#include <iostream>
#include <limits>
#include <string>

using namespace std;
struct flor
{
    string color;
    int posValida;
};

bool esMejorSolucion(int cantidadActual, int mayorCantidad)
{
    return cantidadActual > mayorCantidad;
}

bool esSolucion()
{
    return true;
}

bool esObjetoValido(int florActual, int F)
{
    return florActual >= 0 && florActual < F;
}

void copiarSolucion(string **jardin_actual, string **mejor_jardin, int N)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            mejor_jardin[i][j] = jardin_actual[i][j];
        }
    }
}

bool adyacentesMismoColor(int florActual, flor **flores, string **jardin_actual, int fil, int col, int N)
{
    string color = flores[florActual]->color;

    // Verificar las celdas adyacentes con sus límites
    int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1}; // Desplazamientos en filas
    int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1}; // Desplazamientos en columnas

    for (int i = 0; i < 8; i++)
    { // 8 direcciones alrededor de la celda
        int nuevaFil = fil + dx[i];
        int nuevaCol = col + dy[i];

        if (nuevaFil >= 0 && nuevaFil < N && nuevaCol >= 0 && nuevaCol < N)
        {
            if (jardin_actual[nuevaFil][nuevaCol] == color)
            {
                return true;
            }
        }
    }

    return false;
}

bool estoyEnFilaValida(int florActual, flor **flores, int fila_actual)
{
    if (flores[florActual]->posValida == fila_actual || flores[florActual]->posValida == -1)
    {
        return true;
    }
    return false;
}

bool puedoPonerPlanta(int florActual, flor **flores, int fila_actual, int col_actual, string **jardin_actual, int N)
{
    if (!adyacentesMismoColor(florActual, flores, jardin_actual, fila_actual, col_actual, N) && estoyEnFilaValida(florActual, flores, fila_actual))
        return true;
    return false;
}

void usarPlanta(int &florActual, flor **flores, int fila_actual, int col_actual, string **jardin_actual, int &cantidadActual)
{
    string color = flores[florActual]->color;
    jardin_actual[fila_actual][col_actual] = color;
    cantidadActual++;
}

bool hayEspacioAlaDerecha(int col_actual, int N)
{
    return col_actual < N - 1;
}

bool hayEspacioAbajo(int fila_actual, int N)
{
    return fila_actual < N - 1;
}

void sacarPlanta(int &cantidadActual, string **jardin_actual, int fila_actual, int col_actual)
{
    jardin_actual[fila_actual][col_actual] = "";
    cantidadActual--;
}

bool puedeSerMejorSolucion(int cantidadActual, int mayorCantidad, int fila_actual, int col_actual, int N)
{
    int totalCeldas = N * N;
    int celdasRevisadas = (fila_actual * N) + (col_actual + 1) - 1;
    int cantPosicionesNoVisitadas = totalCeldas - celdasRevisadas;
    return (cantidadActual + cantPosicionesNoVisitadas) > mayorCantidad;
}

void backtracking(int cantidadActual, string **jardin_actual, int fila_actual, int col_actual, int &mayorCantidad, string **mejor_jardin, flor **flores, int F, int N)
{
    if (!puedeSerMejorSolucion(cantidadActual, mayorCantidad, fila_actual, col_actual, N))
    {
        return;
    }

    if (esSolucion() && esMejorSolucion(cantidadActual, mayorCantidad))
    {
        mayorCantidad = cantidadActual;
        copiarSolucion(jardin_actual, mejor_jardin, N);
    }

    for (int i = 0; i < F; i++) // Iterar sobre las flores disponibles
    {
        if (puedoPonerPlanta(i, flores, fila_actual, col_actual, jardin_actual, N))
        {
            usarPlanta(i, flores, fila_actual, col_actual, jardin_actual, cantidadActual);

            if (hayEspacioAlaDerecha(col_actual, N))
            {
                backtracking(cantidadActual, jardin_actual, fila_actual, col_actual + 1, mayorCantidad, mejor_jardin, flores, F, N);
            }
            else if (hayEspacioAbajo(fila_actual, N)) // Avanzar a la siguiente fila
            {
                backtracking(cantidadActual, jardin_actual, fila_actual + 1, 0, mayorCantidad, mejor_jardin, flores, F, N);
            }

            else
            {
                if (esSolucion() && esMejorSolucion(cantidadActual, mayorCantidad))
                {
                    mayorCantidad = cantidadActual;
                    copiarSolucion(jardin_actual, mejor_jardin, N);
                }
            }
            sacarPlanta(cantidadActual, jardin_actual, fila_actual, col_actual); // Deshacer la decisión
        }
    }
    // Caso en el que no se coloca ninguna planta en esta celda
    if (hayEspacioAlaDerecha(col_actual, N))
    {
        backtracking(cantidadActual, jardin_actual, fila_actual, col_actual + 1, mayorCantidad, mejor_jardin, flores, F, N);
    }
    else if (hayEspacioAbajo(fila_actual, N))
    {
        backtracking(cantidadActual, jardin_actual, fila_actual + 1, 0, mayorCantidad, mejor_jardin, flores, F, N);
    }
    else
    {
        if (esSolucion() && esMejorSolucion(cantidadActual, mayorCantidad))
        {
            mayorCantidad = cantidadActual;
            copiarSolucion(jardin_actual, mejor_jardin, N);
        }
    }
}
// VERSION QUE DA MAL 3 Y NO CORRE 5 Y 6
/*void backtracking(int cantidadActual, string **jardin_actual, int florActual, int fila_actual, int col_actual, int &mayorCantidad, string **mejor_jardin, flor **flores, int F, int N)
{
    if (esSolucion() && esMejorSolucion(cantidadActual, mayorCantidad))
    {
        mayorCantidad = cantidadActual;
        copiarSolucion(jardin_actual, mejor_jardin, N);
    }
    for (int i = 0; i < N*2 ; i++)
    { //  ejemplo si son 6 flores son 12 opciones, entonces hacemos for 12. la flor va a ser 12/2 y la opcion va a ser 12%2 (0 o 1)
        int florActual = i/2;
        bool pongoF = i%2 == 0;
        if (esObjetoValido(florActual, F))
        {
            if (pongoF)
            {
                if (puedoPonerPlanta(florActual, flores, fila_actual, col_actual, jardin_actual, N))
                {
                    usarPlanta(florActual, flores, fila_actual, col_actual, jardin_actual, cantidadActual);
                    if (hayEspacioAlaDerecha(col_actual, N))
                    { // avanzo a la der
                        backtracking(cantidadActual, jardin_actual, florActual, fila_actual, col_actual + 1, mayorCantidad, mejor_jardin, flores, F, N);
                    }
                    else if (hayEspacioAbajo(fila_actual, N)) // fila+1 , col 0
                    {
                        backtracking(cantidadActual, jardin_actual, florActual, fila_actual + 1, 0, mayorCantidad, mejor_jardin, flores, F, N);
                    }
                    sacarPlanta(cantidadActual, jardin_actual, fila_actual, col_actual);
                }
            }
            else
            {
                if (hayEspacioAlaDerecha(col_actual, N))
                { // avanzo a la der
                    backtracking(cantidadActual, jardin_actual, florActual, fila_actual, col_actual + 1, mayorCantidad, mejor_jardin, flores, F, N);
                }
                else if (hayEspacioAbajo(fila_actual, N)) // fila =0 , col +1 (bajo)
                {
                    backtracking(cantidadActual, jardin_actual, florActual, fila_actual + 1, 0, mayorCantidad, mejor_jardin, flores, F, N);
                }
            }
        }
    }
}*/

int main()
{
    int N;
    cin >> N;
    string **mejor_jardin = new string *[N];
    string **jardin_actual = new string *[N];
    for (int i = 0; i < N; i++)
    {
        mejor_jardin[i] = new string[N];
        jardin_actual[i] = new string[N];
        for (int j = 0; j < N; j++)
        {
            mejor_jardin[i][j] = "";
            jardin_actual[i][j] = "";
        }
    }

    int F;
    cin >> F;
    flor **flores = new flor *[F];
    for (int i = 0; i < F; i++)
    {
        string nuevo_color;
        int nueva_posValida;
        cin >> nuevo_color >> nueva_posValida;
        flor *nueva_flor = new flor;
        nueva_flor->color = nuevo_color;
        nueva_flor->posValida = nueva_posValida;
        flores[i] = nueva_flor;
    }
    int mayorCantidad = 0;
    int cantidadActual = 0;
    int florActual = 0;
    int fila_actual = 0;
    int col_actual = 0;
    int capacidadRestante = N * N;
    backtracking(cantidadActual, jardin_actual, fila_actual, col_actual, mayorCantidad, mejor_jardin, flores, F, N);
    cout << mayorCantidad;

    for (int i = 0; i < N; i++)
    {
        delete[] jardin_actual[i];
        delete[] mejor_jardin[i];
    }
    delete[] jardin_actual;
    delete[] mejor_jardin;

    for (int i = 0; i < F; i++)
    {
        delete flores[i];
    }
    delete[] flores;

    return 0;
};