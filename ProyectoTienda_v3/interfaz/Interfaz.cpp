#include "Interfaz.hpp"
#include "../productos/operacionesProductos.hpp"
#include "../proveedores/operacionesproveedor.hpp"
#include "../cliente/operacionescliente.hpp"
#include "../transacciones/operacionesTransacciones.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include "../utilidades/Formatos.hpp"
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
        
        std::cout << AZUL << NEGRITA << "MENÚ PRINCIPAL" << RESET << std::endl;
        Formatos::imprimirSeparador(60, '=');
        std::cout << CYAN
                  << "1. Gestión de Productos" << std::endl
                  << "2. Gestión de Proveedores" << std::endl
                  << "3. Gestión de Clientes" << std::endl
                  << "4. Gestión de Transacciones" << std::endl
                  << "5. Mantenimiento del Sistema" << std::endl
                  << "6. Reportes y Estadísticas" << std::endl
                  << "0. Salir del Sistema" << std::endl << RESET;
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
            case 6:
                menuReportes(tienda);
                break;
            case 0:
                std::cout << AMARILLO << "Saliendo del sistema..." << RESET << std::endl;
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
        std::cout << CYAN
                  << "1. Verificar Integridad Referencial" << std::endl
                  << "2. Crear Respaldo (Backup)" << std::endl
                  << "3. Restaurar desde Backup" << std::endl
                  << "4. Limpiar Archivos Temporales" << std::endl
                  << "0. Volver al Menú Principal" << std::endl << RESET;
        Formatos::imprimirSeparador(60, '=');
        
        if (!interfaz.solicitarEntero("Seleccione una opción", opcion)) {
            opcion = -1;
        }
        
        switch (opcion) {
            case 1:
                // TODO: Implementar verificación de integridad
                Formatos::imprimirInformacion("Función de verificación de integridad en desarrollo");
                Formatos::pausar();
                break;
            case 2:
                // TODO: Implementar backup
                Formatos::imprimirInformacion("Función de backup en desarrollo");
                Formatos::pausar();
                break;
            case 3:
                // TODO: Implementar restauración
                Formatos::imprimirInformacion("Función de restauración en desarrollo");
                Formatos::pausar();
                break;
            case 4:
                // TODO: Implementar limpieza
                Formatos::imprimirInformacion("Función de limpieza en desarrollo");
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

void Interfaz::menuReportes(Tienda& tienda) {
    Interfaz interfaz;
    int opcion;
    
    do {
        Formatos::limpiarPantalla();
        Formatos::imprimirTitulo("REPORTES Y ESTADÍSTICAS");
        
        std::cout << CYAN
                  << "1. Reporte de Stock Crítico" << std::endl
                  << "2. Estadísticas de Ventas por Cliente" << std::endl
                  << "3. Productos más Vendidos" << std::endl
                  << "4. Proveedores con más Compras" << std::endl
                  << "5. Reporte de Transacciones por Período" << std::endl
                  << "0. Volver al Menú Principal" << std::endl << RESET;
        Formatos::imprimirSeparador(60, '=');
        
        if (!interfaz.solicitarEntero("Seleccione una opción", opcion)) {
            opcion = -1;
        }
        
        switch (opcion) {
            case 1:
                // TODO: Implementar reporte de stock crítico
                Formatos::imprimirInformacion("Función de reporte de stock crítico en desarrollo");
                Formatos::pausar();
                break;
            case 2:
                // TODO: Implementar estadísticas de ventas
                Formatos::imprimirInformacion("Función de estadísticas de ventas en desarrollo");
                Formatos::pausar();
                break;
            case 3:
                // TODO: Implementar productos más vendidos
                Formatos::imprimirInformacion("Función de productos más vendidos en desarrollo");
                Formatos::pausar();
                break;
            case 4:
                // TODO: Implementar proveedores con más compras
                Formatos::imprimirInformacion("Función de proveedores con más compras en desarrollo");
                Formatos::pausar();
                break;
            case 5:
                // TODO: Implementar reporte por período
                Formatos::imprimirInformacion("Función de reporte por período en desarrollo");
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
    std::cout << AZUL << NEGRITA;
    std::cout << std::setw(25) << "" << "BIENVENIDO AL SISTEMA DE INVENTARIO" << std::endl;
    std::cout << std::setw(28) << "" << "TIENDA JSPORT - VERSIÓN 3.0" << std::endl;
    std::cout << RESET;
    Formatos::imprimirSeparador(80, '=');
    Formatos::pausar();
}

void Interfaz::mostrarDespedida() {
    Formatos::limpiarPantalla();
    Formatos::imprimirSeparador(80, '=');
    std::cout << AZUL << NEGRITA;
    std::cout << std::setw(30) << "" << "¡GRACIAS POR USAR EL SISTEMA!" << std::endl;
    std::cout << std::setw(32) << "" << "TIENDA JSPORT - VERSIÓN 3.0" << std::endl;
    std::cout << RESET;
    Formatos::imprimirSeparador(80, '=');
    std::cout << VERDE << "Todos los datos han sido guardados correctamente." << std::endl;
    std::cout << "El sistema se ha cerrado de forma segura." << RESET << std::endl;
    Formatos::imprimirSeparador(80, '=');
}

void Interfaz::mostrarEncabezadoSistema() {
    std::cout << AZUL << NEGRITA;
    std::cout << "===========================================" << std::endl;
    std::cout << "    TIENDA JSPORT - SISTEMA DE INVENTARIO V3.0    " << std::endl;
    std::cout << "===========================================" << std::endl;
    std::cout << RESET;
}

// Métodos de entrada de datos
bool Interfaz::solicitarTexto(const char* prompt, char* destino, int largo) {
    std::cout << CYAN << prompt << ": " << RESET;
    std::cin.getline(destino, largo);
    
    if (std::cin.fail()) {
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
    std::cout << CYAN << prompt << ": " << RESET;
    std::cin >> valor;
    
    if (std::cin.fail()) {
        Formatos::limpiarBuffer();
        return false;
    }
    
    Formatos::limpiarBuffer();
    return true;
}

bool Interfaz::solicitarFloat(const char* prompt, float& valor) {
    std::cout << CYAN << prompt << ": " << RESET;
    std::cin >> valor;
    
    if (std::cin.fail()) {
        Formatos::limpiarBuffer();
        return false;
    }
    
    Formatos::limpiarBuffer();
    return true;
}

bool Interfaz::solicitarConfirmacion(const char* mensaje) {
    char respuesta;
    std::cout << AMARILLO << mensaje << " (S/N): " << RESET;
    std::cin >> respuesta;
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
