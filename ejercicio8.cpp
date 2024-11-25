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


//PRE: -
//POS: funcion que imprime los valores de un par de ciudades
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
//PRE: left mid right positivos. Dentro del tamano de ciudades.
//POS: combina los valores de left hasta mid y de mid hasta right, dejando los valores ordenados entre left y rigth
// Función para combinar dos mitades ordenadas
void merge(Ciudad **ciudades, int left, int mid, int right, bool ordenarPorX) {
    int largo1 = mid - left + 1;
    int largo2 = right - mid;

    // Crear vectores temporales
    Ciudad **vecIzq = new Ciudad*[largo1];
    Ciudad **vecDer = new Ciudad*[largo2];

    for (int i = 0; i < largo1; i++) { //inicializo valores izquierda
        vecIzq[i] = ciudades[left + i];
    }
    for (int j = 0; j < largo2; j++) { //inicializo valores derecha
        vecDer[j] = ciudades[mid + 1 + j];
    }

    int i = 0, j = 0, k = left;
    while (i < largo1 && j < largo2) { //mientras i y j esten dentro del alcance del vector asociado (izq y der respectivamente)
        bool compare = ordenarPorX ? (vecIzq[i]->x < vecDer[j]->x) : (vecIzq[i]->y < vecDer[j]->y); //si ordenarporX es true lo que compara es las y sino las y
        if (compare) { // si el de la izquierda es mas chico
            ciudades[k] = vecIzq[i]; //pongo en ciudades el valor de izq
            i++; // aumento el indice en izquierda
        } else { // el de la derecha es mayor o igual
            ciudades[k] = vecDer[j];
            j++;
        }
        k++;
    }

    // Copiar los elementos restantes (solo entra a una)
    while (i < largo1) { //si paro porque se quedo sin j va aca
        ciudades[k] = vecIzq[i];
        i++;
        k++;
    }
    while (j < largo2) { // si paro porrque se quedo sin i se va a aca 
        ciudades[k] = vecDer[j];
        j++;
        k++;
    }
    // Liberar memoria
    delete[] vecIzq;
    delete[] vecDer;
}

//PRE: -
//POS: ordena el vector de ciudades. Si el bool ordenarPorX  es true ordena segun X y si es false ordena por Y
void mergeSort(Ciudad **ciudades, int i, int d, bool ordenarPorX) {
    if (i < d) {
        int mid = (i + d) / 2; 
        mergeSort(ciudades, i, mid, ordenarPorX);

        mergeSort(ciudades, mid + 1, d, ordenarPorX);

        merge(ciudades, i, mid, d, ordenarPorX);
    }
}

//PRE: -
//POS: funcion que llama a mergeSort para ordenar por X
void MergeSortX(Ciudad **ciudades, int cantCiudades) {
    mergeSort(ciudades, 0, cantCiudades - 1, true); 
}

//PRE: -
//POS: funcion que llama a mergeSort para ordenar por X
void MergeSortY(Ciudad **ciudades, int cantCiudades) {
    mergeSort(ciudades, 0, cantCiudades - 1, false);
}


//PRE: -
//POS: devuelve la distancia efectiva entre 2 ciudades
float distanciaEntreCiudades(Ciudad *c1, Ciudad *c2)
{
    int x1 = c1->x;
    int x2 = c2->x;
    int y1 = c1->y;
    int y2 = c2->y;
    int p1 = c1->p;
    int p2 = c2->p;
    float de =  sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1)); // sqrt (raiz cuadrada), de = distancia euclediana
    float pob = float(abs(p2 - p1)) / float(max(p2, p1));
    return de + pob;
}

