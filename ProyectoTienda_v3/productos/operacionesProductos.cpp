#include "../productos/operacionesProductos.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include "../persistencia/Constantes.hpp"
#include "../utilidades/Validaciones.hpp"
#include "../utilidades/Formatos.hpp"
#include "../interfaz/Interfaz.hpp"
#include "Producto.hpp"
#include "../proveedores/Proveedor.hpp"
#include <iostream>
#include <string>

using namespace std;

// Función auxiliar
bool productoTieneTransacciones(int idProducto) {
    
    ArchivoHeader headerDetalles = GestorArchivos::leerHeader(ARCHIVO_DETALLES);
    
    if (headerDetalles.registrosActivos == 0) {
        return false; // No hay detalles, por lo tanto no hay transacciones
    }
    
    ifstream archivo(ARCHIVO_DETALLES, ios::binary);
    if (!archivo.is_open()) return false;
    
    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    
    DetalleTransaccion detalle;
    int count = 0;
    
    while (archivo.read(reinterpret_cast<char*>(&detalle), sizeof(DetalleTransaccion)) && 
           count < headerDetalles.cantidadRegistros) {
        if (detalle.getIdProducto() == idProducto) {
            archivo.close();
            return true; // Encontró un detalle con este producto
        }
        count++;
    }
    
    archivo.close();
    return false; // No hay detalles
}


void registrarProducto(Tienda& tienda) {
    Formatos::imprimirSubtitulo("REGISTRAR NUEVO PRODUCTO");
    
    Interfaz interfaz;
    if (!interfaz.solicitarConfirmacion("¿Desea registrar un nuevo producto?")) {
        return;
    }
    
    // Verificar que existan proveedores
    ArchivoHeader headerProveedores = GestorArchivos::leerHeader(ARCHIVO_PROVEEDORES);
    if (headerProveedores.registrosActivos == 0) {
        Formatos::imprimirError("Debe registrar primero al menos un proveedor");
        Formatos::pausar();
        return;
    }
    
    Producto nuevoProducto;
    char buffer[200];
    int valorEntero;
    float valorFloat;
    
    // Solicitar datos del producto
    if (!interfaz.solicitarTexto("Ingrese nombre del producto", buffer, MAX_NOMBRE)) return;
    nuevoProducto.setNombre(buffer);
    
    if (!interfaz.solicitarTexto("Ingrese código del producto", buffer, MAX_CODIGO)) return;
    
    // Validar que el código no exista
    if (GestorArchivos::existeEntidad<Producto>(ARCHIVO_PRODUCTOS, atoi(buffer))) {
        Formatos::imprimirError("El código ya existe");
        Formatos::pausar();
        return;
    }
    nuevoProducto.setCodigo(buffer);
    
    if (!interfaz.solicitarTexto("Ingrese descripción del producto", buffer, MAX_DESCRIPCION)) return;
    nuevoProducto.setDescripcion(buffer);
    
    if (!interfaz.solicitarFloat("Ingrese precio del producto", valorFloat)) return;
    nuevoProducto.setPrecio(valorFloat);
    
    if (!interfaz.solicitarEntero("Ingrese stock inicial", valorEntero)) return;
    nuevoProducto.setStock(valorEntero);
    
    if (!interfaz.solicitarEntero("Ingrese stock mínimo de seguridad", valorEntero)) return;
    nuevoProducto.setStockMinimo(valorEntero);
    
    if (!interfaz.solicitarEntero("Ingrese ID del proveedor", valorEntero)) return;
    if (!GestorArchivos::existeEntidad<Proveedor>(ARCHIVO_PROVEEDORES, valorEntero)) {
        Formatos::imprimirError("El proveedor no existe");
        Formatos::pausar();
        return;
    }
    nuevoProducto.setIdProveedor(valorEntero);
    
    // Mostrar resumen
    Formatos::imprimirSubtitulo("RESUMEN DEL PRODUCTO");
    Formatos::EncabezadoBasicoProducto();
    nuevoProducto.mostrarInformacionBasica();
    
    if (interfaz.solicitarConfirmacion("¿Desea guardar este producto?")) {
        if (GestorArchivos::guardarNuevoRegistro<Producto>(ARCHIVO_PRODUCTOS, nuevoProducto)) {
            Formatos::imprimirExito("Producto guardado correctamente");
            cout << "ID asignado: " << nuevoProducto.getId() << endl;
        } else {
            Formatos::imprimirError("Error al guardar el producto");
        }
    } else {
        Formatos::imprimirInformacion("Operación cancelada");
    }
    
    Formatos::pausar();
}

