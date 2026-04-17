#include "Cliente.hpp"
#include "../utilidades/Validaciones.hpp"
#include "../utilidades/Formatos.hpp"
#include <cstring>
#include <iostream>

using namespace std;

// Constructor por defecto
Cliente::Cliente() {
    id = 0;
    strncpy(cedula, "", MAX_CEDULA - 1);
    cedula[MAX_CEDULA - 1] = '\0';
    strncpy(nombre, "", MAX_NOMBRE - 1);
    nombre[MAX_NOMBRE - 1] = '\0';
    strncpy(telefono, "", MAX_TELEFONO - 1);
    telefono[MAX_TELEFONO - 1] = '\0';
    strncpy(email, "", MAX_EMAIL - 1);
    email[MAX_EMAIL - 1] = '\0';
    strncpy(direccion, "", MAX_DIRECCION - 1);
    direccion[MAX_DIRECCION - 1] = '\0';
    totalCompras = 0.0f;
    cantidadTransacciones = 0;
    strncpy(fechaRegistro, "", MAX_FECHA - 1);
    fechaRegistro[MAX_FECHA - 1] = '\0';
    eliminado = false;
    strncpy(fechaUltimaModificacion, "", MAX_FECHA - 1);
    fechaUltimaModificacion[MAX_FECHA - 1] = '\0';
	for (int i = 0; i < MAX_TRANSACCIONES_ENTIDAD; i++) {
    	transaccionesIds[MAX_TRANSACCIONES_ENTIDAD - 1] = 0;
	}
}

// Constructor parametrizado
Cliente::Cliente(int id, const char* cedula, const char* nombre, const char* telefono, 
             const char* email, const char* direccion, float totalCompras, 
			 const int* transaccionesIds, int cantidadTransacciones) {
			 	
    this->id = id;
    strncpy(this->cedula, cedula, MAX_CEDULA - 1);
    this->cedula[MAX_CEDULA - 1] = '\0';
    strncpy(this->nombre, nombre, MAX_NOMBRE - 1);
    this->nombre[MAX_NOMBRE - 1] = '\0';
    strncpy(this->telefono, telefono, MAX_TELEFONO - 1);
    this->telefono[MAX_TELEFONO - 1] = '\0';
    strncpy(this->email, email, MAX_EMAIL - 1);
    this->email[MAX_EMAIL - 1] = '\0';
    strncpy(this->direccion, direccion, MAX_DIRECCION - 11);
    this->direccion[MAX_DIRECCION - 1] = '\0';
    this->totalCompras = totalCompras;
    this->cantidadTransacciones = cantidadTransacciones;
    strncpy(fechaRegistro, "", MAX_FECHA - 1);
    fechaRegistro[MAX_FECHA - 1] = '\0';
    eliminado = false;
    strncpy(fechaUltimaModificacion, "", MAX_FECHA - 1);
    fechaUltimaModificacion[MAX_FECHA - 1] = '\0';
	for (int i = 0; i < MAX_TRANSACCIONES_ENTIDAD; i++) {
    	this->transaccionesIds[MAX_TRANSACCIONES_ENTIDAD - 1] = 0;
	}
}

// Destructor
Cliente::~Cliente() {
}

// Getters
int Cliente::getId() const { return id; }
const char* Cliente::getCedula() const { return cedula; }
const char* Cliente::getNombre() const { return nombre; }
const char* Cliente::getTlf() const { return telefono; }
const char* Cliente::getEmail() const { return email; }
const char* Cliente::getDireccion() const { return direccion; }
float Cliente::getTotalcompras() const { return totalCompras; }
const char* Cliente::getFechaRegistro() const { return fechaRegistro; }
const int* Cliente::getTransaccionesIds() const { return transaccionesIds; }
int Cliente::getCantidadTransacciones() const { return cantidadTransacciones; }
bool Cliente::isEliminado() const { return eliminado; }
const char* Cliente::getFechaUltimaModificacion() const { return fechaUltimaModificacion; }

// Setters (con validación)
void Cliente::setId(int id) {
    if (id >= 0) {
        this->id = id;
    }
}

void Cliente::setCedula(const char* cedula) {
    if (Validaciones::validarCedula(cedula)) {
        strncpy(this->cedula, cedula, MAX_CEDULA - 1);
        this->cedula[MAX_CEDULA - 1] = '\0';
    }
}

void Cliente::setNombre(const char* nombre) {
    if (Validaciones::validarNombre(nombre)) {
        strncpy(this->nombre, nombre, MAX_NOMBRE - 1);
        this->nombre[MAX_NOMBRE - 1] = '\0';
    }
}

