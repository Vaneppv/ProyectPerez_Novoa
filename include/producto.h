#ifndef PRODUCTO_H
#define PRODUCTO_H

#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;

class Producto {
private:
    int id;
    string codigo;
    string nombre;
    string descripcion;
    int idProveedor;
    float precio;
    int stock;
    string fechaRegistro;
    
    // Estadísticas del registro
    int stockMinimo;                
    int totalVendido;            // Cantidad de productos vendidos
    
    // Metadata de Control Obligatoria
    bool eliminado;              // Para BORRADO LOGICO
    string fechaUltimaModificacion;

public:
    // Constructores
    Producto();
    Producto(int id, const std::string& codigo, const std::string& nombre, 
             const string& descripcion, int idProveedor, float precio, 
             int stock, int stockMinimo = 10);
    
    // Getters
    int getId() const;
    string getCodigo() const;
    string getNombre() const;
    string getDescripcion() const;
    int getIdProveedor() const;
    float getPrecio() const;
    int getStock() const;
    string getFechaRegistro() const;
    int getStockMinimo() const;
    int getTotalVendido() const;
    bool isEliminado() const;
    string getFechaUltimaModificacion() const;
    
    // Setters
    void setId(int id);
    void setCodigo(const std::string& codigo);
    void setNombre(const std::string& nombre);
    void setDescripcion(const std::string& descripcion);
    void setIdProveedor(int idProveedor);
    void setPrecio(float precio);
    void setStock(int stock);
    void setStockMinimo(int stockMinimo);
    void setTotalVendido(int totalVendido);
    void setEliminado(bool eliminado);
    void setFechaUltimaModificacion(const std::string& fecha);
    
    // Métodos de negocio
    bool tieneStockBajo() const;
    bool hayStockSuficiente(int cantidad) const;
    void reducirStock(int cantidad);
    void aumentarStock(int cantidad);
    void registrarVenta(int cantidad);
    string toString() const;
    
    // Métodos estáticos
    static string obtenerFechaActual();
    static bool validarPrecio(float precio);
    static bool validarStock(int stock);
};

class ProductoManager {
private:
    char nombreArchivo[50];
    char archivoProveedores[50];
    
    struct ArchivoHeader {
        int cantidadRegistros;
        int proximoID;
        int registrosActivos;
        int version;
    };
    
    // Métodos privados para manejo de archivos
    ArchivoHeader leerHeader() const;
    bool actualizarHeader(const ArchivoHeader& header);
    bool guardarNuevoRegistro(const Producto& producto);
    bool actualizarRegistro(int indice, const Producto& producto);
    int buscarIndicePorId(int id) const;
    Producto obtenerRegistroPorIndice(int indice) const;
    bool existeEntidad(const char* nombreArchivo, int id) const;
    
public:
    // Constructor
    ProductoManager(const char* archivoProductos = "productos.bin", 
                   const char* archivoProveedores = "proveedores.bin");
    
    // Métodos de inicialización
    bool inicializarArchivo();
    bool verificarProveedorDisponible() const;
    
    // Operaciones CRUD
    bool crearProducto();
    bool buscarProducto() const;
    bool actualizarProducto();
    bool actualizarStock();
    bool listarProductos() const;
    bool eliminarProducto();
    
    // Métodos de búsqueda especializados
    int* buscarPorNombre(const char* nombre, int* numResultados) const;
    int* buscarPorCodigo(const char* codigo, int* numResultados) const;
    Producto* listarPorProveedor(int idProveedor, int* numResultados) const;
    Producto buscarPorId(int id) const;
    
    // Métodos de validación
    bool codigoDuplicado(const char* codigo) const;
    bool validarDatosProducto(const Producto& producto) const;
    
    // Métodos de utilidad
    void mostrarDetalleProducto(const Producto& producto) const;
    void mostrarEncabezadoProductos() const;
    void imprimirSeparador(int ancho = 160, char simbolo = '=') const;
    
    // Métodos de interacción con el usuario
    bool solicitarDatosProducto(Producto& producto) const;
    bool solicitarTexto(const char* prompt, char* destino, int largo) const;
    bool solicitarFloat(const char* prompt, float& valor) const;
    bool solicitarEntero(const char* prompt, int& valor) const;
    
    // Métodos estáticos
    static bool contieneSubstring(const char* texto, const char* busqueda);
    static void convertirAMinusculas(char* cadena);
};

#endif // PRODUCTO_H
