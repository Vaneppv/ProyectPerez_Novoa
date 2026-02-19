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
    strncpy(tienda->nombre, nombre, 99);
    tienda->nombre[99] = '\0';

    strncpy(tienda->rif, rif, 19);
    tienda->rif[19] = '\0';
    
    // Establecer capacidades iniciales
    tienda->capacidadClientes = 5;
    tienda->capacidadProductos = 5;
    tienda->capacidadProveedores = 5;
    tienda->capacidadTransacciones = 5;

    // Reservar capacidad inicial en los arreglos
    tienda->proveedores = new Proveedor [tienda->capacidadProveedores];
    tienda->clientes = new Cliente [tienda->capacidadClientes];
    tienda->productos = new Producto [tienda->capacidadProductos];
    tienda->transacciones = new Transaccion [tienda->capacidadTransacciones];

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
    if (tienda == nullptr){ // Validacion de seguridad
        return;
    }
    if (tienda->numTransacciones >= tienda->capacidadTransacciones) // Verificacion del tamaño del arreglo
    {
        redimensionarTransacciones(tienda);
    }
    
    int ProdID, ProveedorID, cantidad;
    float costoUni;

    cout << endl << "===== REGISTRAR COMPRA A PROVEEDOR =====" << endl;

    // Validar existencia del producto y del proveedor
    cout << "Ingrese ID del producto: ";
    cin >> ProdID;
    if (existeProducto(tienda, ProdID) == false){
        cout << "Error: El producto no existe." << endl;
        return;
    }
    int IndiceProducto = buscarProductoPorId(tienda, ProdID);

    cout << "Ingrese ID del proveedor: ";
    cin >> ProveedorID;
    if (existeProveedor(tienda, ProveedorID) == false)
    {
        cout << "Error: El proveedor no existe." << endl;
        return;
    }

    // Datos de Transaccion
    cout << "Ingrese cantidad de productos: ";
    cin >> cantidad;
    cout << "Ingrese costo del producto por unidad: ";
    cin >> costoUni;

    // Llenar la transaccion en el indice actual
    int i = tienda->numTransacciones;
    tienda->transacciones[i].id = tienda->siguienteIdTransaccion++;
    strcpy(tienda->transacciones[i].tipo, "COMPRA");
    tienda->transacciones[i].cantidad = cantidad;
    tienda->transacciones[i].idProducto = ProdID;
    tienda->transacciones[i].idRelacionado = ProveedorID;
    tienda->transacciones[i].precioUnitario = costoUni;
    tienda->transacciones[i].total = cantidad*costoUni;
    
    obtenerFechaActual(tienda->transacciones[i].fecha);

    cout << "Descripción: ";
    cin.ignore();  // Limpieza del buffer
    cin.getline(tienda->transacciones[i].descripcion, 200);

    char confirmar;
    cout << "¿Desea confirmar y guardar esta Compra? (S/N) ";
    cin >> confirmar;

    if (confirmar == 'S' || confirmar == 's')
    {
    // Actualizar Stock del producto
    tienda->productos[IndiceProducto].stock += cantidad;
    // Actualizacion del contador
    tienda->numTransacciones++;
    
    cout << endl << "Compra registrada con exito! \n Nuevo Stock de: ";
    cout << tienda->productos[IndiceProducto].nombre << ": " << tienda->productos[IndiceProducto].stock;
    } else {
        tienda->siguienteIdTransaccion--; // Revertimos el ID autoincremental;
        cout << endl << "Compra cancelada.";
    }   
}


