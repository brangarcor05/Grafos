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
    
    cout << "\n" ;
    cout << " ROBOT RECOLECTOR AUTONOMO " << endl;
    cout << "  Archivo de entrada: " << archivoEntrada << endl;
    cout << "\n" ;

    
    for (int escenario = 0; escenario < n; escenario++) {
        int k; // Capacidad del robot
        int m; // Número de productos
        
        entrada >> k >> m;
        
        cout << " ESCENARIO " << (escenario + 1) << endl;
        cout << "  Capacidad del robot (k): " << k << endl;
        cout << "  Número de productos (m): " << m << endl;
        
        // Crear grafo para este escenario
        Grafo grafo(k);
        
        // Leer productos
        for (int i = 0; i < m; i++) {
            double x, y;
            entrada >> x >> y;
            grafo.agregarProducto(x, y);
        }
        
        vector<Producto> rutaOptimizada = grafo.resolverEnrutamientoVecinoMasCercano();
        
        cout << "  RESULTADOS " << endl;
        cout << "  * Numero de viajes: " << ((m + k - 1) / k) << endl;
        cout << "  * Productos recogidos: " << m << "/" << m << endl;
        
        // Escribir resultado en archivo de salida
        salida << k << endl;
        salida << m << endl;
        
        for (size_t i = 0; i < rutaOptimizada.size(); i++) {
            const Producto& producto = rutaOptimizada[i];
            salida << producto.x << " " << producto.y << endl;
        }
        cout << "\n" ;
    }
    
    entrada.close();
    salida.close();
    
    cout << "\n* Proceso completado exitosamente" << endl;
    cout << "* Resultados guardados en: " << archivoSalida << endl;
    return 0;
}