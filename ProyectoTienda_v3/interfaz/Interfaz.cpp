#include "Interfaz.hpp"
#include "../productos/operacionesProductos.hpp"
#include "../proveedores/operacionesproveedor.hpp"
#include "../cliente/operacionescliente.hpp"
#include "../transacciones/operacionesTransacciones.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include "../utilidades/Formatos.hpp"
#include "../mantenimiento/IntegridadReferencial.hpp"
#include "../mantenimiento/Backup.hpp"
#include "../reportes/EstadisticasVentas.hpp"
#include <iostream>
#include <limits>

using namespace std;

// Método principal de ejecución
void Interfaz::ejecutar() {
    mostrarBienvenida();
    
    // Crear tienda
    Tienda tienda("TIENDA JSport", "J-123456789");
    
    // Cargar datos existentes
    ArchivoHeader headerTienda = GestorArchivos::leerHeader(ARCHIVO_TIENDA);
    if (headerTienda.registrosActivos > 0) {
        Tienda tiendaCargada = GestorArchivos::obtenerRegistroPorId<Tienda>(ARCHIVO_TIENDA, 1);
        if (tiendaCargada.getId() != -1) {
            tienda = tiendaCargada;
        }
    }
    
    menuPrincipal(tienda);
    mostrarDespedida();
}


// Menú principal
void Interfaz::menuPrincipal(Tienda& tienda) {
    Interfaz interfaz;
    int opcion;
    
    do {
        Formatos::limpiarPantalla();
        mostrarEncabezadoSistema();
        
        cout << AZUL << NEGRITA << "MENÚ PRINCIPAL" << RESET << endl;
        Formatos::imprimirSeparador(60, '=');
        cout << CYAN
                  << "1. Gestión de Productos" << endl
                  << "2. Gestión de Proveedores" << endl
                  << "3. Gestión de Clientes" << endl
                  << "4. Gestión de Transacciones" << endl
                  << "5. Mantenimiento del Sistema" << endl
                  << "0. Salir del Sistema" << endl << RESET;
        Formatos::imprimirSeparador(60, '=');
        
        if (!interfaz.solicitarEntero("Seleccione una opción", opcion)) {
            opcion = -1;
        }
        
        switch (opcion) {
            case 1:
                menuProductos(tienda);
                break;
            case 2:
                menuProveedores(tienda);
                break;
            case 3:
                menuClientes(tienda);
                break;
            case 4:
                menuTransacciones(tienda);
                break;
            case 5:
                menuMantenimiento(tienda);
                break;
            case 0:
                cout << AMARILLO << "Saliendo del sistema..." << RESET << endl;
                break;
            default:
                Formatos::imprimirError("Opción no válida. Intente nuevamente.");
                Formatos::pausar();
                break;
        }   
        
    } while (opcion != 0);
}

// Submenús
void Interfaz::menuMantenimiento(Tienda& tienda) {
    Interfaz interfaz;
    int opcion;
    
    do {
        Formatos::limpiarPantalla();
        Formatos::imprimirTitulo("MENÚ DE MANTENIMIENTO DEL SISTEMA");
        Formatos::imprimirSeparador(60, '=');
        cout << CYAN
             << "1. Verificar Integridad Referencial" << endl
             << "2. Crear Respaldo (Backup)" << endl
             << "3. Estadisticas de ventas totales por Cliente" << endl
             << "0. Volver al Menú Principal" << endl << RESET;
        Formatos::imprimirSeparador(60, '=');
        
        if (!interfaz.solicitarEntero("Seleccione una opción", opcion)) {
            opcion = -1;
        }
        
        switch (opcion) {
            case 1:
                verificarIntegridadReferencial();
                Formatos::pausar();
                break;
            case 2:
                crearBackup();
                Formatos::pausar();
                break;
            case 3:
                estadisticasVentasTotalesPorCliente();
                Formatos::pausar();
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

// Utilidades de interfaz
void Interfaz::mostrarBienvenida() {
    Formatos::limpiarPantalla();
    Formatos::imprimirSeparador(80, '=');
    cout << AZUL << NEGRITA;
    cout << setw(25) << "" << "BIENVENIDO AL SISTEMA DE INVENTARIO" << endl;
    cout << setw(28) << "" << "TIENDA JSPORT - VERSIÓN 3.0" << endl;
    cout << RESET;
    Formatos::imprimirSeparador(80, '=');
    Formatos::pausar();
}

void Interfaz::mostrarDespedida() {
    Formatos::limpiarPantalla();
    Formatos::imprimirSeparador(80, '=');
    cout << AZUL << NEGRITA;
    cout << setw(30) << "" << "¡GRACIAS POR USAR EL SISTEMA!" << endl;
    cout << setw(32) << "" << "TIENDA JSPORT - VERSIÓN 3.0" << endl;
    cout << RESET;
    Formatos::imprimirSeparador(80, '=');
    cout << VERDE << "Todos los datos han sido guardados correctamente." << endl;
    cout << "El sistema se ha cerrado de forma segura." << RESET << endl;
    Formatos::imprimirSeparador(80, '=');
}

void Interfaz::mostrarEncabezadoSistema() {
    cout << AZUL << NEGRITA;
    Formatos::imprimirTitulo("TIENDA JSPORT - SISTEMA DE INVENTARIO V3.0");
    cout << RESET;
}

// Métodos de entrada de datos
bool Interfaz::solicitarTexto(const char* prompt, char* destino, int largo) {
    cout << CYAN << prompt << ": " << RESET;
    cin.getline(destino, largo);
    
    if (cin.fail()) {
        Formatos::limpiarBuffer();
        return false;
    }
    
    // Eliminar saltos de línea
    int len = strlen(destino);
    if (len > 0 && destino[len-1] == '\n') {
        destino[len-1] = '\0';
    }
    
    return strlen(destino) > 0;
}

bool Interfaz::solicitarEntero(const char* prompt, int& valor) {
    cout << CYAN << prompt << ": " << RESET;
    cin >> valor;
    
    if (cin.fail()) {
        Formatos::limpiarBuffer();
        return false;
    }
    
    Formatos::limpiarBuffer();
    return true;
}

bool Interfaz::solicitarFloat(const char* prompt, float& valor) {
    cout << CYAN << prompt << ": " << RESET;
    cin >> valor;
    
    if (cin.fail()) {
        Formatos::limpiarBuffer();
        return false;
    }
    
    Formatos::limpiarBuffer();
    return true;
}

bool Interfaz::solicitarConfirmacion(const char* mensaje) {
    char respuesta;
    cout << AMARILLO << mensaje << " (S/N): " << RESET;
    cin >> respuesta;
    Formatos::limpiarBuffer();
    
    return (respuesta == 's' || respuesta == 'S' || respuesta == 'y' || respuesta == 'Y');
}

// Validación de entrada
bool Interfaz::validarOpcionMenu(int opcion, int min, int max) {
    return opcion >= min && opcion <= max;
}

// Presentación de resultados
void Interfaz::mostrarResultadoOperacion(bool exito, const char* mensajeExito, const char* mensajeError) {
    if (exito) {
        Formatos::imprimirExito(mensajeExito);
    } else {
        Formatos::imprimirError(mensajeError);
    }
}
