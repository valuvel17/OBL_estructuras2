#include <iostream>
#include "funciones/enteros.cpp"
using namespace std;

class Libreria{
    private:
    //Estructuras privadas
    struct libroAVL{
            int id;
            char* titulo;
            bool estado;
            int altura;
            libroAVL *izq, *der;
        };

    //Atributos privados
    Libreria* libreria;
    libroAVL* raiz; 
    int cantidad_total = 0;
    int cantidad_disponible = 0;


    //Funciones privadas
    
    //PRE: Recibe un nodo libroAVL
    //POS: Devuelve la altura en que se encuentra
    int alturaAux(libroAVL* nodo){
        if(!nodo) return 0;
        return 1 + max(alturaAux(nodo->izq), alturaAux(nodo->der));
    }

    //PRE: Recibe un nodo libroAVL
    //POS: Devuelve la diferencia de altura entre sus ramas
    int calcularBalance(libroAVL* nodo){
        int alturaDer = nodo->der ? nodo->der->altura : 0; // si ? entonces : sino
        int alturaIzq = nodo->izq ? nodo->izq->altura : 0;
        return alturaDer - alturaIzq;
    }

    //PRE: Recibe un nodo libroAVL
    //POS: Devuelve el mismo libroAVL pero rotado hacia la izquierda
    libroAVL* rotacionIzquierda(libroAVL* A){
        libroAVL* B = A->der;
        libroAVL* T2 = B->izq;
        A->der = T2;
        B->izq = A;
        A->altura = alturaAux(A);//es importante 
        B->altura = alturaAux(B);//este Orden
        return B;
    }

    //PRE: Recibe un nodo libroAVL
    //POS: Devuelve
    libroAVL* rotacionDerecha(libroAVL* B){
        libroAVL* A = B->izq;
        libroAVL* T2 =A->der;
        A->der = B;
        B->izq = T2;
        B->altura = alturaAux(B);//es importante
        A->altura = alturaAux(A);//este Orden
        return A;
    }

    //PRE: Recibe un nodo libroAVL, la id y el titulo
    //POS: Devuelve el nodo raiz
    libroAVL* addAux(libroAVL* nodo,int id, char* titulo){
        //Paso 1 : Insertar
        if(!nodo) {
                libroAVL* nuevo = new libroAVL;
                nuevo->id = id;
                nuevo->titulo = titulo;
                nuevo->estado = true;
                nuevo->altura = 1;
                nuevo->der = nuevo->izq = NULL;
                cantidad_disponible ++;
                cantidad_total ++;
                return nuevo;
        }
        
        if(id > nodo->id) nodo->der = addAux(nodo->der, id, titulo);
        if(id < nodo->id) nodo->izq = addAux(nodo->izq, id, titulo);
        if(id == nodo->id){
            nodo->titulo = titulo;
            nodo->estado = true;
        }
        
        //Todo lo que pasa a partir de ahora solo se ejecuta en el caso A y B (a la vuelta de la recursion)
        //Paso 2 : Calcular la altura 
        nodo->altura = 1 + max(alturaAux(nodo->izq), alturaAux(nodo->der));
        
        //Paso 3 : Verificar el balance
        int balance = calcularBalance(nodo);
        bool desbalanceDerecha = balance > 1;
        bool desbalanceIzquierda = balance < -1;
        
        //Paso 4: Si hay desbalance, identificar caso
        //CASO DERECHA - DERECHA:
        if (desbalanceDerecha && id > nodo->der->id){ //estoy preguntando si el dato es mayor en este caso seria escenario derecha derecha
            return rotacionIzquierda(nodo);
        }

        //CASO DERECHA - IZQUIERDA:
        if (desbalanceDerecha && id < nodo->der->id){
            nodo->der = rotacionDerecha(nodo->der);
            return rotacionIzquierda(nodo);
        }

        //CASO IZQUIERDA - IZQUIERDA: 
        if (desbalanceIzquierda && id < nodo->izq->id){
            return rotacionDerecha(nodo);
        }

        //CASO IZQUIERDA - DERECHA:
        if(desbalanceIzquierda && id > nodo->izq->id){
            nodo->izq = rotacionIzquierda(nodo->izq); 
            return rotacionDerecha(nodo);
        }

        return nodo;
    }
    
    //PRE: Recibe un nodo libroAVL y un id 
    //POS: Devuelve el título del libro si existe y está habilitado si no devuelve libro_no_encontrado
    char* findAux(libroAVL* nodo, int id){
        if(!nodo) return "libro_no_encontrado";

        if(nodo->id > id) return findAux(nodo->izq,id);

        if(nodo->id < id) return findAux(nodo->der,id);  
        
        if(nodo->estado) return nodo->titulo;
        
        else return "libro_no_encontrado";
    }
    
    //PRE: Recibe un nodo libroAVL y un  id 
    //POS: Cambia el estado del libro si esta en la libreria y retorna si cambio o no el estado
    bool toggleAux(libroAVL*&nodo,int id){
        if(!nodo) return false;
        
        if(nodo->id > id) return toggleAux(nodo->izq,id);
        
        if(nodo->id < id) return toggleAux(nodo->der,id);    
        
        if(nodo->estado) {
            nodo->estado = false;
            cantidad_disponible--;
        }else{
            nodo->estado = true;
            cantidad_disponible++;
        }
        return true;     
    }
    
    public:
    //Funciones publicas

    //PRE: Recibe un id y un titulo
    //POS: Agrega el libro a libreria
    void add(int id, char* titulo){
        raiz = addAux(raiz,id,titulo);
    }
    
    //PRE: Recibe una id
    //POS: Devuelve el título del libro si existe en la libreria
    char* find(int id){
        return findAux(raiz,id);

    }

    //PRE: Recibe la id
    //POS: Devuelve true si esta y false si no.
    bool toggle(int id){
        return toggleAux(raiz, id);
    }
    
    //PRE: - 
    //POS: Devuelve la cantidad de libros en la libreria
    int cantidadTotal(){
        return libreria->cantidad_total;
    }

    //PRE: -
    //POS: Devuelve la cantidad de libros habilitados en la libreria
    int cantidadHabilitados(){
        return libreria->cantidad_disponible;
    }
    
    //PRE: -
    //POS: Devuelve la cantidad de libros deshabilitados en la libreria
    int cantidadDeshabilitados(){
        return libreria->cantidad_total - libreria->cantidad_disponible;
    }
};