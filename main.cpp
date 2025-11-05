#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>
#include "Grafo.h"

using namespace std;

// Función para generar nombre de archivo de salida
string generarNombreSalida(const string& archivoEntrada) {
    // Buscar la última posición del punto
    size_t posPunto = archivoEntrada.find_last_of('.');
    
    if (posPunto != string::npos) {
        // Insertar "_salida" antes de la extensión
        return archivoEntrada.substr(0, posPunto) + "_salida" + archivoEntrada.substr(posPunto);
    } else {
        // Si no hay extensión, agregar "_salida" al final
        return archivoEntrada + "_salida";
    }
}

int main(int argc, char* argv[]) {
    // Verificar argumentos
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <archivo_entrada> [archivo_salida]" << endl;
        cerr << "Ejemplo: " << argv[0] << " test_input.txt" << endl;
        cerr << "         " << argv[0] << " test_input.txt resultado.txt" << endl;
        return 1;
    }
    
    // Nombre de archivos de entrada y salida
    string archivoEntrada = argv[1];
    string archivoSalida;
    
    // Si se proporciona archivo de salida, usarlo; si no, generar automáticamente
    if (argc >= 3) {
        archivoSalida = argv[2];
    } else {
        archivoSalida = generarNombreSalida(archivoEntrada);
    }
    
    ifstream entrada(archivoEntrada);
    if (!entrada.is_open()) {
        cerr << "Error: No se pudo abrir el archivo " << archivoEntrada << endl;
        return 1;
    }
    
    ofstream salida(archivoSalida);
    if (!salida.is_open()) {
        cerr << "Error: No se pudo crear el archivo " << archivoSalida << endl;
        return 1;
    }
    
    int n; // Número de escenarios
    entrada >> n;
    
    salida << n << endl;
    
    cout << "\n+========================================================+" << endl;
    cout << "|   ROBOT RECOLECTOR AUTONOMO - ALGORITMO DE PRIM       |" << endl;
    cout << "+========================================================+" << endl;
    cout << "  Archivo de entrada: " << archivoEntrada << endl;
    cout << "  Archivo de salida:  " << archivoSalida << endl;
    cout << "+========================================================+\n" << endl;
    
    for (int escenario = 0; escenario < n; escenario++) {
        int k; // Capacidad del robot
        int m; // Número de productos
        
        entrada >> k >> m;
        
        cout << "\n+--------------------------------------------------------+" << endl;
        cout << "|  ESCENARIO " << (escenario + 1) << string(44, ' ') << "|" << endl;
        cout << "+--------------------------------------------------------+" << endl;
        cout << "  Capacidad del robot (k): " << k << endl;
        cout << "  Número de productos (m): " << m << endl;
        
        // Crear grafo para este escenario
        Grafo grafo(k);
        
        // Leer productos
        cout << "\n  Productos a recoger:" << endl;
        for (int i = 0; i < m; i++) {
            double x, y;
            entrada >> x >> y;
            grafo.agregarProducto(x, y);
            cout << "    P" << (i+1) << ": (" << fixed << setprecision(2) 
                 << x << ", " << y << ")" << endl;
        }
        
        // MOSTRAR MATRIZ DE DISTANCIAS (si hay pocos productos)
        if (m <= 10) {
            cout << "\n  > Calculando distancias euclidianas entre productos..." << endl;
            grafo.mostrarMatrizDistancias();
        }
        
        // Resolver el problema usando Prim + DFS
        cout << "  > Aplicando algoritmo de Prim..." << endl;
        cout << "  > Construyendo MST (Arbol de Expansion Minima)..." << endl;
        
        // Usar la versión que retorna el MST
        vector<Arista> mstGenerado;
        vector<Producto> rutaOptimizada = grafo.resolverEnrutamientoConMST(mstGenerado);
        
        // MOSTRAR MST GENERADO (si hay pocos productos)
        if (m <= 10) {
            grafo.mostrarMST(mstGenerado);
        }
        
        cout << "  > Recorriendo MST con DFS..." << endl;
        cout << "  > Aplicando restriccion de capacidad..." << endl;
        
        // Calcular distancia total
        double distanciaTotal = grafo.calcularDistanciaTotal(rutaOptimizada);
        
        cout << "\n  +======================================================+" << endl;
        cout << "  |  RESULTADOS                                          |" << endl;
        cout << "  +======================================================+" << endl;
        cout << "  * Distancia total: " << fixed << setprecision(2) 
             << distanciaTotal << " metros" << endl;
        cout << "  * Numero de viajes: " << ((m + k - 1) / k) << endl;
        cout << "  * Productos recogidos: " << m << "/" << m << endl;
        
        // DESGLOSE DE DISTANCIAS (solo si hay pocos productos)
        if (m <= 10 && rutaOptimizada.size() > 0) {
            cout << "\n  DESGLOSE DE DISTANCIAS:" << endl;
            cout << "  ---------------------------------------------------------" << endl;
            Producto posAnterior(0, 0, -1); // Inicia en base
            double distanciaAcumulada = 0.0;
            int segmento = 1;
            
            for (size_t i = 0; i < rutaOptimizada.size(); i++) {
                const Producto& p = rutaOptimizada[i];
                double distSegmento = posAnterior.distanciaA(p);
                distanciaAcumulada += distSegmento;
                
                cout << "    Segmento " << segmento << ": ";
                cout << "(" << fixed << setprecision(2) << posAnterior.x << "," << posAnterior.y << ")";
                cout << " -> (" << p.x << "," << p.y << ")";
                cout << " = " << distSegmento << " m";
                cout << " [Acum: " << distanciaAcumulada << " m]" << endl;
                
                posAnterior = p;
                segmento++;
            }
            cout << "  ---------------------------------------------------------" << endl;
            cout << "  TOTAL: " << distanciaAcumulada << " metros\n" << endl;
        }
        
        // Escribir resultado en archivo de salida
        salida << k << endl;
        salida << rutaOptimizada.size() << endl;
        
        for (size_t i = 0; i < rutaOptimizada.size(); i++) {
            const Producto& producto = rutaOptimizada[i];
            salida << fixed << setprecision(2) << producto.x << " " << producto.y << endl;
        }
        
        // Mostrar ruta en consola
        cout << "\n  RUTA DETALLADA:" << endl;
        cout << "  ---------------------------------------------------------" << endl;
        
        int viaje = 1;
        int productosEnViaje = 0;
        bool enBase = true;
        
        for (size_t i = 0; i < rutaOptimizada.size(); i++) {
            const Producto& p = rutaOptimizada[i];
            
            if (p.x == 0 && p.y == 0) {
                if (!enBase) {
                    cout << "    +-> REGRESO A BASE (0.00, 0.00)" << endl;
                    if (i < rutaOptimizada.size() - 1) {
                        viaje++;
                        productosEnViaje = 0;
                        cout << "\n  VIAJE " << viaje << ":" << endl;
                    }
                    enBase = true;
                }
            } else {
                if (enBase) {
                    cout << "  VIAJE " << viaje << ":" << endl;
                    cout << "    +- Inicio: BASE (0.00, 0.00)" << endl;
                    enBase = false;
                }
                productosEnViaje++;
                cout << "    +-> Producto " << productosEnViaje << ": (" 
                     << fixed << setprecision(2) << p.x << ", " << p.y << ")" << endl;
            }
        }
        
        cout << "\n" << string(60, '=') << endl;
    }
    
    entrada.close();
    salida.close();
    
    cout << "\n* Proceso completado exitosamente" << endl;
    cout << "* Resultados guardados en: " << archivoSalida << endl;
    cout << "\n+========================================================+" << endl;
    cout << "|  Algoritmo utilizado: PRIM + DFS + Restriccion Cap.   |" << endl;
    cout << "+========================================================+\n" << endl;
    
    return 0;
}