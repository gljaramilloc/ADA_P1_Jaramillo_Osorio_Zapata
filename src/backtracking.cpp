#include <iostream>
#include <string>
#include <chrono>
#include <algorithm>
#include <cmath>
using namespace std;


/* En el equipo se decidió que la longitud de la cadena sería 6, permitirá tanto letras minúsculas, números y símbolos especiales (únicamente ! y @). La elección de estos párametros se basa en un equilibrio entre la complejidad de la contraseña y la eficiencia del algoritmo de backtracking. Una longitud de 6 caracteres proporciona un nivel adecuado de seguridad sin comprometer significativamente el rendimiento del algoritmo. En cambio si lo hacemos como inicialmente se planteó en el proyecto, con una longitud de 8 caracteres, el tiempo de ejecución del algoritmo se incrementaría considerablemente, lo que podría afectar la experiencia del usuario , la eficiencia del sistema y nuestras máquinas tendrían que hacer un esfuerzo muy considerable para ejecutarlo. Por lo tanto, se optó por una longitud de 6 caracteres para garantizar un equilibrio entre seguridad y rendimiento.
*/


//Definición del alfabeto y la longitud de la cadena de entrada para el análisis de contraseña.
const std::string ALFABETO = "abcdefghijklmnopqrstuvwxyz0123456789!@";
const int N = 6;

// Contadores de métricas exigidas(almacenrán las salidas de las métricas del algoritmo de backtracking con poda)
long long nodos_visitados_con_poda = 0;
long long estados_podados = 0;
long long soluciones_encontradas = 0;

// Funciones auxiliares para identificar el tipo de carácter
inline bool esMinuscula(char c) { return c >= 'a' && c <= 'z'; }
inline bool esDigito(char c) { return c >= '0' && c <= '9'; }
inline bool esSimbolo(char c) { return c == '!' || c == '@'; }

// Función de backtracking con poda
void bt_con_poda(std::string& actual, int reqL, int reqD, int reqS) {
    nodos_visitados_con_poda++;
    int k = actual.length();

    // Estado Terminal (Hoja)
    if (k == N) {
        if (reqL <= 0 && reqD <= 0 && reqS <= 0) {
            soluciones_encontradas++;
        }
        return;
    }
     // 1. PODA POR INFACTIBILIDAD FUTURA (Espacio restante vs requerimientos pendientes)
    int rem = N - k;
    int pendL = std::max(0, reqL);
    int pendD = std::max(0, reqD);
    int pendS = std::max(0, reqS);

    if (pendL + pendD + pendS > rem) {
        estados_podados++; // Registro del corte de rama
        return;
    }

    for (char c : ALFABETO) {
        // 2. PODA LOCAL: Sin caracteres consecutivos idénticos
        if (k > 0 && actual[k - 1] == c) {
            estados_podados++; // Registro del corte por repetición
            continue;
        }

        actual.push_back(c);
        bt_con_poda(actual, 
                    reqL - (esMinuscula(c) ? 1 : 0),
                    reqD - (esDigito(c) ? 1 : 0),
                    reqS - (esSimbolo(c) ? 1 : 0));
        actual.pop_back(); // Retroceso (Backtrack)
    }
}
    int main() {
    std::string cadena_base = "";
    cout << "====================================================\n";
    cout << "   MÓDULO BT: GENERACIÓN Y PODA DE CONTRASEÑAS      \n";
    cout << "====================================================\n\n";

    // Medición
    auto t1 = std::chrono::high_resolution_clock::now();
    bt_con_poda(cadena_base, 2, 2, 1);
    auto t2 = std::chrono::high_resolution_clock::now();
    double ms = std::chrono::duration<double, std::milli>(t2 - t1).count();

    // Cálculo del total teórico sin poda para referencia
    long long nodos_teoricos_sin_poda = 0;
    for (int i = 0; i <= N; i++) {
        nodos_teoricos_sin_poda += std::pow(ALFABETO.length(), i);
    }

    double porcentaje_reduccion = (1.0 - ((double)nodos_visitados_con_poda / nodos_teoricos_sin_poda)) * 100.0;

    cout << "METRICAS OBTENIDAS (Seccion 8.2 y 9.2):\n";
    cout << "  - Soluciones Validas Encontradas: " << soluciones_encontradas << "\n";
    cout << "  - Nodos Visitados (Con Poda):     " << nodos_visitados_con_poda << "\n";
    cout << "  - Numero de Estados Podados:     " << estados_podados << "\n";
    cout << "  - Nodos Totales Teoricos:        " << nodos_teoricos_sin_poda << "\n";
    cout << "  - Porcentaje de Reduccion:        " << porcentaje_reduccion << " %\n";
    cout << "  - Tiempo de Ejecucion:            " << ms << " ms\n";

    return 0;
}


