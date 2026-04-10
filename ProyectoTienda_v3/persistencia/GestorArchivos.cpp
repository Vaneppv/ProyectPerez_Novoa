#include "GestorArchivos.hpp"
#include "../utilidades/Formatos.hpp"
#include <ctime>
#include <cstring>
#include <iostream>

using namespace std;

// Implementación de métodos estáticos
bool GestorArchivos::inicializarSistemaArchivos() {
    bool resultado = true;
    
    resultado = resultado && inicializarArchivo(ARCHIVO_TIENDA);
    resultado = resultado && inicializarArchivo(ARCHIVO_PRODUCTOS);
    resultado = resultado && inicializarArchivo(ARCHIVO_PROVEEDORES);
    resultado = resultado && inicializarArchivo(ARCHIVO_CLIENTES);
    resultado = resultado && inicializarArchivo(ARCHIVO_TRANSACCIONES);
    resultado = resultado && inicializarArchivo(ARCHIVO_DETALLES);
    resultado = resultado && inicializarArchivo(ARCHIVO_BACKUP);
    
    if (resultado) {
        Formatos::imprimirExito("Sistema de archivos inicializado correctamente");
    } else {
        Formatos::imprimirAdvertencia("Algunos archivos no pudieron inicializarse");
    }
    
    return resultado;
}

bool GestorArchivos::inicializarArchivo(const char* nombreArchivo) {
    ifstream test(nombreArchivo, ios::binary);
    
    if (test.is_open()) {
        test.close();
        return true;
    }

    ofstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()) {
        cout << ROJO << "Error: No se pudo crear el archivo " << nombreArchivo << endl << RESET;
        return false;
    }

    ArchivoHeader header = {0, 1, 0, 1};
    archivo.write(reinterpret_cast<const char*>(&header), sizeof(ArchivoHeader));
    
    bool resultado = archivo.good();
    archivo.close();
    return resultado;
}

ArchivoHeader GestorArchivos::leerHeader(const char* nombreArchivo) {
    ArchivoHeader header = {0, 1, 0, 1};
    
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()) {
        return header;
    }

    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    archivo.close();
    
    return header;
}

bool GestorArchivos::actualizarHeader(const char* nombreArchivo, ArchivoHeader header) {
    fstream archivo(nombreArchivo, ios::binary | ios::in | ios::out);
    
    if (!archivo.is_open()) {
        cout << ROJO << "Error: No se pudo abrir el archivo " << nombreArchivo << endl << RESET;
        return false;
    }

    archivo.seekp(0, ios::beg);
    archivo.write(reinterpret_cast<const char*>(&header), sizeof(ArchivoHeader));
    
    bool resultado = archivo.good();
    archivo.close();
    return resultado;
}

bool GestorArchivos::contieneSubstring(const char* texto, const char* busqueda) {
    if (!texto || !busqueda) return false;
    
    char textoLower[MAX_DESCRIPCION];
    char busquedaLower[MAX_DESCRIPCION];
    
    strncpy(textoLower, texto, MAX_DESCRIPCION - 1);
    textoLower[MAX_DESCRIPCION - 1] = '\0';
    strncpy(busquedaLower, busqueda, MAX_DESCRIPCION - 1);
    busquedaLower[MAX_DESCRIPCION - 1] = '\0';
    
    // Convertir a minúsculas para búsqueda insensible a mayúsculas/minúsculas
    for (int i = 0; textoLower[i]; i++) {
        textoLower[i] = tolower(textoLower[i]);
    }
    for (int i = 0; busquedaLower[i]; i++) {
        busquedaLower[i] = tolower(busquedaLower[i]);
    }
    
    return strstr(textoLower, busquedaLower) != nullptr;
}

void GestorArchivos::obtenerFechaActual(char* buffer) {
    time_t tiempo_actual = time(nullptr);
    struct tm* tm_info = localtime(&tiempo_actual);
    strftime(buffer, MAX_FECHA, "%d/%m/%Y", tm_info);
}

// Implementación de métodos específicos para transacciones
Transaccion* GestorArchivos::buscarTransaccionesPorTipo(const char* nombreArchivo, const char* tipo, int* numResultados) {
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()) {
        cout << ROJO << "Error: No se pudo abrir el archivo " << nombreArchivo << endl << RESET;
        return nullptr;
    }

    ArchivoHeader header;
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    archivo.close();

    if (header.registrosActivos == 0) {
        *numResultados = 0;
        return nullptr;
    }

    // Contar cuántas transacciones coinciden con el tipo
    int count = 0;
    for (int i = 0; i < header.cantidadRegistros; i++) {
        Transaccion temp;
        archivo.read(reinterpret_cast<char*>(&temp), sizeof(Transaccion));
        if (!temp.isEliminado() && strcmp(temp.getTipo(), tipo) == 0) {
            count++;
        }
    }

    if (count == 0) {
        *numResultados = 0;
        return nullptr;
    }

    // Crear array de resultados
    Transaccion* resultados = new Transaccion[count];
    int j = 0;

    // Leer nuevamente y guardar las coincidencias
    archivo.open(nombreArchivo, ios::binary);
    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    
    for (int i = 0; i < header.cantidadRegistros; i++) {
        Transaccion temp;
        archivo.read(reinterpret_cast<char*>(&temp), sizeof(Transaccion));
        if (!temp.isEliminado() && strcmp(temp.getTipo(), tipo) == 0) {
            resultados[j] = temp;
            j++;
        }
    }

    archivo.close();
    *numResultados = count;
    return resultados;
}