// Registrar Venta (a Cliente)
void registrarVenta(Tienda* tienda){
   if (tienda == nullptr){ // Validacion de seguridad
        return;
    }
    if (tienda->numTransacciones >= tienda->capacidadTransacciones) // Verificacion del tamaño del arreglo
    {
        redimensionarTransacciones(tienda);
    }
    
    int ProdID, clienteID, cantidad;

    cout << endl << "===== REGISTRAR VENTA A CLIENTE =====" << endl;

    // Validar existencia del producto y del proveedor
    cout << "Ingrese ID del producto: ";
    cin >> ProdID;
    int IndiceProducto = buscarProductoPorId(tienda, ProdID);
    
    if (IndiceProducto == -1)
    {
        cout << "Error: El producto no existe.";
        return;
    }
    

    cout << "Ingrese ID del Cliente: ";
    cin >> clienteID;
    if (existeCliente(tienda, clienteID) == false)
    {
        cout << "Error: El cliente no existe.";
        return;
    }

    // Datos de Transaccion
    cout << "Ingrese cantidad de productos: ";
    cin >> cantidad;

    // Validacion de Stock
    if (cantidad > tienda->productos[IndiceProducto].stock)
    {
        cout << "Error: Stock insuficiente. \n Stock Disponible: " << tienda->productos[IndiceProducto].stock << endl;
        return;
    }

    // Llenar la transaccion en el indice actual
    int i = tienda->numTransacciones;
    tienda->transacciones[i].id = tienda->siguienteIdTransaccion++;
    strcpy(tienda->transacciones[i].tipo, "VENTA");
    tienda->transacciones[i].cantidad = cantidad;
    tienda->transacciones[i].idProducto = ProdID;
    tienda->transacciones[i].idRelacionado = clienteID;
    
    tienda->transacciones[i].precioUnitario = tienda->productos[IndiceProducto].precio;
    tienda->transacciones[i].total = tienda->transacciones[i].precioUnitario * cantidad;

    obtenerFechaActual(tienda->transacciones[i].fecha);

    cout << "Descripción: ";
    cin.ignore();  // Limpieza del buffer
    cin.getline(tienda->transacciones[i].descripcion, 200);

    cout << endl << "===== Resumen de compra =====" << endl;
    cout << "Cantidad: " << tienda->transacciones[i].cantidad << endl;
    cout << "Producto: " << tienda->productos[IndiceProducto].nombre << endl;
    cout << "Total a pagar: " << tienda->transacciones[i].total << endl;

    char confirmar;
    cout << "¿Desea confirmar y guardar esta Venta? (S/N) ";
    cin >> confirmar;

    if (confirmar == 'S' || confirmar == 's')
    {
    // Actualizar Stock del producto
    tienda->productos[IndiceProducto].stock -= cantidad;
    // Actualizacion del contador
    tienda->numTransacciones++;

    cout << endl << "Venta registrada con exito! \n Nuevo Stock de: ";
    cout << tienda->productos[IndiceProducto].nombre << ": " << tienda->productos[IndiceProducto].stock;
    } 
    else
    {
        tienda->siguienteIdTransaccion--; // Revertimos el ID autoincremental;
        cout << endl << "Venta cancelada.";
    }   
}


// Buscar Transacciones
void buscarTransacciones(Tienda* tienda){
    if (tienda == nullptr){ // Validacion de seguridad
        return;
    }
    
}


// Listar Transacciones
void listarTransacciones(Tienda* tienda){
if (tienda == nullptr){ // Validacion de seguridad
        return;
    }


}


// Cancelar/Anular Transacción
void cancelarTransaccion(Tienda* tienda){

}


// FUNCIONES AUXILIARES

// Redimensionamiento de Arrays
void redimensionarProductos(Tienda* tienda){
    int nuevaCapacidad = tienda->capacidadProductos * 2; // Aumento de la capacidad del arreglo al doble
    Producto* nuevoArray = new Producto[nuevaCapacidad]; // Creacion de un nuevo arreglo con la nueva capacidad

    for (int i = 0; i < tienda->numProductos; i++) // Copia de datos al nuevo arreglo
    {
        nuevoArray[i] = tienda->productos[i];
    }
    
    delete[] tienda->productos; // Liberacion del arreglo viejo
    tienda->capacidadProductos = nuevaCapacidad; // Actualizacion de capacidad
    tienda->productos = nuevoArray; // Actualizacion del punero al nuevo array
}


void redimensionarProveedores(Tienda* tienda){
    int nuevaCapacidad = tienda->capacidadProveedores * 2; // Aumento de la capacidad del arreglo al doble
    Proveedor* nuevoArray = new Proveedor[nuevaCapacidad]; // Creacion de un nuevo arreglo con la nueva capacidad

    for (int i = 0; i < tienda->numProveedores; i++) // Copia de datos al nuevo arreglo
    {
        nuevoArray[i] = tienda->proveedores[i];
    }
    
    delete[] tienda->proveedores; // Liberacion del arreglo viejo
    tienda->capacidadProveedores = nuevaCapacidad; // Actualizacion de capacidad
    tienda->proveedores = nuevoArray; // Actualizacion del punero al nuevo array
}


void redimensionarClientes(Tienda* tienda){
    int nuevaCapacidad = tienda->capacidadClientes * 2; // Aumento de la capacidad del arreglo al doble
    Cliente* nuevoArray = new Cliente[nuevaCapacidad]; // Creacion de un nuevo arreglo con la nueva capacidad

    for (int i = 0; i < tienda->numClientes; i++) // Copia de datos al nuevo arreglo
    {
        nuevoArray[i] = tienda->clientes[i];
    }
    
    delete[] tienda->clientes; // Liberacion del arreglo viejo
    tienda->capacidadClientes = nuevaCapacidad; // Actualizacion de capacidad
    tienda->clientes = nuevoArray; // Actualizacion del punero al nuevo array
}


