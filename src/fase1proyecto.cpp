#include <iostream>

using namespace std;


// Estructura Producto
struct Producto {
    int id;                    
    char codigo[20];           
    char nombre[100];          
    char descripcion[200];     
    int idProveedor;           
    float precio;              
    int stock;                 
    char fechaRegistro[11];
};


// Estructura Proveedor
struct Proveedor {
    int id;                    
    char nombre[100];          
    char rif[20];              
    char telefono[20];         
    char email[100];           
    char direccion[200];       
    char fechaRegistro[11];
};


// Estructura Cliente
struct Cliente {
    int id;                    
    char nombre[100];         
    char cedula[20];           
    char telefono[20];     
    char email[100];  
    char direccion[200];
    char fechaRegistro[11];
};


// Estructura Transacción (puede separarse por transaccion)
struct Transaccion {
    int id;                    
    char tipo[10];             // "COMPRA" o "VENTA"
    int idProducto;            
    int idRelacionado;         // ID del proveedor (compra) o cliente (venta)
    int cantidad;              
    float precioUnitario;      
    float total;               
    char fecha[11];            
    char descripcion[200];     // Notas adicionales (opcional)
};


// Estructura Principal: Tienda
struct Tienda {
    char nombre[100];          // Nombre de la tienda
    char rif[20];              // RIF de la tienda
    
    // Arrays dinámicos de entidades
    Producto* productos;
    int numProductos;
    int capacidadProductos;
    
    Proveedor* proveedores;
    int numProveedores;
    int capacidadProveedores;
    
    Cliente* clientes;
    int numClientes;
    int capacidadClientes;
    
    Transaccion* transacciones;
    int numTransacciones;
    int capacidadTransacciones;
    
    // Contadores para IDs autoincrementales
    int siguienteIdProducto;
    int siguienteIdProveedor;
    int siguienteIdCliente;
    int siguienteIdTransaccion;
};


// Funcion de Inicialización
void inicializarTienda(Tienda* tienda, const char* nombre, const char* rif){

}


// Funcion de Liberación
void liberarTienda(Tienda* tienda){

}


// FUNCIONES CRUD - PRODUCTOS

// Crear Productos
void crearProducto(Tienda* tienda){

}


// Buscar Producto
void buscarProducto(Tienda* tienda){

}


//Actualizar Producto
void actualizarProducto(Tienda* tienda){

}


// Actualizar Stock Manualmente
void actualizarStockProducto(Tienda* tienda){

}


// Listar Productos
void listarProductos(Tienda* tienda){

}

// Eliminar Producto
void eliminarProducto(Tienda* tienda){

}


// FUNCIONES CRUD - PROVEEDORES

// Crear Proveedores
void crearProveedor(Tienda* tienda){

}


// Buscar Proveedor
void buscarProveedor(Tienda* tienda){

}

// Actualizar Proveedor
void actualizarProveedor(Tienda* tienda){

}


// Listar Proveedores
void listarProveedores(Tienda* tienda){

}


// Eliminar Proveedor
void eliminarProveedor(Tienda* tienda){

}


// FUNCIONES CRUD - CLIENTES

// Crear Cliente
void crearCliente(Tienda* tienda){

}


// Buscar Cliente
void buscarCliente(Tienda* tienda){

}


// Actualizar Cliente
void actualizarCliente(Tienda* tienda){

}


// Listar Clientes
void listarClientes(Tienda* tienda){

}


// Eliminar Cliente
void eliminarCliente(Tienda* tienda){

}


// FUNCIONES DE TRANSACCIONES

// Registrar Compra (a Proveedor)
void registrarCompra(Tienda* tienda){

}


// Registrar Venta (a Cliente)
void registrarVenta(Tienda* tienda){

}


// Buscar Transacciones
void buscarTransacciones(Tienda* tienda){

}


// Listar Transacciones
void listarTransacciones(Tienda* tienda){

}


// Cancelar/Anular Transacción
void cancelarTransaccion(Tienda* tienda){

}


// FUNCIONES AUXILIARES

// Redimensionamiento de Arrays
void redimensionarProductos(Tienda* tienda){

}


void redimensionarProveedores(Tienda* tienda){

}


void redimensionarClientes(Tienda* tienda){

}


void redimensionarTransacciones(Tienda* tienda){

}


// VALIDACIONES
bool validarEmail(const char* email){

}


bool validarFecha(const char* fecha){

}


bool existeProducto(Tienda* tienda, int id){

}


bool existeProveedor(Tienda* tienda, int id){

}


bool existeCliente(Tienda* tienda, int id){

}


bool codigoDuplicado(Tienda* tienda, const char* codigo){

}


bool rifDuplicado(Tienda* tienda, const char* rif){

}


// BUSQUEDAS 

int buscarProductoPorId(Tienda* tienda, int id){

}


int buscarProveedorPorId(Tienda* tienda, int id){

}


int buscarClientePorId(Tienda* tienda, int id){

}

// UTILIDADES
void obtenerFechaActual(char* buffer){

}


void convertirAMinusculas(char* cadena){

}


bool contieneSubstring(const char* texto, const char* busqueda){

}


int main(){


    return 0;
}