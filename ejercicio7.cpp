#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

//PRE: -
//POS: se encarga de derificar si un valorMinMax es valido, es decir ninguna cancion demora mas y ademas nos dan lo alumnos para repartir las caciones
bool verificarDistribucionConValor(int *tiempos, int cantCanciones, int cantEstudiantes, int tiempoMedio)
{
    int i = 0;
    int acumulador_tiempo_escuchado = 0;
    int cantEstudiantesUsados = 1;
    while (i < cantCanciones && cantEstudiantesUsados <= cantEstudiantes)
    {
        int tiempo_cancion = tiempos[i];
        if (tiempo_cancion > tiempoMedio)
            return false; // si o si esta cancion no la pueden escuchar entonces retorno false
        if (acumulador_tiempo_escuchado + tiempo_cancion <= tiempoMedio)
        { // puede escuchar la cancion
            acumulador_tiempo_escuchado += tiempo_cancion;
            i++; //cambio de cancion
        }
        else
        {
            acumulador_tiempo_escuchado = 0;
            cantEstudiantesUsados++; //cambio de estudiante
        }
    }
    if (cantEstudiantesUsados > cantEstudiantes || i < cantCanciones)
        return false;
    return true;
}

//PRE: -
//POS: calcula el menor tiempo el cual le debemos asignar a cada estudiante como maximo para que asi en tiempo global de escucha de canciones sea el menor
int calcularTiempoMaxMin(int *timepos, int cantCanciones, int cantEstudiantes, int cota_inferior, int cota_superior)
{
    while (cota_inferior <= cota_superior)
    {
        int tiempoMedio = (cota_superior + cota_inferior) / 2; // tiempo entre cotas
        if (verificarDistribucionConValor(timepos, cantCanciones, cantEstudiantes, tiempoMedio))
        {
            cota_superior = tiempoMedio - 1;
        }
        else
        {
            cota_inferior = tiempoMedio + 1;
        }
    }
    return cota_inferior;
}

//PRE: -
//POS: Se encarga del out del ejercicio
void imprimirResultados(int *tiempos, int cantCanciones, int cantEstudiantes, int tiempoMaxMin)
{
    int i = 0;
    int tiempo_acumulado = 0;
    int IndiceComienzoCancion = 1;
    int cantEstudiantesUsados = 1;

    while (i < cantCanciones)
    {
        int tiempo_cancion = tiempos[i];
        if (tiempo_acumulado + tiempo_cancion <= tiempoMaxMin)
        { 
            tiempo_acumulado += tiempo_cancion;
            i++; // cambio de canción
        }
        else
        {
            cout << tiempo_acumulado << " " << IndiceComienzoCancion << " " << i << endl;
            IndiceComienzoCancion = i + 1;
            tiempo_acumulado = 0;
            cantEstudiantesUsados++;
        }
    }

    if (tiempo_acumulado > 0) {
        cout << tiempo_acumulado << " " << IndiceComienzoCancion << " " << cantCanciones << endl;
    }

    cout << tiempoMaxMin << endl;
}


int main()
{
    // leo los datos
    int cantCanciones;
    cin >> cantCanciones;
    int *tiempos = new int[cantCanciones];
    int minTiempo = 0;
    int sumaTiempos = 0;
    for (int i = 0; i < cantCanciones; i++)
    {
        int t = 0;
        cin >> t;
        // calculo el menor timepo
        if (t > minTiempo)
        {
            minTiempo = t;
        }
        // calculo la suma de todos los timepos
        sumaTiempos += t;
        tiempos[i] = t;
    }
    int cantEstudiantes;
    cin >> cantEstudiantes;
    // calculo el tiempo max min que va a escuchar cada estudiante
    int tiempoMaxMin = calcularTiempoMaxMin(tiempos, cantCanciones, cantEstudiantes, minTiempo, sumaTiempos);
    // hago el procedimiento e imprimo
    imprimirResultados(tiempos, cantCanciones, cantEstudiantes, tiempoMaxMin);
    // libero memoria
    delete[] tiempos;
    return 0;
}