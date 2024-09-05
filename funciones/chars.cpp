#include "chars.h"

char** separarChars(char* cadena) {
    int cantidad_palabras = 1;
    int largo = 0;

    while(cadena[largo] != '\0'){
        if(cadena[largo] == ' ') cantidad_palabras++;
        largo++;
    }
    largo++;

    char** resultado = new char*[cantidad_palabras];
    int inicioPalabra = 0;
    int indicePalabra = 0;

    for (int i = 0; i<largo ; i++) {
        if (cadena[i] == ' '){
            int finPalabra = i;
            int longitudPalabra = finPalabra - inicioPalabra;
            resultado[indicePalabra] = new char[longitudPalabra + 1];
            
            for (int j = 0; j < longitudPalabra; j++) {
                resultado[indicePalabra][j] = cadena[inicioPalabra + j];
            }

            resultado[indicePalabra][longitudPalabra] = '\0';            
            indicePalabra++;
            inicioPalabra = i + 1;
        }
    }
    return resultado;
}