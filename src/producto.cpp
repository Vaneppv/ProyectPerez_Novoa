#include "../include/producto.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <cstring>
#include <ctime>

using namespace std;

// Definición de colores ANSI
#define RESET   "\033[00m"
#define ROJO    "\033[31m"
#define VERDE   "\033[32m"
#define AMARILLO "\033[33m"
#define AZUL    "\033[34m"
#define CYAN    "\033[36m"
#define NEGRITA "\033[1m"

// === CLASE PRODUCTO ===

// Constructores
Producto::Producto() : id(0), idProveedor(0), precio(0.0f), stock(0), 
                      stockMinimo(10), totalVendido(0), eliminado(false) {
    fechaRegistro = Producto::obtenerFechaActual();
    fechaUltimaModificacion = fechaRegistro;
}

Producto::Producto(int id, const string& codigo, const string& nombre, 
                   const string& descripcion, int idProveedor, float precio, 
                   int stock, int stockMinimo) 
    : id(id), codigo(codigo), nombre(nombre), descripcion(descripcion), 
      idProveedor(idProveedor), precio(precio), stock(stock), 
      stockMinimo(stockMinimo), totalVendido(0), eliminado(false) {
    
    fechaRegistro = Producto::obtenerFechaActual();
    fechaUltimaModificacion = fechaRegistro;
}

// Getters
int Producto::getId() const {
    return id;
}

string Producto::getCodigo() const {
    return codigo;
}

string Producto::getNombre() const {
    return nombre;
}

string Producto::getDescripcion() const {
    return descripcion;
}

int Producto::getIdProveedor() const {
    return idProveedor;
}

float Producto::getPrecio() const {
    return precio;
}

int Producto::getStock() const {
    return stock;
}

string Producto::getFechaRegistro() const {
    return fechaRegistro;
}

int Producto::getStockMinimo() const {
    return stockMinimo;
}

int Producto::getTotalVendido() const {
    return totalVendido;
}

bool Producto::isEliminado() const {
    return eliminado;
}

string Producto::getFechaUltimaModificacion() const {
    return fechaUltimaModificacion;
}

// Setters
void Producto::setId(int id) {
    this->id = id;
    fechaUltimaModificacion = Producto::obtenerFechaActual();
}

void Producto::setCodigo(const string& codigo) {
    this->codigo = codigo;
    fechaUltimaModificacion = Producto::obtenerFechaActual();
}

void Producto::setNombre(const string& nombre) {
    this->nombre = nombre;
    fechaUltimaModificacion = Producto::obtenerFechaActual();
}

void Producto::setDescripcion(const string& descripcion) {
    this->descripcion = descripcion;
    fechaUltimaModificacion = Producto::obtenerFechaActual();
}

void Producto::setIdProveedor(int idProveedor) {
    this->idProveedor = idProveedor;
    fechaUltimaModificacion = Producto::obtenerFechaActual();
}

void Producto::setPrecio(float precio) {
    if (validarPrecio(precio)) {
        this->precio = precio;
        fechaUltimaModificacion = Producto::obtenerFechaActual();
    }
}

void Producto::setStock(int stock) {
    if (validarStock(stock)) {
        this->stock = stock;
        fechaUltimaModificacion = Producto::obtenerFechaActual();
    }
}

void Producto::setStockMinimo(int stockMinimo) {
    if (stockMinimo >= 0) {
        this->stockMinimo = stockMinimo;
        fechaUltimaModificacion = Producto::obtenerFechaActual();
    }
}

void Producto::setTotalVendido(int totalVendido) {
    this->totalVendido = totalVendido;
    fechaUltimaModificacion = Producto::obtenerFechaActual();
}

void Producto::setEliminado(bool eliminado) {
    this->eliminado = eliminado;
    fechaUltimaModificacion = Producto::obtenerFechaActual();
}

void Producto::setFechaUltimaModificacion(const string& fecha) {
    this->fechaUltimaModificacion = fecha;
}

// Métodos de negocio
bool Producto::tieneStockBajo() const {
    return stock <= stockMinimo;
}

bool Producto::hayStockSuficiente(int cantidad) const {
    return stock >= cantidad;
}

