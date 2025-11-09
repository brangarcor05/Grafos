#ifndef GRAFO_H
#define GRAFO_H

#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <queue>

// Estructura para representar un producto con sus coordenadas
struct Producto {
    double x;
    double y;
    int id;
    bool visitado;
    
    Producto(double x_ = 0, double y_ = 0, int id_ = -1) 
        : x(x_), y(y_), id(id_), visitado(false) {}
    
    // Calcula distancia euclidiana a otro producto
    double distanciaA(const Producto& otro) const {
        double dx = x - otro.x;
        double dy = y - otro.y;
        return std::sqrt(dx * dx + dy * dy);
    }
};

// Estructura para representar una arista en el grafo
struct Arista {
    int origen;
    int destino;
    double peso;
    
    Arista(int o, int d, double p) : origen(o), destino(d), peso(p) {}
    
    // Para usar en priority_queue (menor peso = mayor prioridad)
    bool operator>(const Arista& otra) const {
        return peso > otra.peso;
    }
};

// Clase Grafo que representa la bodega y los productos
class Grafo {
private:
    std::vector<Producto> productos;
    std::vector<std::vector<double>> matrizAdyacencia;
    Producto base; // Base en (0,0)
    int capacidad; // Capacidad máxima k
    
    // Construye la matriz de adyacencia con todas las distancias
    void construirMatrizAdyacencia();

    
public:
    // Constructor
    Grafo(int k);
    
    // Agrega un producto al grafo
    void agregarProducto(double x, double y);
    
    
    std::vector<Producto> resolverEnrutamientoVecinoMasCercano();
    
    // Calcula la distancia total de una ruta
    double calcularDistanciaTotal(const std::vector<Producto>& ruta);
    
    // Obtiene el número de productos
    int getNumProductos() const { return productos.size(); }
    
    // Limpia el grafo para un nuevo escenario
    void limpiar();
    
    // Obtiene un producto por índice
    Producto getProducto(int index) const { return productos[index]; }
};

#endif