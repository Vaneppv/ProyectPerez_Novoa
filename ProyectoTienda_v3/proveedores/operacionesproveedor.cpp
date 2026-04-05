#include "../productos/operacionesProveedor.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include "../persistencia/Constantes.hpp"
#include "../utilidades/Validaciones.hpp"
#include "../utilidades/Formatos.hpp"
#include <iostream>

using namespace std;

void registrarProveedor(Tienda& tienda) {
    Formatos::imprimirSubtitulo("REGISTRAR NUEVO PROVEEDOR");
    
    if (!Interfaz::solicitarConfirmacion("¿Desea registrar un nuevo proveedor?")) {
        return;
    }
    
    Proveedor nuevoProveedor;
    char buffer[200];
    int valorEntero;
    float valorFloat;
    
    // Solicitar datos del producto
    if (!Interfaz::solicitarTexto("Ingrese nombre del proveedor", buffer, MAX_NOMBRE)) return;
    nuevoProveedor.setNombre(buffer);
    
    if (!Interfaz::solicitarTexto("Ingrese rif del proveedor", buffer, MAX_RIF)) return;
    
    // Validar que el código no exista
    if (GestorArchivos::existeEntidad<Proveedor>(ARCHIVO_PROVEEDOR, atoi(buffer))) {
        Formatos::imprimirError("El rif ya existe");
        Formatos::pausar();
        return;
    }
    nuevoProveedor.setRif(buffer);
    
    if (!Interfaz::solicitarTexto("Ingrese el telefono del proveedor", buffer, MAX_TLF)) return;
    nuevoProveedor.setTlf(buffer);
    
    if (!Interfaz::solicitarTexto("Ingrese la direccion del proveedor", buffer, MAX_DIRECCION)) return;
    nuevoProveedor.setDireccion(buffer);
    
    // Mostrar resumen
    Formatos::imprimirSubtitulo("RESUMEN DEL PROVEEDOR");
    nuevoProveedor.mostrarInformacionCompleta();
    
    if (Interfaz::solicitarConfirmacion("¿Desea guardar este proveedor?")) {
        if (GestorArchivos::guardarNuevoRegistro<Producto>(ARCHIVO_PROVEEDOR, nuevoProveedor)) {
            Formatos::imprimirExito("Producto guardado correctamente");
            cout << "ID asignado: " << nuevoProveedor.getId() << endl;
        } else {
            Formatos::imprimirError("Error al guardar el proveedor");
        }
    } else {
        Formatos::imprimirInformacion("Operación cancelada");
    }
    
    Formatos::pausar();
}

