#include "persistencia/GestorArchivos.hpp"
#include "persistencia/Constantes.hpp"
#include "tienda/Tienda.hpp"
#include "interfaz/Interfaz.hpp"
#include <iostream>
#include <windows.h>

using namespace std;

int main() {
    SetConsoleOutputCP(CP_UTF8);
    try {
        locale::global(locale("C"));
    } catch (...) {
        // en caso de falla el programa no se cierra
    }

    // Inicializar el sistema de archivos
    if (!GestorArchivos::inicializarSistemaArchivos()) {
        cerr << "Error crítico: No se pudo inicializar el sistema de archivos" << endl;
        return 1;
    }

    // Crear e inicializar la tienda
    Tienda tienda("TIENDA JSport", "J-123456789");
    
    // Intentar cargar datos existentes de la tienda
    ArchivoHeader headerTienda = GestorArchivos::leerHeader(ARCHIVO_TIENDA);
    if (headerTienda.registrosActivos > 0) {
        Tienda tiendaCargada = GestorArchivos::obtenerRegistroPorId<Tienda>(ARCHIVO_TIENDA, 1);
        if (tiendaCargada.getId() != -1) {
            tienda = tiendaCargada;
        }
    } else {
        // Guardar la tienda inicial
        GestorArchivos::guardarNuevoRegistro<Tienda>(ARCHIVO_TIENDA, tienda);
    }

    // Ejecutar la interfaz principal
    try {
        Interfaz interfaz;
        interfaz.ejecutar();
    } catch (const exception& e) {
        cerr << "Error durante la ejecución: " << e.what() << endl;
        return 1;
    } catch (...) {
        cerr << "Error desconocido durante la ejecución" << endl;
        return 1;
    }

    cout << VERDE << "Sistema finalizado correctamente. ¡Hasta pronto!" << RESET << endl;
    return 0;
}
