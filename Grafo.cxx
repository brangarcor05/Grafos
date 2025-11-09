#include "Grafo.h"
#include <iostream>
#include <iomanip>

Grafo::Grafo(int k) : base(0, 0, -1), capacidad(k) {}

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


// ALGORITMO DEL VECINO MÁS CERCANO POR VIAJES
std::vector<Producto> Grafo::resolverEnrutamientoVecinoMasCercano() {
    if (productos.empty()) return {};

    construirMatrizAdyacencia();
    std::vector<Producto> rutaFinal;
    std::vector<bool> visitados(productos.size(), false);
    int pendientes = productos.size();

    Producto actual = base;
    rutaFinal.push_back(base);

    while (pendientes > 0) {
        int productosEnViaje = 0;

        while (productosEnViaje < capacidad && pendientes > 0) {
            double menorDist = std::numeric_limits<double>::max();
            int siguiente = -1;

            // Buscar el producto más cercano no visitado
            for (int i = 0; i < (int)productos.size(); ++i) {
                if (!visitados[i]) {
                    double dist = actual.distanciaA(productos[i]);
                    if (dist < menorDist) {
                        menorDist = dist;
                        siguiente = i;
                    }
                }
            }

            // Si ya no hay productos alcanzables, termina el viaje
            if (siguiente == -1) break;

            // Moverse al producto más cercano
            rutaFinal.push_back(productos[siguiente]);
            visitados[siguiente] = true;
            actual = productos[siguiente];
            productosEnViaje++;
            pendientes--;
        }

        // Regresar a base al final del viaje
        rutaFinal.push_back(base);
        actual = base;
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
}

