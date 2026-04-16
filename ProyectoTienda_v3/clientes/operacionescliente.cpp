#include "operacionescliente.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include "../persistencia/Constantes.hpp"
#include "../utilidades/Validaciones.hpp"
#include "../utilidades/Formatos.hpp"
#include "../interfaz/Interfaz.hpp"
#include "Cliente.hpp"
#include <iostream>
#include <string>

using namespace std;

void registrarCliente(Tienda& tienda) {
    Interfaz interfaz;
    Formatos::imprimirSubtitulo("REGISTRAR NUEVO CLIENTE");
    
    if (!interfaz.solicitarConfirmacion("¿Desea registrar un nuevo cliente?")) {
        return;
    }
    
    Cliente nuevoCliente;
    char buffer[200];
    int valorEntero;
    float valorFloat;
    
    // Solicitar datos del cliente
    if (!interfaz.solicitarTexto("Ingrese nombre del cliente", buffer, MAX_NOMBRE)) return;
    nuevoCliente.setNombre(buffer);
    
    if (!interfaz.solicitarTexto("Ingrese cedula del cliente", buffer, MAX_CEDULA)) return;
    
    // Validar que el código no exista
    if (GestorArchivos::existeEntidad<Cliente>(ARCHIVO_CLIENTES, atoi(buffer))) {
        Formatos::imprimirError("La cedula ya existe");
        Formatos::pausar();
        return;
    }
    nuevoCliente.setCedula(buffer);
    
    if (!interfaz.solicitarTexto("Ingrese el telefono del cliente", buffer, MAX_TELEFONO)) return;
    nuevoCliente.setTlf(buffer);
    
    if (!interfaz.solicitarTexto("Ingrese la direccion del cliente", buffer, MAX_DIRECCION)) return;
    nuevoCliente.setDireccion(buffer);
    
    if (!interfaz.solicitarTexto("Ingrese el email del cliente", buffer, MAX_EMAIL)) return;
    
    if (Validaciones::validarEmail(buffer)) {
        Formatos::imprimirError("El email necesita una @");
        Formatos::pausar();
        return;
    }
    nuevoCliente.setEmail(buffer);
    
    // Mostrar resumen
    Formatos::imprimirSubtitulo("RESUMEN DEL CLIENTE");
    Formatos::EncabezadoBasicoCliente();
    nuevoCliente.mostrarInformacionBasica();

    if (interfaz.solicitarConfirmacion("¿Desea guardar este cliente?")) {
        if (GestorArchivos::guardarNuevoRegistro<Cliente>(ARCHIVO_CLIENTES, nuevoCliente)) {
            Formatos::imprimirExito("Cliente guardado correctamente");
            cout << "ID asignado: " << nuevoCliente.getId() << endl;
        } else {
            Formatos::imprimirError("Error al guardar el cliente");
        }
    } else {
        Formatos::imprimirInformacion("Operación cancelada");
    }
    
    Formatos::pausar();
}