//PRE: -
//POS: retorna el par de ciudades cuya distancia efectiva sea menor en caso de empatar resuelve por cantidad de poblacion
ParCiudades *compararPar(ParCiudades *par1, ParCiudades *par2)
{
    float distanciaPar1 = distanciaEntreCiudades(par1->c1, par1->c2);
    float distanciaPar2 = distanciaEntreCiudades(par2->c1, par2->c2);
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

//PRE: -
//POS: devuelve true si la cantidad de ciudades es como mucho 3
bool esSolucionTrivial(Ciudad **ciudades, int i, int f)
{
    return f - i <= 3;
}

//PRE: -
//POS: si son 2 ciudades devueve ese par, si son 3 devuelve el par con menor distancia
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
        float distancia1 = distanciaEntreCiudades(ciudades[i], ciudades[i + 1]);
        float distancia2 = distanciaEntreCiudades(ciudades[i], ciudades[f]);
        float distancia3 = distanciaEntreCiudades(ciudades[i + 1], ciudades[f]);
        float minimaDistancia = min(min(distancia1, distancia2), distancia3);
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

//PRE: -
//POS: retorna la cantidad de elementos no null que hay en el vector franja
int cantElementosFranja(Ciudad **franja)
{
    int i = 0;
    while (franja[i])
    {
        i++;
    }
    return i;
}

//PRE: -
//POS: devuelve el mejor par de ciudades en la franja
ParCiudades *mejorParEnFranja(Ciudad **franja, float menorDistancia)
{
    int cantCiudadesFranja = cantElementosFranja(franja);
    if (cantCiudadesFranja < 2)
    {
        return NULL; // No hay pares posibles en la franja
    }
    
    // tomo las ciudades de la franja ignorando los valores nu
    Ciudad **franjaSinNull = new Ciudad *[cantCiudadesFranja];
    for (int i = 0; i < cantCiudadesFranja; i++)
    {
        franjaSinNull[i] = franja[i];
    }

    // ordeno de menor a mayor segun Y
    MergeSortY(franjaSinNull, cantCiudadesFranja);

    ParCiudades *ret = new ParCiudades;
    ret->c1 = franjaSinNull[0];
    ret->c2 = franjaSinNull[1];
    float mejorDistancia = distanciaEntreCiudades(ret->c1, ret->c2);
    
    // busco la enor distancia entre las ciudades adyacentes ordenadas por menor Y en la franja
    for (int i = 0; i < cantCiudadesFranja - 1; i++)
    {
        float posibleMejorDistancia = distanciaEntreCiudades(franjaSinNull[i], franjaSinNull[i+1]);
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

//PRE: -
//POS: funcion que construye el vector con las ciudades en una franja que se crea a partir de la menor distancia
Ciudad **construirFranja(Ciudad **ciudades, int i, int f, float menorDistancia)
{
   
    Ciudad **franja = new Ciudad *[f - i + 1](); // +1 para garantizar espacio extra
    int j = 0;
    int medio = ciudades[(i + f) / 2]->x;
    for (int k = i; k < f; k++)
    {
        if (ciudades[k]->x > (medio - menorDistancia) && ciudades[k]->x < medio + menorDistancia)
        {
            franja[j] = ciudades[k];
            j++;
        }
    }
    return franja;
}

//PRE: -
//POS: Funcion principal que encuentra el mejor par mediante Divide and Conquer
ParCiudades *mejorParDyC(Ciudad **ciudades, int i, int f)
{
    
    // si es trivial return
    if (esSolucionTrivial(ciudades, i, f))
    {
        return solucionTrivial(ciudades, i, f);
    }
    
    // divido a la mitad y calculo
    int medio = (i + f) / 2;
    ParCiudades *parIzq = mejorParDyC(ciudades, i, medio);
    ParCiudades *parDer = mejorParDyC(ciudades, medio + 1, f);

    // el mejor entre las 2 mitades
    ParCiudades *mejor = compararPar(parIzq, parDer);
    float menorDistancia = distanciaEntreCiudades(mejor->c1, mejor->c2);

    // calculo el mejor par en la franja y lo comparo con el mejor
    Ciudad **franja = construirFranja(ciudades, i, f, menorDistancia);
    if (cantElementosFranja(franja) > 1)
    {
        ParCiudades *parFranja = mejorParEnFranja(franja, menorDistancia);      
        mejor = compararPar(parFranja, mejor);

    }
    
    delete[] franja;
    return mejor;
}

//PRE: -
//POS: funcion que ordena segun X y luego llama a la funcion principal D&C
ParCiudades *mejorPar(Ciudad **ciudades, int cantCiudades)
{
    MergeSortX(ciudades, cantCiudades);
    return mejorParDyC(ciudades, 0, cantCiudades - 1);
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