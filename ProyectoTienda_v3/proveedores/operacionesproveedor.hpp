#ifndef OPERACIONESPROVEEDORES_HPP
#define OPERACIONESPROVEEDORES_HPP

#include "../tienda/Tienda.hpp"

// Funciones de operaciones de proveedores
void registrarProveedor(Tienda& tienda);
void buscarProveedor(Tienda& tienda);
void actualizarProveedor(Tienda& tienda);
void listarProveedores(Tienda& tienda);
void eliminarProveedor(Tienda& tienda);

// Menús de Proveedores
void menuProveedores(Tienda& tienda);

#endif // OPERACIONESPROVEEDORES_HPP
