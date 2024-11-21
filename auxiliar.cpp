#include <iostream>
#include <climits>
using namespace std;

int pesos[4] = {5, 3 , 8 , 4}; // pesos de los elementos
int valores[4] = {10, 20, 25, 8}; // valores de los elementos

int max(int a, int b) {
	return a > b ? a : b;
}

// Importante usar variables nemotecnicas
void mochila01(int capacidad, int cantidadObjetos) {

	// Generamos la matriz de tabulacion
	int ** tab = new int*[cantidadObjetos + 1]; // Objetos +1 para generar la fila de 0 correspondiente a no tener ningun objeto
	for (int objetoActual = 0; objetoActual <= cantidadObjetos; objetoActual++)
	{
		tab[objetoActual] = new int[capacidad + 1](); // +1 para generar la columna de 0 correspondiente a no tener capacidad
	}

	// La fila 0 no la recorremos, solamente la consultamos
	for (int objetoActual = 1; objetoActual <= cantidadObjetos; objetoActual++)
	{
		// Recorremos todas las columnas desde 0 (no tener capacidad) hasta la capacidad maxima inclusive
		for (int capacidadActual = 0; capacidadActual <= capacidad; capacidadActual++)
		{

			int pesoObjetoActual = pesos[objetoActual - 1]; // -1 porque el vector comienza en 0
			int valorObjetoActual = valores[objetoActual - 1]; // -1 porque el vector comienza en 0
			
			// Si el objeto actual no entra en la capacidad actual
			if (pesoObjetoActual > capacidadActual)
			{
				// El valor maximo que podemos obtener es el valor maximo que podemos obtener sin usar el objeto actual
				tab[objetoActual][capacidadActual] = tab[objetoActual - 1][capacidadActual]; 
			} else {
				// El valor maximo que podemos obtener es el valor del objeto actual + el valor maximo que podemos obtener sin usar el objeto actual (fila de arriba) y sin usar la capacidad que ocupa el objeto actual (restar la capacidad actual con el peso del objeto actual)
				int valorDeUsarElObjeto = valorObjetoActual + tab[objetoActual - 1][capacidadActual - pesoObjetoActual];
				
				// MODIFICACION PARA MOCHILA 0-INF
				// Si fuera mochila 0-INF se usa la linea de abajo en vez de la de arriba
				// Solamente cambia el -1 para tener en cuenta el objeto nuevamente
				//int valorDeUsarElObjeto = valorObjetoActual + tab[objetoActual][capacidadActual - pesoObjetoActual];
				
				int valorDeNoUsarElObjeto = tab[objetoActual - 1][capacidadActual]; // Valor de no usar el objeto actual
				tab[objetoActual][capacidadActual] = max(valorDeUsarElObjeto, valorDeNoUsarElObjeto);
			}
		}
	}
	cout << "Valor de la mochila: " << tab[cantidadObjetos][capacidad] << endl;

	// Comparar codigo con Divide&Conquer

	// Reconstruccion de objetos usados en el valor de la solucion
	int objetoActual = cantidadObjetos;
	int capacidadActual = capacidad;
	while (objetoActual > 0) {
		// si el valor en la celda es distinto al valor en la celda de arriba el objeto se uso
		bool useElObjeto = tab[objetoActual][capacidadActual] != tab[objetoActual - 1][capacidadActual];
		if(useElObjeto) {
			cout << "Use el objeto " << objetoActual << ", con peso " << pesos[objetoActual - 1] << " y valor " << valores[objetoActual - 1] << endl;

			// En caso de haber usado el objeto debemos restar la capacidad actual con el peso del objeto actual
			capacidadActual -= pesos[objetoActual - 1];
		}
		// Siempre nos movemos hacia arriba, hayamos uasdo el objeto o no
		objetoActual--;
	}
}

void mochila0INF(int capacidad, int cantidadObjetos)
{

	// Generamos la matriz de tabulacion
	int **tab = new int *[cantidadObjetos + 1]; // Objetos +1 para generar la fila de 0 correspondiente a no tener ningun objeto
	for (int objetoActual = 0; objetoActual <= cantidadObjetos; objetoActual++)
	{
		tab[objetoActual] = new int[capacidad + 1](); // +1 para generar la columna de 0 correspondiente a no tener capacidad
	}

	// La fila 0 no la recorremos, solamente la consultamos
	for (int objetoActual = 1; objetoActual <= cantidadObjetos; objetoActual++)
	{
		// Recorremos todas las columnas desde 0 (no tener capacidad) hasta la capacidad maxima inclusive
		for (int capacidadActual = 0; capacidadActual <= capacidad; capacidadActual++)
		{

			int pesoObjetoActual = pesos[objetoActual - 1];		 // -1 porque el vector comienza en 0
			int valorObjetoActual = valores[objetoActual - 1]; // -1 porque el vector comienza en 0

			// Si el objeto actual no entra en la capacidad actual
			if (pesoObjetoActual > capacidadActual)
			{
				// El valor maximo que podemos obtener es el valor maximo que podemos obtener sin usar el objeto actual
				tab[objetoActual][capacidadActual] = tab[objetoActual - 1][capacidadActual];
			}
			else
			{
				// MODIFICACION PARA MOCHILA 0-INF
				// Solamente cambia el -1 para tener en cuenta el objeto nuevamente
				int valorDeUsarElObjeto = valorObjetoActual + tab[objetoActual][capacidadActual - pesoObjetoActual];

				int valorDeNoUsarElObjeto = tab[objetoActual - 1][capacidadActual]; // Valor de no usar el objeto actual
				tab[objetoActual][capacidadActual] = max(valorDeUsarElObjeto, valorDeNoUsarElObjeto);
			}
		}
	}
	cout << "Valor de la mochila 0-INF: " << tab[cantidadObjetos][capacidad] << endl;

	// Reconstruccion de objetos usados en el valor de la solucion
	int objetoActual = cantidadObjetos;
	int capacidadActual = capacidad;
	while (objetoActual > 0)
	{
		// si el valor en la celda es distinto al valor en la celda de arriba el objeto se uso
		bool useElObjeto = tab[objetoActual][capacidadActual] != tab[objetoActual - 1][capacidadActual];
		if (useElObjeto)
		{
			cout << "Use el objeto " << objetoActual << ", con peso " << pesos[objetoActual - 1] << " y valor " << valores[objetoActual - 1] << endl;

			// En caso de haber usado el objeto debemos restar la capacidad actual con el peso del objeto actual
			capacidadActual -= pesos[objetoActual - 1];

		} else {
			// La diferencia con mochila01 es que no restamos el objeto actual, porque lo podemos usar mas de una vez
			// solamente restamos si no usamos el objeto
			objetoActual--;
		}		
	}
}

int main(){
	int cantidadObjetos = 4; // cantidad de elementos
	int capacidad;
	cin >> capacidad;
	mochila01(capacidad, cantidadObjetos);
	// mochila0INF(capacidad, cantidadObjetos);
	return 0;
}
