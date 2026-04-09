#include "operacionesTransacciones.hpp"
#include "../interfaz/Interfaz.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include "../persistencia/Constantes.hpp"
#include "../utilidades/Validaciones.hpp"
#include "../utilidades/Formatos.hpp"
#include "Transaccion.hpp"
#include "DetalleTransaccion.hpp"
#include "../productos/Producto.hpp"
#include "../proveedores/Proveedor.hpp"
#include "../cliente/Cliente.hpp"
#include "../tienda/Tienda.hpp"
#include <iostream>
#include <string>

using namespace std;

void registrarCompra(Tienda& tienda) {
    Interfaz interfaz;
    Formatos::imprimirSubtitulo("REGISTRAR COMPRA A PROVEEDOR");
    
    if (!interfaz.solicitarConfirmacion("¿Desea registrar una nueva compra?")) {
        return;
    }
    
    // Verificar que haya proveedores registrados
    ArchivoHeader headerProveedores = GestorArchivos::leerHeader(ARCHIVO_PROVEEDORES);
    if (headerProveedores.registrosActivos == 0) {
        Formatos::imprimirError("No hay proveedores registrados en el sistema");
        Formatos::pausar();
        return;
    }
    
    // Verificar que haya productos registrados
    ArchivoHeader headerProductos = GestorArchivos::leerHeader(ARCHIVO_PRODUCTOS);
    if (headerProductos.registrosActivos == 0) {
        Formatos::imprimirError("No hay productos registrados en el sistema");
        Formatos::pausar();
        return;
    }
    
    int proveedorId;
    if (!interfaz.solicitarEntero("Ingrese ID del proveedor", proveedorId)) {
        return;
    }
    
    int indiceProveedor = GestorArchivos::buscarPorId<Proveedor>(ARCHIVO_PROVEEDORES, proveedorId);
    if (indiceProveedor == -1) {
        string errorMsg = "El proveedor con ID " + to_string(proveedorId) + " no existe.";
        Formatos::imprimirError(errorMsg.c_str());
        Formatos::pausar();
        return;
    }
    
    Proveedor proveedor = GestorArchivos::obtenerRegistroPorIndice<Proveedor>(ARCHIVO_PROVEEDORES, indiceProveedor);
    
    // Carrito de compras temporal
    DetalleTransaccion carrito[100];
    int cantItems = 0;
    float totalCompra = 0.0f;
    char continuar;
    
    string infoMsg = "Proveedor seleccionado: " + string(proveedor.getNombre());
    Formatos::imprimirInformacion(infoMsg.c_str());
    
    do {
        int productoId, cantidad;
        float costo;
        
        // Validar existencia del producto
        if (!interfaz.solicitarEntero("Ingrese ID del producto", productoId)) {
            break;
        }
        
        int indiceProducto = GestorArchivos::buscarPorId<Producto>(ARCHIVO_PRODUCTOS, productoId);
        if (indiceProducto == -1) {
            Formatos::imprimirError("El producto no existe.");
            continue;
        }
        
        Producto producto = GestorArchivos::obtenerRegistroPorIndice<Producto>(ARCHIVO_PRODUCTOS, indiceProducto);
        
        if (!interfaz.solicitarEntero("Ingrese cantidad", cantidad)) {
            break;
        }
        
        if (!interfaz.solicitarFloat("Ingrese costo unitario", costo)) {
            break;
        }
        
        // Validar stock disponible
        if (cantidad > producto.getStock()) {
            string errorMsg = "Stock insuficiente. Disponible: " + to_string(producto.getStock());
            Formatos::imprimirError(errorMsg.c_str());
            Formatos::pausar();
            continue;
        }
        
        // Agregar al carrito
        carrito[cantItems].setIdProducto(productoId);
        carrito[cantItems].setCantidad(cantidad);
        carrito[cantItems].setPrecioUnitario(costo);
        
        totalCompra += cantidad * costo;
        cantItems++;
        
        Formatos::imprimirExito("Producto agregado al carrito.");
        
        if (cantItems < 100) {
            cout << "¿Desea agregar otro producto? (S/N): ";
            cin >> continuar;
        } else {
            cout << "Carrito lleno. No se pueden agregar más productos." << endl;
            break;
        }
        
    } while ((continuar == 's' || continuar == 'S') && cantItems < 100);
    
    if (cantItems == 0) {
        Formatos::imprimirError("Compra cancelada: No se agregaron productos al carrito.");
        Formatos::pausar();
        return;
    }
    
    // Mostrar resumen de la compra
    Formatos::imprimirSubtitulo("RESUMEN DE COMPRA");
    Formatos::imprimirEncabezadoTabla();
    
    for (int i = 0; i < cantItems; i++) {
        int indProd = GestorArchivos::buscarPorId<Producto>(ARCHIVO_PRODUCTOS, carrito[i].getIdProducto());
        Producto producto = GestorArchivos::obtenerRegistroPorIndice<Producto>(ARCHIVO_PRODUCTOS, indProd);
        
        cout << left << setw(20) << producto.getNombre() 
             << setw(8) << carrito[i].getCantidad() 
             << setw(12) << carrito[i].getPrecioUnitario() * carrito[i].getCantidad() << endl;
    }
    
    cout << "TOTAL DE COMPRA: $" << fixed << setprecision(2) << totalCompra << endl;
    
    if (interfaz.solicitarConfirmacion("¿Desea confirmar y guardar esta compra?")) {
        // Crear transacción principal
        Transaccion transaccion;
        transaccion.setId(tienda.generarSiguienteIdTransaccion());
        transaccion.setIdRelacionado(proveedorId);
        transaccion.setTipo("COMPRA");
        transaccion.setCantidadItemsDiferentes(cantItems);
        transaccion.setTotal(totalCompra);
        
        // Guardar transacción principal
        if (GestorArchivos::guardarNuevoRegistro<Transaccion>(ARCHIVO_TRANSACCIONES, transaccion)) {
            // Guardar detalles de la transacción
            for (int i = 0; i < cantItems; i++) {
                DetalleTransaccion detalle;
                detalle.setIdTransaccion(transaccion.getId());
                detalle.setIdProducto(carrito[i].getIdProducto());
                detalle.setCantidad(carrito[i].getCantidad());
                detalle.setPrecioUnitario(carrito[i].getPrecioUnitario());
                
                if (!GestorArchivos::guardarNuevoRegistro<DetalleTransaccion>(ARCHIVO_DETALLES, detalle)) {
                    Formatos::imprimirError("Error al guardar detalle de transacción");
                    continue;
                }
            }
            
            // Actualizar stock de productos
            for (int i = 0; i < cantItems; i++) {
                int indProd = GestorArchivos::buscarPorId<Producto>(ARCHIVO_PRODUCTOS, carrito[i].getIdProducto());
                Producto producto = GestorArchivos::obtenerRegistroPorIndice<Producto>(ARCHIVO_PRODUCTOS, indProd);
                producto.setStock(producto.getStock() - carrito[i].getCantidad());
                
                if (!GestorArchivos::actualizarRegistro<Producto>(ARCHIVO_PRODUCTOS, indProd, producto)) {
                    Formatos::imprimirError("Error al actualizar stock del producto");
                }
            }
            
            string successMsg = "Compra registrada correctamente. ID Transacción: " + to_string(transaccion.getId());
            Formatos::imprimirExito(successMsg.c_str());
        } else {
            Formatos::imprimirInformacion("Operación cancelada. No se guardó la compra.");
        }
    } else {
        Formatos::imprimirInformacion("Operación cancelada.");
    }
    
    Formatos::pausar();
}