void Producto::reducirStock(int cantidad) {
    if (hayStockSuficiente(cantidad)) {
        stock -= cantidad;
        totalVendido += cantidad;
        fechaUltimaModificacion = Producto::obtenerFechaActual();
    }
}

void Producto::aumentarStock(int cantidad) {
    if (cantidad > 0) {
        stock += cantidad;
        fechaUltimaModificacion = Producto::obtenerFechaActual();
    }
}

void Producto::registrarVenta(int cantidad) {
    if (hayStockSuficiente(cantidad)) {
        reducirStock(cantidad);
    }
}

string Producto::toString() const {
    stringstream ss;
    ss << "Producto[ID:" << id 
       << ", Codigo:" << codigo 
       << ", Nombre:" << nombre 
       << ", Stock:" << stock 
       << ", Precio:$" << fixed << setprecision(2) << precio 
       << ", Proveedor:" << idProveedor << "]";
    return ss.str();
}

// Métodos estáticos
string Producto::obtenerFechaActual() {
    time_t ahora = time(nullptr);
    tm* tiempoLocal = localtime(&ahora);
    
    char buffer[11];
    strftime(buffer, sizeof(buffer), "%d/%m/%Y", tiempoLocal);
    
    return string(buffer);
}

bool Producto::validarPrecio(float precio) {
    return precio > 0.0f;
}

bool Producto::validarStock(int stock) {
    return stock >= 0;
}

// === GESTION DE PRODUCTOS ===

// Constructor
ProductoManager::ProductoManager(const char* archivoProductos, 
                               const char* archivoProveedores) {
    strcpy(nombreArchivo, archivoProductos);
    strcpy(this->archivoProveedores, archivoProveedores);
}

// Métodos privados para manejo de archivos
ProductoManager::ArchivoHeader ProductoManager::leerHeader() const {
    ArchivoHeader header = {0, 1, 0, 1};
    
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()) {
        return header;
    }

    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    archivo.close();
    
    return header;
}

bool ProductoManager::actualizarHeader(const ArchivoHeader& header) {
    fstream archivo(nombreArchivo, ios::binary | ios::in | ios::out);
    
    if (!archivo.is_open()) {
        cout << ROJO << "Error: No se pudo abrir el archivo " << nombreArchivo << endl << RESET;
        return false;
    }

    archivo.seekp(0, ios::beg);
    archivo.write(reinterpret_cast<const char*>(&header), sizeof(ArchivoHeader));
    
    bool resultado = archivo.good();
    archivo.close();
    return resultado;
}

bool ProductoManager::guardarNuevoRegistro(const Producto& producto) {
    ArchivoHeader header = leerHeader();
    
    fstream archivo(nombreArchivo, ios::in | ios::out | ios::binary);
    if (!archivo.is_open()) {
        cout << ROJO << "Error: No se pudo abrir el archivo " << nombreArchivo << endl << RESET;
        return false;
    }
    
    int posicionEscritura = sizeof(ArchivoHeader) + (header.cantidadRegistros * sizeof(Producto));
    archivo.seekp(posicionEscritura, ios::beg);

    archivo.write(reinterpret_cast<const char*>(&producto), sizeof(Producto));
    
    if (archivo.fail()) {
        cout << ROJO << "Error crítico: No se pudo escribir en el archivo." << endl << RESET;
        archivo.close();
        return false;
    }

    header.cantidadRegistros++;
    header.proximoID++;
    header.registrosActivos++;
    
    archivo.close();
    return actualizarHeader(header);
}

bool ProductoManager::actualizarRegistro(int indice, const Producto& producto) {
    fstream archivo(nombreArchivo, ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) return false;

    archivo.seekp(sizeof(ArchivoHeader) + (indice * sizeof(Producto)), ios::beg);
    archivo.write(reinterpret_cast<const char*>(&producto), sizeof(Producto));

    archivo.close();
    return true;
}

int ProductoManager::buscarIndicePorId(int id) const {
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()) {
        cout << ROJO << "Error: No se pudo abrir el archivo " << nombreArchivo << endl << RESET;
        return -1;
    }

    ArchivoHeader header;
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));

    for (int i = 0; i < header.cantidadRegistros; i++) {
        Producto temp;
        archivo.read(reinterpret_cast<char*>(&temp), sizeof(Producto));
        if (!temp.isEliminado() && temp.getId() == id) {
            archivo.close();
            return i;
        }
    }
    archivo.close();
    return -1;
}

