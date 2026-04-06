#ifndef CLIENTE_HPP
#define CLIENTE_HPP

#include "../persistencia/Constantes.hpp"


class Cliente {
private:
   
	int id;
    char nombre[MAX_NOMBRE];
    char cedula[MAX_CEDULA];         
    char telefono[MAX_TELEFONO];
    char email[MAX_EMAIL];       
    char direccion[MAX_DIRECCION];
    char fechaRegistro[MAX_FECHA];

    float totalCompras;

    int transaccionesIds[MAX_TRANSACCIONES_ENTIDAD];
    int cantidadTransacciones;

    bool eliminado;            // Para BORRADO LOGICO
    char fechaUltimaModificacion[MAX_FECHA];

public:
    // Constructores
    Cliente();
    Cliente(int id, const char* cedula, const char* nombre, const char* telefono, 
             const char* email, const char* direccion, float totalCompras = 0.0f, 
			 const int* transaccionesIds = nullptr, int cantidadTransacciones = 0);
    
    // Destructor
    ~Cliente();
    
    // Getters (const)
    int getId() const;
    const char* getCedula() const;
    const char* getNombre() const;
    const char* getTlf() const;
    const char* getEmail() const;
    const char* getDireccion() const;
    float getTotalcompras() const;
    int getTransaccionesIds() const;
    int getCantidadTransacciones() const;
    const char* getFechaRegistro() const;
    bool isEliminado() const;
    const char* getFechaUltimaModificacion() const;

    // Setters (con validación)
    void setId(int id);
    void setCedula(const char* cedula);
    void setNombre(const char* nombre);
    void setTlf(const char* telefono);
    void setEmail(const char* email);
    void setDireccion(const char* direccion);
    void setFechaRegistro(const char* fechaRegistro);
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
    
    // Método estático
    static int obtenerTamano();
};

#endif // CLIENTE_HPP