void registrarVenta(Tienda& tienda) {
    Interfaz interfaz;
    Formatos::imprimirSubtitulo("REGISTRAR VENTA A CLIENTE");
    
    if (!interfaz.solicitarConfirmacion("¿Desea registrar una nueva venta?")) {
        return;
    }
    
    // Verificar que haya clientes registrados
    ArchivoHeader headerClientes = GestorArchivos::leerHeader(ARCHIVO_CLIENTES);
    if (headerClientes.registrosActivos == 0) {
        Formatos::imprimirError("No hay clientes registrados en el sistema");
        Formatos::pausar();
        return;
    }
    
    // Verificar que haya productos registrados
    ArchivoHeader headerProductos = GestorArchivos::leerHeader(ARCHIVO_PRODUCTOS);
    if (headerProductos.registrosActivos == 0) {
        Formatos::imprimirError("No hay productos registrados en el sistema");
        Formatos::pausar();
        return;
    }
    
    int clienteId;
    if (!interfaz.solicitarEntero("Ingrese ID del cliente", clienteId)) {
        return;
    }
    
    int indiceCliente = GestorArchivos::buscarPorId<Cliente>(ARCHIVO_CLIENTES, clienteId);
    if (indiceCliente == -1) {
        string errorMsg = "El cliente con ID " + to_string(clienteId) + " no existe.";
        Formatos::imprimirError(errorMsg.c_str());
        Formatos::pausar();
        return;
    }
    
    Cliente cliente = GestorArchivos::obtenerRegistroPorIndice<Cliente>(ARCHIVO_CLIENTES, indiceCliente);
    
    // Carrito de ventas temporal
    DetalleTransaccion carrito[100];
    int cantItems = 0;
    float totalVenta = 0.0f;
    char continuar;
    
    string infoMsg = "Cliente seleccionado: " + string(cliente.getNombre());
    Formatos::imprimirInformacion(infoMsg.c_str());
    
    do {
        int productoId, cantidad;
        float precio;
        
        // Validar existencia del producto
        if (!interfaz.solicitarEntero("Ingrese ID del producto", productoId)) {
            break;
        }
        
        int indiceProducto = GestorArchivos::buscarPorId<Producto>(ARCHIVO_PRODUCTOS, productoId);
        if (indiceProducto == -1) {
            Formatos::imprimirError("El producto no existe.");
            continue;
        }
        
        Producto producto = GestorArchivos::obtenerRegistroPorIndice<Producto>(ARCHIVO_PRODUCTOS, indiceProducto);
        
        if (!interfaz.solicitarEntero("Ingrese cantidad a vender", cantidad)) {
            break;
        }
        
        if (cantidad <= 0) {
            Formatos::imprimirError("La cantidad debe ser mayor a cero.");
            continue;
        }
        
        if (!interfaz.solicitarFloat("Ingrese precio unitario", precio)) {
            break;
        }
        
        // Validar stock disponible
        if (cantidad > producto.getStock()) {
            string errorMsg = "Stock insuficiente. Disponible: " + to_string(producto.getStock());
            Formatos::imprimirError(errorMsg.c_str());
            Formatos::pausar();
            continue;
        }
                
        // Agregar al carrito
        DetalleTransaccion nuevoDetalle;
        nuevoDetalle.setIdProducto(productoId);
        nuevoDetalle.setCantidad(cantidad);
        nuevoDetalle.setPrecioUnitario(precio);
        carrito[cantItems] = nuevoDetalle;
                
        totalVenta += cantidad * precio;
        cantItems++;
                
        Formatos::imprimirExito("Producto agregado al carrito.");
                
        
        if (cantItems < 100) {
            cout << "¿Desea agregar otro producto? (S/N): ";
            cin >> continuar;
        } else {
            cout << "Carrito lleno. No se pueden agregar más productos." << endl;
            break;
        }
        
    } while ((continuar == 's' || continuar == 'S') && cantItems < 100);
    
    if (cantItems == 0) {
        Formatos::imprimirError("Venta cancelada: No se agregaron productos al carrito.");
        Formatos::pausar();
        return;
    }
    
    // Mostrar resumen de la venta
    Formatos::imprimirSubtitulo("RESUMEN DE VENTA");
    Formatos::imprimirEncabezadoTabla();
    
    for (int i = 0; i < cantItems; i++) {
        int indProd = GestorArchivos::buscarPorId<Producto>(ARCHIVO_PRODUCTOS, carrito[i].getIdProducto());
        Producto producto = GestorArchivos::obtenerRegistroPorIndice<Producto>(ARCHIVO_PRODUCTOS, indProd);
        
        cout << left << setw(20) << producto.getNombre() 
             << setw(8) << carrito[i].getCantidad() 
             << setw(12) << carrito[i].getPrecioUnitario() * carrito[i].getCantidad() << endl;
    }
    
    cout << "TOTAL DE VENTA: $" << fixed << setprecision(2) << totalVenta << endl;
    
    if (interfaz.solicitarConfirmacion("¿Desea confirmar y guardar esta venta?")) {
        // Crear transacción principal
        Transaccion transaccion;
        transaccion.setId(tienda.generarSiguienteIdTransaccion());
        transaccion.setIdRelacionado(clienteId);
        transaccion.setTipo("VENTA");
        transaccion.setCantidadItemsDiferentes(cantItems);
        transaccion.setTotal(totalVenta);
        
        // Guardar transacción principal
        if (GestorArchivos::guardarNuevoRegistro<Transaccion>(ARCHIVO_TRANSACCIONES, transaccion)) {
            // Guardar detalles de la transacción
            for (int i = 0; i < cantItems; i++) {
                DetalleTransaccion detalle;
                detalle.setIdTransaccion(transaccion.getId());
                detalle.setIdProducto(carrito[i].getIdProducto());
                detalle.setCantidad(carrito[i].getCantidad());
                detalle.setPrecioUnitario(carrito[i].getPrecioUnitario());
                
                if (!GestorArchivos::guardarNuevoRegistro<DetalleTransaccion>(ARCHIVO_DETALLES, detalle)) {
                    Formatos::imprimirError("Error al guardar detalle de transacción");
                    continue;
                }
            }
            
            // Actualizar stock de productos
            for (int i = 0; i < cantItems; i++) {
                int indProd = GestorArchivos::buscarPorId<Producto>(ARCHIVO_PRODUCTOS, carrito[i].getIdProducto());
                Producto producto = GestorArchivos::obtenerRegistroPorIndice<Producto>(ARCHIVO_PRODUCTOS, indProd);
                producto.setStock(producto.getStock() - carrito[i].getCantidad());
                
                if (!GestorArchivos::actualizarRegistro<Producto>(ARCHIVO_PRODUCTOS, indProd, producto)) {
                    Formatos::imprimirError("Error al actualizar stock del producto");
                }
            }
            
            string successMsg = "Venta registrada correctamente. ID Transacción: " + to_string(transaccion.getId());
            Formatos::imprimirExito(successMsg.c_str());
        } else {
            Formatos::imprimirInformacion("Operación cancelada. No se guardó la venta.");
        }
    } else {
        Formatos::imprimirInformacion("Operación cancelada.");
    }
    
    Formatos::pausar();
}

