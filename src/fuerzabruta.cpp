#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>
#include "third_party/picosha2.h"

using namespace std;

const string A1 = "abcdefghijklmnopqrstuvwxyz";
const string A2 = "abcdefghijklmnopqrstuvwxyz0123456789";

// Calcula el hash SHA-256 de una cadena
string sha256(const string& s) {
    return picosha2::hash256_hex_string(s);
}

// Prueba todas las cadenas del alfabeto y compara su hash con el objetivo
string fuerza_bruta(const string& hash_objetivo, const string& alfabeto,
                    int longitud_min, int longitud_max,
                    long long& candidatos, double& tiempo_ms) {
    candidatos = 0;
    int base = (int)alfabeto.size();
    auto inicio = chrono::high_resolution_clock::now();

    // Recorre primero las cadenas cortas y luego las largas
    for (int n = longitud_min; n <= longitud_max; n++) {
        vector<int> indices(n, 0);

        while (true) {
            string candidato(n, ' ');
            for (int i = 0; i < n; i++) candidato[i] = alfabeto[indices[i]];
            candidatos++;

            if (sha256(candidato) == hash_objetivo) {
                auto fin = chrono::high_resolution_clock::now();
                tiempo_ms = chrono::duration<double, milli>(fin - inicio).count();
                return candidato;
            }

            // Avanza el odometro: suma 1 a la derecha y lleva acarreo a la izquierda
            int pos = n - 1;
            while (pos >= 0) {
                indices[pos]++;
                if (indices[pos] < base) break;
                indices[pos] = 0;
                pos--;
            }
            // Si el acarreo se salio, se agoto esta longitud
            if (pos < 0) break;
        }
    }

    auto fin = chrono::high_resolution_clock::now();
    tiempo_ms = chrono::duration<double, milli>(fin - inicio).count();
    return "";
}

// Prueba las palabras del diccionario 
string ataque_diccionario(const string& hash_objetivo, const string& ruta,
                          double& tiempo_ms) {
    auto inicio = chrono::high_resolution_clock::now();
    ifstream archivo(ruta);
    string linea, encontrada = "";
    while (getline(archivo, linea)) {
        // Limpia espacios y saltos de linea al final
        while (!linea.empty() &&
               (linea.back()=='\n' || linea.back()=='\r' || linea.back()==' '))
            linea.pop_back();
        if (linea.empty()) continue;
        if (sha256(linea) == hash_objetivo) { encontrada = linea; break; }
    }
    auto fin = chrono::high_resolution_clock::now();
    tiempo_ms = chrono::duration<double, milli>(fin - inicio).count();
    return encontrada;
}

// Calcula la semilla del equipo a partir de los apellidos
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

// Genera las 5 contrasenas objetivo 
vector<string> generar_objetivos(long long semilla) {
    int longitudes[5] = {4, 4, 5, 5, 6};
    const string* alf[5] = {&A1, &A2, &A1, &A2, &A1};
    vector<string> objetivos;
    long long x = semilla;
    for (int p = 0; p < 5; p++) {
        string pass;
        for (int i = 0; i < longitudes[p]; i++) {
            pass += (*alf[p])[x % (long long)alf[p]->size()];
            x = (1103515245LL * x + 12345LL) % 2147483648LL;
        }
        objetivos.push_back(pass);
    }
    return objetivos;
}

