#include <iostream>
#include <string>

using namespace std;

class Pedidos
{
private:
    struct orden
    {
        int prioridad;
        bool paraLlevar;
        int id;
        string items;
    };
    struct nodo
    {
        int id;
        int posEnHeap;
        nodo *sig;
    };
    typedef nodo *Lista;

    Lista *tablaHash;
    orden *minHeap;
    int tamano;
    int topeHeap;

    // Funciones auxiliares privadas
    int fhash(int id)
    {
        return abs(id * 17) % tamano;
    }

    void insertarInicio(Lista &l, int id, int pos)
    {
        Lista nuevo = new nodo;
        nuevo->id = id;
        nuevo->posEnHeap = pos;
        nuevo->sig = l;
        l = nuevo;
    }

    void eliminarInicio(Lista &l)
    {
        Lista aBorrar = l;
        l = l->sig;
        delete aBorrar;
        aBorrar = NULL;
    }

    void eliminar(Lista &l, int id)
    {
        if (!l)
            return;
        if (l->id == id)
        {
            Lista aborrar = l;
            l = l->sig;
            delete aborrar;
            aborrar = NULL;
        }
        else
            eliminar(l->sig, id);
    }

    bool comparar(orden o1, orden o2)
    {
        if (o2.prioridad < o1.prioridad)
            return true;
        else if (o2.prioridad > o1.prioridad)
            return false;
        else
        {
            if (o2.paraLlevar && !o1.paraLlevar)
                return true;
            else if (!o2.paraLlevar && o1.paraLlevar)
                return false;
            else
                return o2.id < o1.id;
        }
    }

    void swapi(int pos1, int pos2)
    {
        orden auxOrden = minHeap[pos1];
        minHeap[pos1] = minHeap[pos2];
        minHeap[pos2] = auxOrden;

        // Actualizar las posiciones en la tabla hash
        int poshash = fhash(minHeap[pos2].id);
        Lista aux = tablaHash[poshash];
        while (aux)
        {
            if (aux->id == minHeap[pos2].id)
            {
                aux->posEnHeap = pos2;
            }
            aux = aux->sig;
        }

        poshash = fhash(minHeap[pos1].id);
        aux = tablaHash[poshash];
        while (aux)
        {
            if (aux->id == minHeap[pos1].id)
            {
                aux->posEnHeap = pos1;
            }
            aux = aux->sig;
        }
    }

    int flotar(int pos)
    {
        while (pos > 1 && comparar(minHeap[pos / 2], minHeap[pos]))
        {
            swapi(pos, pos / 2);
            pos = pos / 2;
        }
        return pos;
    }

    void hundir(int pos)
    {
        int posIzq = 2 * pos;
        int posDer = (2 * pos) + 1;
        int menor = pos;

        if (posIzq < topeHeap && comparar(minHeap[menor], minHeap[posIzq]))
            menor = posIzq;

        if (posDer < topeHeap && comparar(minHeap[menor], minHeap[posDer]))
            menor = posDer;

        if (menor != pos)
        {
            swapi(pos, menor);
            return hundir(menor);
        }
    }

    int insertarEnHeap(orden o)
    {
        minHeap[topeHeap] = o;
        int posHeap = flotar(topeHeap);
        topeHeap++;
        return posHeap;
    }

    void eliminarEnHeap(int id)
    {
        int posHash = fhash(id);
        int posEnHeap;
        Lista aux = tablaHash[posHash];
        while (aux)
        {
            if (aux->id == id)
            {
                posEnHeap = aux->posEnHeap;
            }
            aux = aux->sig;
        }
        
        swapi(posEnHeap, topeHeap - 1); // Intercambiar con el último
        topeHeap--;                     // Reducir tamaño del heap
        // flotar(posenHeap); // Se ajusta el heap flotando   
        hundir(posEnHeap); // O hundiendo
    }

public:
    Pedidos(int size)
    {
        tamano = size + 1;
        tablaHash = new Lista[tamano];
        for (int i = 0; i < tamano; i++)
        {
            tablaHash[i] = NULL;
        }
        minHeap = new orden[tamano];
        for (int i = 0; i < tamano; i++)
        {
            minHeap[i] = {0, false, 0, ""}; // Inicializar correctamente
        }
        topeHeap = 1;
    }

    ~Pedidos()
    {
        for (int i = 0; i < tamano; i++)
        {
            while (tablaHash[i])
            {
                eliminarInicio(tablaHash[i]);
            }
        }
        delete[] tablaHash;
        delete[] minHeap;
    }

    void add(int id, int prioridad, bool lleva, string item)
    {
        if (topeHeap >= tamano)
        {
            cout << "No hay espacio para más pedidos" << endl;
            return;
        }

        int pos = fhash(id);
        orden o;
        o.id = id;
        o.items = item;
        o.paraLlevar = lleva;
        o.prioridad = prioridad;

        // Insertar en el heap y obtener la posición en el heap
        int posHeap = insertarEnHeap(o);
        insertarInicio(tablaHash[pos], id, posHeap);
    }

    void remove(int id)
    {
        int pos = fhash(id);
        eliminarEnHeap(id);
        eliminar(tablaHash[pos], id);
    }

    void toggle(int id)
    {
        int pos = fhash(id);
        nodo *aux = tablaHash[pos];
        while (aux)
        {
            if (aux->id == id)
            {
                minHeap[aux->posEnHeap].paraLlevar = !minHeap[aux->posEnHeap].paraLlevar;
            }
            aux = aux->sig;
        }
    }

    string peek()
    {
        if (esVacia())
        {
            cout << "El heap está vacío, no se puede realizar peek." << endl;
            return "";
        }

        string ret = "";
        if (minHeap[1].paraLlevar)
        {
            ret = to_string(minHeap[1].id) + " " + to_string(minHeap[1].prioridad) + " true " + minHeap[1].items;
        }
        else
        {
            ret = to_string(minHeap[1].id) + " " + to_string(minHeap[1].prioridad) + " false " + minHeap[1].items;
        }
        remove(minHeap[1].id);
        return ret;
    }

    bool esVacia()
    {
        return topeHeap <= 1;
    }

};
