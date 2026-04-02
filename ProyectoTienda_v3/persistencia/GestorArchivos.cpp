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

bool GestorArchivos::guardarDetalle(const char* archivoDetalles, int idTransaccion, const DetalleTransaccion& detalle) {
    ofstream archivo(archivoDetalles, ios::binary | ios::app);
    if (!archivo.is_open()) {
        cout << ROJO << "Error: No se pudo abrir el archivo de detalles" << endl << RESET;
        return false;
    }

    archivo.write(reinterpret_cast<const char*>(&detalle), sizeof(DetalleTransaccion));
    archivo.close();
    return true;
}

void GestorArchivos::mostrarDetalleTransaccion(const char* archivoDetalles, const char* archivoProductos, const Transaccion& t) {
    ifstream archivo(archivoDetalles, ios::binary);
    if (!archivo.is_open()) {
        cout << ROJO << "Error: no se pudo abrir el archivo de detalles." << endl << RESET;
        return;
    }

    DetalleTransaccion d;
    bool tieneDetalles = false;

    while (archivo.read(reinterpret_cast<char*>(&d), sizeof(DetalleTransaccion))) {
        if (d.getIdTransaccion() == t.getId()) {
            if (!tieneDetalles) {
                Formatos::imprimirSubtitulo("DETALLES DE TRANSACCIÓN");
                tieneDetalles = true;
            }

            int indP = buscarPorId<Producto>(archivoProductos, d.getIdProducto());
            char nombreProd[MAX_NOMBRE] = "No encontrado";
            
            if (indP != -1) {
                Producto p = obtenerRegistroPorIndice<Producto>(archivoProductos, indP);
                strncpy(nombreProd, p.getNombre(), MAX_NOMBRE - 1);
                nombreProd[MAX_NOMBRE - 1] = '\0';
            }

            float totalLinea = d.calcularSubtotal();

            cout << CYAN;
            cout << "ID Producto: " << d.getIdProducto() << " | ";
            cout << "Nombre: " << nombreProd << " | ";
            cout << "Cantidad: " << d.getCantidad() << " | ";
            cout << "Precio Unitario: $" << Formatos::formatearMoneda(d.getPrecioUnitario()) << " | ";
            cout << "Subtotal: $" << Formatos::formatearMoneda(totalLinea) << RESET << endl;
        }
    }
    
    if (!tieneDetalles) {
        cout << AMARILLO << "No se encontraron detalles para la transacción " << t.getId() << endl << RESET;
    } else {
        Formatos::imprimirSeparador();
        cout << CYAN;
        cout << right << setw(72) << "TOTAL GENERAL: $" << Formatos::formatearMoneda(t.getTotal()) << endl << RESET;
    }

    archivo.close();
}

// Métodos privados
void GestorArchivos::obtenerFechaActual(char* buffer) {
    time_t tiempo_actual = time(nullptr);
    struct tm* tm_info = localtime(&tiempo_actual);
    strftime(buffer, MAX_FECHA, "%d/%m/%Y", tm_info);
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
