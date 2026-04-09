#ifndef INTERFAZ_HPP
#define INTERFAZ_HPP

#include "../tienda/Tienda.hpp"

class Interfaz {
public:
    // Método principal de ejecución
    void ejecutar();
    
    // Menú principal
    void menuPrincipal(Tienda& tienda);
    
    // Submenús
    void menuMantenimiento(Tienda& tienda);
    void menuReportes(Tienda& tienda);
    
    // Utilidades de interfaz
    void mostrarBienvenida();
    void mostrarDespedida();
    void mostrarEncabezadoSistema();
    
    // Métodos de entrada de datos (para uso en módulos)
    bool solicitarTexto(const char* prompt, char* destino, int largo);
    bool solicitarEntero(const char* prompt, int& valor);
    bool solicitarFloat(const char* prompt, float& valor);
    bool solicitarConfirmacion(const char* mensaje);

private:
    
    // Validación de entrada
    static bool validarOpcionMenu(int opcion, int min, int max);
    
    // Presentación de resultados
    static void mostrarResultadoOperacion(bool exito, const char* mensajeExito, const char* mensajeError);
};

#endif // INTERFAZ_HPP