void buscarTransacciones(Tienda& tienda) {
    Interfaz interfaz;
    ArchivoHeader header = GestorArchivos::leerHeader(ARCHIVO_TRANSACCIONES);
    if (header.registrosActivos == 0) {
        Formatos::imprimirAdvertencia("No hay transacciones registradas en el sistema");
        Formatos::pausar();
        return;
    }
    
    int opcion;
    do {
        Formatos::imprimirSubtitulo("BUSCAR TRANSACCIONES");
        cout << CYAN
                  << "1. Buscar por ID" << endl
                  << "2. Buscar por tipo (COMPRA/VENTA)" << endl
                  << "3. Buscar por fecha" << endl
                  << "4. Buscar por proveedor/cliente" << endl
                  << "0. Cancelar" << endl << RESET;
        
        if (!interfaz.solicitarEntero("Seleccione una opción", opcion)) {
            opcion = -1;
        }
        
        switch (opcion) {
            case 1: {
                int id;
                if (interfaz.solicitarEntero("Ingrese ID de la transacción", id)) {
                    int indice = GestorArchivos::buscarPorId<Transaccion>(ARCHIVO_TRANSACCIONES, id);
                    if (indice != -1) {
                        Transaccion transaccion = GestorArchivos::obtenerRegistroPorIndice<Transaccion>(ARCHIVO_TRANSACCIONES, indice);
                        Formatos::imprimirExito("Transacción encontrada:");
                        transaccion.mostrarInformacionCompleta();
                    } else {
                        Formatos::imprimirError("Transacción no encontrada.");
                    }
                }
                Formatos::pausar();
                break;
            }
            
            case 2: {
                char tipo[20];
                if (interfaz.solicitarTexto("Ingrese tipo (COMPRA/VENTA)", tipo, 20)) {
                    int numResultados = 0;
                    Transaccion* transacciones = GestorArchivos::buscarTransaccionesPorTipo(ARCHIVO_TRANSACCIONES, tipo, &numResultados);
                    
                    if (transacciones != nullptr && numResultados > 0) {
                        Formatos::imprimirExito("Transacciones encontradas:");
                        for (int i = 0; i < numResultados; i++) {
                            cout << "ID: " << transacciones[i].getId() 
                                 << " | Tipo: " << transacciones[i].getTipo()
                                 << " | Total: $" << transacciones[i].getTotal() << endl;
                        }
                        delete[] transacciones;
                    } else {
                        Formatos::imprimirAdvertencia("No se encontraron transacciones de ese tipo.");
                    }
                }
                Formatos::pausar();
                break;
            }
            
            case 3: {
                char fecha[11];
                if (interfaz.solicitarTexto("Ingrese fecha (DD/MM/AAAA)", fecha, 11)) {
                    int numResultados = 0;
                    Transaccion* transacciones = GestorArchivos::buscarTransaccionesPorFecha(ARCHIVO_TRANSACCIONES, fecha, &numResultados);
                    
                    if (transacciones != nullptr && numResultados > 0) {
                        Formatos::imprimirExito("Transacciones encontradas:");
                        for (int i = 0; i < numResultados; i++) {
                            cout << "ID: " << transacciones[i].getId()
                                 << " | Fecha: " << transacciones[i].getFechaRegistro()
                                 << " | Tipo: " << transacciones[i].getTipo()
                                 << " | Total: $" << transacciones[i].getTotal() << endl;
                        }
                        delete[] transacciones;
                    } else {
                        Formatos::imprimirAdvertencia("No se encontraron transacciones en esa fecha.");
                    }
                }
                Formatos::pausar();
                break;
            }
            
            case 4: {
                int idRelacionado;
                if (interfaz.solicitarEntero("Ingrese ID del proveedor o cliente", idRelacionado)) {
                    int numResultados = 0;
                    Transaccion* transacciones = GestorArchivos::buscarTransaccionesPorRelacionado(ARCHIVO_TRANSACCIONES, idRelacionado, &numResultados);
                    
                    if (transacciones != nullptr && numResultados > 0) {
                        Formatos::imprimirExito("Transacciones encontradas:");
                        for (int i = 0; i < numResultados; i++) {
                            cout << "ID: " << transacciones[i].getId()
                                 << " | Fecha: " << transacciones[i].getFechaRegistro()
                                 << " | Tipo: " << transacciones[i].getTipo()
                                 << " | Total: $" << transacciones[i].getTotal() << endl;
                        }
                        delete[] transacciones;
                    } else {
                        Formatos::imprimirAdvertencia("No se encontraron transacciones para esa entidad.");
                    }
                }
                Formatos::pausar();
                break;
            }
            
            case 0:
                break;
                
            default:
                Formatos::imprimirError("Opción no válida");
                Formatos::pausar();
                break;
        }
        
    } while (opcion != 0);
}

