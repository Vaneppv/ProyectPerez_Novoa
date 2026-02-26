#include <iostream>
#include <iomanip>
#include <cstring>
#include <ctime>
#include <limits>

using namespace std;

struct Producto;
struct Proveedor;
struct Cliente;
struct Transaccion;
struct Tienda;

// PROTOTIPO DE FUNCIONES

// Inicialización y Liberación
void inicializarTienda(Tienda* tienda, const char* nombre, const char* rif);
void liberarTienda(Tienda* tienda);

// FUNCIONES AUXILIARES
// Redimensionamiento de Arrays
void redimensionarProductos(Tienda* tienda);
void redimensionarProveedores(Tienda* tienda);
void redimensionarClientes(Tienda* tienda);
void redimensionarTransacciones(Tienda* tienda);

// VALIDACIONES
bool validarEmail(const char* email);
bool validarFecha(const char* fecha);
bool existeProducto(Tienda* tienda, int id);
bool existeProveedor(Tienda* tienda, int id);
bool existeCliente(Tienda* tienda, int id);
bool codigoDuplicado(Tienda* tienda, const char* codigo);
bool rifDuplicado(Tienda* tienda, const char* rif);

// BUSQUEDAS
int buscarProductoPorId(Tienda* tienda, int id);
int buscarProveedorPorId(Tienda* tienda, int id);
int buscarProveedorPorRif(Tienda* tienda, const char* rif);
int buscarClientePorId(Tienda* tienda, int id);
int buscarTransaccionPorId(Tienda* tienda, int id);
int* buscarProductosPorNombre(Tienda* tienda, const char* nombre, int* numResultados);
int* buscarProductosPorCodigo(Tienda* tienda, const char* codigo, int* numResultados);
int* buscarProveedorPorNombre(Tienda* tienda, const char* nombre, int* numResultados);
int* buscarClientePorNombre(Tienda* tienda, const char* nombre, int* numResultados);
int buscarClientePorCedula(Tienda* tienda, const char* cedula);

// UTILIDADES
void obtenerFechaActual(char* buffer);
void convertirAMinusculas(char* cadena);
bool contieneSubstring(const char* texto, const char* busqueda);
void mostrarDetalleTransaccion(Tienda* tienda, int ind);
void mostrarDetalleProducto(Tienda* tienda, int ind);
void mostrarDetalleProveedor(Tienda* tienda, int i);
void mostrarDetalleCliente(Tienda* tienda, int i);
void imprimirSeparador(int ancho, char simbolo);
void encabezadoTabla();
void encabezadoProductos();
bool floatesPositivo(float valor);
bool IntesPositivo(int valor);
bool solicitarTexto(const char* prompt, char* destino, int largo);
bool solicitarFloat(const char* prompt, float& valor);
bool solicitarEntero(const char* prompt, int& valor);

// FUNCIONES CRUD
void crearProducto(Tienda* tienda);
void buscarProducto(Tienda* tienda);
void actualizarProducto(Tienda* tienda);
void actualizarStockProducto(Tienda* tienda);
void listarProductos(Tienda* tienda);
void eliminarProducto(Tienda* tienda);


void crearProveedor(Tienda* tienda);
void buscarProveedor(Tienda* tienda);
void actualizarProveedor(Tienda* tienda);
void listarProveedores(Tienda* tienda);
void eliminarProveedor(Tienda* tienda);

void crearCliente(Tienda* tienda);
void buscarCliente(Tienda* tienda);
void actualizarCliente(Tienda* tienda);
void listarClientes(Tienda* tienda);
void eliminarCliente(Tienda* tienda);

void registrarCompra(Tienda* tienda);
void registrarVenta(Tienda* tienda);
void buscarTransacciones(Tienda* tienda);
void listarTransacciones(Tienda* tienda);
void cancelarTransaccion(Tienda* tienda);

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
void crearProducto(Tienda* tienda) {
    if (tienda == nullptr){ // Validacion de seguridad
        cout << "Error al crear tienda" << tienda->nombre << " o no hay transacciones en el sistema." << endl;
        return;
    }

    if (tienda->numProductos >= tienda->capacidadProductos) {
        redimensionarProductos(tienda);
        return;
    }

    char decision;
    cout << "¿Desea registrar un nuevo producto? (S/N): ";
    cin >> decision;

    if (decision == 's' || decision == 'S') {
        char nombre[100], codigo[20], descripcion[200];
        float precio;
        int stock, idProveedor;

        // Validaciones de entrada
        if (!solicitarTexto("Ingrese nombre del Producto", nombre, 100)) return;
        if (!solicitarTexto("Ingrese codigo del producto", codigo, 20)) return;
        
        if (codigoDuplicado(tienda, codigo)) {
            cout << "Error: El codigo ya existe." << endl;
            return;
        }

        if (!solicitarTexto("Ingrese descripcion del producto", descripcion, 200)) return;
        
        if (!solicitarFloat("Ingrese precio del producto", precio) || !floatesPositivo(precio)) return;
        
        if (!solicitarEntero("Ingrese Stock del producto", stock) || !IntesPositivo(stock)) return;
        
        if (!solicitarEntero("Ingrese ID del proveedor", idProveedor) || !IntesPositivo(idProveedor)) return;

        if (!existeProveedor(tienda, idProveedor)) {
            cout << "Error: El proveedor no existe. Debe crearlo primero" << endl;
            return;
        }

        // Resumen de nuevos datos
        imprimirSeparador(30, '=');
        cout << "      RESUMEN DE PRODUCTO" << endl;
        imprimirSeparador(30, '=');
        cout << "Nombre: " << nombre << endl;
        cout << "Codigo: " << codigo << endl;
        cout << "Precio: " << precio << endl;
        imprimirSeparador(30, '=');

        cout << "¿Desea Guardar producto? (S/N): ";
        cin >> decision;

        if (decision == 's' || decision == 'S') {
            int i = tienda->numProductos;
            
            strncpy(tienda->productos[i].nombre, nombre, 99);
            strncpy(tienda->productos[i].codigo, codigo, 19);
            strncpy(tienda->productos[i].descripcion, descripcion, 199);
            
            tienda->productos[i].idProveedor = idProveedor;
            tienda->productos[i].precio = precio;
            tienda->productos[i].stock = stock;
            tienda->productos[i].id = tienda->siguienteIdProducto;
            obtenerFechaActual(tienda->productos[i].fechaRegistro);

            tienda->siguienteIdProducto++;
            tienda->numProductos++;

            cout << endl << "¡Producto guardado! ID asignado: " << endl;
            encabezadoProductos();
            mostrarDetalleProducto(tienda, i);
        
        } else {
            cout << endl << "Operacion cancelada." << endl;
        }
    } else {
        cout << endl << "Volviendo al menu." << endl;
    }
}

// Buscar Producto
void buscarProducto(Tienda* tienda) {
    if (tienda == nullptr){ // Validacion de seguridad
        cout << "Error al crear tienda" << tienda->nombre << " o no hay transacciones en el sistema." << endl;
        return;
    }
    if (tienda->numProductos == 0){
        cout << endl << "No hay Productos registrados en el sistema." << endl;
        return;
    }

    int seleccion;
    do {
        cout << endl << "===== MENU DE BUSQUEDA DE PRODUCTOS =====" << endl;
        cout << "1. Buscar por ID" << endl;
        cout << "2. Buscar por nombre (coincidencia parcial)" << endl;
        cout << "3. Buscar por codigo (coincidencia parcial)" << endl;
        cout << "4. Listar por proveedor" << endl;
        cout << "0. Cancelar" << endl;

        if (!solicitarEntero("Seleccione una opcion", seleccion)) {
            break; 
        }

        switch (seleccion) {
            case 1: { // Buscar por ID
                int i, id;
                if (solicitarEntero("Ingrese la ID del producto", id)) {
                    i = buscarProductoPorId(tienda, id); 
                    
                    if (i != -1) {            
                        cout << endl << "Producto encontrado:" << endl;
                        encabezadoProductos();
                        mostrarDetalleProducto(tienda, i);
                        imprimirSeparador(85, '=');
                    } else {
                        cout << "Error: El producto con ID " << id << " no existe." << endl;
                    }
                }
                break;
            }

            case 2: { // Buscar por nombre (coincidencia parcial)
                char nombreBusqueda[100];
                int numEncontrados = 0;

                if (solicitarTexto("Ingrese el nombre (o parte) del producto", nombreBusqueda, 100)) {
                    
                    int* indices = buscarProductosPorNombre(tienda, nombreBusqueda, &numEncontrados);

                    if (indices != nullptr) {
                        cout << endl << "Se encontraron " << numEncontrados << " coincidencias:" << endl;
                        
                        encabezadoProductos();
                        for (int i = 0; i < numEncontrados; i++) {
                            int indiceReal = indices[i];
                            mostrarDetalleProducto(tienda, indiceReal);
                        }
                        imprimirSeparador(85, '=');
                        delete[] indices; // liberamos memoria
                        indices = nullptr;

                    } else {
                        cout << endl << "No se encontraron productos que coincidan con: '" << nombreBusqueda << "'" << endl;
                    }
                }
                break;
            }

            case 3: { // Busqueda por codigo (coincidencia parcial)
                char codigoBusqueda[20];
                int numEncontrados = 0;

                if (solicitarTexto("Ingrese el codigo (o parte) a buscar", codigoBusqueda, 20)) {
                    
                    int* indices = buscarProductosPorCodigo(tienda, codigoBusqueda, &numEncontrados);

                    if (indices != nullptr) {
                        cout << endl << "Se encontraron " << numEncontrados << " coincidencias por codigo:" << endl;
                        
                        encabezadoProductos();
                        for (int i = 0; i < numEncontrados; i++) {
                            mostrarDetalleProducto(tienda, indices[i]);
                        }
                        imprimirSeparador(85, '=');

                        delete[] indices; 
                        indices = nullptr;
                    } else {
                        cout << endl << "No existen productos con el codigo: '" << codigoBusqueda << "'" << endl;
                    }
                }
                break;
            }

            case 4: { // Listar por proveedor
                int idProv;
                bool encontro = false;
                if (solicitarEntero("Ingrese ID del proveedor", idProv)) {
                    encabezadoProductos();
                    for (int i = 0; i < tienda->numProductos; i++) {
                        if (tienda->productos[i].idProveedor == idProv) {
                            mostrarDetalleProducto(tienda, i);
                            encontro = true;
                        }
                    }
                    imprimirSeparador(85, '=');
                    if (!encontro) {
                        cout << "No hay productos registrados para este proveedor." << endl;
                    }
                }
                break;
            }

            case 0:
                cout << "Cancelando busqueda" << endl;
                break;

            default:
                cout << "Opcion no valida." << endl;
                break;
        }
    } while (seleccion != 0);
}

