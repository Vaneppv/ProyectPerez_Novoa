#ifndef CONSTANTES_HPP
#define CONSTANTES_HPP

// Rutas de archivos binarios
extern const char* ARCHIVO_TIENDA;
extern const char* ARCHIVO_PRODUCTOS;
extern const char* ARCHIVO_PROVEEDORES;
extern const char* ARCHIVO_CLIENTES;
extern const char* ARCHIVO_TRANSACCIONES;
extern const char* ARCHIVO_DETALLES;
extern const char* ARCHIVO_BACKUP;

// Límites y constantes del sistema
const int MAX_PRODUCTOS_PROVEEDOR = 100;
const int MAX_TRANSACCIONES_ENTIDAD = 100;
const int MAX_NOMBRE = 100;
const int MAX_DESCRIPCION = 200;
const int MAX_CODIGO = 20;
const int MAX_RIF = 20;
const int MAX_TELEFONO = 20;
const int MAX_EMAIL = 100;
const int MAX_DIRECCION = 200;
const int MAX_FECHA = 11;
const int MAX_TIPO_TRANSACCION = 10;
const int MAX_CEDULA = 20;

// Códigos de colores ANSI para la interfaz
#define RESET   "\033[00m"
#define ROJO    "\033[31m"      // Alertas y errores
#define VERDE   "\033[32m"      // Éxito y confirmaciones
#define AMARILLO "\033[33m"     // Advertencias y stock bajo
#define AZUL    "\033[34m"      // Títulos
#define CYAN    "\033[36m"      // Info secundaria
#define NEGRITA "\033[1m"

#endif // CONSTANTES_HPP
