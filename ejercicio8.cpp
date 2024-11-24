#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include <cmath>

using namespace std;

struct Ciudad
{
    int x;
    int y;
    int p;
};

struct ParCiudades
{
    Ciudad *c1;
    Ciudad *c2;
};

// Función para combinar dos mitades ordenadas
// Función para combinar dos mitades ordenadas
void merge(Ciudad **ciudades, int left, int mid, int right, bool ordenarPorX) {
    int largo1 = mid - left + 1;
    int largo2 = right - mid;

    // Crear arrays temporales
    Ciudad **vecIzq = new Ciudad*[largo1];
    Ciudad **vecDer = new Ciudad*[largo2];

    for (int i = 0; i < largo1; i++) {
        vecIzq[i] = ciudades[left + i];
    }
    for (int j = 0; j < largo2; j++) {
        vecDer[j] = ciudades[mid + 1 + j];
    }

    int i = 0, j = 0, k = left;
    while (i < largo1 && j < largo2) {
        bool compare = ordenarPorX ? (vecIzq[i]->x < vecDer[j]->x) : (vecIzq[i]->y < vecDer[j]->y);
        if (compare) {
            ciudades[k] = vecIzq[i];
            i++;
        } else {
            ciudades[k] = vecDer[j];
            j++;
        }
        k++;
    }

    // Copiar los elementos restantes
    while (i < largo1) {
        ciudades[k] = vecIzq[i];
        i++;
        k++;
    }
    while (j < largo2) {
        ciudades[k] = vecDer[j];
        j++;
        k++;
    }

    // Liberar memoria
    delete[] vecIzq;
    delete[] vecDer;
}
// Función MergeSort recursiva
void mergeSort(Ciudad **ciudades, int i, int d, bool ordenarPorX) {
    if (i < d) {
        int mid = i + (d - i) / 2;

        mergeSort(ciudades, i, mid, ordenarPorX);

        mergeSort(ciudades, mid + 1, d, ordenarPorX);

        // Unir las dos mitades
        merge(ciudades, i, mid, d, ordenarPorX);
    }
}

void MergeSortX(Ciudad **ciudades, int cantCiudades) {
    mergeSort(ciudades, 0, cantCiudades - 1, true);
}

void MergeSortY(Ciudad **ciudades, int cantCiudades) {
    mergeSort(ciudades, 0, cantCiudades - 1, false);
}

int distanciaEntreCiudades(Ciudad *c1, Ciudad *c2)
{
    int x1 = c1->x;
    int x2 = c2->x;
    int y1 = c1->y;
    int y2 = c2->y;
    int p1 = c1->p;
    int p2 = c2->p;
    return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2)) +
           (p1 == 0 && p2 ==0 ? 0 : (abs(p2 - p1)) / double(max(p2, p1)));
    // sqrt (raiz cuadrada), pow (elevado)
}

ParCiudades *compararPar(ParCiudades *par1, ParCiudades *par2)
{
    int distanciaPar1 = distanciaEntreCiudades(par1->c1, par1->c2);
    int distanciaPar2 = distanciaEntreCiudades(par2->c1, par2->c2);
    if (distanciaPar1 < distanciaPar2)
    {
        return par1;
    }
    else if (distanciaPar1 > distanciaPar2)
    {
        return par2;
    }
    else
    {
        int poblacionPar1 = par1->c1->p + par1->c2->p;
        int poblacionPar2 = par2->c1->p + par2->c2->p;
        if (poblacionPar1 > poblacionPar2)
        {
            return par1;
        }
        else 
        {
            return par2;
        }
    }
}

bool esSolucionTrivial(Ciudad **ciudades, int i, int f)
{
    return f - i <= 3;
}

ParCiudades *solucionTrivial(Ciudad **ciudades, int i, int f)
{
    Ciudad *c1;
    Ciudad *c2;
    ParCiudades *par = new ParCiudades;
    if (f - i == 1)
    {
        c1 = ciudades[i];
        c2 = ciudades[f];
        par->c1 = c1;
        par->c2 = c2;
    }
    else
    {
        int distancia1 = distanciaEntreCiudades(ciudades[i], ciudades[i + 1]);
        int distancia2 = distanciaEntreCiudades(ciudades[i], ciudades[f]);
        int distancia3 = distanciaEntreCiudades(ciudades[i + 1], ciudades[f]);
        int minimaDistancia = min(min(distancia1, distancia2), distancia3);
        if (distancia1 == minimaDistancia)
        {
            par->c1 = ciudades[i];
            par->c2 = ciudades[i + 1];
        }
        else if (distancia2 == minimaDistancia)
        {
            par->c1 = ciudades[i];
            par->c2 = ciudades[f];
        }
        else
        {
            par->c1 = ciudades[i + 1];
            par->c2 = ciudades[f];
        }
    }
    return par;
}