//Actualizar Producto
void actualizarProducto(Tienda* tienda) {
    if (tienda == nullptr){ // Validacion de seguridad
        cout << "Error al crear tienda" << tienda->nombre << " o no hay transacciones en el sistema." << endl;
        return;
    }
    if (tienda->numProductos == 0){
        cout << endl << "No hay Productos registrados en el sistema." << endl;
        return;
    }

    int id, i;
    if (!solicitarEntero("Ingrese el ID del producto a buscar", id)) return;

    i = buscarProductoPorId(tienda, id);

    if (i == -1) {
        cout << "Error: El producto con ID " << id << " no existe." << endl;
        return;
    }

    cout << endl << "DATOS ACTUALES DEL PRODUCTO:" << endl;
    encabezadoProductos();
    mostrarDetalleProducto(tienda, i);
    imprimirSeparador(85, '=');

    char nombre[100], codigo[20], descripcion[200];
    float precio;
    int stock, idProveedor;
    int seleccion; 

    bool VerNombre = false, VerCodigo = false, VerDescrip = false;
    bool VerPrecio = false, VerStock = false, VerIdProveedor = false;
    
    // Menú de campos editables
    do {
        cout << endl << "¿Qué desea editar?" << endl;
        cout << "1. Código" << endl;
        cout << "2. Nombre" << endl;
        cout << "3. Descripción" << endl;
        cout << "4. Proveedor" << endl;
        cout << "5. Precio" << endl;
        cout << "6. Stock" << endl;
        cout << "7. Guardar cambios" << endl;
        cout << "0. Cancelar sin guardar" << endl;
        
        if (!solicitarEntero("Seleccione una opción", seleccion)) continue;

        switch (seleccion) {
            case 1:
                if (solicitarTexto("Ingrese nuevo código", codigo, 20)) {
                    if (codigoDuplicado(tienda, codigo) && strcmp(codigo, tienda->productos[i].codigo) != 0) {
                        cout << "Error: El código ya existe." << endl;
                    } else {
                        VerCodigo = true;
                    }
                }
                break;
            case 2:
                if (solicitarTexto("Ingrese nuevo nombre", nombre, 100)){
                    VerNombre = true;
                }
                break;
            case 3:
                if (solicitarTexto("Ingrese nueva descripción", descripcion, 200)) {
                    VerDescrip = true;
                }
                break;
            case 4:
                if (solicitarEntero("Ingrese ID del nuevo proveedor", idProveedor)) {
                    if (existeProveedor(tienda, idProveedor)) {
                        VerIdProveedor = true;
                    }
                    else {
                        cout << "Error: El proveedor no existe." << endl;
                    }
                }
                break;
            case 5:
                if (solicitarFloat("Ingrese nuevo precio", precio) && floatesPositivo(precio)) {
                    VerPrecio = true;
                }
                break;
            case 6:
                if (solicitarEntero("Ingrese nuevo stock", stock) && IntesPositivo(stock)) {
                    VerStock = true;
                }
                break;
            case 7: {
                char decision;
                cout << "¿Desea Guardar los cambios hechos al producto? (S/N): ";
                cin >> decision;
                if (decision == 's' || decision == 'S') {
                    if (VerNombre)      strncpy(tienda->productos[i].nombre, nombre, 100);
                    if (VerCodigo)      strncpy(tienda->productos[i].codigo, codigo, 20);
                    if (VerDescrip)     strncpy(tienda->productos[i].descripcion, descripcion, 200);
                    if (VerIdProveedor) tienda->productos[i].idProveedor = idProveedor;
                    if (VerPrecio)      tienda->productos[i].precio = precio;
                    if (VerStock)       tienda->productos[i].stock = stock;
                    cout << "¡Cambios aplicados exitosamente!" << endl;
                }
                seleccion = 0; 
                break;
            }
            case 0:
                cout << "Operación cancelada. No se guardaron cambios." << endl;
                break;
            default:
                cout << "Opción no válida." << endl;
                break;
        }
    } while (seleccion != 0);
}

// Actualizar Stock Manualmente
void actualizarStockProducto(Tienda* tienda) {
    if (tienda == nullptr){ // Validacion de seguridad
        cout << "Error al crear tienda" << tienda->nombre << " o no hay transacciones en el sistema." << endl;
        return;
    }
    if (tienda->numProductos == 0){
        cout << endl << "No hay Productos registrados en el sistema." << endl;
        return;
    }
    int id, i;
    if (!solicitarEntero("Ingrese la ID del producto", id)){
        return;
    }

    i = buscarProductoPorId(tienda, id);
    
    if (i == -1) {
        cout << "Error: El producto no existe." << endl;
        return;
    }

    int seleccion;
    char decision;
    int stockTemporal = tienda->productos[i].stock;

    do {
        cout << endl << "----------------------------" << endl;
        cout << "Producto: " << tienda->productos[i].nombre << endl;
        cout << "Stock actual del producto: " << tienda->productos[i].stock << endl;
        cout << "----------------------------" << endl;
        cout << "1. Agregar unidades" << endl;
        cout << "2. Quitar unidades" << endl;
        cout << "3. Guardar cambios" << endl;
        cout << "0. Cancelar sin guardar" << endl;
        if (!solicitarEntero("Seleccione una opción", seleccion)){
            return;
        }
        
        switch (seleccion) {
            case 1: {
                int cantidad;
                if (solicitarEntero("¿Cuantas unidades ingresan?", cantidad)) {
                    if (IntesPositivo(cantidad)) {
                        stockTemporal += cantidad; 
                        cout << "Cantidad añadida al calculo." << endl;
                    } else {
                        cout << "Error: La cantidad debe ser positiva." << endl;
                    }
                }
                break;
            }
            case 2: { // Quitar unidades
                int cantidad;
                if (solicitarEntero("¿Cuantas unidades salen?", cantidad)) {
                    if (IntesPositivo(cantidad)) {
                        if (stockTemporal - cantidad >= 0) {
                            stockTemporal -= cantidad;
                            cout << "Cantidad restada al calculo." << endl;
                        } else {
                            cout << "Error: El stock resultante no puede ser negativo." << endl;
                        }
                    } else {
                        cout << "Error: La cantidad debe ser positiva." << endl;
                    }
                }
                break;
            }
            case 3: { // Guardar cambios
                char decision;
                cout << "¿Desea confirmar y guardar el nuevo stock (" << stockTemporal << ")? (S/N): ";
                cin >> decision;
                
                if (decision == 's' || decision == 'S') {
                    tienda->productos[i].stock = stockTemporal;
                    cout << "¡Cambios guardados con exito!" << endl;
                    seleccion = 0; // Para salir del bucle tras guardar
                } else {
                    cout << "Guardado cancelado. Puede seguir editando o salir con 0." << endl;
                }
                break;
            }

            case 0: { // Cancelar
                cout << "Operacion cancelada. No se realizaron cambios en el inventario." << endl;
                break;
            }

            default: {
                cout << "Opcion no valida. Intente de nuevo." << endl;
                break;
            }
        }
    } while (seleccion != 0);
}

// Listar Productos
void listarProductos(Tienda* tienda) {
    if (tienda == nullptr){ // Validacion de seguridad
        cout << "Error al crear tienda" << tienda->nombre << " o no hay transacciones en el sistema." << endl;
        return;
    }
    if (tienda->numProductos == 0){
        cout << endl << "No hay Productos registrados en el sistema." << endl;
        return;
    }

    cout << endl << "          LISTADO GENERAL DE PRODUCTOS" << endl;
    encabezadoProductos();
    for (int i = 0; i < tienda->numProductos; i++) {
        mostrarDetalleProducto(tienda, i);
    }
    imprimirSeparador(85, '=');
}

// Eliminar Producto
void eliminarProducto(Tienda* tienda){

	if (tienda == nullptr){ // Validacion de seguridad
        cout << "Error al crear tienda" << tienda->nombre << " o no hay transacciones en el sistema." << endl;
        return;
    }
    if (tienda->numProductos == 0){
        cout << endl << "No hay Productos registrados en el sistema." << endl;
        return;
    }

    int id, i;
    cout << "Por favor ingrese la ID del producto que se va a eliminar: ";
    cin >> id;

    // 1. Buscamos el índice
    i = buscarProductoPorId(tienda, id);

    // 2. Validamos que exista
    if (i == -1) {
        cout << "Error: El producto con ID " << id << " no existe." << endl;
        return;
    }

    char decision;
    cout << "¿Seguro que desea eliminar '" << tienda->productos[i].nombre << "'? (S/N): ";
    cin >> decision;

    if (decision == 's' || decision == 'S') { // Reajuste de posicion de productos
        for (int j = i; j < tienda->numProductos - 1; ++j) {
            tienda->productos[j] = tienda->productos[j + 1];
        }
        tienda->numProductos--;
        
        cout << "Producto eliminado exitosamente." << endl;
    } else {
        cout << "Operacion cancelada." << endl;
    }
}

// FUNCIONES CRUD - PROVEEDORES

