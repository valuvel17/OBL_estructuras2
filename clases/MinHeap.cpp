#include <iostream>
#include <string>
#include "../funciones/enteros.h"

using namespace std;

class MinHeap
{
private:
    // estructuras privadas
    struct datoHeap
    {
        int id;
        int precio;
    };
    // atributos privados
    datoHeap *arrayHeap;
    int *arrayId;
    int tope;    // ultimoOcupado
    int espacio; // no necesariamente tamano de array (ignoramos el 0 en nuestra implementacion)
    // funciones privadas

    // PRE:
    // POS: Retorna si esta Lleno el heap
    bool estaLleno()
    {
        return tope >= espacio;
    }

    // PRE:
    // POS: Retorna si esta vacio el heap
    bool estaVacio()
    {
        return tope == 1;
    }

    // PRE: Recibe una posicion en el heap
    // POS: Devuelve el padre del objeto en esa posicion
    datoHeap padre(int posHijo)
    {
        return arrayHeap[posHijo / 2];
    }

    // PRE: Recibe 2 objetos
    // POS: Devuelve true si no se cumple la condicion dek heap
    bool comparar(datoHeap padre, datoHeap hijo)
    {
        bool ret = false; // true si es el hijo mas chico y false si es el padre mas chico
        if (hijo.precio < padre.precio)
            ret = true;
        if (hijo.precio == padre.precio)
        {
            if (hijo.id > padre.id)
                ret = true;
        }
        return ret;
    }

    // PRE: Recibe 2 posiciones
    // POS: Intercambia los objetos de esas posiciones
    void swapi(int pos1, int pos2)
    {
        datoHeap aux = arrayHeap[pos1];
        arrayHeap[pos1] = arrayHeap[pos2];
        arrayHeap[pos2] = aux;
        arrayId[arrayHeap[pos1].id] = pos1;
        arrayId[arrayHeap[pos2].id] = pos2;
    }

    // PRE: Recibe una posicion en el heap
    // POS: Flota el objeto de esa posicion hasta que respete la condicion del heap
    int flotar(int pos)
    {
        while (pos > 1 && comparar(arrayHeap[pos / 2], arrayHeap[pos]))
        {
            swapi(pos, pos / 2);
            pos = pos / 2;
        }
        return pos;
    }

    // PRE: Recibe una posicion en el heap
    // POS: Hunde el objeto de esa posicion hasta que respete la condicion del heap
    void hundir(int pos)
    {
        int posIzq = 2 * pos;
        int posDer = 2 * pos + 1;
        int menor = pos;
        if (posIzq <= tope && comparar(arrayHeap[menor], arrayHeap[posIzq]))
            menor = posIzq;
        if (posDer <= tope && comparar(arrayHeap[menor], arrayHeap[posDer]))
            menor = posDer;
        if (menor != pos)
        {
            swapi(pos, menor);
            hundir(menor);
        }
    }

    // PRE: Recibe un id y un precio
    // POS: Agrega un nuevo objeto al heap con ese id y el precio
    void addAux(int id, int precio)
    {
        if (!estaLleno())
        {
            if (arrayId[id] == -1)
            {
                tope++;
                arrayHeap[tope].id = id;
                arrayHeap[tope].precio = precio;
                arrayId[id] = flotar(tope);
            }
            else
            {
                if (precio < arrayHeap[arrayId[id]].precio)
                {
                    arrayHeap[arrayId[id]].precio = precio;
                    flotar(arrayId[id]);
                }
            }
        }
    }

    // PRE:
    // POS: Elimina el objeto con menor precio
    void removeAux()
    {
        swapi(1, tope);
        tope--;
        hundir(1);
    }

public:
    // Constructor
    MinHeap(int size)
    {
        arrayHeap = new datoHeap[size + 1];
        for (int i = 0; i < size + 1; i++)
        {
            arrayHeap[i].id = 0;
            arrayHeap[i].precio = 0;
        }
        this->tope = 0;
        this->espacio = size;
        this->arrayId = new int[size + 1];
        for (int i = 0; i < size + 1; i++)
        {
            this->arrayId[i] = -1;
        }
    }

    // Destructor
    ~MinHeap()
    {
        delete[] this->arrayHeap;
        delete[] this->arrayId;
    }
    // funciones publicas

    // PRE:
    // POS: Funcion que ayuda a debuggear
    void mostrarHeap()
    {
        cout << "elHeap se ve: ";
        cout << "[ ";
        for (int i = 1; i <= tope; i++)
        {
            cout << "( " << this->arrayHeap[i].precio << ", " << this->arrayHeap[i].id << " ) , ";
        }
        cout << "]" << endl;
    }

    // PRE: Recibe un id y un precio
    // POS: Agrega un nuevo objeto al heap con ese id y el precio
    void add(int id, int precio)
    {
        addAux(id, precio);
        // mostrarHeap();
    }

    // PRE:
    // POS: Elimina el objeto con menor precio
    void remove()
    {
        removeAux();
        // mostrarHeap();
    }

    // PRE:
    // POS: Devuelve el objeto con menor precio
    int peak()
    {
        return this->arrayHeap[1].id;
    }
};