int cantElementosFranja(Ciudad **franja)
{
    int i = 0;
    while (franja[i])
    {
        i++;
    }
    return i;
}

ParCiudades *mejorParEnFranja(Ciudad **franja, int menorDistancia)
{
    int cantCiudadesFranja = cantElementosFranja(franja);
    if (cantCiudadesFranja < 2)
    {
        return NULL; // No hay pares posibles en la franja
    }

    Ciudad **franjaSinNull = new Ciudad *[cantCiudadesFranja];
    for (int i = 0; i < cantCiudadesFranja; i++)
    {
        franjaSinNull[i] = franja[i];
    }

    MergeSortY(franjaSinNull, cantCiudadesFranja);

    ParCiudades *ret = new ParCiudades;
    ret->c1 = franjaSinNull[0];
    ret->c2 = franjaSinNull[1];
    int mejorDistancia = distanciaEntreCiudades(ret->c1, ret->c2);

    for (int i = 0; i < cantCiudadesFranja - 1; i++)
    {
        int posibleMejorDistancia = distanciaEntreCiudades(franjaSinNull[i], franjaSinNull[i+1]);
        if (posibleMejorDistancia < mejorDistancia)
        {
            mejorDistancia = posibleMejorDistancia;
            ret->c1 = franjaSinNull[i];
            ret->c2 = franjaSinNull[i+1];
        }
        
    }

    delete[] franjaSinNull;
    return ret;
}


Ciudad **construirFranja(Ciudad **ciudades, int i, int f, int menorDistancia)
{
    // devuelve un vector con las ciudades que se encuentran en la franja
    // es decir las ciudades que se encuentran a una distancia menor a menorDistancia del centro
    Ciudad **franja = new Ciudad *[f - i + 1](); // +1 para garantizar espacio extra
    int j = 0;
    int medio = ciudades[(i + f) / 2]->x;
    for (int k = i; k < f; k++)
    {
        if (ciudades[k]->x > medio - menorDistancia && ciudades[k]->x < medio + menorDistancia)
        {
            franja[j] = ciudades[k];
            j++;
        }
    }
    return franja;
}

ParCiudades *mejorParDyC(Ciudad **ciudades, int i, int f)
{

    if (esSolucionTrivial(ciudades, i, f))
    {
        return solucionTrivial(ciudades, i, f);
    }

    int medio = (i + f) / 2;
    ParCiudades *parIzq = mejorParDyC(ciudades, i, medio);
    ParCiudades *parDer = mejorParDyC(ciudades, medio + 1, f);

    ParCiudades *mejor = compararPar(parIzq, parDer);
    int menorDistancia = distanciaEntreCiudades(mejor->c1, mejor->c2);

    Ciudad **franja = construirFranja(ciudades, i, f, menorDistancia);
    if (cantElementosFranja(franja) > 1)
    {
        ParCiudades *parFranja = mejorParEnFranja(franja, menorDistancia);
        mejor = compararPar(parFranja, mejor);
    }

    delete[] franja;
    return mejor;
}

ParCiudades *mejorPar(Ciudad **ciudades, int cantCiudades)
{
    MergeSortX(ciudades, cantCiudades);
    return mejorParDyC(ciudades, 0, cantCiudades - 1);
}

void imprimir(ParCiudades *sol)
{
    int x1 = sol->c1->x;
    int x2 = sol->c2->x;
    int y1 = sol->c1->y;
    int y2 = sol->c2->y;
    int p1 = sol->c1->p;
    int p2 = sol->c2->p;

    if (x1 < x2)
    {
        cout << x1 << " " << y1 << " " << p1 << endl;
        cout << x2 << " " << y2 << " " << p2 << endl;
    }
    else if (x1 > x2)
    {
        cout << x2 << " " << y2 << " " << p2 << endl;
        cout << x1 << " " << y1 << " " << p1 << endl;
    }
    else
    {
        if (y1 < y2)
        {
            cout << x1 << " " << y1 << " " << p1 << endl;
            cout << x2 << " " << y2 << " " << p2 << endl;
        }
        else
        {
            cout << x2 << " " << y2 << " " << p2 << endl;
            cout << x1 << " " << y1 << " " << p1 << endl;
        }
    }
}

int main()
{
    int cantCiudades;
    cin >> cantCiudades;
    Ciudad **ciudades = new Ciudad *[cantCiudades];
    for (int i = 0; i < cantCiudades; i++)
    {
        int x, y, P;
        cin >> x >> y >> P;
        Ciudad *ciu = new Ciudad;
        ciu->x = x;
        ciu->y = y;
        ciu->p = P;
        ciudades[i] = ciu;
    }
    ParCiudades *sol = mejorPar(ciudades, cantCiudades);
    imprimir(sol);
    delete[] ciudades;
    return 0;
}