// Crear Proveedores
void crearProveedor(Tienda* tienda){

	if (tienda == nullptr){ // Validacion de seguridad
        cout << "Error al crear tienda" << tienda->nombre << " o no hay transacciones en el sistema." << endl;
        return;
    }
    if (tienda->numProveedores >= tienda->capacidadProveedores) {
        redimensionarProveedores(tienda);
        return;
    }

    char decision;
    cout << "¿Desea registrar un nuevo proveedor? (S/N): ";
    cin >> decision;

    if (decision == 's' || decision == 'S') {
        char nombre[100], rif[20], telefono[20], direccion[200], email[100];

        // Validaciones de entrada
        if (!solicitarTexto("Ingrese nombre del Proveedor", nombre, 100)) return;
        if (!solicitarTexto("Ingrese rif del Proveedor", rif, 20)) return;
        
        if (rifDuplicado(tienda, rif)) {
            cout << "Error: El rif ya existe." << endl;
            return;
        }
        
        if (!solicitarTexto("Ingrese el telefono del Proveedor", telefono, 20)) return;
        if (!solicitarTexto("Ingrese la direccion del Proveedor", direccion, 200)) return;
        if (!solicitarTexto("Ingrese la email del Proveedor", email, 100)) return;
        
        if (!validarEmail(email)) {
            cout << "Error: El correo debe incluir un '@' y al menos un '.'" << endl;
            return;
        }


        // Resumen de nuevos datos
        imprimirSeparador(30, '=');
        cout << "      RESUMEN DE PROVEEDOR" << endl;
        imprimirSeparador(30, '=');
        cout << "Nombre: " << nombre << endl;
        cout << "Telefono: " << telefono << endl;
        cout << "Email: " << email << endl;
        imprimirSeparador(30, '=');

        cout << "¿Desea Guardar proveedor? (S/N): ";
        cin >> decision;

        if (decision == 's' || decision == 'S') {
            int i = tienda->numProveedores;
            
            strncpy(tienda->proveedores[i].nombre, nombre, 99);
            strncpy(tienda->proveedores[i].rif, rif, 19);
            strncpy(tienda->proveedores[i].telefono, telefono, 19);
            strncpy(tienda->proveedores[i].direccion, direccion, 199);
            strncpy(tienda->proveedores[i].email, email, 99);

            tienda->proveedores[i].id = tienda->siguienteIdProveedor;
            obtenerFechaActual(tienda->proveedores[i].fechaRegistro);

            cout << endl << "¡Proveedor guardado! ID asignado: " << tienda->proveedores[i].id << endl;
            tienda->siguienteIdProveedor++;
            tienda->numProveedores++;
        
        } else {
            cout << endl << "Operacion cancelada." << endl;
        }
    } else {
        cout << endl << "Volviendo al menu." << endl;
    }
    

}

// Buscar Proveedor
void buscarProveedor(Tienda* tienda){

	if (tienda == nullptr){ // Validacion de seguridad
        cout << "Error al crear tienda" << tienda->nombre << " o no hay transacciones en el sistema." << endl;
        return;
    }
    if (tienda->numProveedores == 0){
        cout << endl << "No hay Proveedores registrados en el sistema." << endl;
        return;
    }

    int seleccion;
    do {
        cout << endl << "===== MENU DE BUSQUEDA DE PROVEEDORES =====" << endl;
        cout << "1. Buscar por ID" << endl;
        cout << "2. Buscar por nombre (coincidencia parcial)" << endl;
        cout << "3. Buscar por rif" << endl;
        cout << "0. Cancelar" << endl;

        if (!solicitarEntero("Seleccione una opcion", seleccion)) {
            break; 
        }

        switch (seleccion) {
            case 1: { // Buscar por ID
                int i, id;
                if (solicitarEntero("Ingrese la ID del proveedor", id)) {
                    i = buscarProveedorPorId(tienda, id); 
                    
                    if (i != -1) {            
                        cout << endl << "Proveedor encontrado:" << endl;
                        mostrarDetalleProveedor(tienda, i);
                        imprimirSeparador(85, '=');
                    } else {
                        cout << "Error: El proveedor con ID " << id << " no existe." << endl;
                    }
                }
                break;
            }

            case 2: { // Buscar por nombre (coincidencia parcial)
                char nombreBusqueda[100];
                int numEncontrados = 0;

                if (solicitarTexto("Ingrese el nombre (o parte) del proveedor", nombreBusqueda, 100)) {
                    
                    int* indices = buscarProveedorPorNombre(tienda, nombreBusqueda, &numEncontrados);

                    if (indices != nullptr) {
                        cout << endl << "Se encontraron " << numEncontrados << " coincidencias:" << endl;
                        
                        encabezadoProductos();
                        for (int i = 0; i < numEncontrados; i++) {
                            int indiceReal = indices[i];
                            mostrarDetalleProveedor(tienda, indiceReal);
                        }
                        imprimirSeparador(85, '=');
                        delete[] indices; // liberamos memoria
                        indices = nullptr;

                    } else {
                        cout << endl << "No se encontraron proveedores que coincidan con: '" << nombreBusqueda << "'" << endl;
                    }
                }
                break;
            }

            case 3: { // Buscar por rif
                char rifBusqueda[20];
                int i;
                if (solicitarTexto("Ingrese el rif completo a buscar", rifBusqueda, 20)) {
                 
                   i = buscarProveedorPorRif(tienda, rifBusqueda); 
                    
                    if (i != -1) {            
                        cout << endl << "Proveedor encontrado:" << endl;
                        mostrarDetalleProveedor(tienda, i);
                        imprimirSeparador(85, '=');
                    } else {
                        cout << "Error: El proveedor con rif " << rifBusqueda << " no existe." << endl;
                    }
                }
                break;            
            }

            case 0:
                cout << "Cancelando busqueda" << endl;
                break;

            default:
                cout << "Opcion no valida." << endl;
                break;
        }
    } while (seleccion != 0);

}

// Actualizar Proveedor
void actualizarProveedor(Tienda* tienda){
	
	if (tienda == nullptr){ // Validacion de seguridad
        cout << "Error al crear tienda" << tienda->nombre << " o no hay transacciones en el sistema." << endl;
        return;
    }
    if (tienda->numProveedores == 0){
        cout << endl << "No hay Proveedores registrados en el sistema." << endl;
        return;
    }

    int id, i;
    if (!solicitarEntero("Ingrese el ID del proveedor a buscar", id)) return;

    i = buscarProveedorPorId(tienda, id);;

    if (i == -1) {
        cout << "Error: El proveedor con ID " << id << " no existe." << endl;
        return;
    }

    cout << endl << "DATOS ACTUALES DEL PROVEEDOR:" << endl;
    mostrarDetalleProveedor(tienda, i);
    imprimirSeparador(85, '=');

    char nombre[100], rif[20], telefono[20], direccion[200], email[100];
    int seleccion; 

    bool VerNombre = false, VerTlf = false, VerEmail = false;
    bool VerRif = false, VerDir = false;

    // Menú de campos editables
    do {
        cout << endl << "¿Qué desea editar?" << endl;
        cout << "1. Nombre" << endl;
        cout << "2. Rif" << endl;
        cout << "3. Telefono" << endl;
        cout << "4. Email" << endl;
        cout << "5. Direccion" << endl;
        cout << "6. Guardar cambios" << endl;
        cout << "0. Cancelar sin guardar" << endl;
        
        if (!solicitarEntero("Seleccione una opción", seleccion)) continue;

        switch (seleccion) {
            case 1:
                if (solicitarTexto("Ingrese nuevo nombre", nombre, 100)) {
    
                        VerNombre = true;
                }
                break;
            case 2:
                if (solicitarTexto("Ingrese nuevo RIF", rif, 20)){
                    
					if (rifDuplicado(tienda, rif)) {
                        cout << "Error: El rif ya existe." << endl; 
                    }
                    else {
                        VerRif = true;
                    }
                }
                break;
            case 3:
                if (solicitarTexto("Ingrese nuevo telefono", telefono, 20)) {
                    VerTlf = true;
                }
                break;
            case 4:
                if (solicitarTexto("Ingrese email del proveedor", email, 100)) {
                    if (!validarEmail(email)) {
            		cout << "Error: El correo debe incluir un '@' y al menos un '.'" << endl;
        			}
                    else {
                        VerEmail = true;
                    }
                }
                break;
            case 5:
                if (solicitarTexto("Ingrese direccion del proveedor", direccion,200)) {
                    VerDir = true;
                }
                break;
    
            case 6: {
                char decision;
                cout << "¿Desea Guardar los cambios hechos al proveedor? (S/N): ";
                cin >> decision;
                if (decision == 's' || decision == 'S') {
                    if (VerNombre)      strncpy(tienda->proveedores[i].nombre, nombre, 100);
                    if (VerRif)      strncpy(tienda->proveedores[i].rif, rif, 20);
                    if (VerTlf)     strncpy(tienda->proveedores[i].telefono, telefono, 20);
                    if (VerEmail) strncpy(tienda->proveedores[i].email, email, 100);
                    if (VerDir)     strncpy(tienda->proveedores[i].direccion, direccion, 200);
                    
                    cout << "¡Cambios aplicados exitosamente!" << endl;
                }
                seleccion = 0; 
                break;
            }
            case 0:
                cout << "Operación cancelada. No se guardaron cambios." << endl;
                break;
            default:
                cout << "Opción no válida." << endl;
                break;
        }
    } while (seleccion != 0);

}

// Listar Proveedores
void listarProveedores(Tienda* tienda){

	if (tienda == nullptr){ // Validacion de seguridad
        cout << "Error al crear tienda" << tienda->nombre << " o no hay transacciones en el sistema." << endl;
        return;
    }
    if (tienda->numProveedores== 0){
        cout << endl << "No hay Proveedores registrados en el sistema." << endl;
        return;
    }

    cout << endl << "          LISTADO GENERAL DE PROVEEDOR" << endl;
    for (int i = 0; i < tienda->numProveedores; i++) {
        mostrarDetalleProveedor(tienda, i);
    }
    imprimirSeparador(85, '=');

}

// Eliminar Proveedor
void eliminarProveedor(Tienda* tienda){

	if (tienda == nullptr){ // Validacion de seguridad
        cout << "Error al crear tienda" << tienda->nombre << " o no hay transacciones en el sistema." << endl;
        return;
    }
    if (tienda->numProveedores == 0){
        cout << endl << "No hay Proveedores registrados en el sistema." << endl;
        return;
    }

    int id, i;
    cout << "Por favor ingrese la ID del proveedor que se va a eliminar: ";
    cin >> id;

    // 1. Buscamos el índice
    i = buscarProveedorPorId(tienda, id);

    // 2. Validamos que exista
    if (i == -1) {
        cout << "Error: El proveedor con ID " << id << " no existe." << endl;
        return;
    }

    char decision;
    cout << "¿Seguro que desea eliminar '" << tienda->proveedores[i].nombre << "'? (S/N): ";
    cin >> decision;

    if (decision == 's' || decision == 'S') { // Reajuste de posicion de productos
        for (int j = i; j < tienda->numProveedores - 1; ++j) {
            tienda->proveedores[j] = tienda->proveedores[j + 1];
        }
        tienda->numProveedores--;
        
        cout << "Proveedor eliminado exitosamente." << endl;
    } else {
        cout << "Operacion cancelada." << endl;
    }

}