void buscarCliente(Tienda& tienda) {
    Interfaz interfaz;
    ArchivoHeader header = GestorArchivos::leerHeader(ARCHIVO_CLIENTES);
    if (header.registrosActivos == 0) {
        Formatos::imprimirAdvertencia("No hay clientes registrados en el sistema");
        Formatos::pausar();
        return;
    }
    
    int opcion;
    do {
        Formatos::imprimirSubtitulo("BUSCAR CLIENTE");
        cout << CYAN
                  << "1. Buscar por ID" << endl
                  << "2. Buscar por nombre (coincidencia parcial)" << endl
                  << "3. Buscar por cedula" << endl
                  << "0. Cancelar" << endl << RESET;
        
        if (!interfaz.solicitarEntero("Seleccione una opción", opcion)) {
            opcion = -1;
        }
        
        switch (opcion) {
            case 1: {
                int id;
                if (interfaz.solicitarEntero("Ingrese el ID del cliente", id)) {
                    int indice = GestorArchivos::buscarPorId<Cliente>(ARCHIVO_CLIENTES, id);
                    if (indice != -1) {
                        Cliente cliente = GestorArchivos::obtenerRegistroPorIndice<Cliente>(ARCHIVO_CLIENTES, indice);
                        Formatos::imprimirExito("Cliente encontrado:");
                        Formatos::EncabezadoCompletoCliente();
                        cliente.mostrarInformacionCompleta();
                    } else {
                        Formatos::imprimirError("Cliente no encontrado");
                    }
                }
                Formatos::pausar();
                break;
            }
            
            case 2: {
                char nombre[100];
                if (interfaz.solicitarTexto("Ingrese el nombre (o parte) a buscar", nombre, MAX_NOMBRE)) {
                    int numResultados = 0;
                    int* indices = GestorArchivos::buscarRegistroPorNombre<Cliente>(ARCHIVO_CLIENTES, nombre, &numResultados);
                    
                    if (indices != nullptr) {
                        Formatos::imprimirExito("Se encontraron coincidencias:");
                        Formatos::EncabezadoBasicoCliente();
                        for (int i = 0; i < numResultados; i++) {
                            Cliente cliente = GestorArchivos::obtenerRegistroPorIndice<Cliente>(ARCHIVO_CLIENTES, indices[i]);
                            cliente.mostrarInformacionBasica();
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
                char cedulaBusqueda[20];
                if (interfaz.solicitarTexto("Ingrese el cedula a buscar", cedulaBusqueda, MAX_CEDULA)) {
                    int numResultados = 0;
                    int* indices = GestorArchivos::buscarRegistroPorNombre<Cliente>(ARCHIVO_CLIENTES, cedulaBusqueda, &numResultados);
                    if (indices != nullptr) {
                        Formatos::imprimirExito("Se encontraron coincidencias:");
                        Formatos::EncabezadoBasicoCliente();
                        for (int i = 0; i < numResultados; i++) {
                            Cliente cliente = GestorArchivos::obtenerRegistroPorIndice<Cliente>(ARCHIVO_CLIENTES, indices[i]);
                            cliente.mostrarInformacionBasica();
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

void actualizarCliente(Tienda& tienda) {
    Interfaz interfaz;
    ArchivoHeader header = GestorArchivos::leerHeader(ARCHIVO_CLIENTES);
    if (header.registrosActivos == 0){
        Formatos::imprimirAdvertencia("No hay clientes en el Sistema.");
        return;
    }
    
    int idCliente;
    if (!interfaz.solicitarEntero("Ingrese el ID del cliente a buscar", idCliente)) return;

    int indice = GestorArchivos::buscarPorId<Cliente>(ARCHIVO_CLIENTES, idCliente);
    if (indice == -1) {
        string errorMsg = "El cliente con ID " + to_string(idCliente) + " no existe.";
        Formatos::imprimirError(errorMsg.c_str());
        return;
    }
    
    Cliente cliente = GestorArchivos::obtenerRegistroPorIndice<Cliente>(ARCHIVO_CLIENTES, indice);

    Formatos::imprimirSubtitulo("DATOS ACTUALES DEL CLIENTE:");
    Formatos::EncabezadoBasicoCliente();
    cliente.mostrarInformacionBasica();
    Formatos::imprimirSeparador();

    char nombre[100], cedula[20], telefono[20], email[100], direccion[200];
    int seleccion; 

    bool verNombre = false, verCedula = false, verTlf = false, verEmail = false, verDireccion = false;
    
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
        
        if (!interfaz.solicitarEntero("Seleccione una opción", seleccion)) continue;

        switch (seleccion) {
            case 1: {
                if (interfaz.solicitarTexto("Ingrese nuevo cedula", cedula, MAX_CEDULA)) {
                    if (!Validaciones::validarCedula(cedula)) {
                        Formatos::imprimirError("El cedula debe tener el formato correcto (Ej: V-12345678)");
                        break;
                    } else {
                        cliente.setCedula(cedula);
                        verCedula = true;
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
                        cliente.setNombre(nombre);
                        verNombre = true;
                    }
                }
                break;
            }
            case 3: {
                if (interfaz.solicitarTexto("Ingrese nuevo telefono", telefono, MAX_TELEFONO)) {
                    if (!Validaciones::validarTelefono(telefono)) {
                        Formatos::imprimirError("El telefono debe tener al menos 10 caracteres");
                        break;
                    } else {
                        cliente.setTlf(telefono);
                        verTlf = true;
                    }
                }
                break;
            }
            case 4: {
                if (interfaz.solicitarTexto("Ingrese el nuevo email", email, MAX_EMAIL)) {
                    if (!Validaciones::validarEmail(email)) {
                        Formatos::imprimirError("El email tiene que tener un @");
                        break;
                    } else {
                        cliente.setEmail(email);
                        verEmail = true;
                    }
                }
                break;
            }
            case 5: {
                if (interfaz.solicitarTexto("Ingrese nueva direccion", direccion, MAX_DIRECCION)) {
                    cliente.setDireccion(direccion);
                    verDireccion = true;
                }
                break;
            }
            
            case 6: {
                if (!verNombre && !verCedula && !verTlf && !verEmail && !verDireccion) {
                    Formatos::imprimirAdvertencia("No se han realizado cambios para guardar.");
                } else {
                    if (interfaz.solicitarConfirmacion("¿Desea guardar los cambios hechos al cliente")) {
                        if (GestorArchivos::actualizarRegistro<Cliente>(ARCHIVO_CLIENTES, indice, cliente)){
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


void listarClientes(Tienda& tienda) {
    ArchivoHeader header = GestorArchivos::leerHeader(ARCHIVO_CLIENTES);
    if (header.registrosActivos == 0) {
        Formatos::imprimirAdvertencia("No hay clientes registrados en el sistema");
        Formatos::pausar();
        return;
    }
    
    Formatos::imprimirSubtitulo("LISTADO DE CLIENTES");
    Formatos::EncabezadoBasicoCliente();
    
    ifstream archivo(ARCHIVO_CLIENTES, ios::binary);
    if (archivo.is_open()) {
        archivo.seekg(sizeof(ArchivoHeader), ios::beg);
        
        Cliente cliente;
        int count = 0;
        Formatos::EncabezadoCompletoCliente();
        while (archivo.read(reinterpret_cast<char*>(&cliente), sizeof(Cliente)) && count < header.cantidadRegistros) {
            if (!cliente.isEliminado()) {
                cliente.mostrarInformacionCompleta();
            }
            count++;
        }
        archivo.close();
    }
    
    Formatos::imprimirPieTabla();
    Formatos::pausar();
}

void eliminarCliente(Tienda& tienda) {
    Interfaz interfaz;
    ArchivoHeader header = GestorArchivos::leerHeader(ARCHIVO_CLIENTES);
    if (header.registrosActivos == 0){
        Formatos::imprimirAdvertencia("No hay clientes en el Sistema.");
        return;
    }

    int idCliente;
    if (!interfaz.solicitarEntero("Ingrese el ID del cliente a buscar", idCliente)) return;

    int indice = GestorArchivos::buscarPorId<Cliente>(ARCHIVO_CLIENTES, idCliente);
    if (indice == -1) {
        string errorMsg = "El cliente con ID " + to_string(idCliente) + " no existe.";
        Formatos::imprimirError(errorMsg.c_str());
        return;
    }
    
    Cliente cliente = GestorArchivos::obtenerRegistroPorIndice<Cliente>(ARCHIVO_CLIENTES, indice);
    
    Formatos::imprimirSubtitulo("DATOS DEL CLIENTE A ELIMINAR:");
    Formatos::EncabezadoBasicoCliente();
    cliente.mostrarInformacionBasica();
    Formatos::imprimirSeparador();    
    

    if (interfaz.solicitarConfirmacion("¿Está seguro que desea eliminar este cliente? Esta acción no se puede deshacer.")) {
        // Marcar como eliminado lógicamente
        GestorArchivos::eliminarRegistroLogico<Cliente>(ARCHIVO_CLIENTES, idCliente);
        Formatos::imprimirExito("Cliente eliminado correctamente.");
    } else {
        Formatos::imprimirInformacion("Operación cancelada. No se guardaron cambios.");
    }
    Formatos::pausar();
}


void menuClientes(Tienda& tienda) {
    Interfaz interfaz;
    int opcion;
    
    do {
        Formatos::limpiarPantalla();
        Formatos::imprimirTitulo("GESTIÓN DE CLIENTES");
        
        cout << CYAN
                  << "1. Registrar nuevo Cliente" << endl
                        << "2. Buscar Cliente" << endl
                        << "3. Actualizar Cliente" << endl
                        << "4. Listar todos los Clientes" << endl
                        << "5. Eliminar Cliente" << endl
                        << "0. Volver al menú principal" << endl << RESET;
        
        if (!interfaz.solicitarEntero("Seleccione una opción", opcion)) {
            opcion = -1;
        }
        
        switch (opcion) {
            case 1:
                registrarCliente(tienda);
                break;
            case 2:
                buscarCliente(tienda);
                break;
            case 3:
                actualizarCliente(tienda);
                break;
            case 4:
                listarClientes(tienda);
                break;
            case 5:
                eliminarCliente(tienda);
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
