#include "Formatos.hpp"
#include <ctime>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <limits>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

// Formatos de salida
void Formatos::formatearFecha(time_t tiempo, char* buffer) {
    struct tm* tm_info = localtime(&tiempo);
    strftime(buffer, MAX_FECHA, "%d/%m/%Y", tm_info);
}

void Formatos::formatearFechaActual(char* buffer) {
    time_t tiempo_actual = time(nullptr);
    formatearFecha(tiempo_actual, buffer);
}

string Formatos::formatearMoneda(float cantidad) {
    stringstream ss;
    ss.imbue(locale("es_VE.UTF-8"));
    ss << fixed << setprecision(2) << cantidad;
    return ss.str();
}

void Formatos::convertirAMayusculas(char* cadena) {
    if (!cadena) return;
    
    for (int i = 0; cadena[i] != '\0'; i++) {
        cadena[i] = toupper(cadena[i]);
    }
}

void Formatos::convertirAMinusculas(char* cadena) {
    if (!cadena) return;
    
    for (int i = 0; cadena[i] != '\0'; i++) {
        cadena[i] = tolower(cadena[i]);
    }
}

void Formatos::capitalizar(char* cadena) {
    if (!cadena || strlen(cadena) == 0) return;
    
    // Primera letra en mayúscula
    cadena[0] = toupper(cadena[0]);
    
    // Resto en minúsculas
    for (int i = 1; cadena[i] != '\0'; i++) {
        if (cadena[i-1] == ' ') {
            cadena[i] = toupper(cadena[i]);
        } else {
            cadena[i] = tolower(cadena[i]);
        }
    }
}

// Utilidades de interfaz
void Formatos::limpiarPantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Formatos::limpiarBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
void Formatos::pausar() {
    pausar("Presione Enter para continuar...");
}

void Formatos::pausar(const char* mensaje) {
    cout << CYAN << mensaje << RESET;
    limpiarBuffer();
    cin.get();
}

void Formatos::imprimirSeparador(int ancho, char simbolo) {
    cout << CYAN;
    for (int i = 0; i < ancho; i++) {
        cout << simbolo;
    }
    cout << RESET << endl;
}

void Formatos::imprimirTitulo(const char* titulo) {
    int longitud = strlen(titulo);
    int padding = (80 - longitud) / 2;
    
    imprimirSeparador(80, '=');
    cout << AZUL << NEGRITA;
    cout << setw(padding) << "" << titulo << endl;
    cout << RESET;
    imprimirSeparador(80, '=');
}

void Formatos::imprimirSubtitulo(const char* subtitulo) {
    cout << AZUL << NEGRITA << subtitulo << RESET << endl;
    imprimirSeparador(60, '-');
}

void Formatos::imprimirError(const char* mensaje) {
    cout << ROJO << NEGRITA << "ERROR: " << mensaje << RESET << endl;
}

void Formatos::imprimirExito(const char* mensaje) {
    cout << VERDE << NEGRITA << "ÉXITO: " << mensaje << RESET << endl;
}

void Formatos::imprimirAdvertencia(const char* mensaje) {
    cout << AMARILLO << NEGRITA << "ADVERTENCIA: " << mensaje << RESET << endl;
}

void Formatos::imprimirInformacion(const char* mensaje) {
    cout << CYAN << "INFO: " << mensaje << RESET << endl;
}

// Formatos de presentación de datos
void Formatos::alinearDerecha(int ancho) {
    cout << setw(ancho) << right;
}

void Formatos::alinearCentro(int ancho) {
    cout << setw(ancho);
}

string Formatos::truncarTexto(string texto, int limite) {
    if (texto.length() > limite) {
        // Corta el texto y deja espacio para los 3 puntos
        return texto.substr(0, limite - 3) + "...";
    }
    return texto;
}

void Formatos::EncabezadoBasicoProducto() {
    cout << CYAN << setw(5) << "ID" << setw(15) << "Código" << setw(20) << "Nombre" 
         << setw(10) << "Precio" << setw(10) << "Stock" << RESET << endl;
}

