# Práctica 1 - Análisis y Diseño de Algoritmos

## Información del curso

- Profesor: Carlos Alberto Álvarez Henao
- Curso: Análisis y Diseño de Algoritmos - 4579
- Universidad: Universidad EAFIT

## Integrantes

- Gisel Lorena Jaramillo Carmona
- Andres David Osorio Moreno
- Jeronimo Zapata Piza

## Descripción general

Este proyecto implementa dos enfoques para la resolución de problemas de generación y recuperación de contraseñas basadas en hashes SHA-256:

- Fuerza bruta: exploración exhaustiva del espacio de búsqueda de contraseñas.
- Backtracking con poda: búsqueda más eficiente con reducción del árbol de exploración usando restricciones del problema.

Además, el repositorio incluye una verificación de la semilla del equipo y exportación de resultados experimentales a archivos CSV para su análisis.

## Estructura del proyecto

```text
ADA_P1_Jaramillo_Osorio_Zapata/
├── README.md
├── report/
│   └── Informe.pdf
├── resources/
│   └── diccionario.txt
├── results/
│   ├── comparacion_fb_diccionario.txt
│   ├── resultados_backtracking.csv
│   ├── resultados_Instancias.txt
│   ├── tiempos_fuerzabruta_modificado.csv
│   └── tiempos_fuerzabruta.csv
├── src/
│   ├── backtracking.cpp
│   ├── fuerzabruta.cpp
│   └── third_party/
│       └── picosha2.h
├── test/
│   ├── instancia_verificacion.txt
│   └── verificar_semilla.cpp
└── build/  (creado al compilar)
```

## Requisitos

- Compilador de C++17 o superior
- Git Bash, PowerShell o terminal del sistema operativo
- Sistema operativo Windows, Linux o macOS

## Instrucciones de compilación

Desde la raíz del proyecto, ejecute los siguientes comandos:

### Windows (PowerShell o CMD)

```powershell
g++ -std=c++17 -O2 .\src\fuerzabruta.cpp -o .\build\fuerzabruta.exe
g++ -std=c++17 -O2 .\src\backtracking.cpp -o .\build\backtracking.exe
g++ -std=c++17 -O2 .\test\verificar_semilla.cpp -o .\build\verificar_semilla.exe
```

### Linux/macOS

```bash
mkdir -p build
g++ -std=c++17 -O2 ./src/fuerzabruta.cpp -o ./build/fuerzabruta
g++ -std=c++17 -O2 ./src/backtracking.cpp -o ./build/backtracking
g++ -std=c++17 -O2 ./test/verificar_semilla.cpp -o ./build/verificar_semilla
```

## Ejecución

### 1. Verificación de la semilla

```powershell
.\build\verificar_semilla.exe
```

```bash
./build/verificar_semilla
```

Este programa imprime la semilla calculada a partir de los apellidos del equipo y confirma el criterio de generación utilizado.

### 2. Fuerza bruta

```powershell
.\build\fuerzabruta.exe
```

```bash
./build/fuerzabruta
```

Este programa:

- valida la instancia de referencia `abc12`;
- genera las contraseñas objetivo del equipo;
- realiza comparación con hashes SHA-256;
- mide tiempo y número de candidatos evaluados;
- compara fuerza bruta contra ataque por diccionario;
- genera el archivo `tiempos_fuerzabruta.csv` en la raíz del proyecto.

### 3. Backtracking con poda

```powershell
.\build\backtracking.exe
```

```bash
./build/backtracking
```

Este programa:

- genera parámetros de la política de contraseñas asignada al equipo;
- ejecuta el algoritmo de backtracking sin poda y con poda;
- compara métricas como nodos visitados, estados podados y soluciones;
- exporta los resultados a `resultados_backtracking.csv`.

## Reproducción de experimentos

Para reproducir los experimentos de forma consistente, se recomienda ejecutar los programas desde la raíz del proyecto, porque varios de ellos escriben archivos CSV en el directorio actual de trabajo.

### Fuerza bruta

1. Compilar el ejecutable `fuerzabruta`.
2. Ejecutarlo desde la raíz del proyecto.
3. El archivo generado será:
   - `tiempos_fuerzabruta.csv`
4. Se comparan distintos alfabetos y longitudes para observar el crecimiento exponencial del espacio de búsqueda.

### Backtracking

1. Compilar el ejecutable `backtracking`.
2. Ejecutarlo desde la raíz del proyecto.
3. El archivo generado será:
   - `resultados_backtracking.csv`
4. Se analiza el efecto de la poda sobre el número de nodos visitados y el tiempo de ejecución.

### Verificación de semilla

1. Compilar `verificar_semilla.cpp`.
2. Ejecutarlo para corroborar que la semilla calculada es coherente con la fórmula del proyecto.

## Archivos generados por la ejecución

Los resultados experimentales se almacenan en:

- `tiempos_fuerzabruta.csv`
- `resultados_backtracking.csv`
- `comparacion_fb_diccionario.txt`
- `resultados_Instancias.txt`

Estos archivos pueden encontrarse en la carpeta `results/` o en la raíz según la ejecución específica del programa.

## Notas importantes

- Los programas fueron diseñados para trabajar con hashing SHA-256 y diccionarios de palabras.
- La comprobación del rendimiento depende del hardware, por lo que los tiempos pueden variar entre equipos.
- La ejecución de algunos casos de fuerza bruta puede tardar bastante tiempo cuando la longitud y el alfabeto aumentan considerablemente.

## Referencias internas

- Código fuente principal: `src/fuerzabruta.cpp`
- Código fuente de backtracking: `src/backtracking.cpp`
- Verificación de semilla: `test/verificar_semilla.cpp`
- Diccionario: `resources/diccionario.txt`

## Conclusión

Este proyecto permite estudiar de manera práctica la diferencia entre un enfoque exhaustivo de fuerza bruta y una estrategia más inteligente basada en backtracking con poda, además de documentar la semilla del equipo y las métricas obtenidas en ejecución.
