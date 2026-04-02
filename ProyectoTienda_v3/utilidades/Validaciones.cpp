#include "Validaciones.hpp"
#include <cstring>
#include <cctype>
#include <iostream>

using namespace std;

// Validaciones de formato generales
bool Validaciones::validarEmail(const char* email) {
    if (!email || strlen(email) == 0) return false;
    
    return tieneArroba(email) && tienePuntoDespuesArroba(email) && 
           longitudValida(email, 5, MAX_EMAIL - 1);
}

bool Validaciones::validarRIF(const char* rif) {
    if (!rif || strlen(rif) < 8) return false;
    
    // Formato: J-12345678-9 o V-12345678
    if (strlen(rif) >= 8 && strlen(rif) <= 12) {
        if (rif[0] == 'J' || rif[0] == 'V' || rif[0] == 'E' || rif[0] == 'G') {
            if (rif[1] == '-') {
                // Verificar que los caracteres después del guión son numéricos
                for (int i = 2; i < strlen(rif); i++) {
                    if (rif[i] == '-' && i > 2) continue; // Permitir segundo guión
                    if (!isdigit(rif[i])) return false;
                }
                return true;
            }
        }
    }
    return false;
}

bool Validaciones::validarTelefono(const char* telefono) {
    if (!telefono || strlen(telefono) == 0) return false;
    
    // Permitir formatos: 0414-1234567, 04141234567, +584141234567
    int longitud = strlen(telefono);
    if (longitud < 10 || longitud > 15) return false;
    
    int digitos = 0;
    for (int i = 0; i < longitud; i++) {
        if (isdigit(telefono[i])) {
            digitos++;
        } else if (telefono[i] != '-' && telefono[i] != '+' && telefono[i] != ' ') {
            return false;
        }
    }
    
    return digitos >= 10;
}

bool Validaciones::validarCedula(const char* cedula) {
    if (!cedula || strlen(cedula) < 7) return false;
    
    // Formato: V-12345678 o E-12345678
    if (strlen(cedula) >= 8 && strlen(cedula) <= 10) {
        if (cedula[0] == 'V' || cedula[0] == 'E') {
            if (cedula[1] == '-') {
                // Verificar que los caracteres después del guión son numéricos
                for (int i = 2; i < strlen(cedula); i++) {
                    if (!isdigit(cedula[i])) return false;
                }
                return true;
            }
        }
    }
    return false;
}

bool Validaciones::validarFecha(const char* fecha) {
    if (!fecha || strlen(fecha) != 10) return false;
    
    // Formato: DD/MM/AAAA
    if (fecha[2] != '/' || fecha[5] != '/') return false;
    
    // Verificar que los caracteres sean dígitos excepto las barras
    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) continue;
        if (!isdigit(fecha[i])) return false;
    }
    
    // Extraer día, mes, año
    int dia = (fecha[0] - '0') * 10 + (fecha[1] - '0');
    int mes = (fecha[3] - '0') * 10 + (fecha[4] - '0');
    int anio = (fecha[6] - '0') * 1000 + (fecha[7] - '0') * 100 + 
              (fecha[8] - '0') * 10 + (fecha[9] - '0');
    
    // Validar rangos
    if (dia < 1 || dia > 31) return false;
    if (mes < 1 || mes > 12) return false;
    if (anio < 1900 || anio > 2100) return false;
    
    return true;
}

bool Validaciones::validarRango(int valor, int min, int max) {
    return valor >= min && valor <= max;
}

bool Validaciones::validarRangoFloat(float valor, float min, float max) {
    return valor >= min && valor <= max;
}

bool Validaciones::validarCodigoProducto(const char* codigo) {
    if (!codigo || strlen(codigo) == 0) return false;
    return longitudValida(codigo, 3, MAX_CODIGO - 1);
}

bool Validaciones::validarNombre(const char* nombre) {
    if (!nombre || strlen(nombre) == 0) return false;
    return longitudValida(nombre, 2, MAX_NOMBRE - 1);
}

bool Validaciones::validarDescripcion(const char* descripcion) {
    if (!descripcion) return false;
    return strlen(descripcion) <= MAX_DESCRIPCION - 1;
}

bool Validaciones::validarTipoTransaccion(const char* tipo) {
    if (!tipo) return false;
    return strcmp(tipo, "COMPRA") == 0 || strcmp(tipo, "VENTA") == 0;
}

// Validaciones de formato específico
bool Validaciones::tieneArroba(const char* email) {
    if (!email) return false;
    return strchr(email, '@') != nullptr;
}

bool Validaciones::tienePuntoDespuesArroba(const char* email) {
    if (!email) return false;
    const char* arroba = strchr(email, '@');
    if (!arroba) return false;
    
    const char* punto = strchr(arroba + 1, '.');
    return punto != nullptr && (punto - arroba) > 1;
}

bool Validaciones::esNumerico(const char* cadena) {
    if (!cadena) return false;
    
    for (int i = 0; cadena[i] != '\0'; i++) {
        if (!isdigit(cadena[i])) return false;
    }
    return true;
}

bool Validaciones::formatoFechaCorrecto(const char* fecha) {
    return validarFecha(fecha);
}

bool Validaciones::longitudValida(const char* cadena, int min, int max) {
    if (!cadena) return false;
    int longitud = strlen(cadena);
    return longitud >= min && longitud <= max;
}

// Utilidades de validación
void Validaciones::convertirAMinusculas(char* cadena) {
    if (!cadena) return;
    
    for (int i = 0; cadena[i] != '\0'; i++) {
        cadena[i] = tolower(cadena[i]);
    }
}

void Validaciones::quitarEspacios(char* cadena) {
    if (!cadena) return;
    
    int i = 0, j = 0;
    while (cadena[i]) {
        if (cadena[i] != ' ') {
            cadena[j++] = cadena[i];
        }
        i++;
    }
    cadena[j] = '\0';
}

bool Validaciones::esVacio(const char* cadena) {
    if (!cadena) return true;
    
    for (int i = 0; cadena[i] != '\0'; i++) {
        if (!isspace(cadena[i])) return false;
    }
    return true;
}