Producto ProductoManager::obtenerRegistroPorIndice(int indice) const {
    Producto producto;
    ifstream archivo(nombreArchivo, ios::binary);
    if (archivo.is_open()) {
        int posicion = sizeof(ArchivoHeader) + (indice * sizeof(Producto));
        archivo.seekg(posicion, ios::beg);
        archivo.read(reinterpret_cast<char*>(&producto), sizeof(Producto));
        archivo.close();
    }
    return producto;
}

bool ProductoManager::existeEntidad(const char* nombreArchivo, int id) const {
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()) return false;

    ArchivoHeader header;
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    
    Producto temp;
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.read(reinterpret_cast<char*>(&temp), sizeof(Producto));
        if (!temp.isEliminado() && temp.getId() == id) {
            archivo.close();
            return true;
        }
    }
    archivo.close();
    return false;
}

// Métodos públicos
bool ProductoManager::inicializarArchivo() {
    ifstream test(nombreArchivo, ios::binary);
    
    if (test.is_open()) {
        test.close();
        return true; 
    }

    ofstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()) {
        cout << ROJO << "Error: No se pudo crear el archivo " << nombreArchivo << endl << RESET;
        return false;
    }

    ArchivoHeader header = {0, 1, 0, 1};
    archivo.write(reinterpret_cast<const char*>(&header), sizeof(ArchivoHeader));
    
    bool resultado = archivo.good();
    archivo.close();
    return resultado;
}

bool ProductoManager::verificarProveedorDisponible() const {
    ArchivoHeader header;
    ifstream archivo(archivoProveedores, ios::binary);
    if (!archivo.is_open()) return false;
    
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    archivo.close();
    
    return header.registrosActivos > 0;
}

bool ProductoManager::crearProducto() {
    if (!verificarProveedorDisponible()) {
        cout << ROJO << "Error: para registrar un producto, debe registrar primero el proveedor." << endl << RESET;
        return false;
    }
    
    char decision;
    cout << "¿Desea registrar un nuevo producto? (S/N): ";
    cin >> decision;

    if (decision == 's' || decision == 'S') {
        Producto nuevoProducto;
        
        if (!solicitarDatosProducto(nuevoProducto)) {
            return false;
        }

        // Resumen de datos
        imprimirSeparador(60, '=');
        cout << AZUL << NEGRITA << "      RESUMEN DE PRODUCTO" << endl << RESET;
        imprimirSeparador(60, '=');
        cout << CYAN << "Código: " << nuevoProducto.getCodigo() << endl;
        cout << "Nombre: " << nuevoProducto.getNombre() << endl;
        cout << "Descripción: " << nuevoProducto.getDescripcion() << endl;
        cout << "Stock: " << nuevoProducto.getStock() << endl;
        cout << "Precio: $" << fixed << setprecision(2) << nuevoProducto.getPrecio() << endl << RESET;
        imprimirSeparador(60, '=');
        
        cout << "¿Desea Guardar producto? (S/N): ";
        cin >> decision;

        if (decision == 's' || decision == 'S') {
            ArchivoHeader header = leerHeader();
            nuevoProducto.setId(header.proximoID);
            
            if (guardarNuevoRegistro(nuevoProducto)) {
                cout << endl << VERDE << NEGRITA << "¡Producto guardado! ID asignado: " << nuevoProducto.getId() << endl << RESET;
                mostrarEncabezadoProductos();
                mostrarDetalleProducto(nuevoProducto);
                return true;
            }
        } else {   
            cout << ROJO << endl << "Operación cancelada. El Producto no fue guardado" << endl << RESET;
        }
    } else {
        cout << AMARILLO << endl << "Volviendo al menú." << endl << RESET;
    }
    return false;
}

