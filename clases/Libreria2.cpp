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
    
    //PRE: Recibe un int cualquiera
    //POS: Devuelve una posicion valida en la tabla
    int hash1(int key){
        return abs(sigPrimo(key)) % size;
    }

    //PRE: Recibe un int cualquiera
    //POS: Devuelve una posicion valida en la tabla
    int hash2(int key) {
        return abs(sigPrimo(key))*17 % size;  
    }

    //PRE:
    //POS:
    int dobleHash(int key, int i){
       return (abs(hash1(key) + i*hash2(key))) % size;
    }
    
    //PRE: 
    //POS: Cambia el tamaño de la tabla y reubica todos los libros 
    void reHash() {
        int old_size = size;
        libro* old_tabla = tabla;  
        bool* old_eliminado = eliminado;  
        
        size = sigPrimo(size*2);  
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
                    pos = dobleHash(old_tabla[i].id, ++intento);
                }
                tabla[pos] = old_tabla[i];
                eliminado[pos] = old_eliminado[i];
            }
        }

        delete[] old_tabla;  
        delete[] old_eliminado;  
    }
    
    //PRE: Recibe una id y el titulo.
    //POS: Lo agrega a la tabla, en caso de que este ocupado y sea la misma id actualiza el titulo y el estado.
    void agregarAHash(int id, string titulo){
        int pos = dobleHash(id,1);
        int i=1;
        //cout << " 1 posicion" << pos << endl;
        while(this->tabla[pos].ocupado){
            if(this->tabla[pos].id == id){
                this->tabla[pos].titulo = titulo;
                if(!this->tabla[pos].estado){
                    cantidad_disponible++;
                    this->tabla[pos].estado = true;
                }
                return;   
            }
            pos = dobleHash(id,++i);
            //cout << "posicion intento" << i << pos << endl;
        }
        this->tabla[pos].id = id;
        this->tabla[pos].titulo = titulo;
        this->tabla[pos].estado = this->tabla[pos].ocupado = true;
        cantidad_total++;
        cantidad_disponible++;
    }
    
    //PRE: Recibe un id y un titulo
    //POS: Agrega el libro a la libreria
    void addAux(int id, string titulo) {
        //cout << "Intentando agregar: ID = " << id << ", Titulo = " << titulo << endl;
        if (((float)cantidad_total / (float)size) > 0.7) {
           // cout << "Realizando reHash..." << endl;
            reHash();
        }
        agregarAHash(id, titulo);
        //cout << "Libro agregado: ID = " << id << endl;
    }
    

    //PRE: Recibe un id
    //POS: Devuelve el titulo y el estado en caso de existir, de lo contrario devuelve libro no encontrado
    string findAux(int id){
        int pos = dobleHash(id, 1);
        int i = 1;

        while (i <= size && (this->tabla[pos].ocupado || (!(this->tabla[pos].ocupado) && this->eliminado[pos]))) {
            if (this->tabla[pos].id == id) {
                if (this->tabla[pos].estado) return this->tabla[pos].titulo + " H";
                else return this->tabla[pos].titulo + " D";
            }
            pos = dobleHash(id, ++i);
        }
        return "libro_no_encontrado"; 
    }

    //PRE: Recibe un id 
    //POS: Cambia el estado del libro con esa id
    bool toggleAux(int id){
        int pos = dobleHash(id, 1);
        int intento = 1;

        while (intento <= size && (this->tabla[pos].ocupado || (!(this->tabla[pos].ocupado) && this->eliminado[pos]))) {
            if (this->tabla[pos].id == id) {
                if (tabla[pos].estado) {
                    tabla[pos].estado = false;
                    cantidad_disponible--;
                } else {
                    tabla[pos].estado = true;
                    cantidad_disponible++;
                }
                return true;
            }
            pos = dobleHash(id, ++intento);  // Actualiza la posición
        }
        return false;
    }
    //Atributos privados

    public:
    //Funciones publicas
    
    //Contructor
    Libreria2(int sizeN){
        size = this->sigPrimo(sizeN);
        tabla = new libro[size];
        eliminado = new bool[size]();
        for(int i = 0; i< size; i++){ 
            tabla[i].estado = false;
            tabla[i].id = 0;
            tabla[i].titulo = "";
            tabla[i].ocupado = false;
        }
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