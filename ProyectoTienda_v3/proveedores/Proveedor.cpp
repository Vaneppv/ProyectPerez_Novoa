#include "Proveedor.hpp"
#include "../utilidades/Validaciones.hpp"
#include "../utilidades/Formatos.hpp"
#include <cstring>
#include <iostream>

using namespace std;

// Constructor por defecto
Proveedor::Proveedor() {
    id = 0;
    strncpy(rif, "", MAX_RIF - 1);
    rif[MAX_RIF - 1] = '\0';
    strncpy(nombre, "", MAX_NOMBRE - 1);
    nombre[MAX_NOMBRE - 1] = '\0';
    strncpy(telefono, "", MAX_TELEFONO - 1);
    telefono[MAX_TELEFONO - 1] = '\0';
    strncpy(email, "", MAX_EMAIL - 1);
    email[MAX_EMAIL - 1] = '\0';
    strncpy(direccion, "", MAX_DIRECCION - 1);
    direccion[MAX_DIRECCION - 1] = '\0';
    cantidadProductos = 0;
    totalCompras = 0.0f;
    cantidadTransacciones = 0;
    strncpy(fechaRegistro, "", MAX_FECHA - 1);
    fechaRegistro[MAX_FECHA - 1] = '\0';
    eliminado = false;
    strncpy(fechaUltimaModificacion, "", MAX_FECHA - 1);
    fechaUltimaModificacion[MAX_FECHA - 1] = '\0';
    for (int i = 0; i < MAX_PRODUCTOS_PROVEEDOR; i++) {
    	productosIDs[MAX_PRODUCTOS_PROVEEDOR - 1] = 0;
	}
	for (int i = 0; i < MAX_TRANSACCIONES_ENTIDAD; i++) {
    	transaccionesIds[MAX_TRANSACCIONES_ENTIDAD - 1] = 0;
	}
}

// Constructor parametrizado
Proveedor::Proveedor(int id, const char* rif, const char* nombre, const char* telefono, 
             const char* email, const char* direccion, int cantidadProductos,
			 const int* productosIDs, float totalCompras, const int* transaccionesIds, int cantidadTransacciones) {
			 	
    this->id = id;
    strncpy(this->rif, rif, MAX_RIF - 1);
    this->rif[MAX_RIF - 1] = '\0';
    strncpy(this->nombre, nombre, MAX_NOMBRE - 1);
    this->nombre[MAX_NOMBRE - 1] = '\0';
    strncpy(this->telefono, telefono, MAX_TELEFONO - 1);
    this->telefono[MAX_TELEFONO - 1] = '\0';
    strncpy(this->email, email, MAX_EMAIL - 1);
    this->email[MAX_EMAIL - 1] = '\0';
    strncpy(this->direccion, direccion, MAX_DIRECCION - 11);
    this->direccion[MAX_DIRECCION - 1] = '\0';
    this->cantidadProductos = cantidadProductos;
    this->totalCompras = totalCompras;
    this->cantidadTransacciones = cantidadTransacciones;
    strncpy(fechaRegistro, "", MAX_FECHA - 1);
    fechaRegistro[MAX_FECHA - 1] = '\0';
    eliminado = false;
    strncpy(fechaUltimaModificacion, "", MAX_FECHA - 1);
    fechaUltimaModificacion[MAX_FECHA - 1] = '\0';
    for (int i = 0; i < MAX_PRODUCTOS_PROVEEDOR; i++) {
    	this->productosIDs[MAX_PRODUCTOS_PROVEEDOR - 1] = 0;
	}
	for (int i = 0; i < MAX_TRANSACCIONES_ENTIDAD; i++) {
    	this->transaccionesIds[MAX_TRANSACCIONES_ENTIDAD - 1] = 0;
	}
}

// Destructor
Proveedor::~Proveedor() {
}

// Getters
int Proveedor::getId() const { return id; }
const char* Proveedor::getRif() const { return rif; }
const char* Proveedor::getNombre() const { return nombre; }
const char* Proveedor::getTlf() const { return telefono; }
const char* Proveedor::getEmail() const { return email; }
const char* Proveedor::getDireccion() const { return direccion; }
int Proveedor::getCantidadproductos() const { return cantidadProductos; }
int Proveedor::getProductosIDs() const { return productosIDs[MAX_PRODUCTOS_PROVEEDOR]; }
float Proveedor::getTotalcompras() const { return totalCompras; }
const char* Proveedor::getFechaRegistro() const { return fechaRegistro; }
int Proveedor::getTransaccionesIds() const { return transaccionesIds[MAX_TRANSACCIONES_ENTIDAD]; }
int Proveedor::getCantidadTransacciones() const { return cantidadTransacciones; }
bool Proveedor::isEliminado() const { return eliminado; }
const char* Proveedor::getFechaUltimaModificacion() const { return fechaUltimaModificacion; }