void listarTransacciones(Tienda& tienda) {
    ArchivoHeader header = GestorArchivos::leerHeader(ARCHIVO_TRANSACCIONES);
    if (header.registrosActivos == 0) {
        Formatos::imprimirAdvertencia("No hay transacciones registradas en el sistema");
        Formatos::pausar();
        return;
    }
    
    Formatos::imprimirSubtitulo("LISTADO DE TRANSACCIONES");
    Formatos::imprimirEncabezadoTabla();
    
    // Leer y mostrar todas las transacciones
    ifstream archivo(ARCHIVO_TRANSACCIONES, ios::binary);
    if (!archivo.is_open()) {
        Formatos::imprimirError("No se pudo abrir el archivo de transacciones");
        Formatos::pausar();
        return;
    }
    
    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    
    int count = 0;
    Transaccion transaccion;
    
    while (archivo.read(reinterpret_cast<char*>(&transaccion), sizeof(Transaccion)) && 
           count < header.cantidadRegistros) {
        if (!transaccion.isEliminado()) {
            transaccion.mostrarInformacionCompleta();
            count++;
        }
    }
    
    archivo.close();
    
    if (count == 0) {
        Formatos::imprimirAdvertencia("No hay transacciones activas para mostrar.");
    } else {
        cout << "Total de transacciones mostradas: " << count << endl;
    }
    
    Formatos::pausar();
}

