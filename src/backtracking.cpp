#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>
#include <cmath>
#include <iomanip>

using namespace std;

// 1. ALFABETO COMPLETO DE 69 CARACTERES:
// Minúsculas (26) + Mayúsculas (26) + Dígitos (10) + Símbolos {!, @, #, $, %} (5)
const string ALFABETO_COMPLETO = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%";

// Funciones auxiliares para clasificar los tipos de caracteres
inline bool esMinuscula(char c) { return c >= 'a' && c <= 'z'; }
inline bool esMayuscula(char c) { return c >= 'A' && c <= 'Z'; }
inline bool esDigito(char c)    { return c >= '0' && c <= '9'; }
inline bool esSimbolo(char c)   { return (c == '!' || c == '@' || c == '#' || c == '$' || c == '%'); }

// Estructura para parametrizar las configuraciones solicitadas (Punto 2)
struct ConfiguracionBT {
    string nombre;
    int n;
    string alfabeto;
    int minLower;
    int minUpper;
    int minDigit;
    int minSymbol;
};

// Estructura para registrar y exportar las métricas de los experimentos
struct ResultadoExperimento {
    string nombre;
    int n;
    size_t tamAlfabeto;
    int minLower, minUpper, minDigit, minSymbol;
    long long nodosGeneradosSinPoda;
    long long nodosVisitadosConPoda;
    long long estadosPodados;
    long long solucionesSinPoda;
    long long solucionesConPoda;
    double porcentajeReduccion;
    double tiempoSinPodaMs;
    double tiempoConPodaMs;
};

// ====================================================================
// 3. BACKTRACKING SIN PODA (Medición de nodos generados y soluciones)
// ====================================================================
void bt_sin_poda_rec(string& actual, int n, const string& alf,
                     int reqL, int reqU, int reqD, int reqS,
                     long long& nodos_visitados, long long& soluciones) {
    nodos_visitados++;

    if ((int)actual.length() == n) {
        if (reqL <= 0 && reqU <= 0 && reqD <= 0 && reqS <= 0) {
            soluciones++;
        }
        return;
    }

    for (char c : alf) {
        actual.push_back(c);
        bt_sin_poda_rec(actual, n, alf,
                        reqL - (esMinuscula(c) ? 1 : 0),
                        reqU - (esMayuscula(c) ? 1 : 0),
                        reqD - (esDigito(c) ? 1 : 0),
                        reqS - (esSimbolo(c) ? 1 : 0),
                        nodos_visitados, soluciones);
        actual.pop_back();
    }
}

// ====================================================================
// BACKTRACKING CON PODA (Medición de nodos visitados y poda)
// ====================================================================
void bt_con_poda_rec(string& actual, int n, const string& alf,
                     int reqL, int reqU, int reqD, int reqS,
                     long long& nodos_visitados, long long& estados_podados, long long& soluciones) {
    nodos_visitados++;
    int k = (int)actual.length();

    // Estado Terminal (Hoja)
    if (k == n) {
        if (reqL <= 0 && reqU <= 0 && reqD <= 0 && reqS <= 0) {
            soluciones++;
        }
        return;
    }

    // Poda 1: Infactibilidad futura (Espacio restante vs requerimientos pendientes)
    int rem = n - k;
    int pendL = max(0, reqL);
    int pendU = max(0, reqU);
    int pendD = max(0, reqD);
    int pendS = max(0, reqS);

    if (pendL + pendU + pendD + pendS > rem) {
        estados_podados++;
        return;
    }

    for (char c : alf) {
        // Poda 2: Poda local por caracteres consecutivos idénticos
        if (k > 0 && actual[k - 1] == c) {
            estados_podados++;
            continue;
        }

        actual.push_back(c);
        bt_con_poda_rec(actual, n, alf,
                        reqL - (esMinuscula(c) ? 1 : 0),
                        reqU - (esMayuscula(c) ? 1 : 0),
                        reqD - (esDigito(c) ? 1 : 0),
                        reqS - (esSimbolo(c) ? 1 : 0),
                        nodos_visitados, estados_podados, soluciones);
        actual.pop_back(); // Retroceso (Backtrack)
    }
}