bool ProductoManager::buscarProducto() const {
    ArchivoHeader header = leerHeader();
    if (header.registrosActivos == 0) {
        cout << ROJO << "No hay productos en el Sistema." << endl << RESET;
        return false;
    }
    
    int seleccion;
    do {
        cout << AZUL << NEGRITA << endl << "===== MENU DE BUSQUEDA DE PRODUCTOS =====" << endl << RESET;
        cout << CYAN << "1. Buscar por ID" << endl;
        cout << "2. Buscar por nombre (coincidencia parcial)" << endl;
        cout << "3. Buscar por código (coincidencia parcial)" << endl;
        cout << "4. Listar por proveedor" << endl;
        cout << "0. Cancelar" << endl << RESET;

        if (!solicitarEntero("Seleccione una opción", seleccion)) {
            break; 
        }

        switch (seleccion) {
            case 1: { // Buscar por ID
                int id;
                if (solicitarEntero("Ingrese el ID del producto", id)) {
                    Producto producto = buscarPorId(id);
                    if (producto.getId() != -1) {
                        cout << VERDE << NEGRITA << endl << "Producto encontrado:" << endl << RESET;
                        mostrarEncabezadoProductos();
                        mostrarDetalleProducto(producto);
                        imprimirSeparador();
                    } else {
                        cout << ROJO << endl << "Error: El producto con ID " << id << " no existe." << endl << RESET;
                    }
                }
                break;
            }

            case 2: { // Buscar por nombre
                char nombreBusqueda[100];
                int numEncontrados = 0;
                
                if (solicitarTexto("Ingrese el nombre (o parte) del producto", nombreBusqueda, 100)) {
                    int* indices = buscarPorNombre(nombreBusqueda, &numEncontrados);
                    if (indices != nullptr) {
                        cout << VERDE << NEGRITA << "Se encontraron " << numEncontrados << " coincidencias:" << endl << RESET;
                        mostrarEncabezadoProductos();
                        
                        for (int i = 0; i < numEncontrados; i++) {
                            Producto producto = obtenerRegistroPorIndice(indices[i]);
                            mostrarDetalleProducto(producto);
                        }
                        imprimirSeparador();
                        delete[] indices;
                    } else {
                        cout << AMARILLO << endl << "No se encontraron productos que coincidan con: '" << nombreBusqueda << "'" << endl << RESET;
                    }
                }
                break;
            }

            case 3: { // Búsqueda por código
                char codigoBusqueda[20];
                int numEncontrados = 0;
                
                if (solicitarTexto("Ingrese el código (o parte) a buscar", codigoBusqueda, 20)) {
                    int* indices = buscarPorCodigo(codigoBusqueda, &numEncontrados);
                    if (indices != nullptr) {
                        cout << VERDE << NEGRITA << endl << "Se encontraron " << numEncontrados << " coincidencias por código:" << endl << RESET;
                        mostrarEncabezadoProductos();
                        for (int i = 0; i < numEncontrados; i++) {
                            Producto producto = obtenerRegistroPorIndice(indices[i]);
                            mostrarDetalleProducto(producto);
                        }
                        imprimirSeparador();
                        delete[] indices;
                    } else {
                        cout << AMARILLO << endl << "No existen productos con el código: '" << codigoBusqueda << "'" << endl << RESET;
                    }
                }
                break;
            }

            case 4: { // Listar por proveedor
                int idProv;
                if (solicitarEntero("Ingrese ID del proveedor", idProv)) {
                    int numEncontrados = 0;
                    Producto* productos = listarPorProveedor(idProv, &numEncontrados);
                    if (productos != nullptr) {
                        cout << VERDE << NEGRITA << "\n PRODUCTOS SUMINISTRADOS POR PROVEEDOR ID: " << idProv << endl << RESET;
                        mostrarEncabezadoProductos();
                        for (int i = 0; i < numEncontrados; i++) {
                            mostrarDetalleProducto(productos[i]);
                        }
                        imprimirSeparador();
                        delete[] productos;
                    } else {
                        cout << AMARILLO << "No se encontraron productos para este proveedor." << endl << RESET;
                    }
                }
                break;
            }

            case 0:
                cout << AMARILLO << "Cancelando búsqueda" << endl << RESET;
                break;

            default:
                cout << ROJO << "Opción no válida." << endl << RESET;
                break;
        }
    } while (seleccion != 0);
    
    return true;
}

Producto ProductoManager::buscarPorId(int id) const {
    int indice = buscarIndicePorId(id);
    if (indice != -1) {
        return obtenerRegistroPorIndice(indice);
    }
    return Producto(); // Retorna producto vacío si no lo encuentra
}

