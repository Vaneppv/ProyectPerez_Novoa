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
#include "../clientes/Cliente.hpp"
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
            string infoMsg = "Indice de producto no encontrado: " + to_string(productoId);
            Formatos::imprimirError(infoMsg.c_str());
            continue;
        }
        
        Producto producto = GestorArchivos::obtenerRegistroPorIndice<Producto>(ARCHIVO_PRODUCTOS, indiceProducto);
        
        if (!interfaz.solicitarEntero("Ingrese cantidad", cantidad)) {
            break;
        }
        
        if (!interfaz.solicitarFloat("Ingrese costo unitario", costo)) {
            break;
        }
        
        // Agregar al carrito
        carrito[cantItems].setIdProducto(productoId);
        carrito[cantItems].setCantidad(cantidad);
        carrito[cantItems].setPrecioUnitario(costo);
        
        totalCompra += cantidad * costo;
        cantItems++;
        
        string infoMsg = "Producto agregado al carrito. ID: " + to_string(productoId);
        Formatos::imprimirExito(infoMsg.c_str());
        
        if (cantItems < 100) {
            cout << "¿Desea agregar otro producto? (S/N): ";
            cin >> continuar;
        } else {
            cout << "Carrito lleno. No se pueden agregar más productos." << endl;
            break;
        }
        
    } while ((continuar == 's' || continuar == 'S') && cantItems < 100);
    
    if (cantItems == 0) {
        string infoMsg = "Compra cancelada: No se agregaron productos al carrito.";
        Formatos::imprimirError(infoMsg.c_str());
        Formatos::pausar();
        return;
    }
    
    // Mostrar resumen de la compra
    Formatos::imprimirSubtitulo("RESUMEN DE COMPRA");
    cout << CYAN << left << setw(20) << "PRODUCTO" << setw(8) << "CANT." << setw(12) << "SUBTOTAL" << endl;
    
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
                    string errorMsg = "Error al guardar detalle de transacción";
                    Formatos::imprimirError(errorMsg.c_str());
                    continue;
                }
            }
            
            // Actualizar stock de productos
            for (int i = 0; i < cantItems; i++) {
                int indProd = GestorArchivos::buscarPorId<Producto>(ARCHIVO_PRODUCTOS, carrito[i].getIdProducto());
                Producto producto = GestorArchivos::obtenerRegistroPorIndice<Producto>(ARCHIVO_PRODUCTOS, indProd);
                producto.setStock(producto.getStock() + carrito[i].getCantidad());
                
                if (!GestorArchivos::actualizarRegistro<Producto>(ARCHIVO_PRODUCTOS, indProd, producto)) {
                    string errorMsg = "Error al actualizar stock del producto";
                    Formatos::imprimirError(errorMsg.c_str());
                }
            }

            Proveedor prov = GestorArchivos::obtenerRegistroPorIndice<Proveedor>(ARCHIVO_PROVEEDORES, indiceProveedor);
            prov.setTotalCompras(prov.getTotalcompras() + totalCompra);       // Acumula el gasto/inversion
            
            // Note: The getter methods in Proveedor class need to be fixed to return proper arrays
            // For now, we'll work around this by using the setter methods directly
            if (prov.getCantidadTransacciones() < 100){
                // Add transaction ID to provider's transaction list
                // This requires proper getter implementation in Proveedor class
                prov.setCantidadTransacciones(prov.getCantidadTransacciones() + 1);
            }

            for (int i = 0; i < cantItems; i++) {
                int idProdActual = carrito[i].getIdProducto();
                bool yaExiste = false;

                // Verifica si el producto ya estaba en la lista del proveedor 
                // This requires proper getter implementation in Proveedor class
                for (int j = 0; j < prov.getCantidadproductos(); j++) {
                    // This requires proper getter implementation in Proveedor class
                    if (false) { // Placeholder until getters are fixed
                        yaExiste = true;
                        break;
                    }
                }
                // Si no esta en la lista del proveedor, se agrega
                if (!yaExiste && prov.getCantidadproductos() < 100) {
                    prov.setCantidadProductos(prov.getCantidadproductos() + 1);
                }
            }
            
            GestorArchivos::actualizarRegistro<Proveedor>(ARCHIVO_PROVEEDORES, indiceProveedor, prov);

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
            string errorMsg = "El producto con ID " + to_string(productoId) + " no existe.";
            Formatos::imprimirError(errorMsg.c_str());
            continue;
        }
        
        Producto producto = GestorArchivos::obtenerRegistroPorIndice<Producto>(ARCHIVO_PRODUCTOS, indiceProducto);
        
        if (!interfaz.solicitarEntero("Ingrese cantidad a vender", cantidad)) {
            break;
        }
        
        if (cantidad <= 0) {
            string errorMsg = "La cantidad debe ser mayor a cero.";
            Formatos::imprimirError(errorMsg.c_str());
            continue;
        }

        // Validar stock disponible
        if (cantidad > producto.getStock()) {
            string errorMsg = "Stock insuficiente. Disponible: " + to_string(producto.getStock());
            Formatos::imprimirError(errorMsg.c_str());
            Formatos::pausar();
            continue;
        }
        
        if (!interfaz.solicitarFloat("Ingrese precio unitario", precio)) {
            break;
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
        string errorMsg = "Venta cancelada: No se agregaron productos al carrito.";
        Formatos::imprimirError(errorMsg.c_str());
        Formatos::pausar();
        return;
    }
    
    // Mostrar resumen de la venta
    Formatos::imprimirSubtitulo("RESUMEN DE VENTA");
    cout << CYAN << left << setw(20) << "PRODUCTO" << setw(8) << "CANT." << setw(12) << "SUBTOTAL" << endl;
    
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

        char descripcion[MAX_DESCRIPCION];
        interfaz.solicitarTexto("Ingrese descripcion de la compra:", descripcion, sizeof(descripcion));
        
        Transaccion transaccion;
        transaccion.setId(tienda.generarSiguienteIdTransaccion());
        transaccion.setIdRelacionado(clienteId);
        transaccion.setTipo("VENTA");
        transaccion.setCantidadItemsDiferentes(cantItems);
        transaccion.setTotal(totalVenta);
        transaccion.setDescripcion(descripcion);
        
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
                    string errorMsg = "Error al guardar detalle de transacción";
                    Formatos::imprimirError(errorMsg.c_str());
                    continue;
                }
            }
            
            // Actualizar stock de productos
            for (int i = 0; i < cantItems; i++) {
                int indProd = GestorArchivos::buscarPorId<Producto>(ARCHIVO_PRODUCTOS, carrito[i].getIdProducto());
                Producto producto = GestorArchivos::obtenerRegistroPorIndice<Producto>(ARCHIVO_PRODUCTOS, indProd);
                producto.setStock(producto.getStock() - carrito[i].getCantidad());
                
                if (!GestorArchivos::actualizarRegistro<Producto>(ARCHIVO_PRODUCTOS, indProd, producto)) {
                    string errorMsg = "Error al actualizar stock del producto";
                    Formatos::imprimirError(errorMsg.c_str());
                }
            }
            
            Cliente client = GestorArchivos::obtenerRegistroPorIndice<Cliente>(ARCHIVO_CLIENTES, indiceCliente);
            client.setTotalCompras(client.getTotalcompras() + totalVenta);      // Sumar gasto del cliente (Venta)

            if (client.getCantidadTransacciones() < 100){
                // Add transaction ID to client's transaction list
                // This requires proper getter implementation in Cliente class
                client.setCantidadTransacciones(client.getCantidadTransacciones() + 1);
            }
            GestorArchivos::actualizarRegistro<Cliente>(ARCHIVO_CLIENTES, indiceCliente, client);

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
                  << "1. Buscar por ID de Transacción" << endl 
                  << "2. Buscar por ID de Producto" << endl
                  << "3. Buscar por tipo (COMPRA/VENTA)" << endl
                  << "4. Buscar por fecha" << endl
                  << "5. Buscar por ID de Proveedor/Cliente" << endl
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
                        Formatos::EncabezadoCompletoTransacciones();
                        transaccion.mostrarInformacionCompleta();
                        DetalleTransaccion* detalles = GestorArchivos::obtenerDetallesPorTransaccion(ARCHIVO_DETALLES, transaccion.getId(), nullptr);
                        if (detalles != nullptr) {
                            Formatos::imprimirInformacion("Detalles de la transacción:");
                            Formatos::EncabezadoDetalles();
                            for (int i = 0; i < transaccion.getCantidadItemsDiferentes(); i++) {
                                detalles[i].mostrarInformacion();
                            }
                            delete[] detalles;
                        }
                    } else {
                        Formatos::imprimirError("Transacción no encontrada.");
                    }
                }
                Formatos::pausar();
                break;
            }
            
            case 2: {
                int id;
                if (interfaz.solicitarEntero("Ingrese ID del producto", id)) {
                    int indice = GestorArchivos::buscarPorId<Producto>(ARCHIVO_PRODUCTOS, id);
                    if (indice == -1) {
                        Formatos::imprimirError("Producto no encontrado");
                    } else {
                        // Buscar todos los detalles de transacción para este producto
                        int idsTransacciones[100];
                        int cantidadTransacciones = 0;
                        
                        ifstream archivoDetalles(ARCHIVO_DETALLES, ios::binary);
                        if (archivoDetalles.is_open()) {
                            archivoDetalles.seekg(sizeof(ArchivoHeader), ios::beg);
                            DetalleTransaccion detalle;
                            
                            while (archivoDetalles.read(reinterpret_cast<char*>(&detalle), sizeof(DetalleTransaccion)) && cantidadTransacciones < 100) {
                                if (!detalle.isEliminado() && detalle.getIdProducto() == id) {
                                    idsTransacciones[cantidadTransacciones] = detalle.getIdTransaccion();
                                    cantidadTransacciones++;
                                }
                            }
                            archivoDetalles.close();
                        }
                        
                        if (cantidadTransacciones == 0) {
                            Formatos::imprimirAdvertencia("No se encontraron transacciones para este producto");
                        } else {
                            Formatos::imprimirExito("Transacciones encontradas para el producto:");
                            Formatos::EncabezadoBasicoTransacciones();
                            // Mostrar información de cada transacción encontrada
                            for (int i = 0; i < cantidadTransacciones; i++) {
                                int indiceTrans = GestorArchivos::buscarPorId<Transaccion>(ARCHIVO_TRANSACCIONES, idsTransacciones[i]);
                                if (indiceTrans != -1) {
                                    Transaccion transaccion = GestorArchivos::obtenerRegistroPorIndice<Transaccion>(ARCHIVO_TRANSACCIONES, indiceTrans);
                                    transaccion.mostrarInformacionBasica();
                                }
                            }
                        }
                    }
                }
                Formatos::pausar();
                break;
            }

            case 3: {
                char tipo[20];
                if (interfaz.solicitarTexto("Ingrese tipo (COMPRA/VENTA)", tipo, 20)) {
                    int numResultados = 0;
                    Transaccion* transacciones = GestorArchivos::buscarTransaccionesPorTipo(ARCHIVO_TRANSACCIONES, tipo, &numResultados);
                    
                    if (transacciones != nullptr && numResultados > 0) {
                        Formatos::imprimirExito("Transacciones encontradas:");
                        Formatos::EncabezadoBasicoTransacciones();
                        for (int i = 0; i < numResultados; i++) {
                            transacciones[i].mostrarInformacionBasica();
                        }
                        delete[] transacciones;
                    } else {
                        Formatos::imprimirAdvertencia("No se encontraron transacciones de ese tipo.");
                    }
                }
                Formatos::pausar();
                break;
            }
            
            case 4: {
                char fecha[11];
                if (interfaz.solicitarTexto("Ingrese fecha (DD/MM/AAAA)", fecha, 11)) {
                    int numResultados = 0;
                    Transaccion* transacciones = GestorArchivos::buscarTransaccionesPorFecha(ARCHIVO_TRANSACCIONES, fecha, &numResultados);
                    
                    if (transacciones != nullptr && numResultados > 0) {
                        Formatos::imprimirExito("Transacciones encontradas:");
                        for (int i = 0; i < numResultados; i++) {
                            transacciones[i].mostrarInformacionBasica();
                        }
                        delete[] transacciones;
                    } else {
                        Formatos::imprimirAdvertencia("No se encontraron transacciones en esa fecha.");
                    }
                }
                Formatos::pausar();
                break;
            }
            
            case 5: {
                int idRelacionado;
                if (interfaz.solicitarEntero("Ingrese ID del proveedor o cliente", idRelacionado)) {
                    int numResultados = 0;
                    Transaccion* transacciones = GestorArchivos::buscarTransaccionesPorRelacionado(ARCHIVO_TRANSACCIONES, idRelacionado, &numResultados);
                    
                    if (transacciones != nullptr && numResultados > 0) {
                        Formatos::imprimirExito("Transacciones encontradas:");
                        Formatos::EncabezadoBasicoTransacciones();
                        for (int i = 0; i < numResultados; i++) {
                            transacciones[i].mostrarInformacionBasica();
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
    Formatos::EncabezadoCompletoTransacciones();
    
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
            
            // Mostrar detalles de la transacción
            int numDetalles = 0;
            DetalleTransaccion* detalles = GestorArchivos::obtenerDetallesPorTransaccion(ARCHIVO_DETALLES, transaccion.getId(), &numDetalles);
            if (detalles != nullptr && numDetalles > 0) {
                Formatos::imprimirInformacion("Detalles de la transacción:");
                Formatos::EncabezadoDetalles();
                for (int i = 0; i < numDetalles; i++) {
                    detalles[i].mostrarInformacion();
                }
                delete[] detalles;
            }
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
    Formatos::EncabezadoCompletoTransacciones();
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
        Formatos::imprimirTitulo("MENÚ DE TRANSACCIONES");
        
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