int main() {
    long long cand;
    double ms;

    cout << "   MODULO FB: FUERZA BRUTA SOBRE HASHES SHA-256   \n";
    cout << "1) VALIDACION (instancia de referencia 'abc12', A2, n=5)\n";
    string hash_ref =
        "8d51feb34e3e69f6fa6dffc577e2c60490cf9a7fcd835f9f6af1505b71d74773";
    string r = fuerza_bruta(hash_ref, A2, 3, 5, cand, ms);
    cout << "   Recuperada: \"" << r << "\"  ("
         << (r == "abc12" ? "CORRECTO" : "REVISAR") << ")\n";
    cout << "   Candidatos: " << cand << "   Tiempo: " << ms << " ms\n\n";

    // Genera y ataca las contrasenas del equipo
    cout << "2) INSTANCIAS DEL EQUIPO\n";
    long long semilla = calcular_semilla({"Jaramillo", "Osorio", "Zapata"});
    cout << "   Semilla: " << semilla << "\n";
    vector<string> objetivos = generar_objetivos(semilla);
    int longitudes[5] = {4, 4, 5, 5, 6};
    const string* alf[5] = {&A1, &A2, &A1, &A2, &A1};

    for (int i = 0; i < 5; i++) {
        string clave = objetivos[i];
        string h = sha256(clave);
        cout << "   Objetivo " << (i+1) << ": \"" << clave << "\" (n="
             << longitudes[i] << ")\n";
        cout << "      hash: " << h << "\n";
        if (longitudes[i] >= 6)
            cout << "      (cuidado: longitud 6 puede tardar varios minutos)\n";
        string rec = fuerza_bruta(h, *alf[i], 3, longitudes[i], cand, ms);
        cout << "      recuperada: \"" << rec << "\"  en " << ms
             << " ms (" << cand << " candidatos)\n";
    }
    cout << "\n";

    // Mide el tiempo de barrido completo para tamanos crecientes
    cout << "3) EXPERIMENTOS DE TIEMPO (barrido completo)\n";
    cout << "   alfabeto  n   espacio        tiempo_ms\n";
    string imposible(64, '0');
    struct Cfg { string nom; string alf; int n; };
    vector<Cfg> cfgs = {
        {"A1", A1, 3}, {"A1", A1, 4}, {"A1", A1, 5}, //{"A1", A1, 6} (Intratable),
        {"A2", A2, 3}, {"A2", A2, 4}, // {"A2", A2, 5} (Intratable)
    };
    ofstream csv("tiempos_fuerzabruta.csv");
    csv << "alfabeto,tam_alfabeto,longitud,espacio_teorico,tiempo_ms\n";
    for (Cfg& c : cfgs) {
        // Calcula el espacio teorico |alfabeto|^n
        long long espacio = 1;
        for (int i = 0; i < c.n; i++) espacio *= (long long)c.alf.size();
        fuerza_bruta(imposible, c.alf, c.n, c.n, cand, ms);
        cout << "     " << c.nom << "      " << c.n << "   " << espacio
             << "        " << ms << "\n";
        csv << c.nom << "," << c.alf.size() << "," << c.n << ","
            << espacio << "," << ms << "\n";
    }
    csv.close();
    cout << "   (tabla guardada en tiempos_fuerzabruta.csv)\n\n";

    // Compara fuerza bruta pura contra ataque por diccionario
    cout << "4) FUERZA BRUTA vs. DICCIONARIO\n";
    struct CasoPrueba {
        string clave;
        string alfabeto;
    };

    vector<CasoPrueba> casos = {
        {"admin", A1}, // "admin" existe en el diccionario y solo usa A1
        {"zx7k", A2}   // "zx7k" no existe en el diccionario y requiere A2
    };

    for (const auto& c : casos) {
        string h = sha256(c.clave);
        double ms_fb, ms_dic;
        string rfb = fuerza_bruta(h, c.alfabeto, 3, (int)c.clave.size(), cand, ms_fb);
        string rdc = ataque_diccionario(h, "../resources/diccionario.txt", ms_dic);
        cout << "   \"" << c.clave << "\":  FB -> "
             << (rfb.empty() ? "no" : "ENCONTRADA") << " (" << ms_fb << " ms)"
             << "   |   Diccionario -> "
             << (rdc.empty() ? "NO encontrada" : "ENCONTRADA")
             << " (" << ms_dic << " ms)\n";
    }
    cout << "\nListo.\n";
    return 0;
}
