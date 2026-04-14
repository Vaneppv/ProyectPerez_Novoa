#include "DetalleTransaccion.hpp"
#include "../utilidades/Validaciones.hpp"
#include "../utilidades/Formatos.hpp"
#include <iostream>

// Constructor por defecto
DetalleTransaccion::DetalleTransaccion() {
    idProducto = 0;
    cantidad = 0;
    precioUnitario = 0.0f;
    idTransaccion = 0;
    eliminado = false;
}

// Constructor parametrizado
DetalleTransaccion::DetalleTransaccion(int idProducto, int cantidad, float precioUnitario, int idTransaccion) {
    this->idProducto = idProducto;
    this->cantidad = cantidad;
    this->precioUnitario = precioUnitario;
    this->idTransaccion = idTransaccion;
    this->eliminado = false;
}

// Destructor
DetalleTransaccion::~DetalleTransaccion() {
}

// Getters
int DetalleTransaccion::getIdProducto() const { return idProducto; }
int DetalleTransaccion::getCantidad() const { return cantidad; }
float DetalleTransaccion::getPrecioUnitario() const { return precioUnitario; }
int DetalleTransaccion::getIdTransaccion() const { return idTransaccion; }

// Setters (con validación)
void DetalleTransaccion::setIdProducto(int idProducto) {
    if (idProducto > 0) {
        this->idProducto = idProducto;
    }
}

void DetalleTransaccion::setCantidad(int cantidad) {
    if (cantidadValida(cantidad)) {
        this->cantidad = cantidad;
    }
}

void DetalleTransaccion::setPrecioUnitario(float precioUnitario) {
    if (precioValido(precioUnitario)) {
        this->precioUnitario = precioUnitario;
    }
}

void DetalleTransaccion::setIdTransaccion(int idTransaccion) {
    if (idTransaccion > 0) {
        this->idTransaccion = idTransaccion;
    }
}

// Métodos de validación propios de la entidad
bool DetalleTransaccion::esValido() const {
    return idProducto > 0 && cantidadValida(cantidad) && precioValido(precioUnitario) && idTransaccion > 0;
}

bool DetalleTransaccion::cantidadValida(int cantidad) const {
    return Validaciones::validarRango(cantidad, 1, 999999);
}

bool DetalleTransaccion::precioValido(float precio) const {
    return Validaciones::validarRangoFloat(precio, 0.01f, 999999.99f);
}

// Métodos de presentación

void DetalleTransaccion::mostrarInformacion() const {
    cout << CYAN;
    cout << setw(5) << idProducto;
    cout << setw(10) << cantidad;
    cout << setw(15) << Formatos::formatearMoneda(precioUnitario);
    cout << setw(5) << idTransaccion;
    cout << setw(15) << Formatos::formatearMoneda(calcularSubtotal()) << RESET << endl;
}

// Métodos de negocio
float DetalleTransaccion::calcularSubtotal() const {
    return cantidad * precioUnitario;
}

// Agregados para compatibilidad con template GestorArchivos
int DetalleTransaccion::getId() const { return idProducto; }
void DetalleTransaccion::setEliminado(bool eliminado) { this->eliminado = eliminado; }
const char* DetalleTransaccion::getTipo() const { return ""; } // No aplica para detalles
bool DetalleTransaccion::isEliminado() const { return eliminado; }
void DetalleTransaccion::setId(int id) { this->idProducto = id; } // Corregido para asignar a idProducto
const char* DetalleTransaccion::getDescripcion() const { return ""; } // No aplica para detalles

// Métodos adicionales para compatibilidad con template
void DetalleTransaccion::setFechaRegistro(const char* fecha) {
    // DetalleTransaccion no tiene fechaRegistro, método vacío para compatibilidad
}

void DetalleTransaccion::setFechaUltimaModificacion(const char* fecha) {
    // DetalleTransaccion no tiene fechaUltimaModificacion, método vacío para compatibilidad
}

// Método estático
int DetalleTransaccion::obtenerTamano() {
    return sizeof(DetalleTransaccion);
}
