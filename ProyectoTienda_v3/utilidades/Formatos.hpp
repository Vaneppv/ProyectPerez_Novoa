#ifndef FORMATOS_HPP
#define FORMATOS_HPP

#include <ctime>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <string>
#include "../persistencia/Constantes.hpp"

using namespace std;

class Formatos {
public:
    // Formatos de salida (métodos estáticos)
    static void formatearFecha(time_t tiempo, char* buffer);
    static void formatearFechaActual(char* buffer);
    static string formatearMoneda(float cantidad);
    static void convertirAMayusculas(char* cadena);
    static void convertirAMinusculas(char* cadena);
    static void capitalizar(char* cadena);
    
    // Utilidades de interfaz
    static void limpiarPantalla();
    static void limpiarBuffer();
    static void pausar();
    static void pausar(const char* mensaje);
    static void imprimirSeparador(int ancho = 160, char simbolo = '=');
    static void imprimirTitulo(const char* titulo);
    static void imprimirSubtitulo(const char* subtitulo);
    static void imprimirError(const char* mensaje);
    static void imprimirExito(const char* mensaje);
    static void imprimirAdvertencia(const char* mensaje);
    static void imprimirInformacion(const char* mensaje);
    
    // Formatos de presentación de datos
    static void alinearDerecha(int ancho);
    static void alinearCentro(int ancho);
    static string truncarTexto(string texto, int limite);
    static void EncabezadoBasicoProducto();
    static void EncabezadoCompletoProducto();
    static void EncabezadoBasicoProveedor();
    static void EncabezadoCompletoProveedor();
    static void EncabezadoBasicoCliente();
    static void EncabezadoCompletoCliente();
    static void EncabezadoBasicoTransacciones();
    static void EncabezadoCompletoTransacciones();
    static void EncabezadoDetalles();
    static void imprimirPieTabla();
    
    // Colores y estilos
    static void setColor(const char* color);
    static void resetColor();
    static void establecerNegrita();
    static void quitarNegrita();
    
private:
    static void obtenerFechaActual(char* buffer);
};

#endif // FORMATOS_HPP