// FUNCIONES CRUD - CLIENTES

// Crear Cliente
void crearCliente(Tienda* tienda){
	if (tienda == nullptr){ // Validacion de seguridad
        cout << "Error al crear tienda" << tienda->nombre << " o no hay transacciones en el sistema." << endl;
        return;
    }
    if (tienda->numClientes >= tienda->capacidadClientes) {
        redimensionarClientes(tienda);
        return;
    }

    char decision;
    cout << "¿Desea registrar un nuevo cliente? (S/N): ";
    cin >> decision;

    if (decision == 's' || decision == 'S') {
        char nombre[100], cedula[20], telefono[20], direccion[200], email[100];

        // Validaciones de entrada
        if (!solicitarTexto("Ingrese nombre del Cliente", nombre, 100)) return;
        if (!solicitarTexto("Ingrese cedula del Cliente", cedula, 20)) return;
        
        if (rifDuplicado(tienda, cedula)) {
            cout << "Error: La cedula ya existe." << endl;
            return;
        }
        
        if (!solicitarTexto("Ingrese el telefono del Cliente", telefono, 20)) return;
        if (!solicitarTexto("Ingrese la direccion del Cliente", direccion, 200)) return;
        if (!solicitarTexto("Ingrese el email del Cliente", email, 100)) return;
        
        if (!validarEmail(email)) {
            cout << "Error: El correo debe incluir un '@' y al menos un '.'" << endl;
            return;
        }

        // Resumen de nuevos datos
        imprimirSeparador(30, '=');
        cout << "      RESUMEN DE CLIENTE" << endl;
        imprimirSeparador(30, '=');
        cout << "Nombre: " << nombre << endl;
        cout << "Telefono: " << telefono << endl;
        cout << "Email: " << email << endl;
        cout << "Cedula:" << cedula << endl;
        imprimirSeparador(30, '=');

        cout << "¿Desea Guardar al cliente? (S/N): ";
        cin >> decision;

        if (decision == 's' || decision == 'S') {
            int i = tienda->numClientes;
            
            strncpy(tienda->clientes[i].nombre, nombre, 99);
            strncpy(tienda->clientes[i].cedula, cedula, 19);
            strncpy(tienda->clientes[i].telefono, telefono, 19);
            strncpy(tienda->clientes[i].direccion, direccion, 199);
            strncpy(tienda->clientes[i].email, email, 99);

            tienda->clientes[i].id = tienda->siguienteIdCliente;
            obtenerFechaActual(tienda->clientes[i].fechaRegistro);

            cout << endl << "¡Cliente guardado! ID asignado: " << tienda->clientes[i].id << endl;
            tienda->siguienteIdCliente++;
            tienda->numClientes++;

        } else {
            cout << endl << "Operacion cancelada." << endl;
        }
    } else {
        cout << endl << "Volviendo al menu." << endl;
    }

}

// Buscar Cliente
void buscarCliente(Tienda* tienda){

	if (tienda == nullptr){ // Validacion de seguridad
        cout << "Error al crear tienda" << tienda->nombre << " o no hay transacciones en el sistema." << endl;
        return;
    }
    if (tienda->numClientes == 0){
        cout << endl << "No hay Clientes registrados en el sistema." << endl;
        return;
    }

    int seleccion;
    do {
        cout << endl << "===== MENU DE BUSQUEDA DE CLIENTES =====" << endl;
        cout << "1. Buscar por ID" << endl;
        cout << "2. Buscar por nombre (coincidencia parcial)" << endl;
        cout << "3. Buscar por cedula" << endl;
        cout << "0. Cancelar" << endl;

        if (!solicitarEntero("Seleccione una opcion", seleccion)) {
            break; 
        }

        switch (seleccion) {
            case 1: { // Buscar por ID
                int i, id;
                if (solicitarEntero("Ingrese la ID del cliente", id)) {
                    i = buscarClientePorId(tienda, id); 
                    
                    if (i != -1) {            
                        cout << endl << "Cliente encontrado:" << endl;
                        mostrarDetalleCliente(tienda, i);
                        imprimirSeparador(85, '=');
                    } else {
                        cout << "Error: El cliente con ID " << id << " no existe." << endl;
                    }
                }
                break;
            }

            case 2: { // Buscar por nombre (coincidencia parcial)
                char nombreBusqueda[100];
                int numEncontrados = 0;

                if (solicitarTexto("Ingrese el nombre (o parte) del cliente", nombreBusqueda, 100)) {
                    
                    int* indices = buscarClientePorNombre(tienda, nombreBusqueda, &numEncontrados);

                    if (indices != nullptr) {
                        cout << endl << "Se encontraron " << numEncontrados << " coincidencias:" << endl;
                        
                        encabezadoProductos();
                        for (int i = 0; i < numEncontrados; i++) {
                            int indiceReal = indices[i];
                            mostrarDetalleCliente(tienda, indiceReal);
                        }
                        imprimirSeparador(85, '=');
                        delete[] indices; // liberamos memoria
                        indices = nullptr;

                    } else {
                        cout << endl << "No se encontraron clientes que coincidan con: '" << nombreBusqueda << "'" << endl;
                    }
                }
                break;
            }

            case 3: { // Buscar por cedula
                char cedulaBusqueda[20];
                int i;
                if (solicitarTexto("Ingrese la cedula completa a buscar", cedulaBusqueda, 20)) {
                 
                   i = buscarClientePorCedula(tienda, cedulaBusqueda); 
                    
                    if (i != -1) {            
                        cout << endl << "Proveedor encontrado:" << endl;
                        mostrarDetalleCliente(tienda, i);
                        imprimirSeparador(85, '=');
                    } else {
                        cout << "Error: El cliente con cedula " << cedulaBusqueda << " no existe." << endl;
                    }
                }
                break;        
            }

            case 0:
                cout << "Cancelando busqueda" << endl;
                break;

            default:
                cout << "Opcion no valida." << endl;
                break;
        }
    } while (seleccion != 0);

}

// Actualizar Cliente
void actualizarCliente(Tienda* tienda){

	if (tienda == nullptr){ // Validacion de seguridad
        cout << "Error al crear tienda" << tienda->nombre << " o no hay transacciones en el sistema." << endl;
        return;
    }
    if (tienda->numClientes == 0){
        cout << endl << "No hay Clientes registrados en el sistema." << endl;
        return;
    }

    int id, i;
    if (!solicitarEntero("Ingrese el ID del cliente a buscar", id)) return;

    i = buscarClientePorId(tienda, id);;

    if (i == -1) {
        cout << "Error: El cliente con ID " << id << " no existe." << endl;
        return;
    }

    cout << endl << "DATOS ACTUALES DEL CLIENTE:" << endl;
    mostrarDetalleCliente(tienda, i);
    imprimirSeparador(85, '=');

    char nombre[100], cedula[20], telefono[20], direccion[200], email[100];
    int seleccion; 

    bool VerNombre = false, VerTlf = false, VerEmail = false;
    bool VerCedula = false, VerDir = false;

    // Menú de campos editables
    do {
        cout << endl << "¿Qué desea editar?" << endl;
        cout << "1. Nombre" << endl;
        cout << "2. Rif" << endl;
        cout << "3. Telefono" << endl;
        cout << "4. Email" << endl;
        cout << "5. Direccion" << endl;
        cout << "6. Guardar cambios" << endl;
        cout << "0. Cancelar sin guardar" << endl;
        
        if (!solicitarEntero("Seleccione una opción", seleccion)) continue;

        switch (seleccion) {
            case 1:
                if (solicitarTexto("Ingrese nuevo nombre", nombre, 100)) {
                    VerNombre = true;
                }
                break;
            case 2:
                if (solicitarTexto("Ingrese nueva Cedula", cedula, 20)){
                    
					if (rifDuplicado(tienda, cedula)) {
                        cout << "Error: El cedula ya existe." << endl; 
                    }
                    else {
                        VerCedula = true;
                    }
                }
                break;
            case 3:
                if (solicitarTexto("Ingrese nuevo telefono", telefono, 20)) {
                    VerTlf = true;
                }
                break;
            case 4:
                if (solicitarTexto("Ingrese el nuevo email", email, 100)) {
                    if (validarEmail(email)) {
            		cout << "Error: El correo debe incluir un '@' y al menos un '.'" << endl;
        			}
                    else {
                        VerEmail = true;
                    }
                }
                break;
            case 5:
                if (solicitarTexto("Ingrese la nueva direccion", direccion,200)) {
                    VerDir = true;
                }
                break;
    
            case 6: {
                char decision;
                cout << "¿Desea Guardar los cambios hechos al cliente? (S/N): ";
                cin >> decision;
                if (decision == 's' || decision == 'S') {
                    if (VerNombre)      strncpy(tienda->clientes[i].nombre, nombre, 100);
                    if (VerCedula)      strncpy(tienda->clientes[i].cedula, cedula, 20);
                    if (VerTlf)     strncpy(tienda->clientes[i].telefono, telefono, 20);
                    if (VerEmail) strncpy(tienda->clientes[i].email, email, 100);
                    if (VerDir)     strncpy(tienda->clientes[i].direccion, direccion, 200);
                    
                    cout << "¡Cambios aplicados exitosamente!" << endl;
                }
                seleccion = 0; 
                break;
            }
            case 0:
                cout << "Operación cancelada. No se guardaron cambios." << endl;
                break;
            default:
                cout << "Opción no válida." << endl;
                break;
        }
    } while (seleccion != 0);

}

