#include <iostream>
#include <string>
#include "..\funciones\enteros.h"

using namespace std;

class Libreria2{
    private:
    //Estructuras privadas
    struct libro{
        int id;
        string titulo;
        bool estado;
    };

    libro* tabla;
    bool* eliminado;
    int size;
    int cantidad_total = 0;
    int cantidad_disponible = 0;

    //Funciones privadas
    int sigPrimo(int nro){
        return 0;
    }
    
    //lineal
    int hash1(int key){
        return 1;
    }

    //cuadratico
    int hash2(int key){
        return 1;
    }
    //dobleHash
    int dobleHash(int key, int i){
        return abs(hash1(key) + i*hash2(key))%size;
    }
    
    void addAux(int id, string titulo){
        
    }
    
    string findAux(int id){
        return "";
    }

    bool toggleAux(int id){
        return true;
    }
    //Atributos privados

    public:

    //Funciones publicas
    void TablaHash(int sizeN){
        int new_size = this->sigPrimo(sizeN);
        this->tabla = new libro[new_size];
        for(int i = 0; i< new_size; i++){ 
            tabla[i].estado = NULL;
            tabla[i].id = NULL;
            tabla[i].titulo = "";
        }
        this->size = new_size;
    }
    
    //PRE: Recibe un id y un titulo
    //POS: Agrega el libro a libreria
    void add(int id, string titulo){
        addAux(id,titulo);
    }
    
    //PRE: Recibe una id
    //POS: Devuelve el título del libro si existe en la libreria
    string find(int id){
        return findAux(id);
    }

    //PRE: Recibe la id
    //POS: Devuelve true si esta y false si no.
    bool toggle(int id){
        return toggleAux(id);
    }
    
    //PRE: - 
    //POS: Devuelve la cantidad de libros en la libreria
    int cantidadTotal(){
        return this->cantidad_total;
    }

    //PRE: -
    //POS: Devuelve la cantidad de libros habilitados en la libreria
    int cantidadHabilitados(){
        return this->cantidad_disponible;
    }
    
    //PRE: -
    //POS: Devuelve la cantidad de libros deshabilitados en la libreria
    int cantidadDeshabilitados(){
        return this->cantidad_total - this->cantidad_disponible;
    }
};