void buscarProveedor(Tienda& tienda) {
    ArchivoHeader header = GestorArchivos::leerHeader(ARCHIVO_PROVEEDOR);
    if (header.registrosActivos == 0) {
        Formatos::imprimirAdvertencia("No hay proveedores registrados en el sistema");
        Formatos::pausar();
        return;
    }
    
    int opcion;
    do {
        Formatos::imprimirSubtitulo("BUSCAR PROVEEDOR");
        cout << CYAN
                  << "1. Buscar por ID" << endl
                  << "2. Buscar por nombre (coincidencia parcial)" << endl
                  << "3. Buscar por rif" << endl
                  << "0. Cancelar" << endl << RESET;
        
        if (!Interfaz::solicitarEntero("Seleccione una opción", opcion)) {
            opcion = -1;
        }
        
        switch (opcion) {
            case 1: {
                int id;
                if (Interfaz::solicitarEntero("Ingrese el ID del producto", id)) {
                    int indice = GestorArchivos::buscarPorId<Proveedor>(ARCHIVO_PROVEEDORES, id);
                    if (indice != -1) {
                        Producto producto = GestorArchivos::obtenerRegistroPorIndice<Proveedor>(ARCHIVO_PROVEEDORES, indice);
                        Formatos::imprimirExito("Proveedor encontrado:");
                        producto.mostrarInformacionCompleta();
                    } else {
                        Formatos::imprimirError("Proveedor no encontrado");
                    }
                }
                Formatos::pausar();
                break;
            }
            
            case 2: {
                char nombre[100];
                if (Interfaz::solicitarTexto("Ingrese el nombre (o parte) a buscar", nombre, MAX_NOMBRE)) {
                    int numResultados = 0;
                    int* indices = GestorArchivos::buscarRegistroPorNombre<Proveedor>(ARCHIVO_PROVEEDORES, nombre, &numResultados);
                    
                    if (indices != nullptr) {
                        Formatos::imprimirExito("Se encontraron coincidencias:");
                        for (int i = 0; i < numResultados; i++) {
                            Producto producto = GestorArchivos::obtenerRegistroPorIndice<Proveedor>(ARCHIVO_PROVEEDORES, indices[i]);
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
                if (Interfaz::solicitarTexto("Ingrese el rif a buscar", rif, MAX_RIF)) {
                    int numResultados = 0;
                    int* indices = GestorArchivos::buscarRegistroPorRif<Proveedor>(ARCHIVO_PROVEEDORES, rif);
                    if (indices != nullptr) {
                        Formatos::imprimirExito("Se encontraron coincidencias:");
                        for (int i = 0; i < numResultados; i++) {
                            Producto producto = GestorArchivos::obtenerRegistroPorIndice<Proveedor>(ARCHIVO_PROVEEDORES, indices[i]);
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
    ArchivoHeader header = GestorArchivos::leerHeader(ARCHIVO_PROVEEDORES);
    if (header.registrosActivos == 0){
        Formatos::imprimirAdvertencia("No hay proveedores en el Sistema.");
        return;
    }
    
    int idProducto;
    if (!Interfaz::solicitarEntero("Ingrese el ID del proveedor a buscar", idProveedor)) return;

    int indice = GestorArchivos::buscarPorId<Proveedor>(ARCHIVO_PROVEEDORES, idProveedor);
    if (indice == -1) {
        Formatos::imprimirError("El producto con ID " + to_string(idProveedor) + " no existe.");
        return;
    }
    
    Proveedor proveedir = GestorArchivos::obtenerRegistroPorIndice<Proveedor>(ARCHIVO_PROVEEDORES, indice);

    Formatos::imprimirSubtitulo("DATOS ACTUALES DEL PROVEEDOR:");
    Formatos::imprimirEncabezadoTabla();
    producto.mostrarInformacionBasica();
    Formatos::imprimirSeparador();

    char nombre[100], rif[20], telefono[20], email[100] direccion[200];
    int seleccion; 

    bool verNombre = false, verRif = false, verTlf = false, verEmail = false, verDireccion = false;
    
    // Menú de campos editables
    do {
        Formatos::imprimirSubtitulo("¿Qué desea editar?");
        cout << CYAN
                  << "1. Rif" << endl
                  << "2. Nombre" << endl
                  << "3. Telefono" << endl
                  << "4. Email" << endl
                  << "5. Direccion" << endl
                  << "6. Guardar cambios" << endl
                  << "0. Cancelar sin guardar" << endl << RESET;
        
        if (!Interfaz::solicitarEntero("Seleccione una opción", seleccion)) continue;

        switch (seleccion) {
            case 1: {
                if (Interfaz::solicitarTexto("Ingrese nuevo rif", rif, MAX_RIF)) {
                    if (!validaciones::validarRIF(rif)) {
                        Formatos::imprimirError("El rif debe tener el formato correcto (Ej: PROD001)");
                        break;
                    } else {
                        proveedor.setRif(rif);
                        verRif = true;
                    }
                }
                break;
            }
            case 2: {
                if (Interfaz::solicitarTexto("Ingrese nuevo nombre", nombre, MAX_NOMBRE)){
                    if (!validaciones::validarNombre(nombre)) {
                        Formatos::imprimirError("El nombre debe tener al menos 3 caracteres");
                        break;
                    } else {
                        proveedor.setNombre(nombre);
                        verNombre = true;
                    }
                }
                break;
            }
            case 3: {
                if (Interfaz::solicitarTexto("Ingrese nuevo telefono", telefono, MAX_TLF)) {
                    if (!validaciones::validarTelefono(telefono)) {
                        Formatos::imprimirError("El telefono debe tener al menos 10 caracteres");
                        break;
                    } else {
                        producto.setTlf(Telefono);
                        verTlf = true;
                    }
                }
                break;
            }
            case 4: {
                if (Interfaz::solicitarTexto("Ingrese el nuevo email", email, MAX_EMAIL)) {
                    if (!validaciones::validarEmail(email)) {
                        Formatos::imprimirError("El email tiene que tener un @");
                        break;
                    } else {
                        proveedor.setEmail(email);
                        verEmail = true;
                    }
                }
                break;
            }
            case 5: {
                if (Interfaz::solicitarTExto("Ingrese nueva direccion", direccion)) {
                   
                        proveedor.setDireccion(direccion);
                        verDireccion = true;
                }
                break;
            }
            
            case 6: {
                if (!verNombre && !verRif && !verTlf && !verEmail && !verDireccion) {
                    Formatos::imprimirAdvertencia("No se han realizado cambios para guardar.");
                } else {
                    if (Interfaz::solicitarConfirmacion("¿Desea guardar los cambios hechos al proveedor")) {
                        if (GestorArchivos::actualizarRegistro<Proveedor>(ARCHIVO_PROVEEDORES, indice, proveedor)){
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


void listarProveedores(Tienda& tienda) {
    ArchivoHeader header = GestorArchivos::leerHeader(ARCHIVO_PROVEEDORES);
    if (header.registrosActivos == 0) {
        Formatos::imprimirAdvertencia("No hay proveedores registrados en el sistema");
        Formatos::pausar();
        return;
    }
    
    Formatos::imprimirSubtitulo("LISTADO DE PROVEEDORES");
    Formatos::imprimirEncabezadoTabla();
    
    ifstream archivo(ARCHIVO_PROVEEDORES, ios::binary);
    if (archivo.is_open()) {
        archivo.seekg(sizeof(ArchivoHeader), ios::beg);
        
        Proveedor proveedor;
        int count = 0;
        while (archivo.read(reinterpret_cast<char*>(&proveedor), sizeof(Proveedor)) && count < header.cantidadRegistros) {
            if (!proveedor.isEliminado()) {
                proveedor.mostrarInformacionCompleta();
            }
            count++;
        }
        archivo.close();
    }
    
    Formatos::imprimirPieTabla();
    Formatos::pausar();
}

void eliminarProveedor(Tienda& tienda) {
    ArchivoHeader header = GestorArchivos::leerHeader(ARCHIVO_PROVEEDORES);
    if (header.registrosActivos == 0){
        Formatos::imprimirAdvertencia("No hay proveedores en el Sistema.");
        return;
    }

    int idProveedor;
    if (!Interfaz::solicitarEntero("Ingrese el ID del proveedor a buscar", idProveedor)) return;

    int indice = GestorArchivos::buscarPorId<Proveedor>(ARCHIVO_PROVEEDORES, idProveedor);
    if (indice == -1) {
        Formatos::imprimirError("El producto con ID " + to_string(idProveedor) + " no existe.");
        return;
    }
    
    Proveedor proveedor = GestorArchivos::obtenerRegistroPorIndice<Proveedor>(ARCHIVO_PROVEEDORES, indice);
    
    Formatos::imprimirSubtitulo("DATOS DEL PROVEEDORES A ELIMINAR:");
    Formatos::imprimirEncabezadoTabla();
    producto.mostrarInformacionBasica();
    Formatos::imprimirSeparador();    
    
    // Verificar si el producto tiene transacciones asociadas

    if (Interfaz::solicitarConfirmacion("¿Está seguro que desea eliminar este proveedor? Esta acción no se puede deshacer.")) {
        // Marcar como eliminado lógicamente
        GestorArchivos::borrarRegistroLogico<Proveedor>(ARCHIVO_PROVEEDORES, idProveedor);
        Formatos::imprimirExito("Proveedor eliminado correctamente.");
    } else {
        Formatos::imprimirInformacion("Operación cancelada. No se guardaron cambios.");
    }
    Formatos::pausar();
}


void menuProveedores(Tienda& tienda) {
    int opcion;
    
    do {
        Formatos::limpiarPantalla();
        Formatos::imprimirSubtitulo("GESTIÓN DE PRODUCTOS");
        
        cout << CYAN
                  << "1. Registrar nuevo Proveedor" << endl
                        << "2. Buscar Proveedor" << endl
                        << "3. Actualizar Proveedor" << endl
                        << "4. Listar todos los Proveedores" << endl
                        << "5. Eliminar Proveedor" << endl
                        << "0. Volver al menú principal" << endl << RESET;
        
        if (!Interfaz::solicitarEntero("Seleccione una opción", opcion)) {
            opcion = -1;
        }
        
        switch (opcion) {
            case 1:
                registrarProveedor(tienda);
                break;
            case 2:
                buscarProveedor(tienda);
                break;
            case 3:
                actualizarProveedor(tienda);
                break;
            case 4:
                listarProveedores(Tienda& tienda);
                break;
            case 5:
                eliminarProveedor(Tienda& tienda);
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
