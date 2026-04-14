#include "../persistencia/GestorArchivos.hpp"
#include "../productos/Producto.hpp"
#include "../cliente/Cliente.hpp"
#include "../transacciones/Transaccion.hpp"
#include "../transacciones/DetalleTransaccion.hpp"
#include "../utilidades/Formatos.hpp"
#include <iostream>
#include <iomanip>

using namespace std;

struct EstadisticaCliente {
    int idCliente;
    string nombreCliente;
    string cedula;
    int cantidadVentas;
    double totalVentas;
    double promedioVenta;
};

void mostrarDetalleTransaccion(int idTransaccion) {
    int numDetalles;
    DetalleTransaccion* detalles = GestorArchivos::obtenerDetallesPorTransaccion(ARCHIVO_DETALLES, idTransaccion, &numDetalles);
    
    if (detalles != nullptr && numDetalles > 0) {
        cout << CYAN << left << setw(20) << "PRODUCTO" << setw(8) << "CANT." << setw(12) << "PRECIO" << setw(12) << "SUBTOTAL" << endl;
        cout << string(60, '-') << endl;
        
        for (int i = 0; i < numDetalles; i++) {
            int indProd = GestorArchivos::buscarPorId<Producto>(ARCHIVO_PRODUCTOS, detalles[i].getIdProducto());
            if (indProd != -1) {
                Producto producto = GestorArchivos::obtenerRegistroPorIndice<Producto>(ARCHIVO_PRODUCTOS, indProd);
                double subtotal = detalles[i].getCantidad() * detalles[i].getPrecioUnitario();
                
                cout << left << setw(20) << producto.getNombre() 
                     << setw(8) << detalles[i].getCantidad()
                     << setw(12) << fixed << setprecision(2) << detalles[i].getPrecioUnitario()
                     << setw(12) << fixed << setprecision(2) << subtotal << endl;
            }
        }
        delete[] detalles;
    }
}

void historialCliente() {
    int idBusq;
    Interfaz interfaz;
    
    if (!interfaz.solicitarEntero("Ingrese ID del cliente para ver su historial", idBusq)) return;

    int indClient = GestorArchivos::buscarPorId<Cliente>(ARCHIVO_CLIENTES, idBusq);
    if (indClient == -1) {
        Formatos::imprimirError("El cliente no existe.");
        return;
    }
    
    Cliente cliente = GestorArchivos::obtenerRegistroPorIndice<Cliente>(ARCHIVO_CLIENTES, indClient);
    
    Formatos::imprimirSubtitulo("HISTORIAL DEL CLIENTE: " + cliente.getNombre());
    cout << CYAN << "Cédula/RIF: " << cliente.getCedula() << " | Total Compras: $" << fixed << setprecision(2) << cliente.getTotalCompras() << RESET << endl;
    cout << string(60, '-') << endl;

    if (cliente.getCantidadTransacciones() == 0) {
        cout << AMARILLO << "No registra movimientos financieros." << RESET << endl;
        return;
    }

    int* transaccionesIds = cliente.getTransaccionesIds();
    
    for (int j = 0; j < cliente.getCantidadTransacciones(); j++) {
        int idTrans = transaccionesIds[j];
        int indTrans = GestorArchivos::buscarPorId<Transaccion>(ARCHIVO_TRANSACCIONES, idTrans);
            
        if (indTrans != -1) {
            Transaccion trans = GestorArchivos::obtenerRegistroPorIndice<Transaccion>(ARCHIVO_TRANSACCIONES, indTrans);
            if (!trans.isEliminado()) {
                cout << CYAN << "\nTransacción ID " << trans.getId() << " | Fecha: " << trans.getFechaRegistro() 
                     << " | Total: $" << fixed << setprecision(2) << trans.getTotal() << RESET << endl;
                mostrarDetalleTransaccion(trans.getId());
            } else {
                cout << AMARILLO << "\nTransacción ID " << trans.getId() << " [ANULADA]" << RESET << endl;
            }
        }
    }
}

void estadisticasVentasTotalesPorCliente() {
    Formatos::imprimirTitulo("ESTADÍSTICAS DE VENTAS TOTALES POR CLIENTE");
    
    // Obtener todos los clientes
    ArchivoHeader headerClientes = GestorArchivos::leerHeader(ARCHIVO_CLIENTES);
    vector<EstadisticaCliente> estadisticas;
    
    for (int i = 0; i < headerClientes.cantidadRegistros; i++) {
        Cliente cliente = GestorArchivos::obtenerRegistroPorIndice<Cliente>(ARCHIVO_CLIENTES, i);
        
        if (!cliente.isEliminado() && cliente.getCantidadTransacciones() > 0) {
            EstadisticaCliente estadistica;
            estadistica.idCliente = cliente.getId();
            estadistica.nombreCliente = cliente.getNombre();
            estadistica.cedula = cliente.getCedula();
            estadistica.cantidadVentas = cliente.getCantidadTransacciones();
            estadistica.totalVentas = cliente.getTotalCompras();
            estadistica.promedioVenta = estadistica.totalVentas / estadistica.cantidadVentas;
            
            estadisticas.push_back(estadistica);
        }
    }
    
    if (estadisticas.empty()) {
        cout << AMARILLO << "No hay clientes con transacciones registradas." << RESET << endl;
        return;
    }
    
    // Ordenar por total de ventas (mayor a menor)
    for (size_t i = 0; i < estadisticas.size() - 1; i++) {
        for (size_t j = i + 1; j < estadisticas.size(); j++) {
            if (estadisticas[j].totalVentas > estadisticas[i].totalVentas) {
                swap(estadisticas[i], estadisticas[j]);
            }
        }
    }
    
    // Mostrar estadísticas
    cout << CYAN << left << setw(8) << "ID" << setw(25) << "CLIENTE" << setw(15) << "CÉDULA" 
         << setw(12) << "VENTAS" << setw(15) << "TOTAL $" << setw(15) << "PROMEDIO $" << endl;
    cout << string(90, '-') << endl;
    
    double totalGeneral = 0;
    int totalVentasGeneral = 0;
    
    for (const auto& est : estadisticas) {
        cout << left << setw(8) << est.idCliente
             << setw(25) << (est.nombreCliente.length() > 24 ? est.nombreCliente.substr(0, 21) + "..." : est.nombreCliente)
             << setw(15) << est.cedula
             << setw(12) << est.cantidadVentas
             << setw(15) << fixed << setprecision(2) << est.totalVentas
             << setw(15) << fixed << setprecision(2) << est.promedioVenta << endl;
        
        totalGeneral += est.totalVentas;
        totalVentasGeneral += est.cantidadVentas;
    }
    
    cout << string(90, '-') << endl;
    cout << CYAN << left << setw(48) << "TOTALES GENERALES:"
         << setw(12) << totalVentasGeneral
         << setw(15) << fixed << setprecision(2) << totalGeneral
         << setw(15) << fixed << setprecision(2) << (totalVentasGeneral > 0 ? totalGeneral / totalVentasGeneral : 0) << RESET << endl;
    
    cout << "\n" << VERDE << "Total de clientes con compras: " << estadisticas.size() << RESET << endl;
}