// Listar Clientes
void listarClientes(Tienda* tienda){

	if (tienda == nullptr){ // Validacion de seguridad
        cout << "Error al crear tienda" << tienda->nombre << " o no hay transacciones en el sistema." << endl;
        return;
    }
    if (tienda->numClientes == 0){
        cout << endl << "No hay Clientes registrados en el sistema." << endl;
        return;
    }

    cout << endl << "          LISTADO GENERAL DE CLIENTES" << endl;
    for (int i = 0; i < tienda->numClientes; i++) {
        mostrarDetalleCliente(tienda, i);
    }
    imprimirSeparador(85, '=');

}

// Eliminar Cliente
void eliminarCliente(Tienda* tienda){

	if (tienda == nullptr){ // Validacion de seguridad
        cout << "Error al crear tienda" << tienda->nombre << " o no hay transacciones en el sistema." << endl;
        return;
    }
    if (tienda->numClientes == 0){
        cout << endl << "No hay Clientes registrados en el sistema." << endl;
        return;
    }

    int id, i;
    cout << "Por favor ingrese la ID del cliente que se va a eliminar: ";
    cin >> id;

    // 1. Buscamos el índice
    i = buscarClientePorId(tienda, id);

    // 2. Validamos que exista
    if (i == -1) {
        cout << "Error: El cliente con ID " << id << " no existe." << endl;
        return;
    }

    char decision;
    cout << "¿Seguro que desea eliminar '" << tienda->clientes[i].nombre << "'? (S/N): ";
    cin >> decision;

    if (decision == 's' || decision == 'S') { // Reajuste de posicion de productos
        for (int j = i; j < tienda->numClientes - 1; ++j) {
            tienda->clientes[j] = tienda->clientes[j + 1];
        }
        tienda->numClientes--;
        
        cout << "Cliente eliminado exitosamente." << endl;
    } else {
        cout << "Operacion cancelada." << endl;
    }

}

// FUNCIONES DE TRANSACCIONES

// Registrar Compra (a Proveedor)
void registrarCompra(Tienda* tienda){
    if (tienda == nullptr){ // Validacion de seguridad
        cout << "Error al crear tienda" << tienda->nombre << " o no hay transacciones en el sistema." << endl;
        return;
    }
    if (tienda->numTransacciones >= tienda->capacidadTransacciones){ // Verificacion del tamaño del arreglo
        redimensionarTransacciones(tienda);
    }
    
    int ProdID, ProveedorID, cantidad;
    float costoUni;

    cout << endl << "===== REGISTRAR COMPRA A PROVEEDOR =====" << endl;

    // Validar existencia del producto y del proveedor
    if (!solicitarEntero("Ingrese ID del producto", ProdID)){
        return;
    }
    int IndiceProducto = buscarProductoPorId(tienda, ProdID);
    if (IndiceProducto == -1){
        cout << "Error: El producto no existe.";
        return;
    }
    
    if (!solicitarEntero("Ingrese ID del proveedor", ProveedorID)){
        return;
    }
    if (existeProveedor(tienda, ProveedorID) == false){
        cout << "Error: El proveedor no existe." << endl;
        return;
    }

    // Datos de Transaccion
    if (!solicitarEntero("Ingrese cantidad de productos", cantidad)){
        return;
    }
    if (!IntesPositivo(cantidad)){
        return;
    }
    
    if (!solicitarFloat("Ingrese costo del producto por unidad", costoUni)){
        return;
    }
    if (!floatesPositivo(costoUni)){
        return;
    }
    
    // Llenar la transaccion en el indice actual
    int i = tienda->numTransacciones;
    strcpy(tienda->transacciones[i].tipo, "COMPRA");
    tienda->transacciones[i].cantidad = cantidad;
    tienda->transacciones[i].idProducto = ProdID;
    tienda->transacciones[i].idRelacionado = ProveedorID;
    tienda->transacciones[i].precioUnitario = costoUni;
    tienda->transacciones[i].total = cantidad*costoUni;
    
    obtenerFechaActual(tienda->transacciones[i].fecha);
    
    if (!solicitarTexto("Ingrese descripción", tienda->transacciones[i].descripcion, 200)){
        strcpy(tienda->transacciones[i].descripcion, "Sin descripción");
    }
    
    cout << endl;
    imprimirSeparador(30, '=');
    cout << "      RESUMEN DE COMPRA" << endl;
    imprimirSeparador(30, '-');
    cout << "Cantidad: " << tienda->transacciones[i].cantidad << endl;
    cout << "Producto: " << tienda->productos[IndiceProducto].nombre << endl;
    cout << "Total a pagar: " << tienda->transacciones[i].total << endl;
    imprimirSeparador(30, '=');

    char confirmar;
    cout << "¿Desea confirmar y guardar esta Compra? (S/N) ";
    cin >> confirmar;

    if (confirmar == 'S' || confirmar == 's'){
    // Actualizar Stock del producto
    tienda->productos[IndiceProducto].stock += cantidad;
    tienda->transacciones[i].id = tienda->siguienteIdTransaccion++;
    // Actualizacion del contador
    tienda->numTransacciones++;
    
    cout << endl << "Compra registrada con exito!" << endl << "Nuevo Stock de: ";
    cout << tienda->productos[IndiceProducto].nombre << ": " << tienda->productos[IndiceProducto].stock;
    } else {
        cout << endl << "Compra cancelada.";
    }   
}

// Registrar Venta (a Cliente)
void registrarVenta(Tienda* tienda){
    if (tienda == nullptr){ // Validacion de seguridad
        cout << "Error al crear tienda" << tienda->nombre << " o no hay transacciones en el sistema." << endl;
        return;
    }
    
    if (tienda->numTransacciones >= tienda->capacidadTransacciones){ // Verificacion del tamaño del arreglo
        redimensionarTransacciones(tienda);
    }
    
    int ProdID, clienteID, cantidad;

    cout << endl << "===== REGISTRAR VENTA A CLIENTE =====" << endl;

    // Validar existencia del producto y del proveedor
    if (!solicitarEntero("Ingrese ID del producto", ProdID)){
        return;
    }
    int IndiceProducto = buscarProductoPorId(tienda, ProdID);
    if (IndiceProducto == -1){
        cout << "Error: El producto no existe.";
        return;
    }

    if (!solicitarEntero("Ingrese ID del cliente", clienteID)){
        return;
    }
    if (existeCliente(tienda, clienteID) == false){
        cout << "Error: El cliente no existe.";
        return;
    }

    // Datos de Transaccion
    if (!solicitarEntero("Ingrese cantidad de productos", cantidad)){
        return;
    }
    if (!IntesPositivo(cantidad)){
        return;
    }
    // Validacion de Stock
    if (cantidad > tienda->productos[IndiceProducto].stock){
        cout << "Error: Stock insuficiente. \n Stock Disponible: " << tienda->productos[IndiceProducto].stock << endl;
        return;
    }

    // Llenar la transaccion en el indice actual
    int i = tienda->numTransacciones;
    strcpy(tienda->transacciones[i].tipo, "VENTA");
    tienda->transacciones[i].cantidad = cantidad;
    tienda->transacciones[i].idProducto = ProdID;
    tienda->transacciones[i].idRelacionado = clienteID;
    
    tienda->transacciones[i].precioUnitario = tienda->productos[IndiceProducto].precio;
    tienda->transacciones[i].total = tienda->transacciones[i].precioUnitario * cantidad;

    obtenerFechaActual(tienda->transacciones[i].fecha);

    if (!solicitarTexto("Ingrese descripción", tienda->transacciones[i].descripcion, 200)){
        strcpy(tienda->transacciones[i].descripcion, "Sin descripción");
    }

    cout << endl;
    imprimirSeparador(30, '=');
    cout << "      RESUMEN DE VENTA" << endl;
    imprimirSeparador(30, '-');
    cout << "Cantidad: " << tienda->transacciones[i].cantidad << endl;
    cout << "Producto: " << tienda->productos[IndiceProducto].nombre << endl;
    cout << "Total a pagar: " << tienda->transacciones[i].total << endl;
    imprimirSeparador(30, '=');

    char confirmar;
    cout << "¿Desea confirmar y guardar esta Venta? (S/N) ";
    cin >> confirmar;

    if (confirmar == 'S' || confirmar == 's'){
    // Actualizar Stock del producto
    tienda->productos[IndiceProducto].stock -= cantidad;
    tienda->transacciones[i].id = tienda->siguienteIdTransaccion++;
    // Actualizacion del contador
    tienda->numTransacciones++;

    cout << endl << "Venta registrada con exito! \n Nuevo Stock de: ";
    cout << tienda->productos[IndiceProducto].nombre << ": " << tienda->productos[IndiceProducto].stock;
    } else{
        cout << endl << "Venta cancelada.";
    }   
}

