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
// RESOLVER ENRUTAMIENTO - Usando Prim + DFS
// ============================================
std::vector<Producto> Grafo::resolverEnrutamiento() {
    if (productos.empty()) {
        return std::vector<Producto>();
    }
    
    // Paso 1: Construir matriz de adyacencia (grafo completo)
    construirMatrizAdyacencia();
    
    // Paso 2: Aplicar algoritmo de Prim para obtener MST
    std::vector<Arista> mst = algoritmoPrim();
    
    // Paso 3: Construir listas de adyacencia del MST
    construirMSTListasAdyacencia(mst);
    
    // Paso 4: Realizar DFS sobre el MST para obtener orden de visita
    std::vector<bool> visitadoDFS(productos.size(), false);
    std::vector<int> ordenVisita;
    
    // Comenzar DFS desde el nodo más cercano a la base
    int nodoInicial = nodoMasCercanoABase(visitadoDFS);
    dfsRecorrido(nodoInicial, visitadoDFS, ordenVisita);
    
    // Paso 5: Construir ruta con restricción de capacidad
    std::vector<Producto> rutaFinal;
    int productosEnViaje = 0;
    
    for (size_t i = 0; i < ordenVisita.size(); i++) {
        int idx = ordenVisita[i];
        
        // Si alcanzamos la capacidad, regresar a base
        if (productosEnViaje == capacidad) {
            rutaFinal.push_back(base);
            productosEnViaje = 0;
        }
        
        // Agregar producto a la ruta
        rutaFinal.push_back(productos[idx]);
        productosEnViaje++;
    }
    
    // Regreso final a la base
    rutaFinal.push_back(base);
    
    return rutaFinal;
}

// Versión que retorna también el MST para visualización
std::vector<Producto> Grafo::resolverEnrutamientoConMST(std::vector<Arista>& mstResultado) {
    if (productos.empty()) {
        return std::vector<Producto>();
    }
    
    // Paso 1: Construir matriz de adyacencia (grafo completo)
    construirMatrizAdyacencia();
    
    // Paso 2: Aplicar algoritmo de Prim para obtener MST
    mstResultado = algoritmoPrim();
    
    // Paso 3: Construir listas de adyacencia del MST
    construirMSTListasAdyacencia(mstResultado);
    
    // Paso 4: Realizar DFS sobre el MST para obtener orden de visita
    std::vector<bool> visitadoDFS(productos.size(), false);
    std::vector<int> ordenVisita;
    
    // Comenzar DFS desde el nodo más cercano a la base
    int nodoInicial = nodoMasCercanoABase(visitadoDFS);
    dfsRecorrido(nodoInicial, visitadoDFS, ordenVisita);
    
    // Paso 5: Construir ruta con restricción de capacidad
    std::vector<Producto> rutaFinal;
    int productosEnViaje = 0;
    
    for (size_t i = 0; i < ordenVisita.size(); i++) {
        int idx = ordenVisita[i];
        
        // Si alcanzamos la capacidad, regresar a base
        if (productosEnViaje == capacidad) {
            rutaFinal.push_back(base);
            productosEnViaje = 0;
        }
        
        // Agregar producto a la ruta
        rutaFinal.push_back(productos[idx]);
        productosEnViaje++;
    }
    
    // Regreso final a la base
    rutaFinal.push_back(base);
    
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

void Grafo::mostrarMST(const std::vector<Arista>& mst) {
    std::cout << "\n=== ARBOL DE EXPANSION MINIMA (Prim) ===" << std::endl;
    double pesoTotal = 0.0;
    
    for (size_t i = 0; i < mst.size(); i++) {
        const Arista& arista = mst[i];
        std::cout << "P" << (arista.origen + 1) << " <-> P" << (arista.destino + 1) 
                  << " : " << arista.peso << " m" << std::endl;
        pesoTotal += arista.peso;
    }
    
    std::cout << "Peso total del MST: " << pesoTotal << " m" << std::endl;
    std::cout << "========================================\n" << std::endl;
}

void Grafo::mostrarMatrizDistancias() {
    int n = productos.size();
    if (n == 0) {
        std::cout << "No hay productos en el grafo." << std::endl;
        return;
    }
    
    // Verificar que la matriz esté construida
    if (matrizAdyacencia.empty() || matrizAdyacencia.size() != (size_t)n) {
        std::cout << "Construyendo matriz de adyacencia..." << std::endl;
        construirMatrizAdyacencia();
    }
    
    std::cout << "\n=== MATRIZ DE DISTANCIAS (Grafo Completo) ===" << std::endl;
    std::cout << "Distancias euclidianas entre todos los productos:\n" << std::endl;
    
    // Encabezado
    std::cout << "      ";
    for (int i = 0; i < n; i++) {
        std::cout << "   P" << (i+1) << "   ";
    }
    std::cout << std::endl;
    
    std::cout << "     ";
    for (int i = 0; i < n; i++) {
        std::cout << "--------";
    }
    std::cout << std::endl;
    
    // Matriz
    for (int i = 0; i < n; i++) {
        std::cout << "P" << (i+1) << " | ";
        for (int j = 0; j < n; j++) {
            if (i == j) {
                std::cout << "   -   ";
            } else {
                std::cout << std::fixed << std::setprecision(2) 
                         << std::setw(6) << matrizAdyacencia[i][j] << " ";
            }
        }
        std::cout << std::endl;
    }
    
    std::cout << "\nDistancias desde la BASE (0,0) a cada producto:" << std::endl;
    for (int i = 0; i < n; i++) {
        std::cout << "  BASE -> P" << (i+1) << ": " 
                  << std::fixed << std::setprecision(2) 
                  << base.distanciaA(productos[i]) << " m" << std::endl;
    }
    std::cout << "=============================================\n" << std::endl;
}