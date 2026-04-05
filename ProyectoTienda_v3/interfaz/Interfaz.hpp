#ifndef INTERFAZ_HPP
#define INTERFAZ_HPP

#include "../tienda/Tienda.hpp"

class Interfaz {
public:
    // Método principal de ejecución
    static void ejecutar();
    
    // Menú principal
    static void menuPrincipal(Tienda& tienda);
    
    // Submenús
    static void menuMantenimiento(Tienda& tienda);
    static void menuReportes(Tienda& tienda);
    
    // Utilidades de interfaz
    static void mostrarBienvenida();
    static void mostrarDespedida();
    static void mostrarEncabezadoSistema();
    
    // Métodos de entrada de datos (para uso en módulos)
    static bool solicitarTexto(const char* prompt, char* destino, int largo);
    static bool solicitarEntero(const char* prompt, int& valor);
    static bool solicitarFloat(const char* prompt, float& valor);
    static bool solicitarConfirmacion(const char* mensaje);

private:
    
    // Validación de entrada
    static bool validarOpcionMenu(int opcion, int min, int max);
    
    // Presentación de resultados
    static void mostrarResultadoOperacion(bool exito, const char* mensajeExito, const char* mensajeError);
};

#endif // INTERFAZ_HPP
