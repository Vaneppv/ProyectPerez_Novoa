#ifndef VALIDACIONES_HPP
#define VALIDACIONES_HPP

#include <cstring>
#include <cctype>

class Validaciones {
public:
    // Validaciones de formato generales (métodos estáticos)
    static bool validarEmail(const char* email);
    static bool validarRIF(const char* rif);
    static bool validarTelefono(const char* telefono);
    static bool validarCedula(const char* cedula);
    static bool validarFecha(const char* fecha);
    static bool validarRango(int valor, int min, int max);
    static bool validarRangoFloat(float valor, float min, float max);
    static bool validarCodigoProducto(const char* codigo);
    static bool validarNombre(const char* nombre);
    static bool validarDescripcion(const char* descripcion);
    static bool validarTipoTransaccion(const char* tipo);
    
    // Validaciones de formato específico
    static bool tieneArroba(const char* email);
    static bool tienePuntoDespuesArroba(const char* email);
    static bool esNumerico(const char* cadena);
    static bool formatoFechaCorrecto(const char* fecha);
    static bool longitudValida(const char* cadena, int min, int max);
    
    // Utilidades de validación
    static void convertirAMinusculas(char* cadena);
    static void quitarEspacios(char* cadena);
    static bool esVacio(const char* cadena);
};

#endif // VALIDACIONES_HPP