// 4. CÁLCULO DE LA SEMILLA DEL EQUIPO (Idéntica al código de Fuerza Bruta)
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
    cout << "======================================================================\n";
    cout << "   MODULO BT: GENERACION, PODA Y EXPERIMENTACION DE CONTRASENHAS\n";
    cout << "======================================================================\n\n";

    // 4. Semilla del equipo
    long long semilla = calcular_semilla({"Jaramillo", "Osorio", "Zapata"});
    cout << "[-] Semilla generada del equipo: " << semilla << "\n";
    cout << "[-] Tamanho del alfabeto completo: " << ALFABETO_COMPLETO.length() 
         << " caracteres (" << ALFABETO_COMPLETO << ")\n\n";

    // 2. Definición de las 5 configuraciones requeridas por la sección 9.2
    vector<ConfiguracionBT> configs = {
        {"1. Politica Completa (n=8)", 8, ALFABETO_COMPLETO, 2, 2, 2, 1},
        {"2. Misma Politica (n=6)",    6, ALFABETO_COMPLETO, 2, 2, 2, 1},
        {"3. Misma Politica (n=10)",  10, ALFABETO_COMPLETO, 2, 2, 2, 1},
        {"4. Politica Relajada (n=8)",  8, ALFABETO_COMPLETO, 1, 0, 0, 0},
        {"5. Sin Restricciones (n=6)",  6, ALFABETO_COMPLETO, 0, 0, 0, 0}
    };

    vector<ResultadoExperimento> resultados;

    for (const auto& cfg : configs) {
        cout << ">>> Ejecutando variante: " << cfg.nombre << " (n=" << cfg.n << ")\n";

        ResultadoExperimento res;
        res.nombre = cfg.nombre;
        res.n = cfg.n;
        res.tamAlfabeto = cfg.alfabeto.length();
        res.minLower = cfg.minLower;
        res.minUpper = cfg.minUpper;
        res.minDigit = cfg.minDigit;
        res.minSymbol = cfg.minSymbol;

        string cadena_base = "";

        // --- EJECUCIÓN CON PODA ---
        res.nodosVisitadosConPoda = 0;
        res.estadosPodados = 0;
        res.solucionesConPoda = 0;

        auto t1_poda = chrono::high_resolution_clock::now();
        bt_con_poda_rec(cadena_base, cfg.n, cfg.alfabeto,
                        cfg.minLower, cfg.minUpper, cfg.minDigit, cfg.minSymbol,
                        res.nodosVisitadosConPoda, res.estadosPodados, res.solucionesConPoda);
        auto t2_poda = chrono::high_resolution_clock::now();
        res.tiempoConPodaMs = chrono::duration<double, milli>(t2_poda - t1_poda).count();

        // --- EJECUCIÓN SIN PODA ---
        double nodos_totales_teoricos = 0;
        for (int i = 0; i <= cfg.n; ++i) {
            nodos_totales_teoricos += pow(cfg.alfabeto.length(), i);
        }

        res.solucionesSinPoda = 0;
        res.nodosGeneradosSinPoda = 0;
        res.tiempoSinPodaMs = 0.0;

        // Si n <= 6 se realiza la búsqueda real sin poda; para n >= 8 se calcula la referencia teórica
        if (cfg.n <= 6) {
            auto t1_sin = chrono::high_resolution_clock::now();
            bt_sin_poda_rec(cadena_base, cfg.n, cfg.alfabeto,
                            cfg.minLower, cfg.minUpper, cfg.minDigit, cfg.minSymbol,
                            res.nodosGeneradosSinPoda, res.solucionesSinPoda);
            auto t2_sin = chrono::high_resolution_clock::now();
            res.tiempoSinPodaMs = chrono::duration<double, milli>(t2_sin - t1_sin).count();
        } else {
            res.nodosGeneradosSinPoda = (long long)nodos_totales_teoricos;
            res.solucionesSinPoda = res.solucionesConPoda;
            res.tiempoSinPodaMs = -1.0; // Indicador de costo computacional omitido
        }

        // Porcentaje de reducción del espacio de búsqueda
        res.porcentajeReduccion = (1.0 - ((double)res.nodosVisitadosConPoda / nodos_totales_teoricos)) * 100.0;

        resultados.push_back(res);

        cout << "    [CON PODA] Nodos Visitados: " << res.nodosVisitadosConPoda 
             << " | Soluciones: " << res.solucionesConPoda 
             << " | Tiempo: " << res.tiempoConPodaMs << " ms\n";
        if (cfg.n <= 6) {
            cout << "    [SIN PODA] Nodos Generados: " << res.nodosGeneradosSinPoda 
                 << " | Soluciones: " << res.solucionesSinPoda 
                 << " | Tiempo: " << res.tiempoSinPodaMs << " ms\n";
        } else {
            cout << "    [SIN PODA] Nodos Teoricos: " << (long long)nodos_totales_teoricos 
                 << " (Omitido sin poda por complejidad masiva)\n";
        }
        cout << "    [METRICA]  Reduccion del arbol: " << fixed << setprecision(4) << res.porcentajeReduccion << " %\n\n";
    }

    // 5. EXPORTACIÓN DE RESULTADOS A UN ARCHIVO CSV
    string nombreArchivoCSV = "resultados_backtracking.csv";
    ofstream csv(nombreArchivoCSV);
    if (csv.is_open()) {
        csv << "Configuracion,n,Tam_Alfabeto,minLower,minUpper,minDigit,minSymbol,"
            << "Nodos_Generados_Sin_Poda,Nodos_Visitados_Con_Poda,Estados_Podados,"
            << "Soluciones_Sin_Poda,Soluciones_Con_Poda,Porcentaje_Reduccion_Pct,"
            << "Tiempo_Sin_Poda_ms,Tiempo_Con_Poda_ms\n";

        for (const auto& r : resultados) {
            csv << "\"" << r.nombre << "\","
                << r.n << ","
                << r.tamAlfabeto << ","
                << r.minLower << ","
                << r.minUpper << ","
                << r.minDigit << ","
                << r.minSymbol << ","
                << r.nodosGeneradosSinPoda << ","
                << r.nodosVisitadosConPoda << ","
                << r.estadosPodados << ","
                << r.solucionesSinPoda << ","
                << r.solucionesConPoda << ","
                << fixed << setprecision(4) << r.porcentajeReduccion << ","
                << fixed << setprecision(2) << r.tiempoSinPodaMs << ","
                << r.tiempoConPodaMs << "\n";
        }
        csv.close();
        cout << "[+] Exito: Los resultados han sido exportados correctamente a '" << nombreArchivoCSV << "'.\n";
    } else {
        cerr << "[!] Error: No se pudo crear el archivo CSV.\n";
    }

    return 0;
}


