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
        bool ocupado;
    };

    Libreria2* libreria;
    libro* tabla;
    bool* eliminado;
    int size;
    int cantidad_total = 0;
    int cantidad_disponible = 0;

    //Funciones privadas

    //PRE: Recibe un int cualquiera
    //POS: Devuelve true si es un numero primo
    bool esPrimo(int n) {
        if (n <= 1) return false;
        if (n <= 3) return true;
        if (n % 2 == 0 || n % 3 == 0) return false;
        for (int i=5; i*i <= n; i += 6) {
            if (n % i == 0 || n % (i + 2) == 0) return false;
        }
        return true;
    }
    
    //PRE: Recibe un int cualquiera
    //POS: Devuelve el siguiente numero primo
    int sigPrimo(int primo) {
        int candidato = primo + 1;
        while (!esPrimo(candidato)) {
            candidato++;
        }
        return candidato;
    }
    

    int hash1(int key){
        return (key*17) % size;
    }

    int hash2(int key){
        return (key*key) % size;
    }

    //dobleHash
    int dobleHash(int key, int i){
        return abs(hash1(key) + i*hash2(key))%size;
    }

    void reHash() {
        int old_size = size;
        libro* old_tabla = tabla;  // Save old tabla
        bool* old_eliminado = eliminado;  // Save old eliminado
        
        size = sigPrimo(size + 1);  // Update size to next prime
        tabla = new libro[size];
        eliminado = new bool[size]();
        
        for (int i = 0; i < size; i++) {
            tabla[i].ocupado = false;
            tabla[i].estado = false;
        }
        
        for (int i = 0; i < old_size; i++) {
            if (old_tabla[i].ocupado) {
                int pos = dobleHash(old_tabla[i].id, 1);
                int intento = 1;
                while (tabla[pos].ocupado) {
                    pos = dobleHash(old_tabla[i].id, intento);
                    intento++;
                }
                tabla[pos] = old_tabla[i];
                eliminado[pos] = old_eliminado[i];
            }
        }

        delete[] old_tabla;  // Delete old tabla
        delete[] old_eliminado;  // Delete old eliminado
    }
    
    void agregarAHash(int id, string titulo){
        int pos = dobleHash(id,1);
        int i=1;
        while(this->tabla[pos].ocupado){
            pos = dobleHash(id,i+1);
            i++;
        }
        this->tabla[pos].id = id;
        this->tabla[pos].titulo = titulo;
        this->tabla[pos].estado = this->tabla[pos].ocupado = true;
        size++;
        cantidad_total++;
        cantidad_disponible++;
    }
    
    void addAux(int id, string titulo){
        if (((float)cantidad_total / (float)size) > 0.7) {
	        reHash();
        }
        agregarAHash(id,titulo);
    }  
    
    string findAux(int id){
        int pos = dobleHash(id,1);
        
        if(this->tabla[pos].id == id){
            if(this->tabla[pos].estado) return this->tabla[pos].titulo + " H";
            else return this->tabla[pos].titulo + " D";
        }else{
            int i=1;
            while(i < size && (this->tabla[pos].ocupado || (!(this->tabla[pos].ocupado) && this->eliminado[pos]))){ // (esta en rango && (esta ocupado || (no esta ocupado && fue eliminado)))
                if(this->tabla[pos].id == id){
                    if(this->tabla[pos].estado) return this->tabla[pos].titulo + " H";
                    else return this->tabla[pos].titulo + " D";
                }
                i++;
            }
            return "libro_no_encontrado"; 
        }
    }

    bool toggleAux(int id){
        int pos = dobleHash(id,1);
        
        if(this->tabla[pos].id == id){
            if(tabla[pos].estado) {
                tabla[pos].estado = false;
                cantidad_disponible--;
            }
            else{
                tabla[pos].estado = true;
                cantidad_disponible++;
            }
            return true;
        }
        else {
            int intento = 2;
            while(intento < size && (this->tabla[pos].ocupado || (!(this->tabla[pos].ocupado) && this->eliminado[pos]))){
                pos = dobleHash(id,intento);
                if(this->tabla[pos].id == id){
                   if(tabla[pos].estado) {
                       tabla[pos].estado = false;
                       cantidad_disponible--;
                   }
                   else{
                       tabla[pos].estado = true;
                       cantidad_disponible++;
                   }
                   return true;
                }
                intento++;
            }
            return false;
        }
    }
    //Atributos privados

    public:

    //Funciones publicas
    
    //Contructor
    Libreria2(int sizeN){
        int new_size = this->sigPrimo(sizeN);
        tabla = new libro[new_size];
        for(int i = 0; i< new_size; i++){ 
            tabla[i].estado = false;
            tabla[i].id = 0;
            tabla[i].titulo = "";
            tabla[i].ocupado = false;
        }
        size = new_size;
        cantidad_disponible = 0;
        cantidad_total = 0;
    }
    //Destructor
    ~Libreria2(){
        delete[] this->tabla;
        delete[] this->eliminado;
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