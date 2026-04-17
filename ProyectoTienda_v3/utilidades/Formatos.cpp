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
    ss.imbue(locale("C")); 
    ss << fixed << setprecision(2) << cantidad;
    
    string s = ss.str();
    
    size_t punto = s.find('.');
    if (punto != string::npos) {
        s.replace(punto, 1, ",");
    }
    
    return "$" + s;
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
    cout << CYAN << "Información: " << mensaje << RESET << endl;
}

// Formatos de presentación de datos
void Formatos::alinearDerecha(int ancho) {
    cout << setw(ancho) << right;
}

void Formatos::alinearCentro(int ancho) {
    cout << setw(ancho);
}

string Formatos::truncarTexto(string texto, int limite) {
    if (static_cast<int>(texto.length()) > limite) {
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
    imprimirSeparador(130, '=');
    cout << CYAN << NEGRITA << left;
    cout << setw(5)  << "ID";
    cout << setw(10) << "COD";
    cout << setw(18) << "NOMBRE";
    cout << setw(25) << "DESCRIPCION";
    cout << setw(15) << "PROVEEDOR";
    cout << setw(10) << "PRECIO";
    cout << setw(6)  << "STK";   
    cout << setw(6)  << "MIN";   
    cout << setw(11) << "REGISTRO";
    cout << setw(11) << "ULT.MOD";
    cout << setw(8)  << "ESTADO" << RESET << endl;
    imprimirSeparador(130, '-');
}

void Formatos::EncabezadoBasicoProveedor() {
    cout << CYAN << setw(5) << "ID" << setw(15) << "RIF" << setw(20) << "Nombre" << setw(15) << "Telefono" << setw(30) << "Direccion" << RESET << endl;
}

void Formatos::EncabezadoCompletoProveedor() {
    imprimirSeparador(130, '=');
    cout << CYAN << left;
    cout << setw(5)  << "ID";
    cout << setw(11) << "RIF";
    cout << setw(17) << "NOMBRE";
    cout << setw(11) << "TELEFONO";
    cout << setw(18) << "DIRECCION";
    cout << setw(17) << "EMAIL"; 
    cout << setw(6)  << "PRD";   // Productos
    cout << setw(9)  << "COMPRAS";
    cout << setw(11) << "REGISTRO";
    cout << setw(11) << "ULT.MOD";
    cout << setw(8)  << "ESTADO" << RESET << endl;
    imprimirSeparador(130, '-');
}

void Formatos::EncabezadoBasicoCliente() {
    cout << CYAN << setw(5) << "ID" << setw(15) << "Cedula" << setw(20) << "Nombre" << setw(15) << "Telefono" << setw(30) << "Direccion" << RESET << endl;
}

void Formatos::EncabezadoCompletoCliente() {
    imprimirSeparador(130, '=');
    cout << CYAN << left;
    cout << setw(5)  << "ID";
    cout << setw(11) << "CEDULA";
    cout << setw(18) << "NOMBRE";
    cout << setw(12) << "TELEFONO";
    cout << setw(20) << "DIRECCION";
    cout << setw(18) << "EMAIL"; 
    cout << setw(9)  << "COMPRAS";
    cout << setw(6)  << "TRA";    // Transacciones
    cout << setw(11) << "REGISTRO";
    cout << setw(11) << "ULT.MOD";
    cout << setw(9)  << "ESTADO" << RESET << endl;
    imprimirSeparador(130, '-');
}

void Formatos::EncabezadoBasicoTransacciones() {
    cout << CYAN << left;
    cout << setw(10) << "ID TRANS";
    cout << setw(12) << "Tipo";
    cout << setw(15) << "Total";
    cout << setw(15) << "Fecha Reg";
    cout << setw(8) << "Items" << RESET << endl;
}

void Formatos::EncabezadoCompletoTransacciones() {
    imprimirSeparador(130, '=');
    cout << CYAN << NEGRITA << left;
    cout << setw(10) << "ID TRANS"; 
    cout << setw(12) << "TIPO";       
    cout << setw(8)  << "ID REL";     
    cout << setw(15) << "TOTAL";      
    cout << setw(18) << "DESCRIPCION"; 
    cout << setw(12) << "REGISTRO";
    cout << setw(8)  << "ITEMS";
    cout << setw(12) << "ULT.MOD";
    cout << setw(10) << "ESTADO" << RESET << endl;
    imprimirSeparador(130, '-');
}

void Formatos::EncabezadoDetalles() {
    imprimirSeparador(80, '=');
    cout << CYAN << left;
    cout << setw(10) << "ID Prod.";
    cout << setw(12) << "Cantidad";
    cout << setw(12) << "Precio";
    cout << setw(12) << "ID Trans.";
    cout << setw(20) << "Subtotal" << RESET << endl;
    imprimirSeparador(80, '-');
}

void Formatos::imprimirPieTabla() {
    imprimirSeparador(130, '=');
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