// Buscar Transacciones
void buscarTransacciones(Tienda* tienda){
    if (tienda == nullptr){ // Validacion de seguridad
        cout << "Error al crear tienda" << tienda->nombre << " o no hay transacciones en el sistema." << endl;
        return;
    }

    if (tienda->numTransacciones == 0){
        cout << endl << "No hay transacciones registradas en el sistema." << endl;
        return;
    }
    
    int op;
    bool encontro;

    cout << endl << "===== Buscar Transacciones =====" << endl;
    cout << "Seleccione opcion de busqueda: " << endl 
    << "1. Buscar por ID de Transacción" << endl 
    << "2. Buscar por ID de Producto" << endl
    << "3. Buscar por ID de Cliente" << endl
    << "4. Buscar por ID de Proveedor" << endl
    << "5. Buscar por fecha exacta" << endl
    << "6. Buscar por tipo de transación (COMPRA/VENTA)" << endl
    << "0. Cancelar" << endl;
    cin >> op;
    
    if (op == 0){
        return;
    }
    
    switch (op){
    case 1:{ // Busqueda por ID de transacción
	        int TransID;
	        if (!solicitarEntero("Ingrese ID de la transacción", TransID)){
	        return;
	        }
	        int i = buscarTransaccionPorId(tienda, TransID); // Extraer ID
	        if (i == -1){ // Validación de existencia
	            cout << "Error: La transaccion no existe.";
	            break;
	        }
		
	        encabezadoTabla();
	        mostrarDetalleTransaccion(tienda, i);
	        imprimirSeparador(91, '=');
	        break;
    	}
    	
    case 2: {// Busqueda por ID del Producto
	        int prodID;
	
	        if (!solicitarEntero("Ingrese ID del producto", prodID)){
	        return;
	        }
	        if (existeProducto(tienda, prodID) == false){ // Validacion de existencia
	            cout << "Error: El producto no existe.";
	            break;
	        }
	        encontro = false;
	        encabezadoTabla();
	        
	        for (int i = 0; i < tienda->numTransacciones; i++){
	            if (tienda->transacciones[i].idProducto == prodID){ // Si encuentra el ID del producto lo muestra
	            mostrarDetalleTransaccion(tienda, i);
	            encontro = true; // Actualización de la variable booleana
	            }
	        }
	        imprimirSeparador(91, '=');
	        if (encontro == false){ // No encontro el ID en las transacciones
	            cout << "No hay transacciones para este producto." << endl;
	        }
	        break;
		}
    case 3: {
	
        int clienteID;

        if (!solicitarEntero("Ingrese ID del cliente", clienteID)){
        return;
        }
        if (existeCliente(tienda, clienteID) == false){ // Validación de existencia del cliente
            cout << "Error: El cliente no existe.";
            break; 
        }
        encontro = false;
        encabezadoTabla();

        for (int i = 0; i < tienda->numTransacciones; i++){
            if (tienda->transacciones[i].idRelacionado == clienteID){ // Si encuentra el ID del producto lo muestra
                mostrarDetalleTransaccion(tienda, i);
                encontro = true; // Actualización de la variable boolena
            }
        }
        imprimirSeparador(91, '=');
        if (encontro == false){ // No encontro el ID en las transacciones
            cout << "No hay transacciones registradas para este cliente." << endl; 
        }
    break;
	}
	
    case 4: { // Busqueda por ID de proveedor
	        int proveedorID;
	
	        if (!solicitarEntero("Ingrese ID del proveedor", proveedorID)){
	        return;
	        }
	        if (existeProveedor(tienda, proveedorID) == false){
	            cout << "Error: El proveedor no existe.";
	            break;
	        }
	
	        encabezadoTabla();
	        encontro = false;
	
	        for (int i = 0; i < tienda->numTransacciones; i++){
	            if (tienda->transacciones[i].idRelacionado == proveedorID){
	                mostrarDetalleTransaccion(tienda, i);
	                encontro = true;
	            }
	        }
	        imprimirSeparador(91, '=');
	        if (encontro == false){
	            cout << "No hay transacciones registradas para este proveedor.";
	        }
	    break;
	}
    case 5: { // Busqueda por fecha
	        char fecha[11];
	
	        if (!solicitarTexto("Ingrese fecha de la Transacción (YYYY-MM-DD)", fecha, 11)){
	            return;
	        }
	        if(validarFecha(fecha) == false){
	            cout << "Error: Formato de fecha no valido (Use YYYY-MM-DD)";
	            break;
	        }
	
	        encontro = false; 
	        encabezadoTabla();
	
	        for (int i = 0; i < tienda->numTransacciones; i++){
	            if (strcmp(tienda->transacciones[i].fecha, fecha) == 0){
	                mostrarDetalleTransaccion(tienda, i);
	                encontro = true;
	            }
	        }
	        imprimirSeparador(91, '=');
	        if (encontro == false){
	            cout << "No hay transacciones registradas en la fecha especificada.";
	        }
	    break;
	}
    case 6: {// busqueda por tipo de transaccion

        int tipo;
        cout << "1. Compra (A Proveedor)" << endl
             << "2. Venta (A Cliente)" << endl
             << "0. Cancelar" << endl
             << endl;
        if (!solicitarEntero("Ingrese tipo de transacción", tipo)){
            break;
            }
        if (tipo == 0){
            break;
        }
        if (tipo < 1 || tipo > 2){
            cout << "Opcion Invalida";
            break;
        }
        
        encontro = false;
        const char* tipoTransaccion = (tipo == 1) ? "COMPRA" : "VENTA"; 
        encabezadoTabla();

        for (int i = 0; i < tienda->numTransacciones; i++){
            if (strcmp(tienda->transacciones[i].tipo, tipoTransaccion) == 0){
                mostrarDetalleTransaccion(tienda, i);
                encontro = true;
            }
        }
        imprimirSeparador(91, '=');
        if (encontro == false){
            cout << "No hay transacciones de este tipo";
        }
        break;
	}
    default:
        break;
    }
}

// Listar Transacciones
void listarTransacciones(Tienda* tienda){
    if (tienda == nullptr){ // Validacion de seguridad
        cout << "Error al crear tienda" << tienda->nombre << " o no hay transacciones en el sistema." << endl;
        return;
    }

    if (tienda->numTransacciones == 0){
        cout << endl << "No hay transacciones registradas en el sistema." << endl;
        return;
    }
    
    cout << "LISTADO GENERAL DE TRANSACCIONES" << endl; 
    encabezadoTabla();
    for (int i = 0; i < tienda->numTransacciones; i++){
        mostrarDetalleTransaccion(tienda, i);
    }
    imprimirSeparador(91, '=');
}

// Cancelar/Anular Transacción
void cancelarTransaccion(Tienda* tienda){
    if (tienda == nullptr){ // Validacion de seguridad
        cout << "Error al crear tienda" << tienda->nombre << " o no hay transacciones en el sistema." << endl;
        return;
    }

    if (tienda->numTransacciones == 0){
        cout << endl << "No hay transacciones registradas en el sistema." << endl;
        return;
    }
    int TransID;
    cout << endl << "===== CANCELAR TRANSACCIÓN =====" << endl;
    if (!solicitarEntero("Ingrese ID de la transacción a cancelar", TransID)){
        return;
    }
    int IndiceTrans = buscarTransaccionPorId(tienda, TransID);
    if (IndiceTrans == -1)
    {
        cout << "Error: Transacción no encontrada.";
        return;
    }
    encabezadoTabla();
    mostrarDetalleTransaccion(tienda, IndiceTrans);
    imprimirSeparador(91, '=');

    char confirmar;
    cout << "¿Desea confirmar y anular esta transacción? (S/N) ";
    cin >> confirmar;

    if (confirmar == 'S' || confirmar == 's'){
        int prodID = tienda->transacciones[IndiceTrans].idProducto;
        int IndiceProd = buscarProductoPorId(tienda, prodID);
        int cantidad = tienda->transacciones[IndiceTrans].cantidad;

        // Sumar cantidad al Stock si cancelo una venta, restar cantidad al Stock si cancelo una compra
        if (IndiceProd != -1){
            if (strcmp(tienda->transacciones[IndiceTrans].tipo, "COMPRA") == 0){
                tienda->productos[IndiceProd].stock -= cantidad;
            } else {
                tienda->productos[IndiceProd].stock += cantidad;
            }
        }

        // Eliminar datos de la transacción moviendo los elementos del arreglo
        for (int j = IndiceTrans; j < tienda->numTransacciones-1; j++){
            tienda->transacciones[j] = tienda->transacciones[j + 1];
        }
        tienda->numTransacciones--;

        cout << endl << "Transacción anulada y stock actualizado" << endl;
    } else {
        cout << endl << "Operación cancelada. No se realizaron cambios en la transacción." <<  endl;
    }
}

// FUNCIONES AUXILIARES

// Redimensionamiento de Arrays
void redimensionarProductos(Tienda* tienda){
    int nuevaCapacidad = tienda->capacidadProductos * 2; // Aumento de la capacidad del arreglo al doble
    Producto* nuevoArray = new Producto[nuevaCapacidad]; // Creacion de un nuevo arreglo con la nueva capacidad

    for (int i = 0; i < tienda->numProductos; i++){ // Copia de datos al nuevo arreglo
        nuevoArray[i] = tienda->productos[i];
    }
    
    delete[] tienda->productos; // Liberacion del arreglo viejo
    tienda->capacidadProductos = nuevaCapacidad; // Actualizacion de capacidad
    tienda->productos = nuevoArray; // Actualizacion del punero al nuevo array
}

