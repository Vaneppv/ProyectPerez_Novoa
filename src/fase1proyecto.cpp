#include <iostream>
#include <iomanip>
#include <cstring>
#include <ctime>
#include <limits>

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
    char nombre[100]; // Nombre de la tienda
    char rif[20];     // RIF de la tienda
    
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
    
    // Informacion de la tienda 
    for (int i = 0; i < 99 && nombre[i] != '\0' ; i++)
    {
        tienda->nombre[i] = nombre[i];
        tienda->nombre[i + 1] = '\0';
    }
    for (int i = 0; i < 19 && rif[i] != '\0'; i++)
    {
        tienda->rif[i] = rif[i];
        tienda->rif[i + 1] = '\0'; 
    }
    

    // Reservar capacidad inicial en los arreglos
    tienda->proveedores = new Proveedor [5];
    tienda->clientes = new Cliente [5];
    tienda->productos = new Producto [5];
    tienda->transacciones = new Transaccion [5];

    // Inicializar contadores de cantidad (stock) en 0
    tienda->numProductos = 0;
    tienda->numProveedores = 0;
    tienda->numClientes = 0;
    tienda->numTransacciones = 0;

    // Inicializar IDs Autoincrementales
    tienda->siguienteIdTransaccion = 1;
    tienda->siguienteIdCliente = 1;
    tienda->siguienteIdProducto = 1;
    tienda->siguienteIdProveedor = 1;

    // Establecer capacidades iniciales
    tienda->capacidadClientes = 5;
    tienda->capacidadProductos = 5;
    tienda->capacidadProveedores = 5;
    tienda->capacidadTransacciones = 5;

}


// Funcion de Liberación
void liberarTienda(Tienda* tienda){

    // Liberar arreglos dinámicos para evitar fugas de memoria
    delete[] tienda->clientes;
    delete[] tienda->proveedores;
    delete[] tienda->productos;
    delete[] tienda->transacciones;

    // Apuntar a NULL para evitar punteros colgantes
    tienda->clientes = nullptr;
    tienda->proveedores = nullptr;
    tienda->productos = nullptr;
    tienda->transacciones = nullptr;
    
    // Reiniciar contadores de cantidad y capacidad 
    tienda->numProductos = 0;
    tienda->numProveedores = 0;
    tienda->numClientes = 0;
    tienda->numTransacciones = 0;

    tienda->capacidadClientes = 0;
    tienda->capacidadProductos = 0;
    tienda->capacidadProveedores = 0;
    tienda->capacidadTransacciones = 0;

    // Reiniciar IDs Autoincrementales
    tienda->siguienteIdTransaccion = 0;
    tienda->siguienteIdCliente = 0;
    tienda->siguienteIdProducto = 0;
    tienda->siguienteIdProveedor = 0;

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
    int nuevaCapacidad = tienda->capacidadProductos * 2;
    Producto* nuevoArray = new Producto[nuevaCapacidad];

    for (int i = 0; i < tienda->numProductos; i++)
    {
        nuevoArray[i] = tienda->productos[i];
    }
    
    delete[] tienda->productos;
    tienda->capacidadProductos = nuevaCapacidad;
    tienda->productos = nuevoArray;
}


void redimensionarProveedores(Tienda* tienda){
    int nuevaCapacidad = tienda->capacidadProveedores * 2;
    Proveedor* nuevoArray = new Proveedor[nuevaCapacidad];

    for (int i = 0; i < tienda->numProveedores; i++)
    {
        nuevoArray[i] = tienda->proveedores[i];
    }
    
    delete[] tienda->proveedores;
    tienda->capacidadProveedores = nuevaCapacidad;
    tienda->proveedores = nuevoArray;
}


void redimensionarClientes(Tienda* tienda){
    int nuevaCapacidad = tienda->capacidadClientes * 2;
    Cliente* nuevoArray = new Cliente[nuevaCapacidad];

    for (int i = 0; i < tienda->numClientes; i++)
    {
        nuevoArray[i] = tienda->clientes[i];
    }
    
    delete[] tienda->clientes;
    tienda->capacidadClientes = nuevaCapacidad;
    tienda->clientes = nuevoArray;
}


void redimensionarTransacciones(Tienda* tienda){
    int nuevaCapacidad = tienda->capacidadTransacciones * 2;
    Transaccion* nuevoArray = new Transaccion[nuevaCapacidad];

    for (int i = 0; i < tienda->numTransacciones; i++)
    {
        nuevoArray[i] = tienda->transacciones[i];
    }
    
    delete[] tienda->transacciones;
    tienda->capacidadTransacciones = nuevaCapacidad;
    tienda->transacciones = nuevoArray;
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