// Setters (con validación)
void Proveedor::setId(int id) {
    if (id >= 0) {
        this->id = id;
    }
}

void Proveedor::setRif(const char* rif) {
    if (Validaciones::validarRIF(rif)) {
        strncpy(this->rif, rif, MAX_RIF - 1);
        this->rif[MAX_RIF - 1] = '\0';
    }
}

void Proveedor::setNombre(const char* nombre) {
    if (Validaciones::validarNombre(nombre)) {
        strncpy(this->nombre, nombre, MAX_NOMBRE - 1);
        this->nombre[MAX_NOMBRE - 1] = '\0';
    }
}

void Proveedor::setEmail(const char* email) {
    if (Validaciones::validarEmail(email)) {
        strncpy(this->email, email, MAX_EMAIL - 1);
    	this->email[MAX_EMAIL - 1] = '\0';
    }
}

void Proveedor::setDireccion(const char* direccion) {
    strncpy(this->direccion, direccion, MAX_DIRECCION - 11);
    this->direccion[MAX_DIRECCION - 1] = '\0';
}

void Proveedor::setTlf(const char* telefono){
	strncpy(this->telefono, telefono, MAX_TELEFONO - 1);
    this->telefono[MAX_TELEFONO - 1] = '\0';
}

void Proveedor::setCantidadProductos(int cantidadProductos) {
        this->cantidadProductos = cantidadProductos;
}

void Proveedor::setTotalCompras(float totalCompras) {
    this->totalCompras = totalCompras;
}

void Proveedor::setFechaRegistro(const char* fechaRegistro) {
    if (Validaciones::validarFecha(fechaRegistro)) {
        strncpy(this->fechaRegistro, fechaRegistro, MAX_FECHA - 1);
        this->fechaRegistro[MAX_FECHA - 1] = '\0';
    }
}

void Proveedor::setProductosIDs(const int* productosIDs, int cantidad) {
   for (int i = 0; i < cantidad && i < MAX_PRODUCTOS_PROVEEDOR; i++) {
    	this->productosIDs[i] = productosIDs[i];
	}
}

void Proveedor::setTransaccionesIds(const int* transaccionesIds, int cantidad){
	for (int i = 0; i < cantidad && i < MAX_TRANSACCIONES_ENTIDAD; i++) {
    	this->transaccionesIds[i] = transaccionesIds[i];
	}
}

void Proveedor::setCantidadTransacciones(int cantidadTransacciones){
	this->cantidadTransacciones = cantidadTransacciones;
}

void Proveedor::setEliminado(bool eliminado) {
    this->eliminado = eliminado;
}

void Proveedor::setFechaUltimaModificacion(const char* fechaUltimaModificacion) {
    if (Validaciones::validarFecha(fechaUltimaModificacion)) {
        strncpy(this->fechaUltimaModificacion, fechaUltimaModificacion, MAX_FECHA - 1);
        this->fechaUltimaModificacion[MAX_FECHA - 1] = '\0';
    }
}

// Métodos de validación propios de la entidad
bool Proveedor::esValido() const {
    return tieneDatosCompletos();
}

bool Proveedor::tieneDatosCompletos() const {
    return strlen(rif) > 0 && strlen(nombre) > 0 && id > 0;
}

// Métodos de presentación
void Proveedor::mostrarInformacionBasica() const {
    string nombreTruncado = Formatos::truncarTexto(nombre, 20);
    string direccionTruncada = Formatos::truncarTexto(direccion, 30);
    cout << RESET << setw(5) << id << setw(15) << rif << setw(20) << nombreTruncado 
         << setw(15) << telefono << setw(30) << direccionTruncada << endl;
}

void Proveedor::mostrarInformacionCompleta() const {
    
    string nombreTruncado = Formatos::truncarTexto(nombre, 17);
    string direccionTruncada = Formatos::truncarTexto(direccion, 19);
    string emailTruncado = Formatos::truncarTexto(email, 17);

    cout << RESET << left; 
    cout << setw(5)  << id;
    cout << setw(13) << rif;
    cout << setw(18) << nombreTruncado;
    cout << setw(13) << telefono;
    cout << setw(20) << direccionTruncada;
    cout << setw(18) << emailTruncado;
    cout << setw(6)  << cantidadProductos;
    cout << setw(10) << (int)totalCompras; // (int) para quitar decimales y ahorrar espacio
    cout << setw(8)  << cantidadTransacciones;
    cout << setw(12) << fechaRegistro;
    cout << setw(12) << fechaUltimaModificacion;
    cout << setw(10) << (eliminado ? "Elim." : "Activo") << endl;
}

// Método estático
int Proveedor::obtenerTamano() {
    return sizeof(Proveedor);
}
