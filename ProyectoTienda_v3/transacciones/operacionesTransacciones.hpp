#ifndef OPERACIONESTRANSACCIONES_HPP
#define OPERACIONESTRANSACCIONES_HPP

#include "../tienda/Tienda.hpp"

// Funciones de operaciones de transacciones
void registrarCompra(Tienda& tienda);
void registrarVenta(Tienda& tienda);
void buscarTransacciones(Tienda& tienda);
void listarTransacciones(Tienda& tienda);
void cancelarTransaccion(Tienda& tienda);


#endif // OPERACIONESTRANSACCIONES_HPP