void redimensionarTransacciones(Tienda* tienda){
    int nuevaCapacidad = tienda->capacidadTransacciones * 2;// Aumento de la capacidad del arreglo al doble
    Transaccion* nuevoArray = new Transaccion[nuevaCapacidad]; // Creacion de un nuevo arreglo con la nueva capacidad

    for (int i = 0; i < tienda->numTransacciones; i++)  // Copia de datos al nuevo arreglo
    {
        nuevoArray[i] = tienda->transacciones[i];
    }
    
    delete[] tienda->transacciones; // Liberacion del arreglo viejo
    tienda->capacidadTransacciones = nuevaCapacidad; // Actualizacion de capacidad
    tienda->transacciones = nuevoArray; // Actualizacion del punero al nuevo array
}


// VALIDACIONES
bool validarEmail(const char* email){
    const char* at = nullptr; 
    const char* dot = nullptr;

    // Recorrido de la cadena en busca del '@'
    for (int i = 0; email[i] != '\0' ; i++) 
    {
        if (email[i] == '@') 
        {
            // Una vez localizado se le asigna la direccion de memoria
            at = &email[i];
            // Recorrido de la cadena en busca del '.' 
            for (int j = i + 1; email[j] != '\0'; j++)
            {
                if (email[j] == '.')
                {
                    // Una vez localizado se le asigna la direccion de memoria
                    dot = &email[j];
                    break;
                }
            }
        break;   
        }
    }
    return (at != nullptr && dot != nullptr); // Retorna True si ambos son diferentes a Nullptr
}


bool validarFecha(const char* fecha) {

    if (strlen(fecha) != 10) return false; // Validar el tamaño de caracteres (YYYY-MM-DD)
    if (fecha[4] != '-' || fecha[7] != '-') return false; // Validar de caracteres en posicion 4 y 5 (deben ser '-')

    // Validar que sean numeros excepto los '-'
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (fecha[i] < '0' || fecha[i] > '9') return false;
    }

    // Conversion de texto char a int 
    int anio = (fecha[0]-'0')*1000 + (fecha[1]-'0')*100 + (fecha[2]-'0')*10 + (fecha[3]-'0');
    int mes  = (fecha[5]-'0')*10 + (fecha[6]-'0');
    int dia  = (fecha[8]-'0')*10 + (fecha[9]-'0');

    // Validad que la fecha este en los rangos logicos
    if (anio < 1900 || anio > 2100) return false; 
    if (mes < 1 || mes > 12) return false;
    if (dia < 1 || dia > 31) return false;

    // Validar dias para meses de 30 dias y Feb
    if ((mes == 4 || mes == 6 || mes == 9 || mes == 11) && dia > 30) return false;
    if (mes == 2) 
    {
        if (dia > 29) return false;
    }
    return true; // Fecha valida
}


bool existeProducto(Tienda* tienda, int id){

    if (tienda == nullptr){ // Validacion de seguridad
    return false;
    } 
    // Recorrido del arreglo en busca del id
    for (int i = 0; i < tienda->numProductos; i++) 
    {
        if (tienda->productos[i].id == id) 
        {
            return true; // Si lo encuenta retorna True (Si existe el producto)
        }
    }
    return false; // No existe
}


bool existeProveedor(Tienda* tienda, int id){
    if (tienda == nullptr){ // Validacion de seguridad
    return false;
    } 
    // Recorrido del arreglo en busca del id
    for (int i = 0; i < tienda->numProveedores; i++)
    {
        if (tienda->proveedores[i].id == id)
        {
            return true; // Si lo encuenta retorna True (Si existe el proveedor)
        }
    }
    return false; // No existe
}


bool existeCliente(Tienda* tienda, int id){
    if (tienda == nullptr){ // Validacion de seguridad
        return false;
    }
    // Recorrido del arreglo en busca del id
    for (int i = 0; i < tienda->numClientes; i++)
    {
        if (tienda->clientes[i].id == id)
        {
            return true; // Si lo encuenta retorna True (Si existe el cliente)
        }
    }
    return false; // No existe
}


bool codigoDuplicado(Tienda* tienda, const char* codigo){
    if (tienda == nullptr){ // Validacion de seguridad
        return false;
    }
    // Recorrido del arreglo en busca del codigo
    for (int i = 0; i < tienda->numProductos; i++)
    {
        if (strcmp(tienda->productos[i].codigo, codigo) == 0)
        {
            return true; // Si lo encuenta retorna True (Si existe el codigo)
        }
    }
    return false; // No esta duplicado
}


