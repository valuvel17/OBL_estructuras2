#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include "clases/Libreria2.cpp"

using namespace std;

int main()
{
    int cantidad_operaciones = 0;
    Libreria2* nueva = new Libreria2(cantidad_operaciones/2); // asumimos que la mitad de las entradas son adds
    cin >> cantidad_operaciones;
    
    for(int i=0; i<cantidad_operaciones; i++){
        char* aux = new char[7];
        cin >> aux;
        char letra = aux[0];
        switch (letra){
            case 'A':{
                int id = 0;
                string titulo = "";
                cin >> id;
                cin >> titulo;
                nueva->add(id,titulo);
            } break;
            
            case 'F': {
                int id = 0;
                cin >> id;
                string respuesta = nueva->find(id);
                cout << respuesta << endl;
            } break;

            case 'T':{
                int id = 0;
                cin >> id;
                if (!(nueva->toggle(id))) cout << "libro_no_encontrado" << endl;
            } break;

            case 'C':{
                int total = nueva->cantidadTotal();
                int hab = nueva->cantidadHabilitados();
                int noHab = nueva->cantidadDeshabilitados();
                cout << total << " " << hab << " " << noHab << endl;
            } break;
            
            default:
                cout << "comando incompatible" << endl;
            break;
        }
        delete[] aux;
    }
    return 0;
}