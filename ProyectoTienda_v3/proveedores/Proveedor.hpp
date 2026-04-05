#ifndef PROVEEDOR_HPP
#define PROVEEDOR_HPP

#include "../persistencia/Constantes.hpp"

class Proveedor {
private:
   
	int id;
    char nombre[MAX_NOMBRE];
    char rif[MAX_RIF];         
    char telefono[MAX_TLF];
    char email[MAX_EMAIL];       
    char direccion[MAX_DIRECCION];
    char fechaRegistro[MAX_FECHA];
	
    int cantidadProductos;     // Cantidad de productos bajo el proveedor
    int productosIDs[MAX_ID];     // Arreglo de todos los productos comprados a ese proveedor
    float totalCompras;

    int transaccionesIds[MAX_ID];
    int cantidadTransacciones;

    bool eliminado;            // Para BORRADO LOGICO
    char fechaUltimaModificacion[MAX_FECHA];

public:
    // Constructores
    Proveedor();
    Proveedor(int id, const char* rif, const char* nombre, const char* telefono, 
             const char* email, const char* direccion, int cantidadProductos
			 int productosIDs, float totalCompras, int transaccionesIds, int cantidadTransacciones);
    
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
    int getProductosIDs() const;
    float getTotalcompras() const;
    int getTransaccionesIds() const;
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
    void setFechaRegistro(const char* fechaRegistro);
    void setProductosIDs(int productosIDs);
    void setTotalCompras(int totalCompras);
    void setTransaccionesIds(int transaccionesIds);
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
    
    // Método estático
    static int obtenerTamano();
};

#endif // PROVEEDOR_HPP