int* ProductoManager::buscarPorNombre(const char* nombre, int* numResultados) const {
    ArchivoHeader header = leerHeader();
    *numResultados = 0;

    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()) {
        cout << ROJO << "Error abriendo archivo de búsqueda." << endl << RESET;
        return nullptr;
    }

    Producto producto;
    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    
    // Primero contar cuántos coinciden
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.read(reinterpret_cast<char*>(&producto), sizeof(Producto));
        if (!producto.isEliminado() && contieneSubstring(producto.getNombre().c_str(), nombre)) {
            (*numResultados)++;
        }
    }
    
    if (*numResultados == 0) {
        archivo.close();
        return nullptr;
    }
    
    // Reservar memoria para los índices
    int* indices = new int[*numResultados];
    int posicion = 0;
    
    archivo.clear();
    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    
    // Llenar el arreglo con los índices
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.read(reinterpret_cast<char*>(&producto), sizeof(Producto));
        if (!producto.isEliminado() && contieneSubstring(producto.getNombre().c_str(), nombre)) {
            indices[posicion] = i;
            posicion++;
        }
    }
    
    archivo.close();
    return indices;
}

int* ProductoManager::buscarPorCodigo(const char* codigo, int* numResultados) const {
    ArchivoHeader header = leerHeader();
    *numResultados = 0;

    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()) {
        cout << ROJO << "Error abriendo archivo de búsqueda." << endl << RESET;
        return nullptr;
    }

    Producto producto;
    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    
    // Primero contar cuántos coinciden
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.read(reinterpret_cast<char*>(&producto), sizeof(Producto));
        if (!producto.isEliminado() && contieneSubstring(producto.getCodigo().c_str(), codigo)) {
            (*numResultados)++;
        }
    }
    
    if (*numResultados == 0) {
        archivo.close();
        return nullptr;
    }
    
    // Reservar memoria para los índices
    int* indices = new int[*numResultados];
    int posicion = 0;
    
    archivo.clear();
    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    
    // Llenar el arreglo con los índices
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.read(reinterpret_cast<char*>(&producto), sizeof(Producto));
        if (!producto.isEliminado() && contieneSubstring(producto.getCodigo().c_str(), codigo)) {
            indices[posicion] = i;
            posicion++;
        }
    }
    
    archivo.close();
    return indices;
}

Producto* ProductoManager::listarPorProveedor(int idProveedor, int* numResultados) const {
    ArchivoHeader header = leerHeader();
    *numResultados = 0;

    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()) {
        return nullptr;
    }

    Producto producto;
    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    
    // Primero contar cuántos coinciden
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.read(reinterpret_cast<char*>(&producto), sizeof(Producto));
        if (!producto.isEliminado() && producto.getIdProveedor() == idProveedor) {
            (*numResultados)++;
        }
    }
    
    if (*numResultados == 0) {
        archivo.close();
        return nullptr;
    }
    
    // Reservar memoria para los productos
    Producto* productos = new Producto[*numResultados];
    int posicion = 0;
    
    archivo.clear();
    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    
    // Llenar el arreglo con los productos
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.read(reinterpret_cast<char*>(&producto), sizeof(Producto));
        if (!producto.isEliminado() && producto.getIdProveedor() == idProveedor) {
            productos[posicion] = producto;
            posicion++;
        }
    }
    
    archivo.close();
    return productos;
}

bool ProductoManager::codigoDuplicado(const char* codigo) const {
    ArchivoHeader header = leerHeader();
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()) return false;

    Producto producto;
    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.read(reinterpret_cast<char*>(&producto), sizeof(Producto));
        if (!producto.isEliminado() && strcmp(producto.getCodigo().c_str(), codigo) == 0) {
            archivo.close();
            return true;
        }
    }
    archivo.close();
    return false;
}

void ProductoManager::mostrarDetalleProducto(const Producto& producto) const {
    cout << CYAN << left 
               << setw(6) << producto.getId()
               << setw(15) << producto.getCodigo()
               << setw(30) << producto.getNombre().substr(0, 29)
               << setw(40) << producto.getDescripcion().substr(0, 39)
               << setw(10) << producto.getStock()
               << setw(12) << producto.getStockMinimo()
               << setw(15) << fixed << setprecision(2) << producto.getPrecio()
               << setw(12) << producto.getIdProveedor()
               << setw(15) << producto.getFechaRegistro() << RESET;
    
    if (producto.tieneStockBajo()) {
        cout << ROJO << " [STOCK BAJO]" << RESET;
    }
    cout << endl;
}

