#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include "clases/MinHeap.cpp"

using namespace std;

int main()
{
    int cantAgregar = 0;
    cin >> cantAgregar;
    // cout << cantAgregar  << endl;
    MinHeap *nuevo = new MinHeap(cantAgregar);
    for (int i = 0; i < cantAgregar; i++)
    {
        int id = 0;
        int precio = 0;
        cin >> id;
        cin >> precio;
        // cout << id << endl;
        // cout << precio << endl;

        nuevo->add(id, precio);
    }
    int cantMostrar = 0;
    cin >> cantMostrar;
    // cout << cantMostrar;
    for (int i = 0; i < cantMostrar; i++)
    {
        cout << nuevo->peak() << endl;
        nuevo->remove();
    }
    return 0;
}