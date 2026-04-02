#include "Producto.hpp"
#include "../utilidades/Validaciones.hpp"
#include "../utilidades/Formatos.hpp"
#include <cstring>
#include <iostream>

using namespace std;

// Constructor por defecto
Producto::Producto() {
    id = 0;
    strncpy(codigo, "", MAX_CODIGO - 1);
    codigo[MAX_CODIGO - 1] = '\0';
    strncpy(nombre, "", MAX_NOMBRE - 1);
    nombre[MAX_NOMBRE - 1] = '\0';
    strncpy(descripcion, "", MAX_DESCRIPCION - 1);
    descripcion[MAX_DESCRIPCION - 1] = '\0';
    idProveedor = 0;
    precio = 0.0f;
    stock = 0;
    strncpy(fechaRegistro, "", MAX_FECHA - 1);
    fechaRegistro[MAX_FECHA - 1] = '\0';
    stockMinimo = 0;
    totalVendido = 0;
    eliminado = false;
    strncpy(fechaUltimaModificacion, "", MAX_FECHA - 1);
    fechaUltimaModificacion[MAX_FECHA - 1] = '\0';
}

// Constructor parametrizado
Producto::Producto(int id, const char* codigo, const char* nombre, const char* descripcion, 
                   int idProveedor, float precio, int stock, int stockMinimo) {
    this->id = id;
    strncpy(this->codigo, codigo, MAX_CODIGO - 1);
    this->codigo[MAX_CODIGO - 1] = '\0';
    strncpy(this->nombre, nombre, MAX_NOMBRE - 1);
    this->nombre[MAX_NOMBRE - 1] = '\0';
    strncpy(this->descripcion, descripcion, MAX_DESCRIPCION - 1);
    this->descripcion[MAX_DESCRIPCION - 1] = '\0';
    this->idProveedor = idProveedor;
    this->precio = precio;
    this->stock = stock;
    strncpy(fechaRegistro, "", MAX_FECHA - 1);
    fechaRegistro[MAX_FECHA - 1] = '\0';
    this->stockMinimo = stockMinimo;
    totalVendido = 0;
    eliminado = false;
    strncpy(fechaUltimaModificacion, "", MAX_FECHA - 1);
    fechaUltimaModificacion[MAX_FECHA - 1] = '\0';
}

// Destructor
Producto::~Producto() {
}

// Getters
int Producto::getId() const { return id; }
const char* Producto::getCodigo() const { return codigo; }
const char* Producto::getNombre() const { return nombre; }
const char* Producto::getDescripcion() const { return descripcion; }
int Producto::getIdProveedor() const { return idProveedor; }
float Producto::getPrecio() const { return precio; }
int Producto::getStock() const { return stock; }
const char* Producto::getFechaRegistro() const { return fechaRegistro; }
int Producto::getStockMinimo() const { return stockMinimo; }
int Producto::getTotalVendido() const { return totalVendido; }
bool Producto::isEliminado() const { return eliminado; }
const char* Producto::getFechaUltimaModificacion() const { return fechaUltimaModificacion; }

// Setters (con validación)
void Producto::setId(int id) {
    if (id >= 0) {
        this->id = id;
    }
}

void Producto::setCodigo(const char* codigo) {
    if (Validaciones::validarCodigoProducto(codigo)) {
        strncpy(this->codigo, codigo, MAX_CODIGO - 1);
        this->codigo[MAX_CODIGO - 1] = '\0';
    }
}

void Producto::setNombre(const char* nombre) {
    if (Validaciones::validarNombre(nombre)) {
        strncpy(this->nombre, nombre, MAX_NOMBRE - 1);
        this->nombre[MAX_NOMBRE - 1] = '\0';
    }
}

void Producto::setDescripcion(const char* descripcion) {
    if (Validaciones::validarDescripcion(descripcion)) {
        strncpy(this->descripcion, descripcion, MAX_DESCRIPCION - 1);
        this->descripcion[MAX_DESCRIPCION - 1] = '\0';
    }
}

