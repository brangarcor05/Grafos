#include "Grafo.h"
#include <iostream>
#include <iomanip>

Grafo::Grafo(int k) : capacidad(k), base(0, 0, -1) {}

void Grafo::agregarProducto(double x, double y) {
    productos.push_back(Producto(x, y, productos.size()));
}

void Grafo::construirMatrizAdyacencia() {
    int n = productos.size();
    matrizAdyacencia.assign(n, std::vector<double>(n, 0.0));
    
    // Calcula distancias entre todos los pares de productos (grafo completo)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j) {
                matrizAdyacencia[i][j] = productos[i].distanciaA(productos[j]);
            }
        }
    }
}

// ============================================
// ALGORITMO DE PRIM - Árbol de Expansión Mínima
// ============================================
std::vector<Arista> Grafo::algoritmoPrim() {
    int n = productos.size();
    if (n == 0) {
        return std::vector<Arista>();
    }
    
    if (n == 1) {
        // Solo hay un producto, no hay árbol que construir
        return std::vector<Arista>();
    }
    
    std::vector<Arista> mst; // Árbol de expansión mínima resultante
    std::vector<bool> enMST(n, false); // Nodos ya incluidos en el MST
    std::priority_queue<Arista, std::vector<Arista>, std::greater<Arista>> pq;
    
    // Paso 1: Comenzar desde el nodo más cercano a la base
    int nodoInicial = nodoMasCercanoABase(enMST);
    enMST[nodoInicial] = true;
    
    // Paso 2: Agregar todas las aristas del nodo inicial a la cola de prioridad
    for (int i = 0; i < n; i++) {
        if (i != nodoInicial && matrizAdyacencia.size() > 0) {
            pq.push(Arista(nodoInicial, i, matrizAdyacencia[nodoInicial][i]));
        }
    }
    
    // Paso 3: Algoritmo de Prim - Seleccionar arista de menor peso
    while (!pq.empty() && (int)mst.size() < n - 1) {
        Arista aristaActual = pq.top();
        pq.pop();
        
        int destino = aristaActual.destino;
        
        // Si el nodo destino ya está en el MST, ignorar esta arista
        if (enMST[destino]) continue;
        
        // Agregar arista al MST
        mst.push_back(aristaActual);
        enMST[destino] = true;
        
        // Agregar todas las aristas del nuevo nodo a la cola
        for (int i = 0; i < n; i++) {
            if (!enMST[i]) {
                pq.push(Arista(destino, i, matrizAdyacencia[destino][i]));
            }
        }
    }
    
    return mst;
}

void Grafo::construirMSTListasAdyacencia(const std::vector<Arista>& mst) {
    int n = productos.size();
    listasAdyacencia.clear();
    listasAdyacencia.assign(n, std::vector<int>());
    
    // Construir grafo no dirigido (árbol) desde el MST
    for (size_t i = 0; i < mst.size(); i++) {
        const Arista& arista = mst[i];
        
        // Verificar índices válidos
        if (arista.origen >= 0 && arista.origen < n &&
            arista.destino >= 0 && arista.destino < n) {
            listasAdyacencia[arista.origen].push_back(arista.destino);
            listasAdyacencia[arista.destino].push_back(arista.origen);
        }
    }
}

// ============================================
// DFS - Recorrido en Profundidad del MST
// ============================================
void Grafo::dfsRecorrido(int nodo, std::vector<bool>& visitado, std::vector<int>& recorrido) {
    // Verificar límites
    if (nodo < 0 || nodo >= (int)productos.size()) {
        return;
    }
    
    if (nodo >= (int)visitado.size()) {
        return;
    }
    
    visitado[nodo] = true;
    recorrido.push_back(nodo);
    
    // Visitar todos los vecinos en el MST
    if (nodo < (int)listasAdyacencia.size()) {
        for (size_t i = 0; i < listasAdyacencia[nodo].size(); i++) {
            int vecino = listasAdyacencia[nodo][i];
            if (vecino >= 0 && vecino < (int)visitado.size() && !visitado[vecino]) {
                dfsRecorrido(vecino, visitado, recorrido);
            }
        }
    }
}

int Grafo::nodoMasCercanoABase(const std::vector<bool>& visitado) {
    double minDist = std::numeric_limits<double>::max();
    int masCercano = 0;
    
    for (size_t i = 0; i < productos.size(); i++) {
        if (!visitado[i]) {
            double dist = base.distanciaA(productos[i]);
            if (dist < minDist) {
                minDist = dist;
                masCercano = i;
            }
        }
    }
    
    return masCercano;
}

// ============================================
// ALGORITMO DEL VECINO MÁS CERCANO POR VIAJES
// ============================================
std::vector<Producto> Grafo::resolverEnrutamientoVecinoMasCercano() {
    if (productos.empty()) return std::vector<Producto>();
    
    // 1. Construir MST
    construirMatrizAdyacencia();
    std::vector<Arista> mst = algoritmoPrim();
    construirMSTListasAdyacencia(mst);
    
    std::vector<Producto> rutaFinal;
    std::vector<bool> visitados(productos.size(), false);
    int pendientes = productos.size();
    
    rutaFinal.push_back(base); // Comenzar en base
    
    while (pendientes > 0) {
        // Encontrar nodo más cercano a la base NO visitado
        int nodoActual = -1;
        double minDist = std::numeric_limits<double>::max();
        
        for (int i = 0; i < productos.size(); i++) {
            if (!visitados[i]) {
                double dist = base.distanciaA(productos[i]);
                if (dist < minDist) {
                    minDist = dist;
                    nodoActual = i;
                }
            }
        }
        
        if (nodoActual == -1) break;
        
        // Hacer DFS desde este nodo para recoger k productos
        std::vector<bool> visitadoLocal(productos.size(), false);
        std::vector<int> recorrido;
        dfsRecorrido(nodoActual, visitadoLocal, recorrido);
        
        // Agregar productos al recorrido (hasta completar k)
        int productosEnViaje = 0;
        for (int idx : recorrido) {
            if (!visitados[idx] && productosEnViaje < capacidad) {
                rutaFinal.push_back(productos[idx]);
                visitados[idx] = true;
                productosEnViaje++;
                pendientes--;
            }
            if (productosEnViaje >= capacidad) break;
        }
        
        // Regresar a base si quedan productos
        if (pendientes > 0) {
            rutaFinal.push_back(base);
        }
    }
    
    // Regreso final
    if (rutaFinal.back().x != 0 || rutaFinal.back().y != 0) {
        rutaFinal.push_back(base);
    }
    
    return rutaFinal;
}

double Grafo::calcularDistanciaTotal(const std::vector<Producto>& ruta) {
    if (ruta.empty()) return 0.0;
    
    double distanciaTotal = 0.0;
    Producto posActual = base;
    
    for (size_t i = 0; i < ruta.size(); i++) {
        const Producto& producto = ruta[i];
        distanciaTotal += posActual.distanciaA(producto);
        posActual = producto;
    }
    
    return distanciaTotal;
}

void Grafo::limpiar() {
    productos.clear();
    matrizAdyacencia.clear();
    listasAdyacencia.clear();
}

