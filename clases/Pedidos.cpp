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
    orden **minHeap;
    int tamano;
    int topeHeap;

    // PRE: recibe una id
    // POS: devuelve el bucket de la lista donde se encuentra la posEnHash de esa id
    int fhash(int id)
    {
        return abs(id * 17) % tamano;
    }

    // PRE: se llama cuando la carga de la tabla hash es alta
    // POS: redimensiona la tabla hash y redistribuye las entradas
    void rehash()
    {
        // Guarda el tamaño actual y crea un nuevo tamaño
        int nuevoTamano = tamano * 2;
        Lista *nuevaTablaHash = new Lista[nuevoTamano];

        // Inicializa la nueva tabla hash
        for (int i = 0; i < nuevoTamano; i++)
        {
            nuevaTablaHash[i] = NULL;
        }

        // Redistribuir las entradas de la tabla hash original a la nueva tabla
        for (int i = 0; i < tamano; i++)
        {
            Lista aux = tablaHash[i];
            while (aux)
            {
                // Obtiene la posición en la nueva tabla hash
                int nuevaPos = abs(aux->id * 17) % nuevoTamano;

                // Inserta el nodo en la nueva tabla hash
                insertarInicio(nuevaTablaHash[nuevaPos], aux->id, aux->posEnHeap);
                aux = aux->sig; // Mueve al siguiente nodo
            }
        }

        // Elimina la tabla hash anterior y actualiza el puntero
        for (int i = 0; i < tamano; i++)
        {
            while (tablaHash[i])
            {
                eliminarInicio(tablaHash[i]);
            }
        }
        delete[] tablaHash;

        // Actualiza el puntero de la tabla hash y el tamaño
        tablaHash = nuevaTablaHash;
        tamano = nuevoTamano;
    }

    // PRE: recibe una lista, un id y una pos
    // POS: inserta al comienzo de la lista un nodo con la id y la posEnHeap de la id
    void insertarInicio(Lista &l, int id, int pos)
    {
        Lista nuevo = new nodo;
        nuevo->id = id;
        nuevo->posEnHeap = pos;
        nuevo->sig = l;
        l = nuevo;
    }

    // PRE: recibe una lista
    // POS: elimina el primer elemento de la lista
    void eliminarInicio(Lista &l)
    {
        Lista aBorrar = l;
        l = l->sig;
        delete aBorrar;
        aBorrar = NULL;
    }

    // PRE: recibe una lista y un id
    // POS: elimina el nodo que contenga esa id en la lista
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

    // PRE: recibe 2 ordenes
    // POS: retorna true si el o2 es más prioritario que o1
    bool comparar(orden *o1, orden *o2)
    {
        // Primero se compara la prioridad
        if (o2->prioridad < o1->prioridad)
            return true;
        else if (o2->prioridad > o1->prioridad)
            return false;

        // Si las prioridades son iguales, se compara si es para llevar
        if (o2->paraLlevar && !o1->paraLlevar)
            return true;
        else if (!o2->paraLlevar && o1->paraLlevar)
            return false;

        // Si ambas condiciones anteriores son iguales, compara por id
        return o2->id < o1->id;
    }

    // PRE: recibe 2 posiciones
    // POS: intercambia sus valores en el heap y sus respectivas posiciones en la tabla de hash
    void swapi(int pos1, int pos2)
    {
        orden *auxOrden = minHeap[pos1];
        minHeap[pos1] = minHeap[pos2];
        minHeap[pos2] = auxOrden;

        // Actualizar las posiciones en la tabla hash
        int poshash = fhash(minHeap[pos2]->id);
        Lista aux = tablaHash[poshash];
        while (aux)
        {
            if (aux->id == minHeap[pos2]->id)
            {
                aux->posEnHeap = pos2;
            }
            aux = aux->sig;
        }

        poshash = fhash(minHeap[pos1]->id);
        aux = tablaHash[poshash];
        while (aux)
        {
            if (aux->id == minHeap[pos1]->id)
            {
                aux->posEnHeap = pos1;
            }
            aux = aux->sig;
        }
    }

    // PRE: recibe una posicion en el heap
    // POS: flota el elemento en esa posicion y retorna su nueva posicion
    int flotar(int pos)
    {
        if (pos == 1)
            return 1;
        else
        {
            int posPadre = pos / 2;
            orden *valorPadre = minHeap[posPadre];
            orden *valorActual = minHeap[pos];
            if (comparar(valorPadre, valorActual))
            {
                swapi(posPadre, pos);
                return flotar(posPadre);
            }
        }
        return pos;
    }

    // PRE: recibe una posicion en el heap
    // POS: hunde el elemento de esa posiscion en el heap
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
            hundir(menor);
        }
    }

    // PRE: recibe una orden
    // POS: inserta la orden en el heap
    int insertarEnHeap(int id, string item, bool lleva, int prioridad)
    {
        minHeap[topeHeap]->id = id;
        minHeap[topeHeap]->items = item;
        minHeap[topeHeap]->paraLlevar = lleva;
        minHeap[topeHeap]->prioridad = prioridad;
        int posHeap = flotar(topeHeap);
        topeHeap++;
        return posHeap;
    }

    // PRE: recibe un id
    // POS: elimina esa orden del heap y su respectivo valor en la tabla de hash
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
        topeHeap--;                     // Reducir tamaño del heap (borrado logico)
        flotar(posEnHeap);              // Se ajusta el heap flotando
        hundir(posEnHeap);              // O hundiendo
    }