void ProductoManager::mostrarEncabezadoProductos() const {
    cout << AZUL << NEGRITA << left
               << setw(6) << "ID"
               << setw(15) << "CÓDIGO"
               << setw(30) << "NOMBRE"
               << setw(40) << "DESCRIPCIÓN"
               << setw(10) << "STOCK"
               << setw(12) << "STOCK MIN"
               << setw(15) << "PRECIO"
               << setw(12) << "PROV_ID"
               << setw(15) << "FECHA REG." << RESET << endl;
    imprimirSeparador(160, '-');
}

void ProductoManager::imprimirSeparador(int ancho, char simbolo) const {
    cout << string(ancho, simbolo) << endl;
}

bool ProductoManager::solicitarDatosProducto(Producto& producto) const {
    char temp[200];
    
    if (!solicitarTexto("Ingrese nombre del Producto", temp, 100)) return false;
    producto.setNombre(temp);
    
    if (!solicitarTexto("Ingrese código del producto", temp, 20)) return false;
    if (codigoDuplicado(temp)) {
        cout << ROJO << "Error: El código ya existe." << endl << RESET;
        return false;
    }
    producto.setCodigo(temp);

    if (!solicitarTexto("Ingrese descripción del producto", temp, 200)) return false;
    producto.setDescripcion(temp);
    
    float precio;
    if (!solicitarFloat("Ingrese precio del producto", precio) || !Producto::validarPrecio(precio)) return false;
    producto.setPrecio(precio);
    
    int stock;
    if (!solicitarEntero("Ingrese Stock del producto", stock) || stock < 0) return false;
    producto.setStock(stock);
    
    int stockMinimo;
    if (!solicitarEntero("Ingrese Stock Mínimo de seguridad", stockMinimo) || stockMinimo < 0) return false;
    producto.setStockMinimo(stockMinimo);
    
    int idProveedor;
    if (!solicitarEntero("Ingrese ID del proveedor", idProveedor) || idProveedor <= 0) return false;
    if (!existeEntidad(archivoProveedores, idProveedor)) {
        cout << endl << ROJO << "Error: El proveedor no existe. Debe crearlo primero" << endl << RESET;
        return false;
    }
    producto.setIdProveedor(idProveedor);
    
    return true;
}

bool ProductoManager::solicitarTexto(const char* prompt, char* destino, int largo) const {
    cout << CYAN << prompt << ": " << RESET;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.getline(destino, largo);
    
    if (strlen(destino) == 0 || strlen(destino) >= largo) {
        cout << ROJO << "Error: El texto no puede estar vacío ni exceder " << largo-1 << " caracteres." << endl << RESET;
        return false;
    }
    return true;
}

bool ProductoManager::solicitarFloat(const char* prompt, float& valor) const {
    cout << CYAN << prompt << ": " << RESET;
    cin >> valor;
    
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << ROJO << "Error: Debe ingresar un número válido." << endl << RESET;
        return false;
    }
    return true;
}

bool ProductoManager::solicitarEntero(const char* prompt, int& valor) const {
    cout << CYAN << prompt << ": " << RESET;
    cin >> valor;
    
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << ROJO << "Error: Debe ingresar un número entero válido." << endl << RESET;
        return false;
    }
    return true;
}

// Métodos estáticos
bool ProductoManager::contieneSubstring(const char* texto, const char* busqueda) {
    char textoLower[200];
    char busquedaLower[200];
    
    strcpy(textoLower, texto);
    strcpy(busquedaLower, busqueda);
    
    convertirAMinusculas(textoLower);
    convertirAMinusculas(busquedaLower);
    
    return strstr(textoLower, busquedaLower) != nullptr;
}

void ProductoManager::convertirAMinusculas(char* cadena) {
    if (cadena == nullptr) return;
    
    for (int i = 0; cadena[i] != '\0'; i++) {
        cadena[i] = tolower(cadena[i]);
    }
}

