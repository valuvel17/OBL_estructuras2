#include <iostream>
#include <string>

class Cola
{
private:
    struct nodoLista
    {
        int dato;
        nodoLista *sig;
    };

    unsigned int cantElem;
    nodoLista *final;
    nodoLista *ppio;

public:
    // Constructor
    Cola()
    {
        cantElem = 0;
        final = NULL;
        ppio = NULL;
    }
    // Destructor
    ~Cola()
    {
        while (ppio)
        {
            nodoLista *aBorrar = ppio;
            ppio = ppio->sig;
            delete aBorrar;
            aBorrar = NULL;
        }
    }

    // PRE: recibe un int e
    // POS: lo inserta en la cola
    void encolar(int e)
    {
        nodoLista *nuevo = new nodoLista;
        nuevo->dato = e;
        nuevo->sig = NULL;
        if (final)
        {
            final->sig = nuevo;
        }
        else
        {
            ppio = nuevo;
        }
        final = nuevo;
        cantElem++;
    }

    // PRE: 
    // POS: desencola de la cola y elimina el desencolado
    int desencolar()
    {
        int ret = ppio->dato;
        nodoLista *aBorrar = ppio;
        ppio = ppio->sig;
        if (!ppio)
        {
            final = NULL;
        }
        delete aBorrar;
        aBorrar = NULL;
        cantElem--;
        return ret;
    }

    // PRE: 
    // POS: retorna true si es vacia
    bool esVacia()
    {
        return cantElem == 0;
    }
};
