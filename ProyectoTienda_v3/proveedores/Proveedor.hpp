#ifndef PROVEEDOR_HPP
#define PROVEEDOR_HPP

#include "../persistencia/Constantes.hpp"


class Proveedor {
private:
   
	int id;
    char nombre[MAX_NOMBRE];
    char rif[MAX_RIF];         
    char telefono[MAX_TELEFONO];
    char email[MAX_EMAIL];       
    char direccion[MAX_DIRECCION];
    char fechaRegistro[MAX_FECHA];
	
    int cantidadProductos;     // Cantidad de productos bajo el proveedor
    int productosIDs[MAX_PRODUCTOS_PROVEEDOR];     // Arreglo de todos los productos comprados a ese proveedor
    float totalCompras;

    int transaccionesIds[MAX_TRANSACCIONES_ENTIDAD];
    int cantidadTransacciones;

    bool eliminado;            // Para BORRADO LOGICO
    char fechaUltimaModificacion[MAX_FECHA];

public:
    // Constructores
    Proveedor();
    Proveedor(int id, const char* rif, const char* nombre, const char* telefono, 
             const char* email, const char* direccion, int cantidadProductos = 0,
			 const int* productosIDs = nullptr, float totalCompras = 0.0f, 
			 const int* transaccionesIds = nullptr, int cantidadTransacciones = 0);
    
    // Destructor
    ~Proveedor();
    
    // Getters (const)
    int getId() const;
    const char* getRif() const;
    const char* getNombre() const;
    const char* getTlf() const;
    const char* getEmail() const;
    const char* getDireccion() const;
    int getCantidadproductos() const;
    const int* getProductosIDs() const;
    float getTotalcompras() const;
    const int* getTransaccionesIds() const;
    int getCantidadTransacciones() const;
    const char* getFechaRegistro() const;
    bool isEliminado() const;
    const char* getFechaUltimaModificacion() const;

    // Setters (con validación)
    void setId(int id);
    void setRif(const char* rif);
    void setNombre(const char* nombre);
    void setTlf(const char* telefono);
    void setEmail(const char* email);
    void setDireccion(const char* direccion);
    void setFechaRegistro(const char* fechaRegistro);
    void setCantidadProductos(int cantidadProductos);
    void setProductosIDs(const int* productosIDs, int cantidad);
    void setTotalCompras(float totalCompras);
    void setTransaccionesIds(const int* transaccionesIds, int cantidad);
    void setCantidadTransacciones(int cantidadTransacciones);
    void setEliminado(bool eliminado);
    void setFechaUltimaModificacion(const char* fechaUltimaModificacion);

    // Métodos de validación propios de la entidad
    bool esValido() const;
    bool tieneDatosCompletos() const;
    bool emailValido() const;

    // Métodos de presentación
    void mostrarInformacionBasica() const;
    void mostrarInformacionCompleta() const;
    
    // Métodos adicionales para manejo de arreglos
    void agregarProductoID(int idProducto);
    void agregarTransaccionID(int idTransaccion);
    bool tieneProductoID(int idProducto) const;
    bool tieneTransaccionID(int idTransaccion) const;
    
    // Método estático
    static int obtenerTamano();
};

#endif // PROVEEDOR_HPP
