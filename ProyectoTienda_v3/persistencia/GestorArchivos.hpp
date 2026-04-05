#ifndef GESTORARCHIVOS_HPP
#define GESTORARCHIVOS_HPP

#include <fstream>
#include <cstring>
#include <iostream>
#include "Constantes.hpp"

using namespace std;

// Estructura para el header de archivos
struct ArchivoHeader {
    int cantidadRegistros;      // Total historico de registros
    int proximoID;              // Siguiente ID a asignar (Autoincremental)
    int registrosActivos;       // Registros que no estan marcados como eliminados
    int version;                // Control de version del archivo
};

class GestorArchivos {
public:
    // Inicialización del sistema de archivos
    static bool inicializarSistemaArchivos();
    
    // Operaciones de header
    static bool inicializarArchivo(const char* nombreArchivo);
    static ArchivoHeader leerHeader(const char* nombreArchivo);
    static bool actualizarHeader(const char* nombreArchivo, ArchivoHeader header);
    
    // Template para operaciones CRUD genéricas
    template<typename T>
    static int buscarPorId(const char* nombreArchivo, int id);
    
    template<typename T>
    static T obtenerRegistroPorId(const char* nombreArchivo, int id);
    
    template<typename T>
    static T obtenerRegistroPorIndice(const char* nombreArchivo, int indice);
    
    template<typename T>
    static bool guardarNuevoRegistro(const char* nombreArchivo, T& registro);
    
    template<typename T>
    static bool actualizarRegistro(const char* nombreArchivo, int indice, const T& datosNuevos);
    
    template<typename T>
    static bool eliminarRegistroLogico(const char* nombreArchivo, int id);
    
    template<typename T>
    static bool existeEntidad(const char* nombreArchivo, int id);
    
    template<typename T>
    static int* buscarRegistroPorNombre(const char* nombreArchivo, const char* consulta, int* numResultados);
    
    // Operaciones específicas para detalles de transacciones (TODO: Implementar cuando existan las clases)
    // static bool guardarDetalle(const char* archivoDetalles, int idTransaccion, const DetalleTransaccion& detalle);
    // static void mostrarDetalleTransaccion(const char* archivoDetalles, const char* archivoProductos, const Transaccion& t);
    
    // Utilidades
    template<typename T>
    static int calcularPosicionRegistro(int indice);
    
private:
    static void obtenerFechaActual(char* buffer);
    static bool contieneSubstring(const char* texto, const char* busqueda);
};

// Implementación de templates
template<typename T>
int GestorArchivos::buscarPorId(const char* nombreArchivo, int id) {
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()) {
        cout << ROJO << "Error: No se pudo abrir el archivo " << nombreArchivo << endl << RESET;
        return -1;
    }

    ArchivoHeader header;
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));

    for (int i = 0; i < header.cantidadRegistros; i++) {
        T temp;
        archivo.read(reinterpret_cast<char*>(&temp), sizeof(T));
        if (!temp.isEliminado() && temp.getId() == id) {
            archivo.close();
            return i;
        }
    }
    archivo.close();
    return -1;
}

template<typename T>
T GestorArchivos::obtenerRegistroPorId(const char* nombreArchivo, int id) {
    int indice = buscarPorId<T>(nombreArchivo, id);
    T registro;
    
    if (indice != -1) {
        registro = obtenerRegistroPorIndice<T>(nombreArchivo, indice);
    }
    return registro;
}

template<typename T>
T GestorArchivos::obtenerRegistroPorIndice(const char* nombreArchivo, int indice) {
    T registro;
    ifstream archivo(nombreArchivo, ios::binary);
    if (archivo.is_open()) {
        int posicion = sizeof(ArchivoHeader) + (indice * sizeof(T));
        archivo.seekg(posicion, ios::beg);
        archivo.read(reinterpret_cast<char*>(&registro), sizeof(T));
        archivo.close();
    }
    return registro;
}

