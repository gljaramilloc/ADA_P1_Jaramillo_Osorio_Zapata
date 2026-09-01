// Calcular la semilla del equipo apartir de los apellidos, ordenados alfabeticamente
// en minusculas y usando la suma de los codigos ASCII

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

// Calcular la semilla
long long calcular_semilla(vector<string> apellidos) {
    for (string& a : apellidos)
        for (char& c : a) c = (char)tolower((unsigned char)c);
    sort(apellidos.begin(), apellidos.end());
    string cadena;
    for (const string& a : apellidos) cadena += a;
    long long suma = 0;
    for (char c : cadena) suma += (unsigned char)c;
    return suma % 100000;
}

int main() {
    vector<string> apellidos = {"Jaramillo", "Osorio", "Zapata"};
    // Reconstruye la cadena concatenada solo para mostrarla (mismo criterio de ordenamiento y minusculas)
    vector<string> ordenados = apellidos;
    for (string& a : ordenados)
        for (char& c : a) c = (char)tolower((unsigned char)c);
    sort(ordenados.begin(), ordenados.end());
    string cadena;
    for (const string& a : ordenados) cadena += a;
 
    long long semilla = calcular_semilla(apellidos);

    cout << "=====================================================\n";
    cout << "  VERIFICACION DE SEMILLA DEL EQUIPO (Seccion 9.1)\n";
    cout << "=====================================================\n\n";

    cout << "Apellidos ingresados (sin ordenar): ";
    for (size_t i = 0; i < apellidos.size(); i++) {
        cout << apellidos[i] << (i + 1 < apellidos.size() ? ", " : "\n");
    }
 
    cout << "Apellidos ordenados alfabeticamente: ";
    for (size_t i = 0; i < ordenados.size(); i++) {
        cout << ordenados[i] << (i + 1 < ordenados.size() ? ", " : "\n");
    }
 
    cout << "Cadena concatenada (minusculas, sin espacios): \"" << cadena << "\"\n";
 
    long long suma_ascii = 0;
    for (char c : cadena) suma_ascii += (unsigned char)c;
    cout << "Suma de codigos ASCII: " << suma_ascii << "\n";
 
    cout << "Semilla = " << suma_ascii << " mod 100000 = " << semilla << "\n\n";
 
    cout << "Esta semilla debe coincidir exactamente con la semilla impresa\n";
    cout << "por fuerzabruta.cpp y backtracking.cpp al ejecutarse.\n";
 
    return 0;
}