bool ProductoManager::listarProductos() const {
    ArchivoHeader header = leerHeader();
    if (header.registrosActivos == 0) {
        cout << ROJO << "No hay productos en el Sistema." << endl << RESET;
        return false;
    }
    
    cout << VERDE << NEGRITA << "LISTADO COMPLETO DE PRODUCTOS" << endl << RESET;
    mostrarEncabezadoProductos();
    
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()) {
        cout << ROJO << "Error: No se pudo abrir el archivo de productos." << endl << RESET;
        return false;
    }
    
    Producto producto;
    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.read(reinterpret_cast<char*>(&producto), sizeof(Producto));
        if (!producto.isEliminado()) {
            mostrarDetalleProducto(producto);
        }
    }
    
    archivo.close();
    imprimirSeparador();
    return true;
}

bool ProductoManager::actualizarProducto() {
    ArchivoHeader header = leerHeader();
    if (header.registrosActivos == 0) {
        cout << ROJO << "No hay productos en el Sistema." << endl << RESET;
        return false;
    }
    
    int id;
    if (!solicitarEntero("Ingrese el ID del producto a actualizar", id)) return false;
    
    int indice = buscarIndicePorId(id);
    if (indice == -1) {
        cout << ROJO << "Error: El producto con ID " << id << " no existe." << endl << RESET;
        return false;
    }
    
    Producto producto = obtenerRegistroPorIndice(indice);
    
    cout << AZUL << NEGRITA << "DATOS ACTUALES DEL PRODUCTO:" << endl << RESET;
    mostrarEncabezadoProductos();
    mostrarDetalleProducto(producto);
    imprimirSeparador();
    
    // Aquí iría la lógica de actualización de campos
    cout << AMARILLO << "Función de actualización completa en desarrollo." << endl << RESET;
    return true;
}

bool ProductoManager::actualizarStock() {
    ArchivoHeader header = leerHeader();
    if (header.registrosActivos == 0) {
        cout << ROJO << "No hay productos en el Sistema." << endl << RESET;
        return false;
    }
    
    int id;
    if (!solicitarEntero("Ingrese el ID del producto", id)) return false;
    
    int indice = buscarIndicePorId(id);
    if (indice == -1) {
        cout << ROJO << "Error: El producto con ID " << id << " no existe." << endl << RESET;
        return false;
    }
    
    Producto producto = obtenerRegistroPorIndice(indice);
    cout << CYAN << "Stock actual: " << producto.getStock() << endl << RESET;
    
    int nuevoStock;
    if (!solicitarEntero("Ingrese el nuevo stock", nuevoStock) || nuevoStock < 0) {
        cout << ROJO << "Stock inválido." << endl << RESET;
        return false;
    }
    
    producto.setStock(nuevoStock);
    if (actualizarRegistro(indice, producto)) {
        cout << VERDE << "Stock actualizado correctamente." << endl << RESET;
        return true;
    }
    
    cout << ROJO << "Error al actualizar el stock." << endl << RESET;
    return false;
}

bool ProductoManager::eliminarProducto() {
    ArchivoHeader header = leerHeader();
    if (header.registrosActivos == 0) {
        cout << ROJO << "No hay productos en el Sistema." <<endl << RESET;
        return false;
    }
    
    int id;
    if (!solicitarEntero("Ingrese el ID del producto a eliminar", id)) return false;
    
    int indice = buscarIndicePorId(id);
    if (indice == -1) {
        cout << ROJO << "Error: El producto con ID " << id << " no existe." << endl << RESET;
        return false;
    }
    
    Producto producto = obtenerRegistroPorIndice(indice);
    cout << AZUL << NEGRITA << "PRODUCTO A ELIMINAR:" << endl << RESET;
    mostrarEncabezadoProductos();
    mostrarDetalleProducto(producto);
    
    char decision;
    cout << ROJO << "¿Está seguro de eliminar este producto? (S/N): " << RESET;
    cin >> decision;
    
    if (decision == 's' || decision == 'S') {
        producto.setEliminado(true);
        if (actualizarRegistro(indice, producto)) {
            header.registrosActivos--;
            actualizarHeader(header);
            cout << VERDE << "Producto eliminado correctamente." << endl << RESET;
            return true;
        }
    }
    
    cout << AMARILLO << "Operación cancelada." << endl << RESET;
    return false;
}