template<typename T>
bool GestorArchivos::guardarNuevoRegistro(const char* nombreArchivo, T& registro) {
    ArchivoHeader header = leerHeader(nombreArchivo);
    registro.setId(header.proximoID);
    registro.setEliminado(false);
    
    char fechaActual[MAX_FECHA];
    obtenerFechaActual(fechaActual);
    registro.setFechaRegistro(fechaActual);
    registro.setFechaUltimaModificacion(fechaActual);
    
    fstream archivo(nombreArchivo, ios::in | ios::out | ios::binary);
    if (!archivo.is_open()) {
        cout << ROJO << "Error: No se pudo abrir el archivo " << nombreArchivo << endl << RESET;
        return false;
    }
    
    int posicionEscritura = sizeof(ArchivoHeader) + (header.cantidadRegistros * sizeof(T));
    archivo.seekp(posicionEscritura, ios::beg);
    archivo.write(reinterpret_cast<const char*>(&registro), sizeof(T));
    
    if (archivo.fail()) {
        cout << ROJO << "Error crítico: No se pudo escribir en el archivo." << endl << RESET;
        archivo.close();
        return false;
    }

    header.cantidadRegistros++;
    header.proximoID++;
    header.registrosActivos++;
    
    archivo.close();
    return actualizarHeader(nombreArchivo, header);
}

template<typename T>
bool GestorArchivos::actualizarRegistro(const char* nombreArchivo, int indice, const T& datosNuevos) {
    fstream archivo(nombreArchivo, ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) return false;

    archivo.seekp(sizeof(ArchivoHeader) + (indice * sizeof(T)), ios::beg);
    archivo.write(reinterpret_cast<const char*>(&datosNuevos), sizeof(T));

    archivo.close();
    return true;
}

template<typename T>
bool GestorArchivos::eliminarRegistroLogico(const char* nombreArchivo, int id) {
    int indice = buscarPorId<T>(nombreArchivo, id);
    if (indice == -1) return false;
    
    fstream archivo(nombreArchivo, ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) {
        cout << ROJO << "Error: No se pudo abrir el archivo " << nombreArchivo << endl << RESET;
        return false;
    }
    
    T temp = obtenerRegistroPorIndice<T>(nombreArchivo, indice);
    temp.setEliminado(true);
    
    char fechaActual[MAX_FECHA];
    obtenerFechaActual(fechaActual);
    temp.setFechaUltimaModificacion(fechaActual);
    
    archivo.seekp(sizeof(ArchivoHeader) + (indice * sizeof(T)), ios::beg);
    archivo.write(reinterpret_cast<char*>(&temp), sizeof(T));
    
    ArchivoHeader header;
    archivo.seekg(0, ios::beg);
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    archivo.close();
    
    header.registrosActivos--;
    return actualizarHeader(nombreArchivo, header);
}

template<typename T>
bool GestorArchivos::existeEntidad(const char* nombreArchivo, int id) {
    return buscarPorId<T>(nombreArchivo, id) != -1;
}

template<typename T>
int* GestorArchivos::buscarRegistroPorNombre(const char* nombreArchivo, const char* consulta, int* numResultados) {
    ArchivoHeader header = leerHeader(nombreArchivo);
    *numResultados = 0;

    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()) {
        cout << ROJO << "Error abriendo archivo de busqueda." << endl << RESET;
        return nullptr;
    }

    T registro;
    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    
    // Primera pasada: contar coincidencias
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.read(reinterpret_cast<char*>(&registro), sizeof(T));
        if (!registro.isEliminado()) {
            if (contieneSubstring(registro.getNombre(), consulta)) {
                (*numResultados)++;
            }
        }
    }
    
    if (*numResultados == 0) {
        archivo.close();
        return nullptr;
    }
    
    // Reservar memoria y segunda pasada: recolectar índices
    int* indices = new int[*numResultados];
    int posicion = 0;

    archivo.clear();
    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.read(reinterpret_cast<char*>(&registro), sizeof(T));
        if (!registro.isEliminado()) {
            if (contieneSubstring(registro.getNombre(), consulta)) {
                indices[posicion] = i;
                posicion++;
            }
        }
    }
    
    archivo.close();
    return indices;
}

template<typename T>
int GestorArchivos::calcularPosicionRegistro(int indice) {
    return sizeof(ArchivoHeader) + (indice * sizeof(T));
}

#endif // GESTORARCHIVOS_HPP
