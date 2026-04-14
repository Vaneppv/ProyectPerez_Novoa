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

    // Clientes -> Valida ID transacción
    cout << "\n" << CYAN << "Verificando relaciones Clientes -> Transacciones..." << RESET << endl;
    ArchivoHeader headerClientes = GestorArchivos::leerHeader(ARCHIVO_CLIENTES);
    
    for (int i = 0; i < headerClientes.cantidadRegistros; i++) {
        Cliente cliente = GestorArchivos::obtenerRegistroPorIndice<Cliente>(ARCHIVO_CLIENTES, i);
        if (!cliente.isEliminado()) {
            // Obtener las transacciones del cliente (necesita método en clase Cliente)
            int* transaccionesIds = cliente.getTransaccionesIds();
            int cantidadTransacciones = cliente.getCantidadTransacciones();
            
            for (int j = 0; j < cantidadTransacciones; j++) {
                int idBusq = transaccionesIds[j];
                if (!GestorArchivos::existeEntidad<Transaccion>(ARCHIVO_TRANSACCIONES, idBusq)) {
                    cout << ROJO << "[ERROR] Cliente ID " << cliente.getId() << " refiere a Transacción ID " 
                         << idBusq << " inexistente." << RESET << endl;
                    errores++;
                } else {
                    // Valida que la transacción asociada sea una "Venta"
                    Transaccion transaccion = GestorArchivos::obtenerRegistroPorId<Transaccion>(ARCHIVO_TRANSACCIONES, idBusq);
                    if (strcmp(transaccion.getTipo(), "VENTA") != 0) {
                        cout << ROJO << "[ERROR] Cliente ID " << cliente.getId() << " tiene en su histórial la Transacción ID " 
                             << idBusq << " que NO es una VENTA." << RESET << endl;
                        errores++;
                    }
                }
            }
        }
    }

    // Proveedores -> Validar ID transacción(Compras) y ID Productos
    cout << "\n" << CYAN << "Verificando relaciones Proveedores -> Transacciones/Productos..." << RESET << endl;
    ArchivoHeader headerProveedores = GestorArchivos::leerHeader(ARCHIVO_PROVEEDORES);
    
    for (int i = 0; i < headerProveedores.cantidadRegistros; i++) {
        Proveedor proveedor = GestorArchivos::obtenerRegistroPorIndice<Proveedor>(ARCHIVO_PROVEEDORES, i);
        if (!proveedor.isEliminado()) {
            // Validación del historial de transacciones
            int* transaccionesIds = proveedor.getTransaccionesIds();
            int cantidadTransacciones = proveedor.getCantidadTransacciones();
            
            for (int j = 0; j < cantidadTransacciones; j++) {
                int idBusq = transaccionesIds[j];
                if (!GestorArchivos::existeEntidad<Transaccion>(ARCHIVO_TRANSACCIONES, idBusq)) {
                    cout << ROJO << "[ERROR] Proveedor ID " << proveedor.getId() << " refiere a Transacción ID " 
                         << idBusq << " inexistente." << RESET << endl;
                    errores++;
                } else {
                    // Valida que la transacción sea una COMPRA
                    Transaccion transaccion = GestorArchivos::obtenerRegistroPorId<Transaccion>(ARCHIVO_TRANSACCIONES, idBusq);
                    if (strcmp(transaccion.getTipo(), "COMPRA") != 0) {
                        cout << ROJO << "[ERROR] Proveedor ID " << proveedor.getId() << " tiene en su historial la Transacción ID " 
                             << idBusq << " que es una " << transaccion.getTipo() << " (deberia ser COMPRA)." << RESET << endl;
                        errores++;
                    }
                }
            }
            
            // Valida la lista de productos suministrados
            int* productosIds = proveedor.getProductosIds();
            int cantidadProductos = proveedor.getCantidadProductos();
            
            for (int k = 0; k < cantidadProductos; k++) {
                int idProdBusq = productosIds[k];
                if (!GestorArchivos::existeEntidad<Producto>(ARCHIVO_PRODUCTOS, idProdBusq)) {
                    cout << ROJO << "[ERROR] Proveedor ID " << proveedor.getId() << " ofrece el Producto ID " 
                         << idProdBusq << " que no existe en el inventario." << RESET << endl;
                    errores++;
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
