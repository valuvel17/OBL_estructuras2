#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include "clases/Libreria.cpp"
#include "funciones/chars.cpp"

using namespace std;

int main(){
    Libreria* nueva = new Libreria;
    int cantidad_operaciones = 0;
    cin >> cantidad_operaciones;
    for(int i=0; i<cantidad_operaciones; i++){
        char* aux = "";
        cin >> aux;
        char** auxChar= separarChars(aux);
        char letra = aux[0];
        switch (letra){
            case 'A':
                nueva->add((int)auxChar[1],auxChar[2]);
            break;
            
            case 'F':
                cout << nueva->find((int)auxChar[1]) << endl;
            break;

            case 'T':
                if (!nueva->toggle((int)auxChar[1])) cout << "libro_no_encontrado" << endl;
            break;

            case 'C':
                cout << nueva->cantidadTotal() << " " << nueva->cantidadHabilitados() << " " << nueva->cantidadDeshabilitados() << endl;
            break;
            default:
                cout << "comando incompatible" << endl;
            break;
        }
    }
    return 0;
}