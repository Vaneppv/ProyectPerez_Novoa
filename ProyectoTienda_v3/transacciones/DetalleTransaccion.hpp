#ifndef DETALLETRANSACCION_HPP
#define DETALLETRANSACCION_HPP

#include "../persistencia/Constantes.hpp"

class DetalleTransaccion {
private:
    int idProducto;
    int cantidad;
    float precioUnitario;
    int idTransaccion;
    bool eliminado;

public:
    // Constructores
    DetalleTransaccion();
    DetalleTransaccion(int idProducto, int cantidad, float precioUnitario, int idTransaccion);
    
    // Destructor
    ~DetalleTransaccion();

    // Getters (const)
    int getIdProducto() const;
    int getCantidad() const;
    float getPrecioUnitario() const;
    int getIdTransaccion() const;

    // Setters (con validación)
    void setIdProducto(int idProducto);
    void setCantidad(int cantidad);
    void setPrecioUnitario(float precioUnitario);
    void setIdTransaccion(int idTransaccion);

    // Métodos de validación propios de la entidad
    bool esValido() const;
    bool cantidadValida(int cantidad) const;
    bool precioValido(float precio) const;
    
    // Métodos de presentación
    void mostrarInformacionBasica() const;
    void mostrarInformacionCompleta() const;
    
    // Métodos de negocio
    float calcularSubtotal() const;
    
    // Agregados para compatibilidad con template GestorArchivos
    int getId() const;
    void setEliminado(bool eliminado);
    const char* getTipo() const;
    bool isEliminado() const;
    void setId(int id);
    const char* getDescripcion() const;
    
    // Métodos adicionales para compatibilidad con template
    void setFechaRegistro(const char* fecha);
    void setFechaUltimaModificacion(const char* fecha);
    
    // Método estático
    static int obtenerTamano();
};

#endif // DETALLETRANSACCION_HPP