void Formatos::EncabezadoCompletoProducto() {
    imprimirSeparador(160, '=');
    cout << CYAN << NEGRITA;
    cout << left;
    cout << setw(5) << "ID";
    cout << setw(15) << "CÓDIGO";
    cout << setw(20) << "NOMBRE";
    cout << setw(30) << "DESCRIPCIÓN";
    cout << setw(15) << "PROVEEDOR";
    cout << setw(10) << "PRECIO";
    cout << setw(10) << "STOCK ACT";
    cout << setw(5) << "STOCK MIN";
    cout << setw(15) << "TOTAL VENDIDO";
    cout << setw(12) << "FECHA REG";
    cout << setw(12) << "FECHA MOD";
    cout << setw(15) << "ESTADO" << RESET << endl;
    imprimirSeparador(160, '-');
}

void Formatos::EncabezadoBasicoProveedor() {
    cout << CYAN << setw(5) << "ID" << setw(15) << "RIF" << setw(20) << "Nombre" << setw(10) << "Telefono" << setw(50) << "Direccion" << RESET << endl;
}

void Formatos::EncabezadoCompletoProveedor() {

    cout << CYAN;
    cout << left;
    cout << setw(5) << "ID";
    cout << setw(15) << "Rif";
    cout << setw(20) << "Nombre";
    cout << setw(12) << "Telefono";
    cout << setw(30) << "Direccion";
    cout << setw(20) << "Email";
    cout << setw(8) << "Cantidad de Productos";
    cout << setw(12) << "Total de Compras";
    cout << setw(8) << "Cantidad de Transacciones";
    cout << setw(12) << "Fecha Registro";
    cout << setw(12) << "Fecha Última Modificación";
    cout << setw(15) << "Estado" << RESET << endl;
}

void Formatos::EncabezadoBasicoCliente() {
    cout << CYAN << setw(5) << "ID" << setw(15) << "Cedula" << setw(20) << "Nombre" << setw(12) << "Telefono" << setw(30) << "Direccion" << RESET << endl;
}

void Formatos::EncabezadoCompletoCliente() {
    cout << CYAN;
    cout << left;
    cout << setw(5) << "ID";
    cout << setw(15) << "Cedula";
    cout << setw(20) << "Nombre";
    cout << setw(12) << "Telefono";
    cout << setw(30) << "Direccion";
    cout << setw(20) << "Email";
    cout << setw(12) << "Total de Compras";
    cout << setw(8) << "Cantidad de Transacciones";
    cout << setw(12) << "Fecha Registro";
    cout << setw(12) << "Fecha Última Modificación";
    cout << setw(15) << "Estado" << RESET << endl;
}

void Formatos::EncabezadoBasicoTransacciones() {
    cout << CYAN;
    cout << left;
    cout << setw(5) << "ID TRANS";
    cout << setw(10) << "Tipo";
    cout << setw(15) << "Total";
    cout << setw(12) << "Fecha Reg";
    cout << setw(5) << "Items" << RESET << endl;
}

void Formatos::EncabezadoCompletoTransacciones() {
    cout << CYAN;
    cout << left;
    cout << setw(5) << "ID TRANS";
    cout << setw(10) << "Tipo";
    cout << setw(5) << "ID";
    cout << setw(15) << "Total";
    cout << setw(30) << "Descripción";
    cout << setw(12) << "Fecha Reg";
    cout << setw(5) << "Items";
    cout << setw(12) << "Fecha Mod";
    cout << setw(15) << "Estado" << RESET << endl;
}

void Formatos::EncabezadoDetalles() {
    cout << CYAN;
    cout << left;
    cout << setw(5) << "ID Producto";
    cout << setw(10) << "Cantidad";
    cout << setw(15) << "Precio Unitario";
    cout << setw(5) << "ID Transacción";
    cout << setw(15) << "Subtotal" << RESET << endl;
}

void Formatos::imprimirPieTabla() {
    imprimirSeparador(160, '=');
}

// Colores y estilos
void Formatos::setColor(const char* color) {
    cout << color;
}

void Formatos::resetColor() {
    cout << RESET;
}

void Formatos::establecerNegrita() {
    cout << NEGRITA;
}

void Formatos::quitarNegrita() {
    cout << RESET;
}

// Métodos privados
void Formatos::obtenerFechaActual(char* buffer) {
    formatearFechaActual(buffer);
}