Transaccion* GestorArchivos::buscarTransaccionesPorFecha(const char* nombreArchivo, const char* fecha, int* numResultados) {
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()) {
        cout << ROJO << "Error: No se pudo abrir el archivo " << nombreArchivo << endl << RESET;
        return nullptr;
    }

    ArchivoHeader header;
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    archivo.close();

    if (header.registrosActivos == 0) {
        *numResultados = 0;
        return nullptr;
    }

    // Contar cuántas transacciones coinciden con la fecha
    int count = 0;
    for (int i = 0; i < header.cantidadRegistros; i++) {
        Transaccion temp;
        archivo.read(reinterpret_cast<char*>(&temp), sizeof(Transaccion));
        if (!temp.isEliminado() && strstr(temp.getFechaRegistro(), fecha) != nullptr) {
            count++;
        }
    }

    if (count == 0) {
        *numResultados = 0;
        return nullptr;
    }

    // Crear array de resultados
    Transaccion* resultados = new Transaccion[count];
    int j = 0;

    // Leer nuevamente y guardar las coincidencias
    archivo.open(nombreArchivo, ios::binary);
    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    
    for (int i = 0; i < header.cantidadRegistros; i++) {
        Transaccion temp;
        archivo.read(reinterpret_cast<char*>(&temp), sizeof(Transaccion));
        if (!temp.isEliminado() && strstr(temp.getFechaRegistro(), fecha) != nullptr) {
            resultados[j] = temp;
            j++;
        }
    }

    archivo.close();
    *numResultados = count;
    return resultados;
}

Transaccion* GestorArchivos::buscarTransaccionesPorRelacionado(const char* nombreArchivo, int idRelacionado, int* numResultados) {
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()) {
        cout << ROJO << "Error: No se pudo abrir el archivo " << nombreArchivo << endl << RESET;
        return nullptr;
    }

    ArchivoHeader header;
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    archivo.close();

    if (header.registrosActivos == 0) {
        *numResultados = 0;
        return nullptr;
    }

    // Contar cuántas transacciones coinciden con el ID relacionado
    int count = 0;
    for (int i = 0; i < header.cantidadRegistros; i++) {
        Transaccion temp;
        archivo.read(reinterpret_cast<char*>(&temp), sizeof(Transaccion));
        if (!temp.isEliminado() && temp.getIdRelacionado() == idRelacionado) {
            count++;
        }
    }

    if (count == 0) {
        *numResultados = 0;
        return nullptr;
    }

    // Crear array de resultados
    Transaccion* resultados = new Transaccion[count];
    int j = 0;

    // Leer nuevamente y guardar las coincidencias
    archivo.open(nombreArchivo, ios::binary);
    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    
    for (int i = 0; i < header.cantidadRegistros; i++) {
        Transaccion temp;
        archivo.read(reinterpret_cast<char*>(&temp), sizeof(Transaccion));
        if (!temp.isEliminado() && temp.getIdRelacionado() == idRelacionado) {
            resultados[j] = temp;
            j++;
        }
    }

    archivo.close();
    *numResultados = count;
    return resultados;
}

int GestorArchivos::verificarProductoEnTransaccion(const char* archivoDetalles, int idProd) {
    ifstream archivo(archivoDetalles, ios::binary);
    if (!archivo.is_open()) {
        cout << ROJO << "Error: No se pudo abrir el archivo " << archivoDetalles << endl << RESET;
        return -1;
    }
    
    // Saltar el header del archivo
    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    
    DetalleTransaccion trans;
    while(archivo.read(reinterpret_cast<char*>(&trans), sizeof(DetalleTransaccion))) {
        if(!trans.isEliminado() && trans.getIdProducto() == idProd) {
            archivo.close();
            return trans.getIdTransaccion();
        }
    }
    archivo.close();
    return -1;
}

DetalleTransaccion* GestorArchivos::obtenerDetallesPorTransaccion(const char* archivoDetalles, int idTransaccion, int* numResultados) {
    ifstream archivo(archivoDetalles, ios::binary);
    if (!archivo.is_open()) {
        cout << ROJO << "Error: No se pudo abrir el archivo " << archivoDetalles << endl << RESET;
        if (numResultados) *numResultados = 0;
        return nullptr;
    }

    ArchivoHeader header;
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    archivo.close();

    if (header.registrosActivos == 0) {
        if (numResultados) *numResultados = 0;
        return nullptr;
    }

    // Contar cuántos detalles coinciden con la transacción
    int count = 0;
    ifstream archivoConteo(archivoDetalles, ios::binary);
    archivoConteo.seekg(sizeof(ArchivoHeader), ios::beg);
    
    DetalleTransaccion temp;
    while (archivoConteo.read(reinterpret_cast<char*>(&temp), sizeof(DetalleTransaccion))) {
        if (!temp.isEliminado() && temp.getIdTransaccion() == idTransaccion) {
            count++;
        }
    }
    archivoConteo.close();

    if (count == 0) {
        if (numResultados) *numResultados = 0;
        return nullptr;
    }

    // Crear array de resultados
    DetalleTransaccion* resultados = new DetalleTransaccion[count];
    int j = 0;

    // Leer nuevamente y guardar las coincidencias
    ifstream archivoLectura(archivoDetalles, ios::binary);
    archivoLectura.seekg(sizeof(ArchivoHeader), ios::beg);
    
    while (archivoLectura.read(reinterpret_cast<char*>(&temp), sizeof(DetalleTransaccion))) {
        if (!temp.isEliminado() && temp.getIdTransaccion() == idTransaccion) {
            resultados[j] = temp;
            j++;
        }
    }

    archivoLectura.close();
    
    if (numResultados) *numResultados = count;
    return resultados;
}
