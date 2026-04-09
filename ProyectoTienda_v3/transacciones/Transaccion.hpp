#ifndef TRANSACCION_HPP
#define TRANSACCION_HPP

#include "DetalleTransaccion.hpp"
#include "../persistencia/Constantes.hpp"

class Transaccion {
private:
    int id;
    char tipo[MAX_TIPO_TRANSACCION];             // "COMPRA" o "VENTA"
    int idRelacionado;         // ID del proveedor (compra) o cliente (venta)
    float total;
    char descripcion[MAX_DESCRIPCION];
    char fechaRegistro[MAX_FECHA];
    int cantidadItemsDiferentes;
    bool eliminado;
    char fechaUltimaModificacion[MAX_FECHA];

public:
    // Constructores
    Transaccion();
    Transaccion(int id, const char* tipo, int idRelacionado, float total, 
                const char* descripcion);
    
    // Destructor
    ~Transaccion();

    // Getters (const)
    int getId() const;
    const char* getTipo() const;
    int getIdRelacionado() const;
    float getTotal() const;
    const char* getDescripcion() const;
    const char* getFechaRegistro() const;
    int getCantidadItemsDiferentes() const;
    bool isEliminado() const;
    const char* getFechaUltimaModificacion() const;

    // Setters (con validación)
    void setId(int id);
    void setTipo(const char* tipo);
    void setIdRelacionado(int idRelacionado);
    void setTotal(float total);
    void setDescripcion(const char* descripcion);
    void setFechaRegistro(const char* fecha);
    void setCantidadItemsDiferentes(int cantidad);
    void setEliminado(bool eliminado);
    void setFechaUltimaModificacion(const char* fecha);

    // Métodos de validación propios de la entidad
    bool esValido() const;
    bool tieneDatosCompletos() const;
    bool tipoValido(const char* tipo) const;
    bool totalValido(float total) const;

    // Métodos de presentación
    void mostrarInformacionBasica() const;
    void mostrarInformacionCompleta() const;

    // Métodos de negocio
    bool esCompra() const;
    bool esVenta() const;
    void agregarDetalle(float monto);
    void incrementarCantidadItems();
    
    // Getters adicionales
    
    // Setters adicionales
    
    // Método estático
    static int obtenerTamano();
};

#endif // TRANSACCION_HPP
