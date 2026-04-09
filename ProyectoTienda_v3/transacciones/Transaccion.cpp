#include "Transaccion.hpp"
#include "../utilidades/Validaciones.hpp"
#include "../utilidades/Formatos.hpp"
#include <cstring>
#include <iostream>

using namespace std;

// Constructor por defecto
Transaccion::Transaccion() {
    id = 0;
    strncpy(tipo, "", MAX_TIPO_TRANSACCION - 1);
    tipo[MAX_TIPO_TRANSACCION - 1] = '\0';
    idRelacionado = 0;
    total = 0.0f;
    strncpy(descripcion, "", MAX_DESCRIPCION - 1);
    descripcion[MAX_DESCRIPCION - 1] = '\0';
    strncpy(fechaRegistro, "", MAX_FECHA - 1);
    fechaRegistro[MAX_FECHA - 1] = '\0';
    cantidadItemsDiferentes = 0;
    eliminado = false;
    strncpy(fechaUltimaModificacion, "", MAX_FECHA - 1);
    fechaUltimaModificacion[MAX_FECHA - 1] = '\0';
}

// Constructor parametrizado
Transaccion::Transaccion(int id, const char* tipo, int idRelacionado, float total, const char* descripcion) {
    this->id = id;
    strncpy(this->tipo, tipo, MAX_TIPO_TRANSACCION - 1);
    this->tipo[MAX_TIPO_TRANSACCION - 1] = '\0';
    this->idRelacionado = idRelacionado;
    this->total = total;
    strncpy(this->descripcion, descripcion, MAX_DESCRIPCION - 1);
    this->descripcion[MAX_DESCRIPCION - 1] = '\0';
    strncpy(fechaRegistro, "", MAX_FECHA - 1);
    fechaRegistro[MAX_FECHA - 1] = '\0';
    cantidadItemsDiferentes = 0;
    eliminado = false;
    strncpy(fechaUltimaModificacion, "", MAX_FECHA - 1);
    fechaUltimaModificacion[MAX_FECHA - 1] = '\0';
}

// Destructor
Transaccion::~Transaccion() {
}

// Getters
int Transaccion::getId() const { return id; }
const char* Transaccion::getTipo() const { return tipo; }
int Transaccion::getIdRelacionado() const { return idRelacionado; }
float Transaccion::getTotal() const { return total; }
const char* Transaccion::getDescripcion() const { return descripcion; }
const char* Transaccion::getFechaRegistro() const { return fechaRegistro; }
int Transaccion::getCantidadItemsDiferentes() const { return cantidadItemsDiferentes; }
bool Transaccion::isEliminado() const { return eliminado; }
const char* Transaccion::getFechaUltimaModificacion() const { return fechaUltimaModificacion; }

// Setters (con validación)
void Transaccion::setId(int id) {
    if (id >= 0) {
        this->id = id;
    }
}

void Transaccion::setTipo(const char* tipo) {
    if (tipoValido(tipo)) {
        strncpy(this->tipo, tipo, MAX_TIPO_TRANSACCION - 1);
        this->tipo[MAX_TIPO_TRANSACCION - 1] = '\0';
    }
}

void Transaccion::setIdRelacionado(int idRelacionado) {
    if (idRelacionado > 0) {
        this->idRelacionado = idRelacionado;
    }
}

void Transaccion::setTotal(float total) {
    if (totalValido(total)) {
        this->total = total;
    }
}

void Transaccion::setDescripcion(const char* descripcion) {
    if (strlen(descripcion) < MAX_DESCRIPCION) {
        strncpy(this->descripcion, descripcion, MAX_DESCRIPCION - 1);
        this->descripcion[MAX_DESCRIPCION - 1] = '\0';
    }
}

void Transaccion::setFechaRegistro(const char* fechaRegistro) {
    if (Validaciones::validarFecha(fechaRegistro)) {
        strncpy(this->fechaRegistro, fechaRegistro, MAX_FECHA - 1);
        this->fechaRegistro[MAX_FECHA - 1] = '\0';
    }
}

void Transaccion::setCantidadItemsDiferentes(int cantidad) {
    if (cantidad >= 0) {
        cantidadItemsDiferentes = cantidad;
    }
}

void Transaccion::setEliminado(bool eliminado) {
    this->eliminado = eliminado;
}

void Transaccion::setFechaUltimaModificacion(const char* fechaUltimaModificacion) {
    if (Validaciones::validarFecha(fechaUltimaModificacion)) {
        strncpy(this->fechaUltimaModificacion, fechaUltimaModificacion, MAX_FECHA - 1);
        this->fechaUltimaModificacion[MAX_FECHA - 1] = '\0';
    }
}

// Métodos de validación propios de la entidad
bool Transaccion::esValido() const {
    return tieneDatosCompletos() && tipoValido(tipo) && totalValido(total) && idRelacionado > 0;
}

bool Transaccion::tieneDatosCompletos() const {
    return strlen(tipo) > 0 && idRelacionado > 0;
}

bool Transaccion::tipoValido(const char* tipo) const {
    return Validaciones::validarTipoTransaccion(tipo);
}

bool Transaccion::totalValido(float total) const {
    return Validaciones::validarRangoFloat(total, 0.01f, 999999999.99f);
}

// Métodos de presentación
void Transaccion::mostrarInformacionBasica() const {
    cout << CYAN << "ID: " << id << " | Tipo: " << tipo << " | Total: $" << Formatos::formatearMoneda(total) 
              << " | Fecha: " << fechaRegistro << " | Items: " << cantidadItemsDiferentes << RESET << endl;
}

void Transaccion::mostrarInformacionCompleta() const {
    cout << CYAN;
    cout << "ID: " << id << endl;
    cout << "Tipo: " << tipo << endl;
    cout << "ID Relacionado: " << idRelacionado << endl;
    cout << "Total: $" << Formatos::formatearMoneda(total) << endl;
    cout << "Descripción: " << descripcion << endl;
    cout << "Fecha Registro: " << fechaRegistro << endl;
    cout << "Cantidad Items Diferentes: " << cantidadItemsDiferentes << endl;
    cout << "Fecha Última Modificación: " << fechaUltimaModificacion << endl;
    cout << "Estado: " << (eliminado ? "Eliminada" : "Activa") << RESET << endl;
}

// Métodos de negocio
bool Transaccion::esCompra() const {
    return strcmp(tipo, "COMPRA") == 0;
}

bool Transaccion::esVenta() const {
    return strcmp(tipo, "VENTA") == 0;
}

void Transaccion::agregarDetalle(float monto) {
    if (monto > 0) {
        total += monto;
    }
}

void Transaccion::incrementarCantidadItems() {
    cantidadItemsDiferentes++;
}

// Método estático
int Transaccion::obtenerTamano() {
    return sizeof(Transaccion);
}
