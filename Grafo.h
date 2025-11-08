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
    std::vector<std::vector<int>> listasAdyacencia; // Para el MST
    Producto base; // Base en (0,0)
    int capacidad; // Capacidad máxima k
    
    // Construye la matriz de adyacencia con todas las distancias
    void construirMatrizAdyacencia();
    
    // Algoritmo de Prim para construir MST
    std::vector<Arista> algoritmoPrim();
    
    // Construye listas de adyacencia del MST
    void construirMSTListasAdyacencia(const std::vector<Arista>& mst);
    
    // DFS para recorrer el MST
    void dfsRecorrido(int nodo, std::vector<bool>& visitado, std::vector<int>& recorrido);
    
    // Encuentra el nodo más cercano a la base no visitado
    int nodoMasCercanoABase(const std::vector<bool>& visitado);
    
public:
    // Constructor
    Grafo(int k);
    
    // Agrega un producto al grafo
    void agregarProducto(double x, double y);
    
    // Resuelve el problema de enrutamiento usando Prim + DFS
    std::vector<Producto> resolverEnrutamiento();
    
    // Resuelve y retorna también el MST para visualización
   // std::vector<Producto> resolverEnrutamientoConMST(std::vector<Arista>& mstResultado);

    std::vector<Producto> resolverEnrutamientoVecinoMasCercano();
    
    // Calcula la distancia total de una ruta
    double calcularDistanciaTotal(const std::vector<Producto>& ruta);
    
    // Obtiene el número de productos
    int getNumProductos() const { return productos.size(); }
    
    // Limpia el grafo para un nuevo escenario
    void limpiar();
    
    // Muestra información del MST (para depuración)
    //void mostrarMST(const std::vector<Arista>& mst);
    
    // Muestra la matriz de distancias completa
    //void mostrarMatrizDistancias();
    
    // Obtiene un producto por índice
    Producto getProducto(int index) const { return productos[index]; }
};

#endif