void cancelarTransaccion(Tienda& tienda) {
    Interfaz interfaz;
    ArchivoHeader header = GestorArchivos::leerHeader(ARCHIVO_TRANSACCIONES);
    if (header.registrosActivos == 0) {
        Formatos::imprimirAdvertencia("No hay transacciones registradas en el sistema");
        Formatos::pausar();
        return;
    }
    
    int id;
    if (!interfaz.solicitarEntero("Ingrese ID de la transacción a cancelar", id)) {
        return;
    }
    
    int indice = GestorArchivos::buscarPorId<Transaccion>(ARCHIVO_TRANSACCIONES, id);
    if (indice == -1) {
        string errorMsg = "La transacción con ID " + to_string(id) + " no existe.";
        Formatos::imprimirError(errorMsg.c_str());
        Formatos::pausar();
        return;
    }
    
    Transaccion transaccion = GestorArchivos::obtenerRegistroPorIndice<Transaccion>(ARCHIVO_TRANSACCIONES, indice);
    
    Formatos::imprimirSubtitulo("DATOS DE LA TRANSACCIÓN A CANCELAR");
    transaccion.mostrarInformacionCompleta();
    
    if (interfaz.solicitarConfirmacion("¿Está seguro que desea cancelar esta transacción? Esta acción no se puede deshacer.")) {
        // Marcar como eliminada lógicamente
        if (GestorArchivos::eliminarRegistroLogico<Transaccion>(ARCHIVO_TRANSACCIONES, id)) {
            Formatos::imprimirExito("Transacción cancelada correctamente.");
        } else {
            Formatos::imprimirError("Error al cancelar la transacción.");
        }
    } else {
        Formatos::imprimirInformacion("Operación cancelada. No se guardaron cambios.");
    }
    
    Formatos::pausar();
}

void menuTransacciones(Tienda& tienda) {
    Interfaz interfaz;
    int opcion;
    
    do {
        Formatos::limpiarPantalla();
        Formatos::imprimirSubtitulo("MENÚ DE TRANSACCIONES");
        
        cout << CYAN
                  << "1. Registrar Compra" << endl
                  << "2. Registrar Venta" << endl
                  << "3. Buscar Transacciones" << endl
                  << "4. Listar Todas las Transacciones" << endl
                  << "5. Cancelar Transacción" << endl
                  << "0. Volver al menú principal" << endl << RESET;
        
        if (!interfaz.solicitarEntero("Seleccione una opción", opcion)) {
            opcion = -1;
        }
        
        switch (opcion) {
            case 1:
                registrarCompra(tienda);
                break;
            case 2:
                registrarVenta(tienda);
                break;
            case 3:
                buscarTransacciones(tienda);
                break;
            case 4:
                listarTransacciones(tienda);
                break;
            case 5:
                cancelarTransaccion(tienda);
                break;
            case 0:
                break;
            default:
                Formatos::imprimirError("Opción no válida");
                Formatos::pausar();
                break;
        }
        
    } while (opcion != 0);
}