public:
    // Constructor
    Pedidos(int size)
    {
        tamano = size + 1;
        tablaHash = new Lista[tamano];
        for (int i = 0; i < tamano; i++)
        {
            tablaHash[i] = NULL;
        }
        minHeap = new orden *[tamano];
        for (int i = 0; i < tamano; i++)
        {
            orden *nueva = new orden;
            nueva->id = 0;
            nueva->items = "";
            nueva->paraLlevar = false;
            nueva->prioridad = 0;
            minHeap[i] = nueva;
        }
        topeHeap = 1;
    }
    // Destructor
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

    // PRE: recibe todos los datos de un pedido
    // POS: lo agrega al heap y guarda su posicion en una tabla de hash
    void add(int id, int prioridad, bool lleva, string item)
    {
        if (topeHeap >= tamano * 0.70)
        {
            rehash();
        } 
        if (topeHeap >= tamano)
        {
            cout << "No hay espacio para más pedidos" << endl;
            return;
        }

        int pos = fhash(id);
        // Insertar en el heap y obtener la posición en el heap
        int posHeap = insertarEnHeap(id, item, lleva, prioridad);
        insertarInicio(tablaHash[pos], id, posHeap);
    }

    // PRE: recibe una id
    // POS: lo elimina del heap y elimina su posicion en una tabla de hash
    void remove(int id)
    {
        int pos = fhash(id);
        eliminarEnHeap(id);
        eliminar(tablaHash[pos], id);
    }

    // PRE: recibe un id
    // POS: cambia el valor del atributo lleva de la orden con esa id
    void toggle(int id)
    {
        int pos = fhash(id);
        nodo *aux = tablaHash[pos];
        while (aux)
        {
            if (aux->id == id)
            {
                minHeap[aux->posEnHeap]->paraLlevar = !minHeap[aux->posEnHeap]->paraLlevar;
                int nuevaPos = flotar(aux->posEnHeap);
                hundir(nuevaPos);
                break;
            }
            aux = aux->sig;
        }
    }

    // PRE:
    // POS: retorna un string con la informacion de la orden peek y la elimina
    string peek()
    {
        if (esVacia())
        {
            cout << "El heap está vacío, no se puede realizar peek." << endl;
            return "";
        }

        string ret = "";
        if (minHeap[1]->paraLlevar)
        {
            ret = to_string(minHeap[1]->id) + " " + to_string(minHeap[1]->prioridad) + " true " + minHeap[1]->items;
        }
        else
        {
            ret = to_string(minHeap[1]->id) + " " + to_string(minHeap[1]->prioridad) + " false " + minHeap[1]->items;
        }
        remove(minHeap[1]->id);
        return ret;
    }

    // PRE:
    // POS: retorna true si esta vacio el heap
    bool esVacia()
    {
        return topeHeap <= 1;
    }
};
