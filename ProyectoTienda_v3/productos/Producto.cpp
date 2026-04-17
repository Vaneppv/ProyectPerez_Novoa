#include "Producto.hpp"
#include "../utilidades/Validaciones.hpp"
#include "../utilidades/Formatos.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include "../proveedores/Proveedor.hpp"
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
    string nombreTruncado = Formatos::truncarTexto(nombre, 20);

    cout << RESET << setw(5) << id << setw(15) << codigo << setw(20) << nombreTruncado 
         << setw(10) << Formatos::formatearMoneda(precio) << setw(10) << stock << endl;
}

void Producto::mostrarInformacionCompleta() const {
    int indice = GestorArchivos::buscarPorId<Proveedor>(ARCHIVO_PROVEEDORES, idProveedor);
    Proveedor proveedor;
    if (indice != -1) {
        proveedor = GestorArchivos::obtenerRegistroPorIndice<Proveedor>(ARCHIVO_PROVEEDORES, indice);
    }
string nombreTruncado = Formatos::truncarTexto(nombre, 16);      // setw(18)
    string descripcionTruncada = Formatos::truncarTexto(descripcion, 23); // setw(25)
    string proveedorTruncado = Formatos::truncarTexto(proveedor.getNombre(), 13); // setw(15)

    cout << RESET << left; 
    cout << setw(5)  << id;
    cout << setw(10) << codigo;
    cout << setw(18) << nombreTruncado;
    cout << setw(25) << descripcionTruncada;
    cout << setw(15) << proveedorTruncado;
    cout << setw(10) << Formatos::formatearMoneda(precio);
    cout << setw(6)  << stock;
    cout << setw(6)  << stockMinimo;
    cout << setw(11) << fechaRegistro;
    cout << setw(11) << fechaUltimaModificacion;
    cout << setw(8)  << (eliminado ? "Elim." : "Act.") << endl;
     
    if (estaBajoStockMinimo()) {
        cout << AMARILLO << "  " << (char)175 << " Stock Bajo: " << stock << RESET << endl;
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
