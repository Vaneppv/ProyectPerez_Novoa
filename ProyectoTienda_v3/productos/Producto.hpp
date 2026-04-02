#ifndef PRODUCTO_HPP
#define PRODUCTO_HPP

#include "../persistencia/Constantes.hpp"

class Producto {
private:
    int id;
    char codigo[MAX_CODIGO];
    char nombre[MAX_NOMBRE];
    char descripcion[MAX_DESCRIPCION];
    int idProveedor;
    float precio;
    int stock;
    char fechaRegistro[MAX_FECHA];
    int stockMinimo;
    int totalVendido;
    bool eliminado;
    char fechaUltimaModificacion[MAX_FECHA];

public:
    // Constructores
    Producto();
    Producto(int id, const char* codigo, const char* nombre, const char* descripcion, 
             int idProveedor, float precio, int stock, int stockMinimo);
    
    // Destructor
    ~Producto();
    
    // Getters (const)
    int getId() const;
    const char* getCodigo() const;
    const char* getNombre() const;
    const char* getDescripcion() const;
    int getIdProveedor() const;
    float getPrecio() const;
    int getStock() const;
    const char* getFechaRegistro() const;
    int getStockMinimo() const;
    int getTotalVendido() const;
    bool isEliminado() const;
    const char* getFechaUltimaModificacion() const;

    // Setters (con validación)
    void setId(int id);
    void setCodigo(const char* codigo);
    void setNombre(const char* nombre);
    void setDescripcion(const char* descripcion);
    void setIdProveedor(int idProveedor);
    void setPrecio(float precio);
    void setStock(int stock);
    void setFechaRegistro(const char* fechaRegistro);
    void setStockMinimo(int stockMinimo);
    void setTotalVendido(int totalVendido);
    void setEliminado(bool eliminado);
    void setFechaUltimaModificacion(const char* fechaUltimaModificacion);

    // Métodos de validación propios de la entidad
    bool esValido() const;
    bool tieneDatosCompletos() const;
    bool precioValido(float precio) const;
    bool stockValido(int stock) const;

    // Métodos de presentación
    void mostrarInformacionBasica() const;
    void mostrarInformacionCompleta() const;

    // Métodos de gestión de relaciones
    void registrarVenta(int cantidad);
    void agregarStock(int cantidad);
    void reducirStock(int cantidad);
    bool stockDisponible(int cantidad) const;
    bool estaBajoStockMinimo() const;
    
    // Método estático
    static int obtenerTamano();
};

#endif // PRODUCTO_HPP
