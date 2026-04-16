#include "../persistencia/GestorArchivos.hpp"
#include "../productos/Producto.hpp"
#include "../proveedores/Proveedor.hpp"
#include "../cliente/Cliente.hpp"
#include "../transacciones/Transaccion.hpp"
#include "../transacciones/DetalleTransaccion.hpp"
#include "../utilidades/Formatos.hpp"
#include <iostream>
#include <iomanip>

using namespace std;

void verificarIntegridadReferencial() {
    Formatos::imprimirTitulo("DIAGNÓSTICO DE INTEGRIDAD REFERENCIAL");
    int errores = 0;

    // Productos -> Valida ID Proveedores
    cout << "\n" << CYAN << "Verificando relaciones Productos -> Proveedores..." << RESET << endl;
    ArchivoHeader headerProductos = GestorArchivos::leerHeader(ARCHIVO_PRODUCTOS);
    
    for (int i = 0; i < headerProductos.cantidadRegistros; i++) {
        Producto producto = GestorArchivos::obtenerRegistroPorIndice<Producto>(ARCHIVO_PRODUCTOS, i);
        if (!producto.isEliminado()) {
            if (!GestorArchivos::existeEntidad<Proveedor>(ARCHIVO_PROVEEDORES, producto.getIdProveedor())) {
                cout << ROJO << "[ERROR] Producto ID " << producto.getId() << " (" << producto.getNombre() 
                     << ") refiere a Proveedor ID " << producto.getIdProveedor() << " inexistente." << RESET << endl;
                errores++;
            }
        }
    }

    // Detalles -> Valida ID Productos y ID Transacciones
    cout << "\n" << CYAN << "Verificando relaciones Detalles -> Productos/Transacciones..." << RESET << endl;
    ArchivoHeader headerDetalles = GestorArchivos::leerHeader(ARCHIVO_DETALLES);
    
    for (int i = 0; i < headerDetalles.cantidadRegistros; i++) {
        DetalleTransaccion detalle = GestorArchivos::obtenerRegistroPorIndice<DetalleTransaccion>(ARCHIVO_DETALLES, i);
        
        if (!GestorArchivos::existeEntidad<Producto>(ARCHIVO_PRODUCTOS, detalle.getIdProducto())) {
            cout << ROJO << "[ERROR] Detalle refiere a Producto ID " << detalle.getIdProducto() << " inexistente." << RESET << endl;
            errores++;
        }
        
        if (!GestorArchivos::existeEntidad<Transaccion>(ARCHIVO_TRANSACCIONES, detalle.getIdTransaccion())) {
            cout << ROJO << "[ERROR] Detalle refiere a Transacción ID " << detalle.getIdTransaccion() << " inexistente." << RESET << endl;
            errores++;
        }
    }

    // Transacciones -> Valida ID Productos y ID Clientes/Proveedores
    cout << "\n" << CYAN << "Verificando relaciones Transacciones -> Productos/Clientes/Proveedores..." << RESET << endl;
    ArchivoHeader headerTransacciones = GestorArchivos::leerHeader(ARCHIVO_TRANSACCIONES);
    
    for (int i = 0; i < headerTransacciones.cantidadRegistros; i++) {
        Transaccion transaccion = GestorArchivos::obtenerRegistroPorIndice<Transaccion>(ARCHIVO_TRANSACCIONES, i);
        
        if (!transaccion.isEliminado()) {
            // Verificar que el tipo de transacción sea válido
            const char* tipo = transaccion.getTipo();
            int idRelacionado = transaccion.getIdRelacionado();
            
            if (strcmp(tipo, "VENTA") == 0) {
                // Para ventas, verificar que el cliente exista
                if (!GestorArchivos::existeEntidad<Cliente>(ARCHIVO_CLIENTES, idRelacionado)) {
                    cout << ROJO << "[ERROR] Transacción VENTA ID " << transaccion.getId() 
                         << " refiere a Cliente ID " << idRelacionado << " inexistente." << RESET << endl;
                    errores++;
                }
            } else if (strcmp(tipo, "COMPRA") == 0) {
                // Para compras, verificar que el proveedor exista
                if (!GestorArchivos::existeEntidad<Proveedor>(ARCHIVO_PROVEEDORES, idRelacionado)) {
                    cout << ROJO << "[ERROR] Transacción COMPRA ID " << transaccion.getId() 
                         << " refiere a Proveedor ID " << idRelacionado << " inexistente." << RESET << endl;
                    errores++;
                }
            }
            
            // Verificar que los detalles de la transacción tengan productos válidos
            ArchivoHeader headerDetalles = GestorArchivos::leerHeader(ARCHIVO_DETALLES);
            for (int j = 0; j < headerDetalles.cantidadRegistros; j++) {
                DetalleTransaccion detalle = GestorArchivos::obtenerRegistroPorIndice<DetalleTransaccion>(ARCHIVO_DETALLES, j);
                
                if (detalle.getIdTransaccion() == transaccion.getId()) {
                    if (!GestorArchivos::existeEntidad<Producto>(ARCHIVO_PRODUCTOS, detalle.getIdProducto())) {
                        cout << ROJO << "[ERROR] Transacción ID " << transaccion.getId() 
                             << " tiene detalle con Producto ID " << detalle.getIdProducto() << " inexistente." << RESET << endl;
                        errores++;
                    }
                }
            }
        }
    }

    
    // Reporte final
    cout << "\n" << AMARILLO << "========================================" << RESET << endl;
    if (errores == 0) {
        cout << VERDE << "REPORTE FINAL: No se encontraron inconsistencias." << RESET << endl;
        cout << VERDE << "La integridad referencial es óptima." << RESET << endl;
    } else {
        cout << ROJO << "REPORTE FINAL: Se encontraron " << errores << " inconsistencias." << RESET << endl;
        cout << AMARILLO << "Se recomienda revisar y corregir los errores detectados." << RESET << endl;
    }
    cout << AMARILLO << "========================================" << RESET << endl;
}
