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
             const char* email, const char* direccion, float totalCompras, int transaccionesIds, int cantidadTransacciones) {
			 	
    this->id = id;
    strncpy(this-cedula, cedula, MAX_CEDULA - 1);
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
int Cliente::getTransaccionesIds() const { return transaccionesIds[MAX_TRANSACCIONES_ENTIDAD]; }
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
    	strncpy(this-cedula, cedula, MAX_CEDULA - 1);
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

void Cliente::setTotalCompras(int totalCompras); {
    this->totalCompras = totalCompras;
}

void Cliente::setFechaRegistro(const char* fechaRegistro) {
    if (Validaciones::validarFecha(fechaRegistro)) {
        strncpy(this->fechaRegistro, fechaRegistro, MAX_FECHA - 1);
        this->fechaRegistro[MAX_FECHA - 1] = '\0';
    }
}

void Cliente::setTransaccionesIds(int transaccionesIds);{
	for (int i = 0; i < MAX_ID_TRANSACCIONES; i++) {
    	this->transaccionesIds[MAX_ID_TRANSACCIONES - 1] = 0;
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
    return strlen(cedula) > 0 && strlen(nombre) > 0 && idCliente > 0;
}


// Métodos de presentación
void Producto::mostrarInformacionBasica() const {
    cout << CYAN << "ID: " << id << " | Cedula: " << cedula << " | Nombre: " << nombre 
              << " | Telefono: " << telefono << " | Direccion: " << direccion << " | Email: " << email << RESET << endl;
}

void Producto::mostrarInformacionCompleta() const {
    cout << CYAN;
    cout << "ID: " << id << endl;
    cout << "Cedula: " << cedula << endl;
    cout << "Nombre: " << nombre << endl;
    cout << "Telefono: " << telefono << endl;
    cout << "Direccion: " << direccion << endl;
    cout << "Email:" << email << endl;
    cout << "Total de Compras:" << totalCompras << endl;
    cout << "Cantidad de Transacciones: " << cantidadTransacciones << endl;
    cout << "Fecha Registro: " << fechaRegistro << endl;
    cout << "Fecha Última Modificación: " << fechaUltimaModificacion << endl;
    cout << "Estado: " << (eliminado ? "Eliminado" : "Activo") << RESET << endl;
}

// Método estático
int Cliente::obtenerTamano() {
    return sizeof(Cliente);
}
