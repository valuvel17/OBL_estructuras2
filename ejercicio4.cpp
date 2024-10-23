#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include "clases/Pedidos.cpp"

using namespace std;

int main()
{
    int cantidad_pedidos = 0;
    cin >> cantidad_pedidos;
    int cantidad_operaciones = 0;
    cin >> cantidad_operaciones;
    Pedidos *nueva = new Pedidos(cantidad_pedidos);
    for (int i = 0; i < cantidad_operaciones; i++)
    {
        char aux;
        cin >> aux;
        switch (aux)
        {
        case 'I':
        {
            int id = 0;
            int prioridad = 0;
            string llevar = "";
            string item = "";
            cin >> id;
            cin >> prioridad;
            cin >> llevar;
            cin >> item;
            bool paraLlevar = false;
            if (llevar == "true")
                paraLlevar = true;
            nueva->add(id, prioridad, paraLlevar, item);
        }
        break;
        case 'E':
        {
            int id = 0;
            cin >> id;
            nueva->remove(id);
        }
        break;

        case 'C':
        {
            int id = 0;
            cin >> id;
            nueva->toggle(id);
        }
        break;

        default:
            cout << "comando incompatible" << endl;
            break;
        }
    }
    while (!nueva->esVacia())
    {
        cout << nueva->peek() << endl;
    } 
    return 0;
}