void redimensionarProveedores(Tienda* tienda){
    int nuevaCapacidad = tienda->capacidadProveedores * 2; // Aumento de la capacidad del arreglo al doble
    Proveedor* nuevoArray = new Proveedor[nuevaCapacidad]; // Creacion de un nuevo arreglo con la nueva capacidad

    for (int i = 0; i < tienda->numProveedores; i++){ // Copia de datos al nuevo arreglo
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

    for (int i = 0; i < tienda->numTransacciones; i++){ // Copia de datos al nuevo arreglo
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
    for (int i = 0; email[i] != '\0' ; i++){
        if (email[i] == '@'){
            // Una vez localizado se le asigna la direccion de memoria
            at = &email[i];
            // Recorrido de la cadena en busca del '.' 
            for (int j = i + 1; email[j] != '\0'; j++){
                if (email[j] == '.'){
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


bool validarFecha(const char* fecha){

    if (strlen(fecha) != 10) return false; // Validar el tamaño de caracteres (YYYY-MM-DD)
    if (fecha[4] != '-' || fecha[7] != '-') return false; // Validar de caracteres en posicion 4 y 5 (deben ser '-')

    // Validar que sean numeros excepto los '-'
    for (int i = 0; i < 10; i++){
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
    if (mes == 2){
        if (dia > 29){
            return false;
        } 
    }
    return true; // Fecha valida
}


bool existeProducto(Tienda* tienda, int id){

    if (tienda == nullptr){ // Validacion de seguridad
    return false;
    } 
    // Recorrido del arreglo en busca del id
    for (int i = 0; i < tienda->numProductos; i++){
        if (tienda->productos[i].id == id){
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
    for (int i = 0; i < tienda->numProveedores; i++){
        if (tienda->proveedores[i].id == id){
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
    for (int i = 0; i < tienda->numClientes; i++){
        if (tienda->clientes[i].id == id){
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
    for (int i = 0; i < tienda->numProductos; i++){
        if (strcmp(tienda->productos[i].codigo, codigo) == 0){
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
    for (int i = 0; i < tienda->numProveedores; i++){
        if (strcmp(tienda->proveedores[i].rif, rif) == 0){
            return true; // Si lo encuenta retorna True (Si existe el rif)
        }
    }
    // Recorrido del arreglo en busca de la cedula
    for (int j = 0; j < tienda->numClientes; j++){
        if (strcmp(tienda->clientes[j].cedula, rif) == 0){
            return true; // Si lo encuenta retorna True (Si existe la cedula)
        }
    }
    return false; // Si llega hasta aqui, no esta duplicado
}

// BUSQUEDAS 

int buscarProductoPorId(Tienda* tienda, int id){
    if (tienda == nullptr){ 
        return -1;
    }
    // Recorrido del arreglo en busca del producto
    for (int i = 0; i < tienda->numProductos; i++){
        if (tienda->productos[i].id == id){
            return i; // Si lo encuentra retorna el indice
        }
    }
    return -1; // Si llega hasta aqui, no existe
}

int buscarProveedorPorId(Tienda* tienda, int id){
    if (tienda == nullptr){ // Validacion de seguridad
        return -1;
    }
    // Recorrido del arreglo en busca del proveedor
    for (int i = 0; i < tienda->numProveedores; i++){
        if (tienda->proveedores[i].id == id){
            return i; // Si lo encuentra retorna el indice
        }
    }
    return -1; // Si llega hasta aqui, No existe
}

int buscarClientePorId(Tienda* tienda, int id){
    if (tienda == nullptr){ // Validacion de seguridad
        return -1;
    }
    // Recorrido del arreglo en busca del cliente    
    for (int i = 0; i < tienda->numClientes; i++){
        if (tienda->clientes[i].id == id){
            return i; // Si lo encuentra retorna el indice
        }
    }
    return -1; // Si llega hasta aqui, No existe
}

int buscarProveedorPorRif(Tienda* tienda, const char* rif){
    if (tienda == nullptr){ // Validacion de seguridad
        return -1;
    }
    // Recorrido del arreglo en busca del proveedor
    for (int i = 0; i < tienda->numProveedores; i++){
        if (strcmp(tienda->proveedores[i].rif, rif)){
            return i; // Si lo encuentra retorna el indice
        }
    }
    return -1; // Si llega hasta aqui, No existe
}

int buscarClientePorCedula(Tienda* tienda, const char* cedula){
    if (tienda == nullptr){ // Validacion de seguridad
        return -1;
    }
    // Recorrido del arreglo en busca del cliente
    for (int i = 0; i < tienda->numClientes; i++){
        if (strcmp(tienda->clientes[i].cedula, cedula)== 0){
            return i; // Si lo encuentra retorna el indice
        }
    }
    return -1; // Si llega hasta aqui, No existe
}


int buscarTransaccionPorId(Tienda* tienda, int id){
    if (tienda == nullptr){ 
        return -1;
    }
    // Recorrido del arreglo en busca de la transaccion
    for (int i = 0; i < tienda->numTransacciones; i++){
        if (tienda->transacciones[i].id == id){
            return i; // Si lo encuentra retorna el indice
        }
    }
    return -1; // Si llega hasta aqui, no existe
}

int* buscarProductosPorNombre(Tienda* tienda, const char* nombre, int* numResultados){
    if (tienda == nullptr){ // Validacion de seguridad
        return nullptr;
    }
    // Inicializacion del contador de resultados encontrados
    *numResultados = 0;
    // Recorrido para saber tamaño del arreglo de productos encontrados
    for (int i = 0; i < tienda->numProductos; i++){ // Verificar si hay coincidencias
        if (contieneSubstring(tienda->productos[i].nombre, nombre)){
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
    for (int i = 0; i < tienda->numProductos; i++){ // Verificar si hay coincidencias
        if (contieneSubstring(tienda->productos[i].nombre, nombre)){
            indiceProductos[posicion] = i;
            posicion++;
        }
    }
    return indiceProductos; // Retorna el puntero al arreglo de indices
}

int* buscarProveedorPorNombre(Tienda* tienda, const char* nombre, int* numResultados){
    if (tienda == nullptr){ // Validacion de seguridad
        return nullptr;
    }
    // Inicializacion del contador de resultados encontrados
    *numResultados = 0;
    // Recorrido para saber tamaño del arreglo de proveedor encontrados
    for (int i = 0; i < tienda->numProveedores; i++){ // Verificar si hay coincidencias
        if (contieneSubstring(tienda->proveedores[i].nombre, nombre)){
            (*numResultados)++; 
        }
    }
    if (*numResultados == 0){ // Si no hay coincidencias retorna nullptr
        return nullptr;
    }
    // Reservar memoria para arreglo de proveedores encontrados
    int* indiceProveedores = new int[*numResultados];
    int posicion = 0;
    // Llenar arreglo con indices reales de los proveedores encontrados
    for (int i = 0; i <  tienda->numProveedores; i++){ // Verificar si hay coincidencias
        if (contieneSubstring(tienda->proveedores[i].nombre, nombre)){
            indiceProveedores[posicion] = i;
            posicion++;
        }
    }
    return indiceProveedores; // Retorna el puntero al arreglo de indices
}

int* buscarClientePorNombre(Tienda* tienda, const char* nombre, int* numResultados){
    if (tienda == nullptr){ // Validacion de seguridad
        return nullptr;
    }
    // Inicializacion del contador de resultados encontrados
    *numResultados = 0;
    // Recorrido para saber tamaño del arreglo de clientes encontrados
    for (int i = 0; i < tienda->numClientes; i++){ // Verificar si hay coincidencias
        if (contieneSubstring(tienda->clientes[i].nombre, nombre)){
            (*numResultados)++; 
        }
    }
    if (*numResultados == 0){ // Si no hay coincidencias retorna nullptr
        return nullptr;
    }
    // Reservar memoria para arreglo de clientes encontrados
    int* indiceClientes = new int[*numResultados];
    int posicion = 0;
    // Llenar arreglo con indices reales de los clientes encontrados
    for (int i = 0; i <  tienda->numClientes; i++){ // Verificar si hay coincidencias
        if (contieneSubstring(tienda->clientes[i].nombre, nombre)){
            indiceClientes[posicion] = i;
            posicion++;
        }
    }
    return indiceClientes; // Retorna el puntero al arreglo de indices
}

int* buscarProductosPorCodigo(Tienda* tienda, const char* codigo, int* numResultados){
    if (tienda == nullptr){ // Validacion de seguridad
        return nullptr;
    }
    // Inicializacion del contador de resultados encontrados
    *numResultados = 0;
    // Recorrido para saber tamaño del arreglo de productos encontrados
    for (int i = 0; i < tienda->numProductos; i++){ // Verificar si hay coincidencias
        if (contieneSubstring(tienda->productos[i].codigo, codigo)){
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
    for (int i = 0; i < tienda->numProductos; i++){ // Verificar si hay coincidencias
        if (contieneSubstring(tienda->productos[i].codigo, codigo)){
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
    for (int i = 0; cadena[i] != 0; i++){
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

void mostrarDetalleTransaccion(Tienda* tienda, int ind){
    if (tienda == nullptr || ind < 0 || ind >= tienda->numTransacciones){ // Validacion de seguridad
        return;
    }
    int indprod = buscarProductoPorId(tienda, tienda->transacciones[ind].idProducto);
    char nombreProd[100];

    if (indprod != -1){
        strcpy(nombreProd, tienda->productos[indprod].nombre);
    } else {
        strcpy(nombreProd, "No encontrado");
    }
    
    cout << left << setw(10) << tienda->transacciones[ind].id 
         << setw(12) << tienda->transacciones[ind].fecha 
         << setw(10) << tienda->transacciones[ind].tipo 
         << setw(12) << tienda->transacciones[ind].idRelacionado 
         << setw(12) << tienda->transacciones[ind].idProducto
         << setw(15) << nombreProd
         << setw(10) << tienda->transacciones[ind].cantidad
         << "$" << fixed << setprecision(2) << tienda->transacciones[ind].total << endl;
    
}

void mostrarDetalleProducto(Tienda* tienda, int i) {
    if (tienda == nullptr || i < 0 || i >= tienda->numProductos) return;

    cout << left 
         << setw(8)  << tienda->productos[i].id
         << setw(15) << tienda->productos[i].codigo
         << setw(20) << tienda->productos[i].nombre
         << setw(10) << tienda->productos[i].stock
         << setw(12) << fixed << setprecision(2) << tienda->productos[i].precio
         << setw(12) << tienda->productos[i].idProveedor
         << setw(15) << tienda->productos[i].fechaRegistro << endl;
}

void mostrarDetalleProveedor(Tienda* tienda, int i) {
    if (tienda == nullptr || i < 0 || i >= tienda->numProveedores) return;

    cout << left 
         << setw(8)  << tienda->proveedores[i].id
         << setw(15) << tienda->proveedores[i].rif
         << setw(20) << tienda->proveedores[i].nombre
         << setw(10) << tienda->proveedores[i].telefono
         << setw(12) << fixed << setprecision(2) << tienda->proveedores[i].direccion
         << setw(12) << tienda->proveedores[i].email
         << setw(15) << tienda->proveedores[i].fechaRegistro << endl;
}

void mostrarDetalleCliente(Tienda* tienda, int i) {
    if (tienda == nullptr || i < 0 || i >= tienda->numClientes) return;

    cout << left 
         << setw(8)  << tienda->clientes[i].id
         << setw(15) << tienda->clientes[i].cedula
         << setw(20) << tienda->clientes[i].nombre
         << setw(10) << tienda->clientes[i].telefono
         << setw(12) << fixed << setprecision(2) << tienda->clientes[i].direccion
         << setw(12) << tienda->clientes[i].email
         << setw(15) << tienda->clientes[i].fechaRegistro << endl;
}

void encabezadoTabla(){
    cout << endl;
    imprimirSeparador(91, '=');

    cout << left 
         << setw(10) << "ID Trans" 
         << setw(12) << "Fecha" 
         << setw(10) << "Tipo" 
         << setw(12) << "ID Relac." 
         << setw(12) << "ID Prod"
         << setw(15) << "Producto" 
         << setw(10) << "Cant" 
         << setw(10) << "Total" << endl;

    imprimirSeparador(91, '=');
}

void encabezadoProductos() {
    cout << endl;
    imprimirSeparador(100, '=');
    
    cout << left 
         << setw(8)  << "ID" 
         << setw(15) << "CODIGO" 
         << setw(20) << "NOMBRE" 
         << setw(10) << "STOCK" 
         << setw(12) << "PRECIO" 
         << setw(12) << "ID PROV." 
         << setw(15) << "F.REGISTRO" << endl; // Nueva columna
         
    imprimirSeparador(100, '=');
}

void imprimirSeparador(int ancho = 91, char simbolo = '-'){
    
    cout << setfill(simbolo) << setw(ancho) << "" << setfill(' ') << endl;
}

bool floatesPositivo(float valor){
    if (valor <= 0){
        cout << "Error: El valor debe ser mayor a cero." << endl;
        return false;
    }
    return true;
}

bool IntesPositivo(int valor){
    if (valor <= 0){
        cout << "Error: El valor debe ser mayor a cero." << endl;
        return false;
    }
    return true;
}

bool solicitarTexto(const char* prompt, char* destino, int largo) {
    if (cin.peek() == '\n') { // Verifica si hay un enter en el buffer
        cin.ignore(); // Si encuentra un enter. Limpia el buffer 
    }

    char temp[200];
    cout << prompt << " (o Enter para cancelar): "; // Limpia restos del buffer    
    cin.getline(temp, 200);

    if (strlen(temp) == 0) {
        cout << "Operacion cancelada." << endl;
        imprimirSeparador();
        return false; 
    }

    strncpy(destino, temp, largo);
    return true;
}

bool solicitarEntero(const char* prompt, int& valor) {
    string entrada;
    cout << prompt << " (o Ingresa 's' para salir): ";
    cin >> entrada;

    if (entrada == "s" || entrada == "S") {
        cout << "Entrada cancelada." << endl;
        return false;
    }

    // Intentar convertir a numero
    try {
        valor = stoi(entrada);
        return true;
    } catch (...) {
        cout << "Error: Debe ingresar un numero valido." << endl;
        return solicitarEntero(prompt, valor); // Reintento (Recursividad)
    }
}

bool solicitarFloat(const char* prompt, float& valor) {
    string entrada;
    cout << prompt << " (o Ingresa 's' para salir): ";
    cin >> entrada;

    if (entrada == "s" || entrada == "S") return false;

    try {
        valor = stof(entrada);
        return true;
    } catch (...) {
        cout << "Error: Numero decimal invalido." << endl;
        return solicitarFloat(prompt, valor);
    }
}


int main(){

    Tienda* Negocio= nullptr;
    Negocio= new Tienda;

    char nombre[100] = {"JSport Maracaibo"}, rif[20]={"V-15163126"};
    int op;

    if (Negocio == nullptr) {
        cout << "Error al asignar memoria.";
        return 1;
    }
    inicializarTienda(Negocio, nombre, rif);   
    
    
    do {
        system("cls");
        imprimirSeparador(60,'=');
        cout << "             SISTEMA DE GESTION DE INVENTARIO" << endl
         << "               Tienda: " << Negocio->nombre << endl;
        imprimirSeparador(60,'=');

        cout << "Seleccione una opción: " << endl
             << "1. Gestión de Productos" << endl
             << "2. Gestión de Proveedores" << endl
             << "3. Gestión de Clientes" << endl
             << "4. Gestión de Transacciones" << endl
             << "5. Salir" << endl;
        imprimirSeparador(60, '=');
        cin >> op;
        if (!IntesPositivo(op)){
            return 1;
        }

        switch (op)
            {
            case 1: { // Gestión de Productos
            
            imprimirSeparador(60,'=');
            cout << "             SISTEMA DE GESTION DE PRODUCTOS" << endl;
            imprimirSeparador(60,'=');
            int opProd;
            do {
                cout << "Seleccione una opción: " << endl
                << "1. Registrar nuevo Productos" << endl
                << "2. Buscar Producto" << endl
                << "3. Actualizar Producto" << endl
                << "4. Actualizar Stock manualmente" << endl
                << "5. Listar todos los Productos" << endl
                << "6. Eliminar Producto" << endl
                << "0. Volver al menu principal" << endl;
                imprimirSeparador(60, '=');
                if (!IntesPositivo(opProd)){
                    return 1;
                }
                cin >> opProd;
                
                switch (opProd) {
                    case 1:{ // Registrar nuevo productos
                        crearProducto(Negocio);
                        break;
                    }
                    case 2:{ // Buscar producto
                        buscarProducto(Negocio);
                        break;
                    }
                    case 3:{ // Actualizar Producto
                        actualizarProducto(Negocio);
                        break;
                    }
                    case 4: { // Actualizar Stock manualmente
                        actualizarStockProducto(Negocio);
                        break;
                    }
                    case 5: { // Listar todos los productos
                        listarProductos(Negocio);
                        break;
                    }
                    case 6: { // Eliminar prododucto
                        eliminarProducto(Negocio);
                        break;
                    }
                    case 0: { // Volver al menu principal
                        cout << "Volviendo al menú principal...";
                        break;
                    }
                    default:
                        cout << "Opción Invalida.";
                        break;
                }
            } while (opProd != 0);

            break;
            }
            case 2: { // Gestión de Proveedores
                imprimirSeparador(60,'=');
                cout << "             SISTEMA DE GESTION DE PROVEEDORES" << endl;
                imprimirSeparador(60,'=');
                int opProv;
                do {
                    cout << "Seleccione una opción: " << endl
                    << "1. Registrar nuevo Proveedor" << endl
                    << "2. Buscar Proveedor" << endl
                    << "3. Actualizar Proveedor" << endl
                    << "4. Listar todos los Proveedores" << endl
                    << "5. Eliminar Proveedor" << endl
                    << "0. Volver al menu principal" << endl;
                    imprimirSeparador(60, '=');
                    if (!IntesPositivo(opProv)){
                        return 1;
                    }
                    cin >> opProv;
                    
                    switch (opProv) {
                        case 1:{ // Registrar nuevo proveedor
                            crearProveedor(Negocio);
                            break;
                        }
                        case 2:{ // Buscar proveedor
                            buscarProveedor(Negocio);
                            break;
                        }
                        case 3:{ // Actualizar proveedor
                            actualizarProveedor(Negocio);
                            break;
                        }
                        case 4: { // Listar todos los proveedores
                            listarProveedores(Negocio);
                            break;
                        }
                        case 5: { // Eliminar proveedor
                            eliminarProveedor(Negocio);
                            break;
                        }
                        case 0: { // Volver al menu principal
                            cout << "Volviendo al menú principal...";
                            break;
                        }
                        default:
                            cout << "Opción Invalida.";
                            break;
                    }
                } while (opProv != 0);

                break;
            }
            case 3:{ // Gestión de Clientes
                imprimirSeparador(60,'=');
                cout << "             SISTEMA DE GESTION DE CLIENTES" << endl;
                imprimirSeparador(60,'=');
                int opCliente;
                do {
                    cout << "Seleccione una opción: " << endl
                    << "1. Registrar nuevo Cliente" << endl
                    << "2. Buscar Cliente" << endl
                    << "3. Actualizar Cliente" << endl
                    << "4. Listar todos los Cliente" << endl
                    << "5. Eliminar Cliente" << endl
                    << "0. Volver al menu principal" << endl;
                    imprimirSeparador(60, '=');
                    if (!IntesPositivo(opCliente)){
                        return 1;
                    }
                    cin >> opCliente;
                    cin.ignore();

                    switch (opCliente) {
                        case 1:{ // Registrar nuevo Cliente
                            crearCliente(Negocio);
                            break;
                        }
                        case 2:{ // Buscar Cliente
                            buscarCliente(Negocio);
                            break;
                        }
                        case 3:{ // Actualizar Cliente
                            actualizarCliente(Negocio);
                            break;
                        }
                        case 4: { // Listar todos los Cliente
                            listarClientes(Negocio);
                            break;
                        }
                        case 5: { // Eliminar Cliente
                            eliminarCliente(Negocio);
                            break;
                        }
                        case 0: { // Volver al menu principal
                            cout << "Volviendo al menú principal...";
                            break;
                        }
                        default:
                            cout << "Opción Invalida.";
                            break;
                    }
                } while (opCliente != 0);

                break;
            }
            case 4:{ // Gestión de Transacciones
                imprimirSeparador(60,'=');
                cout << "             SISTEMA DE GESTION DE TRANSACCIONES" << endl;
                imprimirSeparador(60,'=');
                int opTrans;
                do {
                    cout << "Seleccione una opción: " << endl
                    << "1. Registrar Compra a Proveedor" << endl
                    << "2. Registrar Venta a Cliente" << endl
                    << "3. Buscar Transacciones" << endl
                    << "4. Listar Transacciones" << endl
                    << "5. Cancelar Transacción" << endl
                    << "0. Volver al menu principal" << endl;
                    imprimirSeparador(60, '=');
                    if (!IntesPositivo(opTrans)){
                        return 1;
                    }
                    cin >> opTrans;
                    
                    switch (opTrans)
                    {
                    case 1: { // Registrar compra a proveedor
                        registrarCompra(Negocio);      
                        break;
                    }
                    case 2: { // Registrar venta a cliente
                        registrarVenta(Negocio);
                        break;
                    }
                    case 3: { // Buscar transacciones
                        buscarTransacciones(Negocio);
                        break;    
                    }
                    case 4: { // Listar transacciones
                        listarTransacciones(Negocio);
                        break;
                    }
                    case 5: { // Cancelar transaccion
                        cancelarTransaccion(Negocio);
                        break;
                    }
                    case 0: { // Volver al menu principal
                        cout << "Volviendo al menú principal...";
                        break;
                    }
                    default:
                        cout << "Opción Invalida.";
                        break;
                    }
                } while (opTrans != 0);
                break;
            }
            case 5: { // Salir
                cout << "Saliendo del programa...";
                break;
            }
            default:
                cout << "Opción Invalida.";
                break;
            }
    } while (op != 5);

    liberarTienda(Negocio);
    delete Negocio;
    Negocio = nullptr;

    return 0;
}