void Producto::setIdProveedor(int idProveedor) {
    if (idProveedor > 0) {
        this->idProveedor = idProveedor;
    }
}

void Producto::setPrecio(float precio) {
    if (precioValido(precio)) {
        this->precio = precio;
    }
}

void Producto::setStock(int stock) {
    if (stockValido(stock)) {
        this->stock = stock;
    }
}

void Producto::setFechaRegistro(const char* fechaRegistro) {
    if (Validaciones::validarFecha(fechaRegistro)) {
        strncpy(this->fechaRegistro, fechaRegistro, MAX_FECHA - 1);
        this->fechaRegistro[MAX_FECHA - 1] = '\0';
    }
}

void Producto::setStockMinimo(int stockMinimo) {
    if (stockMinimo >= 0) {
        this->stockMinimo = stockMinimo;
    }
}

void Producto::setTotalVendido(int totalVendido) {
    if (totalVendido >= 0) {
        this->totalVendido = totalVendido;
    }
}

void Producto::setEliminado(bool eliminado) {
    this->eliminado = eliminado;
}

void Producto::setFechaUltimaModificacion(const char* fechaUltimaModificacion) {
    if (Validaciones::validarFecha(fechaUltimaModificacion)) {
        strncpy(this->fechaUltimaModificacion, fechaUltimaModificacion, MAX_FECHA - 1);
        this->fechaUltimaModificacion[MAX_FECHA - 1] = '\0';
    }
}

// Métodos de validación propios de la entidad
bool Producto::esValido() const {
    return tieneDatosCompletos() && precioValido(precio) && stockValido(stock);
}

bool Producto::tieneDatosCompletos() const {
    return strlen(codigo) > 0 && strlen(nombre) > 0 && idProveedor > 0;
}

bool Producto::precioValido(float precio) const {
    return Validaciones::validarRangoFloat(precio, 0.01f, 999999.99f);
}

bool Producto::stockValido(int stock) const {
    return Validaciones::validarRango(stock, 0, 999999);
}

// Métodos de presentación
void Producto::mostrarInformacionBasica() const {
    cout << CYAN << "ID: " << id << " | Código: " << codigo << " | Nombre: " << nombre 
              << " | Precio: $" << Formatos::formatearMoneda(precio) << " | Stock: " << stock << RESET << endl;
}

void Producto::mostrarInformacionCompleta() const {
    cout << CYAN;
    cout << "ID: " << id << endl;
    cout << "Código: " << codigo << endl;
    cout << "Nombre: " << nombre << endl;
    cout << "Descripción: " << descripcion << endl;
    cout << "ID Proveedor: " << idProveedor << endl;
    cout << "Precio: $" << Formatos::formatearMoneda(precio) << endl;
    cout << "Stock Actual: " << stock << endl;
    cout << "Stock Mínimo: " << stockMinimo << endl;
    cout << "Total Vendido: " << totalVendido << endl;
    cout << "Fecha Registro: " << fechaRegistro << endl;
    cout << "Fecha Última Modificación: " << fechaUltimaModificacion << endl;
    cout << "Estado: " << (eliminado ? "Eliminado" : "Activo") << RESET << endl;
    
    if (estaBajoStockMinimo()) {
        cout << AMARILLO << "⚠ ADVERTENCIA: Stock por debajo del mínimo" << RESET << endl;
    }
}

// Métodos de gestión de relaciones
void Producto::registrarVenta(int cantidad) {
    if (stockDisponible(cantidad)) {
        stock -= cantidad;
        totalVendido += cantidad;
    }
}

void Producto::agregarStock(int cantidad) {
    if (cantidad > 0) {
        stock += cantidad;
    }
}

void Producto::reducirStock(int cantidad) {
    if (stockDisponible(cantidad)) {
        stock -= cantidad;
    }
}

bool Producto::stockDisponible(int cantidad) const {
    return stock >= cantidad && cantidad > 0;
}

bool Producto::estaBajoStockMinimo() const {
    return stock <= stockMinimo && stockMinimo > 0;
}

// Método estático
int Producto::obtenerTamano() {
    return sizeof(Producto);
}
