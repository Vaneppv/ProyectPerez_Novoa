#ifndef OPERACIONESPRODUCTOS_HPP
#define OPERACIONESPRODUCTOS_HPP

#include "../tienda/Tienda.hpp"

// Funciones de operaciones de productos
void registrarProducto(Tienda& tienda);
void buscarProducto(Tienda& tienda);
void actualizarProducto(Tienda& tienda);
void actualizarStockProducto(Tienda& tienda);
void listarProductos(Tienda& tienda);
void eliminarProducto(Tienda& tienda);
void productosStockCritico(Tienda& tienda);
void listarPorProveedor(Tienda& tienda, int idProveedor);

// Menús de productos
void menuProductos(Tienda& tienda);

#endif // OPERACIONESPRODUCTOS_HPP
