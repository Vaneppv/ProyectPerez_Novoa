#ifndef CONSTANTES_HPP
#define CONSTANTES_HPP

// Rutas de archivos binarios
const char* ARCHIVO_TIENDA = "datos/tienda.bin";
const char* ARCHIVO_PRODUCTOS = "datos/productos.bin";
const char* ARCHIVO_PROVEEDORES = "datos/proveedores.bin";
const char* ARCHIVO_CLIENTES = "datos/clientes.bin";
const char* ARCHIVO_TRANSACCIONES = "datos/transacciones.bin";
const char* ARCHIVO_DETALLES = "datos/detallesTransacciones.bin";
const char* ARCHIVO_BACKUP = "datos/backup.bin";

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

// Códigos de colores ANSI para la interfaz
#define RESET   "\033[00m"
#define ROJO    "\033[31m"      // Alertas y errores
#define VERDE   "\033[32m"      // Éxito y confirmaciones
#define AMARILLO "\033[33m"     // Advertencias y stock bajo
#define AZUL    "\033[34m"      // Títulos
#define CYAN    "\033[36m"      // Info secundaria
#define NEGRITA "\033[1m"

#endif // CONSTANTES_HPP