void buscarProducto(Tienda& tienda) {
    Interfaz interfaz;
    ArchivoHeader header = GestorArchivos::leerHeader(ARCHIVO_PRODUCTOS);
    if (header.registrosActivos == 0) {
        Formatos::imprimirAdvertencia("No hay productos registrados en el sistema");
        Formatos::pausar();
        return;
    }
    
    int opcion;
    do {
        Formatos::imprimirSubtitulo("BUSCAR PRODUCTO");
        cout << CYAN
                  << "1. Buscar por ID" << endl
                  << "2. Buscar por nombre (coincidencia parcial)" << endl
                  << "3. Buscar por código (coincidencia parcial)" << endl
                  << "4. Listar por proveedor" << endl
                  << "0. Cancelar" << endl << RESET;
        
        if (!interfaz.solicitarEntero("Seleccione una opción", opcion)) {
            opcion = -1;
        }
        
        switch (opcion) {
            case 1: {
                int id;
                if (interfaz.solicitarEntero("Ingrese el ID del producto", id)) {
                    int indice = GestorArchivos::buscarPorId<Producto>(ARCHIVO_PRODUCTOS, id);
                    if (indice != -1) {
                        Producto producto = GestorArchivos::obtenerRegistroPorIndice<Producto>(ARCHIVO_PRODUCTOS, indice);
                        Formatos::imprimirExito("Producto encontrado:");
                        Formatos::EncabezadoCompletoProducto();
                        producto.mostrarInformacionCompleta();
                    } else {
                        Formatos::imprimirError("Producto no encontrado");
                    }
                }
                Formatos::pausar();
                break;
            }
            
            case 2: {
                char nombre[100];
                if (interfaz.solicitarTexto("Ingrese el nombre (o parte) a buscar", nombre, MAX_NOMBRE)) {
                    int numResultados = 0;
                    int* indices = GestorArchivos::buscarRegistroPorNombre<Producto>(ARCHIVO_PRODUCTOS, nombre, &numResultados);
                    
                    if (indices != nullptr) {
                        Formatos::imprimirExito("Se encontraron coincidencias:");
                        Formatos::EncabezadoBasicoProducto();
                        for (int i = 0; i < numResultados; i++) {
                            Producto producto = GestorArchivos::obtenerRegistroPorIndice<Producto>(ARCHIVO_PRODUCTOS, indices[i]);
                            producto.mostrarInformacionBasica();
                        }
                        delete[] indices;
                    } else {
                        Formatos::imprimirAdvertencia("No se encontraron coincidencias");
                    }
                }
                Formatos::pausar();
                break;
            }
            
            case 3: {
                char codigo[20];
                if (interfaz.solicitarTexto("Ingrese el código (o parte) a buscar", codigo, MAX_CODIGO)) {
                    int numResultados = 0;
                    int* indices = GestorArchivos::buscarRegistroPorNombre<Producto>(ARCHIVO_PRODUCTOS, codigo, &numResultados);
                    if (indices != nullptr) {
                        Formatos::imprimirExito("Se encontraron coincidencias:");
                        Formatos::EncabezadoBasicoProducto();
                        for (int i = 0; i < numResultados; i++) {
                            Producto producto = GestorArchivos::obtenerRegistroPorIndice<Producto>(ARCHIVO_PRODUCTOS, indices[i]);
                            producto.mostrarInformacionBasica();
                        }
                        delete[] indices;
                    } else {
                        Formatos::imprimirAdvertencia("No se encontraron coincidencias");
                    }
                }
                Formatos::pausar();
                break;
            }
            
            case 4: {
                int idProveedor;
                if (interfaz.solicitarEntero("Ingrese ID del proveedor", idProveedor)) {
                    listarPorProveedor(tienda, idProveedor);
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

void actualizarProducto(Tienda& tienda) {
    Interfaz interfaz;
    ArchivoHeader header = GestorArchivos::leerHeader(ARCHIVO_PRODUCTOS);
    if (header.registrosActivos == 0){
        Formatos::imprimirAdvertencia("No hay productos en el Sistema.");
        return;
    }
    
    int idProducto;
    if (!interfaz.solicitarEntero("Ingrese el ID del producto a buscar", idProducto)) return;

    int indice = GestorArchivos::buscarPorId<Producto>(ARCHIVO_PRODUCTOS, idProducto);
    if (indice == -1) {
        string errorMsg = "El producto con ID " + to_string(idProducto) + " no existe.";
        Formatos::imprimirError(errorMsg.c_str());
        return;
    }
    
    Producto producto = GestorArchivos::obtenerRegistroPorIndice<Producto>(ARCHIVO_PRODUCTOS, indice);

    Formatos::imprimirSubtitulo("DATOS ACTUALES DEL PRODUCTO:");
    Formatos::EncabezadoBasicoProducto();
    producto.mostrarInformacionBasica();
    Formatos::imprimirSeparador();

    char nombre[100], codigo[20], descripcion[200];
    float precio;
    int stock, idProveedor, stockMinimo;
    int seleccion; 

    bool verNombre = false, verCodigo = false, verDescrip = false, verPrecio = false, verStock = false, verIdProveedor = false;
    
    // Menú de campos editables
    do {
        Formatos::imprimirSubtitulo("¿Qué desea editar?");
        cout << "1. Código" << endl
             << "2. Nombre" << endl
             << "3. Descripción" << endl
             << "4. Proveedor" << endl
             << "5. Precio" << endl
             << "6. Stock" << endl
             << "7. Stock Mínimo (Alerta)" << endl
             << "8. Guardar cambios" << endl
             << "0. Cancelar sin guardar" << endl << RESET;
        
        if (!interfaz.solicitarEntero("Seleccione una opción", seleccion)) continue;

        switch (seleccion) {
            case 1: {
                if (interfaz.solicitarTexto("Ingrese nuevo código", codigo, MAX_CODIGO)) {
                    if (!Validaciones::validarCodigoProducto(codigo)) {
                        Formatos::imprimirError("El código debe tener el formato correcto (Ej: PROD001)");
                        break;
                    } else {
                        producto.setCodigo(codigo);
                        verCodigo = true;
                    }
                }
                break;
            }
            case 2: {
                if (interfaz.solicitarTexto("Ingrese nuevo nombre", nombre, MAX_NOMBRE)){
                    if (!Validaciones::validarNombre(nombre)) {
                        Formatos::imprimirError("El nombre debe tener al menos 3 caracteres");
                        break;
                    } else {
                        producto.setNombre(nombre);
                        verNombre = true;
                    }
                }
                break;
            }
            case 3: {
                if (interfaz.solicitarTexto("Ingrese nueva descripción", descripcion, MAX_DESCRIPCION)) {
                    if (!Validaciones::validarDescripcion(descripcion)) {
                        Formatos::imprimirError("La descripción debe tener al menos 10 caracteres");
                        break;
                    } else {
                        producto.setDescripcion(descripcion);
                        verDescrip = true;
                    }
                }
                break;
            }
            case 4: {
                if (interfaz.solicitarEntero("Ingrese ID del nuevo proveedor", idProveedor)) {
                    if (!Validaciones::validarRango(idProveedor, 1, 1000)) {
                        Formatos::imprimirError("El ID del proveedor debe estar entre 1 y 1000");
                        break;
                    } else if (GestorArchivos::existeEntidad<Proveedor>(ARCHIVO_PROVEEDORES, idProveedor)) {
                        producto.setIdProveedor(idProveedor);
                        verIdProveedor = true;
                    } else {
                        Formatos::imprimirError("El proveedor no existe.");
                    }
                }
                break;
            }
            case 5: {
                if (interfaz.solicitarFloat("Ingrese nuevo precio", precio)) {
                    if (!Validaciones::validarRangoFloat(precio, 0.01, 999999.99)) {
                        Formatos::imprimirError("El precio debe estar entre 0.01 y 999999.99");
                        break;
                    } else {
                        producto.setPrecio(precio);
                        verPrecio = true;
                    }
                }
                break;
            }
            case 6: {
                if (interfaz.solicitarEntero("Ingrese nuevo stock", stock)) {
                    if (!Validaciones::validarRango(stock, 0, 999999)) {
                        Formatos::imprimirError("El stock debe estar entre 0 y 999999");
                        break;
                    } else {
                        producto.setStock(stock);
                        verStock = true;
                    }
                }
                break;
            }
            case 7: {
                if (interfaz.solicitarEntero("Ingrese nuevo stock minimo", stockMinimo)) {
                    if (!Validaciones::validarRango(stockMinimo, 0, 999999)) {
                        Formatos::imprimirError("El stock minimo debe estar entre 0 y 999999");
                        break;
                    } else {
                        producto.setStockMinimo(stockMinimo);
                    }
                }
                break;
            }
            case 8: {
                if (!verNombre && !verCodigo && !verDescrip && !verPrecio && !verStock && !verIdProveedor) {
                    Formatos::imprimirAdvertencia("No se han realizado cambios para guardar.");
                } else {
                    if (interfaz.solicitarConfirmacion("¿Desea guardar los cambios hechos al producto?")) {
                        if (GestorArchivos::actualizarRegistro<Producto>(ARCHIVO_PRODUCTOS, indice, producto)){
                            Formatos::imprimirExito("¡Cambios aplicados exitosamente!");
                        } else {
                            Formatos::imprimirError("Error al intentar escribir en el archivo.");
                        }
                    } 
                }
                seleccion = 0; 
                break;
            }
            case 0: {
                Formatos::imprimirInformacion("Operación cancelada. No se guardaron cambios.");
                break;
            }
            default: {
                Formatos::imprimirError("Opción no válida.");
                break;
            }
        }
    } while (seleccion != 0);
}

void actualizarStockProducto(/* Tienda& tienda */) {
    Interfaz interfaz;
    ArchivoHeader header = GestorArchivos::leerHeader(ARCHIVO_PRODUCTOS);
    if (header.registrosActivos == 0){
        Formatos::imprimirAdvertencia("No hay productos en el Sistema.");
        return;
    }
    
    int idProducto;
    if (!interfaz.solicitarEntero("Ingrese el ID del producto a buscar", idProducto)) return;

    int indice = GestorArchivos::buscarPorId<Producto>(ARCHIVO_PRODUCTOS, idProducto);
    if (indice == -1) {
        string errorMsg = "El producto con ID " + to_string(idProducto) + " no existe.";
        Formatos::imprimirError(errorMsg.c_str());
        return;
    }
    
    Producto producto = GestorArchivos::obtenerRegistroPorIndice<Producto>(ARCHIVO_PRODUCTOS, indice);
    
    // Menú de campos editables
    int seleccion;
    do {
        Formatos::imprimirTitulo("Actualización de Stock");
        string infoMsg = "Producto: " + string(producto.getNombre());
        Formatos::imprimirInformacion(infoMsg.c_str());
        string stockMsg = "Stock actual: " + to_string(producto.getStock());
        Formatos::imprimirInformacion(stockMsg.c_str());
        Formatos::imprimirSubtitulo("¿Qué desea editar?");
        cout << CYAN << "1. Agregar unidades" << endl;
        cout << "2. Quitar unidades" << endl;
        cout << "3. Guardar cambios" << endl;
        cout << "0. Cancelar sin guardar" << endl << RESET;
        if (!interfaz.solicitarEntero("Seleccione una opción", seleccion)) {
            return;
        }

        switch (seleccion) {
            case 1: { // Quitar unidades
                int cantidad;
                if (interfaz.solicitarEntero("¿Cuantas unidades ingresan?", cantidad)) {
                    if (Validaciones::validarRango(cantidad, 1, 999999)) {
                        producto.setStock(producto.getStock() + cantidad);
                        Formatos::imprimirExito("Cantidad añadida al stock.");
                    } else {
                        Formatos::imprimirError("La cantidad debe ser positiva.");
                    }
                }
                break;
            }
            case 2: { // Agregar unidades
                int cantidad;
                if (interfaz.solicitarEntero("¿Cuantas unidades se quitan?", cantidad)) {
                    if (Validaciones::validarRango(cantidad, 1, 999999) && cantidad <= producto.getStock()) {
                        producto.setStock(producto.getStock() - cantidad);
                        Formatos::imprimirExito("Cantidad restada del stock.");
                    } else {
                        Formatos::imprimirError("La cantidad debe ser positiva y no mayor al stock actual.");
                    }
                }
                break;
            }
            case 3: { // Guardar cambios
                if (GestorArchivos::actualizarRegistro<Producto>(ARCHIVO_PRODUCTOS, indice, producto)) {
                    Formatos::imprimirExito("Stock actualizado correctamente.");
                } else {
                    Formatos::imprimirError("Error al actualizar el stock.");
                }
                break;
            }
            case 0: { // Cancelar sin guardar
                Formatos::imprimirInformacion("Operación cancelada. No se guardaron cambios.");
                break;
            }
            default: { // Opción no válida
                Formatos::imprimirError("Opción no válida.");
                break;
            }
        }
    }while(seleccion != 0);
    Formatos::pausar();
}

void listarProductos(/* Tienda& tienda */) {
    ArchivoHeader header = GestorArchivos::leerHeader(ARCHIVO_PRODUCTOS);
    if (header.registrosActivos == 0) {
        Formatos::imprimirAdvertencia("No hay productos registrados en el sistema");
        Formatos::pausar();
        return;
    }
    
    Formatos::imprimirSubtitulo("LISTADO DE PRODUCTOS");
    Formatos::EncabezadoCompletoProducto();
    
    ifstream archivo(ARCHIVO_PRODUCTOS, ios::binary);
    if (archivo.is_open()) {
        archivo.seekg(sizeof(ArchivoHeader), ios::beg);
        
        Producto producto;
        int count = 0;
        while (archivo.read(reinterpret_cast<char*>(&producto), sizeof(Producto)) && count < header.cantidadRegistros) {
            if (!producto.isEliminado()) {
                producto.mostrarInformacionCompleta();
            }
            count++;
        }
        archivo.close();
    }
    
    Formatos::imprimirPieTabla();
    Formatos::pausar();
}

void eliminarProducto(/* Tienda& tienda */) {
    Interfaz interfaz;
    ArchivoHeader header = GestorArchivos::leerHeader(ARCHIVO_PRODUCTOS);
    if (header.registrosActivos == 0){
        Formatos::imprimirAdvertencia("No hay productos en el Sistema.");
        return;
    }

    int idProducto;
    if (!interfaz.solicitarEntero("Ingrese el ID del producto a buscar", idProducto)) return;

    int indice = GestorArchivos::buscarPorId<Producto>(ARCHIVO_PRODUCTOS, idProducto);
    if (indice == -1) {
        string errorMsg = "El producto con ID " + to_string(idProducto) + " no existe.";
        Formatos::imprimirError(errorMsg.c_str());
        return;
    }
    
    Producto producto = GestorArchivos::obtenerRegistroPorIndice<Producto>(ARCHIVO_PRODUCTOS, indice);
    
    Formatos::imprimirSubtitulo("DATOS DEL PRODUCTO A ELIMINAR:");
    Formatos::EncabezadoBasicoProducto();
    producto.mostrarInformacionBasica();
    Formatos::imprimirSeparador();    
    
    // Verificar si el producto tiene transacciones asociadas
    if (productoTieneTransacciones(idProducto)) {
        Formatos::imprimirError("Este producto tiene transacciones asociadas.");
        Formatos::imprimirInformacion("Eliminarlo podria afectar la integridad de de los registros historicos.");
    }

    if (interfaz.solicitarConfirmacion("¿Está seguro que desea eliminar este producto? Esta acción no se puede deshacer.")) {
        // Marcar como eliminado lógicamente
        GestorArchivos::eliminarRegistroLogico<Producto>(ARCHIVO_PRODUCTOS, idProducto);
        Formatos::imprimirExito("Producto eliminado correctamente.");
    } else {
        Formatos::imprimirInformacion("Operación cancelada. No se guardaron cambios.");
    }
    Formatos::pausar();
}

void productosStockCritico(/* Tienda& tienda */) {
    ArchivoHeader header = GestorArchivos::leerHeader(ARCHIVO_PRODUCTOS);
    if (header.registrosActivos == 0){
        Formatos::imprimirAdvertencia("No hay productos en el Sistema.");
        return;
    }

    Formatos::imprimirSubtitulo("ALERTA: PRODUCTOS POR DEBAJO DEL MÍNIMO");
    Formatos::imprimirSeparador(60, '=');
    
    cout << CYAN << left 
         << setw(10) << "ID" 
         << setw(25) << "NOMBRE" 
         << setw(10) << "STOCK" 
         << setw(10) << "MÍNIMO" << endl << RESET;
    Formatos::imprimirSeparador(50, '-');

    bool huboAlertas = false;

    for (int i = 0; i < header.cantidadRegistros; i++) {
        Producto producto = GestorArchivos::obtenerRegistroPorIndice<Producto>(ARCHIVO_PRODUCTOS, i);
        
        if (!producto.isEliminado() && producto.estaBajoStockMinimo()) {
            huboAlertas = true;
            
            if (producto.getStock() == 0) {
                cout << ROJO << NEGRITA; // Rojo si no hay nada
            } else {
                cout << AMARILLO; // Amarillo si es solo advertencia
            }

            cout << left << setw(8) << producto.getId() 
                 << setw(25) << producto.getNombre() 
                 << setw(12) << producto.getStock() 
                 << setw(12) << producto.getStockMinimo();
            
            if (producto.getStock() == 0) {
                cout << " [AGOTADO]";
            }
            
            cout << RESET << endl;
        }
    }

    if (!huboAlertas) {
        Formatos::imprimirExito("No hay productos en estado crítico. ¡Todo en orden!");
    }
    Formatos::pausar();
}

void listarPorProveedor(Tienda& /* tienda */, int idProveedor) {
    // Verificar que el proveedor exista
    if (!GestorArchivos::existeEntidad<Proveedor>(ARCHIVO_PROVEEDORES, idProveedor)) {
        Formatos::imprimirError("El proveedor no existe");
        return;
    }
    
    // Obtener información del proveedor
    Proveedor proveedor = GestorArchivos::obtenerRegistroPorId<Proveedor>(ARCHIVO_PROVEEDORES, idProveedor);
    
    Formatos::imprimirSubtitulo("PRODUCTOS DEL PROVEEDOR");
    cout << AZUL << "Proveedor: " << proveedor.getNombre() << " (ID: " << idProveedor << ")" << RESET << endl;
    Formatos::imprimirSeparador();
    
    // Buscar productos de este proveedor
    ArchivoHeader header = GestorArchivos::leerHeader(ARCHIVO_PRODUCTOS);
    if (header.registrosActivos == 0) {
        Formatos::imprimirAdvertencia("No hay productos registrados en el sistema");
        Formatos::pausar();
        return;
    }
    
    ifstream archivo(ARCHIVO_PRODUCTOS, ios::binary);
    if (archivo.is_open()) {
        archivo.seekg(sizeof(ArchivoHeader), ios::beg);
        
        Producto producto;
        int count = 0;
        bool encontrados = false;
        
        Formatos::EncabezadoCompletoProducto();
        
        while (archivo.read(reinterpret_cast<char*>(&producto), sizeof(Producto)) && count < header.cantidadRegistros) {
            if (!producto.isEliminado() && producto.getIdProveedor() == idProveedor) {
                producto.mostrarInformacionCompleta();
                encontrados = true;
            }
            count++;
        }
        archivo.close();
        
        if (!encontrados) {
            Formatos::imprimirAdvertencia("Este proveedor no tiene productos registrados");
        } else {
            Formatos::imprimirPieTabla();
        }
    }
    
    Formatos::pausar();
}