bool rifDuplicado(Tienda* tienda, const char* rif){
    if (tienda == nullptr){ // Validacion de seguridad
        return false;
    }
    // Recorrido del arreglo en busca del rif
    for (int i = 0; i < tienda->numProveedores; i++)
    {
        if (strcmp(tienda->proveedores[i].rif, rif) == 0)
        {
            return true; // Si lo encuenta retorna True (Si existe el rif)
        }
    }
    // Recorrido del arreglo en busca de la cedula
    for (int j = 0; j < tienda->numClientes; j++)
    {
        if (strcmp(tienda->clientes[j].cedula, rif) == 0)
        {
            return true; // Si lo encuenta retorna True (Si existe la cedula)
        }
    }
    return false; // No esta duplicado
}


// BUSQUEDAS 

int buscarProductoPorId(Tienda* tienda, int id){
    if (tienda == nullptr){ 
        return -1;
    }
    // Recorrido del arreglo en busca del producto
    for (int i = 0; i < tienda->numProductos; i++)
    {
        if (tienda->productos[i].id == id)
        {
            return i; // Si lo encuentra retorna el indice
        }
    }
    return -1; // No existe
}


int buscarProveedorPorId(Tienda* tienda, int id){
    if (tienda == nullptr){ // Validacion de seguridad
        return -1;
    }
    // Recorrido del arreglo en busca del proveedor
    for (int i = 0; i < tienda->numProveedores; i++)
    {
        if (tienda->proveedores[i].id == id)
        {
            return i; // Si lo encuentra retorna el indice
        }
    }
    return -1; // No existe
}


int buscarClientePorId(Tienda* tienda, int id){
    if (tienda == nullptr){ // Validacion de seguridad
        return -1;
    }
    // Recorrido del arreglo en busca del cliente    
    for (int i = 0; i < tienda->numClientes; i++)
    {
        if (tienda->clientes[i].id == id)
        {
            return i; // Si lo encuentra retorna el indice
        }
    }
    return -1; // No existe
}

int* buscarProductosPorNombre(Tienda* tienda, const char* nombre, int* numResultados){
    if (tienda == nullptr){ // Validacion de seguridad
        return nullptr;
    }
    // Inicializacion del contador de resultados encontrados
    *numResultados = 0;
    // Recorrido para saber tamaño del arreglo de productos encontrados
    for (int i = 0; i < tienda->numProductos; i++) // Verificar si hay coincidencias
    {
        if (contieneSubstring(tienda->productos[i].nombre, nombre))
        {
            (*numResultados)++; 
        }
    }
    if (*numResultados == 0){ // Si no hay coincidencias retorna nullptr
        return nullptr;
    }
    // Reservar memoria para arreglo de productos encontrados
    int* indiceProductos = new int[*numResultados];
    int posicion = 0;
    // Llenar arreglo con indices reales de los productos encontrados
    for (int i = 0; i < tienda->numProductos; i++) // Verificar si hay coincidencias
    {
        if (contieneSubstring(tienda->productos[i].nombre, nombre))
        {
            indiceProductos[posicion] = i;
            posicion++;
        }
    }
    return indiceProductos; // Retorna el puntero al arreglo de indices
}

// UTILIDADES
void obtenerFechaActual(char* buffer){
    time_t tiempoActual = time (0); // Obtener tiempo actual del sistema
    
    tm* fechaActual = localtime(&tiempoActual); // Conversion a estructura local

    strftime(buffer, 11, "%Y-%m-%d", fechaActual); // Organizar la informacion al formato YYYY-MM-DD
}


void convertirAMinusculas(char* cadena){
    if (cadena == nullptr){ // Validacion de Seguridad
        return;
    }
    // Conversion de caracteres a letras minusculas 
    for (int i = 0; cadena[i] != 0; i++)
    {
        cadena[i] = tolower((unsigned char)cadena[i]);
    }
}


bool contieneSubstring(const char* texto, const char* busqueda){
    if (texto == nullptr || busqueda == nullptr){ // Validacion de Seguridad
        return false;
    }
    // Creacion de variables para evitar modificar las originales
    char textoP[100];
    char busquedaP[100];
    
    // Copiar y convertir a minusculas  
    strncpy(textoP, texto, 99);
    textoP[99] = '\0';
    convertirAMinusculas(textoP);

    strncpy(busquedaP, busqueda, 99);
    busquedaP[99] = '\0';
    convertirAMinusculas(busquedaP);

    // Busqueda de coincidencias
    if (strstr(textoP, busquedaP) != nullptr){
        return true;
    }

    return false; // Retorna False si no hubo coincidencias
}


int main(){


    return 0;
}