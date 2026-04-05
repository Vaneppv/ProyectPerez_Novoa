#ifndef TIENDA_HPP
#define TIENDA_HPP

#include "../persistencia/Constantes.hpp"

class Tienda {
private:
    char nombre[MAX_NOMBRE];
    char rif[MAX_RIF];
    
    // Contadores para IDs autoincrementales
    int siguienteIdProducto;
    int siguienteIdProveedor;
    int siguienteIdCliente;
    int siguienteIdTransaccion;

public:
    // Constructores
    Tienda();
    Tienda(const char* nombre, const char* rif);
    
    // Destructor
    ~Tienda();
    
    // Getters
    const char* getNombre() const;
    const char* getRif() const;
    int getSiguienteIdProducto() const;
    int getSiguienteIdProveedor() const;
    int getSiguienteIdCliente() const;
    int getSiguienteIdTransaccion() const;
    
    // Setters
    void setNombre(const char* nombre);
    void setRif(const char* rif);
    void setSiguienteIdProducto(int id);
    void setSiguienteIdProveedor(int id);
    void setSiguienteIdCliente(int id);
    void setSiguienteIdTransaccion(int id);
    
    // Métodos de negocio
    int generarSiguienteIdProducto();
    int generarSiguienteIdProveedor();
    int generarSiguienteIdCliente();
    int generarSiguienteIdTransaccion();
    
    // Métodos de presentación
    void mostrarInformacionBasica() const;
    void mostrarInformacionCompleta() const;
    
    // Método estático
    static int obtenerTamano();
};

#endif // TIENDA_HPP