void Cliente::setEmail(const char* email) {
    if (Validaciones::validarEmail(email)) {
        strncpy(this->email, email, MAX_EMAIL - 1);
    	this->email[MAX_EMAIL - 1] = '\0';
    }
}

void Cliente::setDireccion(const char* direccion) {
    strncpy(this->direccion, direccion, MAX_DIRECCION - 11);
    this->direccion[MAX_DIRECCION - 1] = '\0';
}

void Cliente::setTlf(const char* telefono){
	strncpy(this->telefono, telefono, MAX_TELEFONO - 1);
    this->telefono[MAX_TELEFONO - 1] = '\0';
}

void Cliente::setTotalCompras(float totalCompras) {
    this->totalCompras = totalCompras;
}

void Cliente::setFechaRegistro(const char* fechaRegistro) {
    if (Validaciones::validarFecha(fechaRegistro)) {
        strncpy(this->fechaRegistro, fechaRegistro, MAX_FECHA - 1);
        this->fechaRegistro[MAX_FECHA - 1] = '\0';
    }
}

void Cliente::setTransaccionesIds(const int* transaccionesIds, int cantidad){
	for (int i = 0; i < cantidad && i < MAX_TRANSACCIONES_ENTIDAD; i++) {
    	this->transaccionesIds[i] = transaccionesIds[i];
	}
}

void Cliente::setCantidadTransacciones(int cantidadTransacciones){
	this->cantidadTransacciones = cantidadTransacciones;
}

void Cliente::setEliminado(bool eliminado) {
    this->eliminado = eliminado;
}

void Cliente::setFechaUltimaModificacion(const char* fechaUltimaModificacion) {
    if (Validaciones::validarFecha(fechaUltimaModificacion)) {
        strncpy(this->fechaUltimaModificacion, fechaUltimaModificacion, MAX_FECHA - 1);
        this->fechaUltimaModificacion[MAX_FECHA - 1] = '\0';
    }
}

// Métodos de validación propios de la entidad
bool Cliente::esValido() const {
    return tieneDatosCompletos();
}

bool Cliente::tieneDatosCompletos() const {
    return strlen(cedula) > 0 && strlen(nombre) > 0 && id > 0;
}

// Métodos de presentación
void Cliente::mostrarInformacionBasica() const {
    string nombreTruncado = Formatos::truncarTexto(nombre, 20);
    string direccionTruncada = Formatos::truncarTexto(direccion, 30);

    cout << CYAN << setw(5) << id << setw(15) << cedula << setw(20) << nombreTruncado 
              << setw(15) << telefono << setw(30) << direccionTruncada << RESET << endl;
}

void Cliente::mostrarInformacionBasicaTabla() const {
    string nombreTruncado = Formatos::truncarTexto(nombre, 20);
    string direccionTruncada = Formatos::truncarTexto(direccion, 30);

    cout << RESET << setw(5) << id << setw(15) << cedula << setw(20) << nombreTruncado 
              << setw(15) << telefono << setw(30) << direccionTruncada << endl;
}

void Cliente::mostrarInformacionCompleta() const {
    string nombreTruncado = Formatos::truncarTexto(nombre, 20);
    string direccionTruncada = Formatos::truncarTexto(direccion, 30);
    string emailTruncado = Formatos::truncarTexto(email, 20);
    
    cout << RESET;
    cout << setw(5)  << id;
    cout << setw(11) << cedula;
    cout << setw(18) << nombreTruncado;
    cout << setw(12) << telefono;
    cout << setw(20) << direccionTruncada;
    cout << setw(18) << emailTruncado;
    cout << setw(9)  << (int)totalCompras; // (int) para ahorrar espacio en tabla
    cout << setw(6)  << cantidadTransacciones;
    cout << setw(11) << fechaRegistro;
    cout << setw(11) << fechaUltimaModificacion;
    cout << setw(9)  << (eliminado ? "Elim." : "Act.") << endl;
}

// Métodos adicionales para manejo de arreglos
void Cliente::agregarTransaccionID(int idTransaccion) {
    if (cantidadTransacciones < MAX_TRANSACCIONES_ENTIDAD && !tieneTransaccionID(idTransaccion)) {
        transaccionesIds[cantidadTransacciones] = idTransaccion;
        cantidadTransacciones++;
    }
}

bool Cliente::tieneTransaccionID(int idTransaccion) const {
    for (int i = 0; i < cantidadTransacciones; i++) {
        if (transaccionesIds[i] == idTransaccion) {
            return true;
        }
    }
    return false;
}

// Método estático
int Cliente::obtenerTamano() {
    return sizeof(Cliente);
}
