#include "../persistencia/GestorArchivos.hpp"
#include "../utilidades/Formatos.hpp"
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <cstdlib>

using namespace std;

void copiarArchivo(const char* origen, const char* destino) {
    ifstream src(origen, ios::binary);
    ofstream dst(destino, ios::binary);
    
    if (!src.is_open() || !dst.is_open()) {
        cout << ROJO << "Error al copiar archivo: " << origen << RESET << endl;
        return;
    }
    
    dst << src.rdbuf();
    src.close();
    dst.close();
}

void crearBackup() {
    Formatos::imprimirTitulo("CREAR RESPALDO DE SEGURIDAD");
    
    // Obtener fecha y hora actual
    time_t ahora = time(nullptr);
    tm* tiempoLocal = localtime(&ahora);
    
    char fechaHora[50];
    strftime(fechaHora, sizeof(fechaHora), "BACKUP_%d-%m-%Y_%H-%M-%S", tiempoLocal);
    
    // Crear carpeta de backup
    string nombreCarpeta = fechaHora;
    
    // Crear carpeta de backup usando comando del sistema
    string comando = "mkdir \"" + nombreCarpeta + "\"";
    int resultado = system(comando.c_str());
    
    if (resultado == 0) {
        cout << VERDE << "Carpeta de respaldo creada: " << nombreCarpeta << RESET << endl;
    } else {
        cout << AMARILLO << "La carpeta ya existe o hubo un error: " << nombreCarpeta << RESET << endl;
    }
    
    // Archivos a respaldar
    const char* archivos[] = {
        ARCHIVO_PRODUCTOS,
        ARCHIVO_PROVEEDORES,
        ARCHIVO_CLIENTES,
        ARCHIVO_TRANSACCIONES,
        ARCHIVO_DETALLES
    };
    
    const char* nombresArchivos[] = {
        "productos.bin",
        "proveedores.bin",
        "clientes.bin",
        "transacciones.bin",
        "detallesTransacciones.bin"
    };
    
    cout << CYAN << "\nIniciando proceso de respaldo..." << RESET << endl;
    
    int archivosRespaldo = 0;
    for (int i = 0; i < 5; i++) {
        string rutaDestino = nombreCarpeta + "/" + nombresArchivos[i];
        
        // Verificar que el archivo origen exista
        ifstream verificar(archivos[i]);
        if (verificar.good()) {
            verificar.close();
            copiarArchivo(archivos[i], rutaDestino.c_str());
            cout << "  " << VERDE << "[OK]" << RESET << " " << nombresArchivos[i] << " guardado." << endl;
            archivosRespaldo++;
        } else {
            cout << "  " << AMARILLO << "[ADVERTENCIA]" << RESET << " " << nombresArchivos[i] << " no existe, omitiendo." << endl;
        }
    }
    
    // Crear archivo de información del backup
    string infoPath = nombreCarpeta + "/backup_info.txt";
    ofstream infoFile(infoPath);
    if (infoFile.is_open()) {
        infoFile << "=== INFORMACIÓN DE RESPALDO ===\n";
        infoFile << "Fecha y hora: " << fechaHora << "\n";
        infoFile << "Archivos respaldados: " << archivosRespaldo << " de 5\n";
        infoFile << "Versión del sistema: ProyectoTienda_v3\n";
        infoFile << "================================\n";
        infoFile.close();
        cout << "  " << VERDE << "[OK]" << RESET << " backup_info.txt creado." << endl;
    }
    
    cout << "\n" << VERDE << "¡Respaldo de seguridad finalizado!" << RESET << endl;
    cout << CYAN << "Ubicación: " << nombreCarpeta << RESET << endl;
    cout << CYAN << "Archivos procesados: " << archivosRespaldo << "/5" << RESET << endl;
}
