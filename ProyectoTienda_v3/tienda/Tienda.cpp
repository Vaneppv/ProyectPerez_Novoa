#include "Tienda.hpp"
#include "../utilidades/Formatos.hpp"
#include <cstring>
#include <iostream>

using namespace std;

// Constructor por defecto
Tienda::Tienda() {
    strncpy(nombre, "Tienda JSport", MAX_NOMBRE - 1);
    nombre[MAX_NOMBRE - 1] = '\0';
    strncpy(rif, "J-123456789", MAX_RIF - 1);
    rif[MAX_RIF - 1] = '\0';
    eliminado = false;
    
    siguienteIdProducto = 1;
    siguienteIdProveedor = 1;
    siguienteIdCliente = 1;
    siguienteIdTransaccion = 1;
}

// Constructor parametrizado
Tienda::Tienda(const char* nombre, const char* rif) {
    strncpy(this->nombre, nombre, MAX_NOMBRE - 1);
    this->nombre[MAX_NOMBRE - 1] = '\0';
    strncpy(this->rif, rif, MAX_RIF - 1);
    this->rif[MAX_RIF - 1] = '\0';
    eliminado = false;
    
    siguienteIdProducto = 1;
    siguienteIdProveedor = 1;
    siguienteIdCliente = 1;
    siguienteIdTransaccion = 1;
}

// Destructor
Tienda::~Tienda() {
    // No hay recursos dinámicos que liberar
}

// Getters
const char* Tienda::getNombre() const {
    return nombre;
}

const char* Tienda::getRif() const {
    return rif;
}

int Tienda::getSiguienteIdProducto() const {
    return siguienteIdProducto;
}

int Tienda::getSiguienteIdProveedor() const {
    return siguienteIdProveedor;
}

int Tienda::getSiguienteIdCliente() const {
    return siguienteIdCliente;
}

int Tienda::getSiguienteIdTransaccion() const {
    return siguienteIdTransaccion;
}

// Métodos adicionales para compatibilidad con template
int Tienda::getId() const {
    return 1; // Tienda siempre tiene ID 1
}

bool Tienda::isEliminado() const {
    return eliminado;
}

void Tienda::setEliminado(bool eliminado) {
    this->eliminado = eliminado;
}

void Tienda::setId(int id) {
    // Tienda no puede cambiar su ID, siempre será 1
}

void Tienda::setFechaRegistro(const char* fecha) {
    // Tienda no tiene fecha de registro, pero implementado para compatibilidad
}

void Tienda::setFechaUltimaModificacion(const char* fecha) {
    // Tienda no tiene fecha de última modificación, pero implementado para compatibilidad
}

// Setters
void Tienda::setNombre(const char* nombre) {
    strncpy(this->nombre, nombre, MAX_NOMBRE - 1);
    this->nombre[MAX_NOMBRE - 1] = '\0';
}

void Tienda::setRif(const char* rif) {
    strncpy(this->rif, rif, MAX_RIF - 1);
    this->rif[MAX_RIF - 1] = '\0';
}

void Tienda::setSiguienteIdProducto(int id) {
    siguienteIdProducto = id;
}

void Tienda::setSiguienteIdProveedor(int id) {
    siguienteIdProveedor = id;
}

void Tienda::setSiguienteIdCliente(int id) {
    siguienteIdCliente = id;
}

void Tienda::setSiguienteIdTransaccion(int id) {
    siguienteIdTransaccion = id;
}

// Métodos de negocio
int Tienda::generarSiguienteIdProducto() {
    return siguienteIdProducto++;
}

int Tienda::generarSiguienteIdProveedor() {
    return siguienteIdProveedor++;
}

int Tienda::generarSiguienteIdCliente() {
    return siguienteIdCliente++;
}

int Tienda::generarSiguienteIdTransaccion() {
    return siguienteIdTransaccion++;
}

// Métodos de presentación
void Tienda::mostrarInformacionBasica() const {
    cout << "Nombre: " << nombre << endl;
    cout << "RIF: " << rif << endl;
}

void Tienda::mostrarInformacionCompleta() const {
    Formatos::imprimirTitulo("INFORMACIÓN DE LA TIENDA");
    std::cout << CYAN << "Nombre: " << nombre << std::endl;
    std::cout << "RIF: " << rif << std::endl;
    std::cout << "Siguiente ID Producto: " << siguienteIdProducto << std::endl;
    std::cout << "Siguiente ID Proveedor: " << siguienteIdProveedor << std::endl;
    std::cout << "Siguiente ID Cliente: " << siguienteIdCliente << std::endl;
    std::cout << "Siguiente ID Transacción: " << siguienteIdTransaccion << RESET << std::endl;
    Formatos::imprimirSeparador();
}

// Método estático
int Tienda::obtenerTamano() {
    return sizeof(Tienda);
}
