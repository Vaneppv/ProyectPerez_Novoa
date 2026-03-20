#include <iostream>
#include <iomanip>
#include <cstring>
#include <ctime>
#include <limits>
#include <fstream>
#include <direct.h>
#include <windows.h>

using namespace std;

const char* ARCHIVO_PRODUCTOS = "productos.bin";
const char* ARCHIVO_PROVEEDORES = "proveedores.bin";
const char* ARCHIVO_CLIENTES = "clientes.bin";
const char* ARCHIVO_TRANSACCIONES = "transacciones.bin";
const char* ARCHIVO_DETALLES = "detallesTransacciones.bin";
const char* ARCHIVO_BACKUP = "backup.bin";

// Definición de colores ANSI
#define RESET   "\033[00m"
#define ROJO    "\033[31m"      // Alertas y errores
#define VERDE   "\033[32m"      // Éxito y confirmaciones
#define AMARILLO "\033[33m"     // Advertencias y stock bajo
#define AZUL    "\033[34m"      // Títulos
#define CYAN    "\033[36m"      // Info secundaria
#define NEGRITA "\033[1m"

// STRUCTS
struct Producto {
    int id;
    char codigo[20];
    char nombre[100];
    char descripcion[200];
    int idProveedor;
    float precio;
    int stock;
    char fechaRegistro[11];
    // Estadisticas del registro
    int stockMinimo;                
    int totalVendido;            // Cantidad de productos vendidos
    // Metadata de Control Obligatoria
    bool eliminado;              // Para BORRADO LOGICO
    char fechaUltimaModificacion[11];
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
	
    int cantidadProductos;     // Cantidad de productos bajo el proveedor
    int productosIDs[100];     // Arreglo de todos los productos comprados a ese proveedor
    float totalCompras;

    int transaccionesIds[100];
    int cantidadTransacciones;

    bool eliminado;            // Para BORRADO LOGICO
    char fechaUltimaModificacion[11];
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
    
    float totalCompras;

    int transaccionesIds[100];
    int cantidadTransacciones;

    bool eliminado; // Para BORRADO LOGICO
    char fechaUltimaModificacion[11];
};

struct DetalleTransaccion {
    int idProducto;
    int cantidad;
    float precioUnitario; 
    int idTransaccion;
};

// Estructura Transacción (puede separarse por transaccion)
struct Transaccion {
    int id;                    
    char tipo[10];             // "COMPRA" o "VENTA"          
    int idRelacionado;         // ID del proveedor (compra) o cliente (venta)
    float total;                         
    char descripcion[200];     // Notas adicionales (opcional)
    char fechaRegistro[11];    // Arreglo de productos en la transaccion
    int cantidadItemsDiferentes;

    bool eliminado;            // Para BORRADO LOGICO
    char fechaUltimaModificacion[11];
};

// Estructura Principal: Tienda
struct Tienda {
    char nombre[100]; // Nombre de la tienda
    char rif[20];     // RIF de la tienda
    
    // Contadores para IDs autoincrementales
    int siguienteIdProducto;
    int siguienteIdProveedor;
    int siguienteIdCliente;
    int siguienteIdTransaccion;
};

struct ArchivoHeader {
    int cantidadRegistros;      // Total historico de registros
    int proximoID;              // Siguiente ID a asignar (Autoincremental)
    int registrosActivos;       // Registros que no estan marcados como eliminados
    int version;                // Control de version del archivo
};

// PROTOTIPO DE FUNCIONES

// Inicialización y Liberación
void inicializarTienda(Tienda* tienda, const char* nombre, const char* rif);
bool inicializarArchivo(const char* nombreArchivo);
bool actualizarHeader(const char* nombreArchivo, ArchivoHeader header);

// FUNCIONES AUXILIARES
int calcularPosicionRegistro(int indice);
void guardarDetalle(const char* archivoDetalles, int idTransaccion, const DetalleTransaccion &detalle);

// VALIDACIONES
bool validarEmail(const char* email);
bool validarFecha(const char* fecha);
bool productoTieneTransacciones(const char* archivoTransacciones, const char* archivoDetalles, int idProducto);
bool verificarProductoEnTransaccion(const char* archivoDetalles, int idTransaccion, int idProducto);
bool idTieneTransacciones(const char* archivoTransacciones, int idRelacionado);
bool codigoDuplicado(const char* nombreArchivo, const char* codigo);
bool rifDuplicado(const char* nombreArchivo, const char* rif);
bool cedulaDuplicada(const char* nombreArchivo, const char* cedula);

// BUSQUEDAS
template<typename Registro>
int* buscarRegistroPorNombre(const char* nombreArchivo, const char* consulta, int* numResultados);
int* buscarProductoPorCodigo(const char* nombreArchivo, const char* consulta, int* numResultados);
int buscarProveedorPorRif(const char* nombreArchivo, const char* rif);
int buscarClientePorCedula(const char* nombreArchivo, const char* cedula);

// UTILIDADES
void obtenerFechaActual(char* buffer);
void convertirAMinusculas(char* cadena);
bool contieneSubstring(const char* texto, const char* busqueda);
void mostrarLineaDetalle(const Transaccion& t, const char* nombreProd, int idProducto, int cantidad, float total);
void mostrarDetalleTransaccion(const char* archivoDetalles, const char* archivoProductos, const Transaccion& t);
void mostrarDetalleGeneralTransaccion(const Transaccion &trans);
void mostrarDetalleProducto(const Producto& producto, const char* archivoProveedores);
void mostrarDetalleProveedor(const Proveedor& proveedor);
void mostrarDetalleCliente(const Cliente& cliente);
void encabezadoTransacciones();
void encabezadoDetalleTransaccion();
void imprimirSeparador(int ancho = 160, char simbolo = '=');
void encabezadoProductos();
void encabezadoProveedorCliente();
bool floatesPositivo(float valor);
bool intesPositivo(int valor);
bool solicitarTexto(const char* prompt, char* destino, int largo);
bool solicitarFloat(const char* prompt, float& valor);
bool solicitarEntero(const char* prompt, int& valor);

// FUNCIONES CRUD
void crearProducto(const char* archivoProductos, const char* archivoProveedores);
void buscarProducto(const char* archivoProductos, const char* archivoProveedores);
void actualizarProducto(const char* archivoProductos, const char* archivoProveedores);
void actualizarStockProducto(const char* archivoProductos);
void listarProductos(const char* archivoProductos, const char* archivoProveedores);
void eliminarProducto(const char* archivoProductos, const char* archivoProveedores, const char* archivoTransacciones, const char* archivoDetalles);

void crearProveedor(const char* archivoProveedores);
void buscarProveedor(const char* archivoProveedores);
void actualizarProveedor(const char* archivoProveedores);
void listarProveedores(const char* archivoProveedores);
void eliminarProveedor(const char* archivoProveedores, const char* archivoTransacciones);

void crearCliente(const char* archivoClientes);
void buscarCliente(const char* archivoClientes);
void actualizarCliente(const char* archivoClientes);
void listarClientes(const char* archivoClientes);
void eliminarCliente(const char* archivoClientes, const char* archivoTransacciones);

void registrarCompra(const char* archivoTransacciones, const char* archivoDetalles, const char* archivoProductos, const char* archivoProveedores);
void registrarVenta(const char* archivoTransacciones, const char* archivoDetalles, const char* archivoProductos, const char* archivoProveedores, const char* archivoClientes);
void buscarTransacciones(const char* archivoTransacciones, const char* archivoDetalles, const char* archivoProductos, const char* archivoClientes, const char* archivoProveedores);
void listarTransacciones(const char* archivoTransacciones);
void cancelarTransaccion(const char* archivoTransacciones, const char* archivoDetalles, const char* archivoProductos, const char* archivoProveedores, const char* archivoClientes);

// Funcion de Inicialización
void inicializarTienda(Tienda* tienda, const char* nombre, const char* rif){
    
    // Informacion de la tienda 
    strncpy(tienda->nombre, nombre, 99);
    tienda->nombre[99] = '\0';
    strncpy(tienda->rif, rif, 19);
    tienda->rif[19] = '\0';


    bool prod = inicializarArchivo(ARCHIVO_PRODUCTOS);
    bool prov = inicializarArchivo(ARCHIVO_PROVEEDORES);
    bool cliente = inicializarArchivo(ARCHIVO_CLIENTES);
    bool trans = inicializarArchivo(ARCHIVO_TRANSACCIONES);
    bool backUp = inicializarArchivo(ARCHIVO_BACKUP);

    if (prod && prov && cliente && trans && backUp) {
        cout << VERDE << NEGRITA << "Sistema de archivos inicializado correctamente." << endl << RESET;
    } else {
        cout << ROJO << "Advertencia: Algunos archivos no pudieron inicializarse." << endl << RESET;
    }
    cout << RESET;
}

//LOGICA DE OPERACIONES CON ARCHIVOS (CRUD)

// Inicializacion y Gestion de Header
bool inicializarArchivo(const char* nombreArchivo) {
    // Intentamos abrir el archivo para lectura para ver si ya existe
    ifstream test(nombreArchivo, ios::binary);
    
    if (test.is_open()) {
        // El archivo ya existe
        test.close();
        return true; 
    }

    // Si no existe, lo crea
    ofstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()) {
        cout << ROJO << "Error: No se pudo crear el archivo " << nombreArchivo << endl << RESET;
        return false;
    }

    // Se inicializa: 0 registros, proximoID = 1, 0 activos, version 1
    ArchivoHeader header = {0, 1, 0, 1};
    
    // Escribimos el header al inicio
    archivo.write(reinterpret_cast<const char*>(&header), sizeof(ArchivoHeader));
    
    bool resultado = archivo.good();
    archivo.close();
    return resultado;
}

// Lee los primeros bytes correspondientes al header y lo retorna.
ArchivoHeader leerHeader(const char* nombreArchivo) {
    ArchivoHeader header = {0, 1, 0, 1}; // Valores por defecto en caso de error
    
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()) {
        return header; // Retorna header vacio si el archivo no existe
    }

    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    archivo.close();
    
    return header;
}

// Sobrescribe Unicamente la seccion del header.
bool actualizarHeader(const char* nombreArchivo, ArchivoHeader header) {
    
    fstream archivo(nombreArchivo, ios::binary | ios::in | ios::out);
    
    if (!archivo.is_open()){
        cout << ROJO << "Error: No se pudo abrir el archivo " << nombreArchivo << endl << RESET;
        return false;
    }

    // Se posiciona en el byte 0 (principio del archivo)
    archivo.seekp(0, ios::beg);
    // Sobrescribe los nuevos datos del header
    archivo.write(reinterpret_cast<const char*>(&header), sizeof(ArchivoHeader));
    
    bool resultado = archivo.good();
    archivo.close();
    return resultado;
}

// Funciones template Utilidades

template <typename Registro> // Template para tomar cualquier registro (productos, cliente, proveedor o transacciones)
int buscarPorId(const char* nombreArchivo, int id){
    
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()){
        cout << ROJO << "Error: No se pudo abrir el archivo " << nombreArchivo << endl << RESET;
        return -1;
    }

    ArchivoHeader header;
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));

    // Recorrido del archivo en busca de la informacion
    for (int i = 0; i < header.cantidadRegistros; i++){
        Registro temp;
        // Lee el registro actual
        archivo.read(reinterpret_cast<char*>(&temp), sizeof(Registro));
        if (!temp.eliminado && temp.id == id){
            archivo.close();
            return i; // Si lo encuentra retorna el indice
        }
    }
    archivo.close(); // Cerrar archivo al no encontrar el producto
    return -1; 
}

template<typename Registro>
Registro obtenerRegistroPorId(const char* nombreArchivo, int id){

    int indice = buscarPorId<Registro>(nombreArchivo, id);
    Registro registro;
    memset(&registro, 0, sizeof(Registro));
    registro.id =-1; // Se marca como no encontrado por defecto.

    if (indice != -1){
        ifstream archivo(nombreArchivo, ios::binary);
        if(archivo.is_open()){
            int posicion = sizeof(ArchivoHeader) + (indice*sizeof(Proveedor));
            archivo.seekg(posicion, ios::beg);
            archivo.read(reinterpret_cast<char*>(&registro), sizeof(Registro));
            archivo.close();
        } 
    }
    return registro;
}

template <typename Registro>
Registro obtenerRegistroPorIndice(const char* nombreArchivo, int indice) {
    Registro registro;
    ifstream archivo(nombreArchivo, ios::binary);
    if (archivo.is_open()) {
        int posicion = sizeof(ArchivoHeader) + (indice * sizeof(Registro));
        archivo.seekg(posicion, ios::beg);
        archivo.read(reinterpret_cast<char*>(&registro), sizeof(Registro));
        archivo.close();
    }
    return registro;
}

// Borrado Logico (Delete)
template <typename Registro>
bool eliminarRegistroLogico(const char* nombreArchivo, int id){
    
    int indice = buscarPorId<Registro>(nombreArchivo, id);
    if (indice == -1){
        return false;
    }
    fstream archivo(nombreArchivo, ios::binary | ios::in | ios::out);
    if (!archivo.is_open()){
        cout << ROJO << "Error: No se pudo abrir el archivo " << nombreArchivo << endl << RESET;
        return false;
    }
    Registro temp;
    archivo.seekg((sizeof(ArchivoHeader) + indice*sizeof(Registro)), ios::beg);
    archivo.read(reinterpret_cast<char*>(&temp), sizeof(Registro));
    
    temp.eliminado = true;
    obtenerFechaActual(temp.fechaUltimaModificacion);
    
    archivo.seekp((sizeof(ArchivoHeader) + indice*sizeof(Registro)), ios::beg);
    archivo.write(reinterpret_cast<char*>(&temp), sizeof(Registro));
    
    ArchivoHeader header;
    archivo.seekg(0, ios::beg);
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));

    archivo.close();
    header.registrosActivos--;    
    
    return actualizarHeader(nombreArchivo, header);
}

template<typename Registro>
int* buscarRegistroPorNombre(const char* nombreArchivo, const char* consulta, int* numResultados){
    ArchivoHeader header = leerHeader(nombreArchivo);
    // Inicializacion del contador de resultados encontrados
    *numResultados = 0;

    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()){
        cout << ROJO << "Error abriendo archivo de busqueda." << endl << RESET;
        return nullptr;
    }

    Registro registro;
    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    // Recorrido para saber tamaño del arreglo de productos encontrados
    for (int i = 0; i < header.cantidadRegistros; i++){ // Verificar si hay coincidencias
        archivo.read(reinterpret_cast<char*>(&registro), sizeof(Registro));

        // Logica de comparacion
        if(!registro.eliminado){
            if(contieneSubstring(registro.nombre, consulta)){
                (*numResultados)++;
            }
        }
    }
    if (*numResultados == 0){ // Si no hay coincidencias retorna nullptr
        archivo.close();
        return nullptr;
    }
    // Reservar memoria para arreglo de productos encontrados
    int* indices = new int[*numResultados];
    int posicion = 0;

    archivo.clear();
    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    // Llenar arreglo con indices reales de los productos encontrados
    for (int i = 0; i < header.cantidadRegistros; i++){ // Verificar si hay coincidencias
        archivo.read(reinterpret_cast<char*>(&registro), sizeof(Registro));

        if(!registro.eliminado){
            if(contieneSubstring(registro.nombre, consulta)){
                indices[posicion]= i;
                posicion++;
            }
        }
    }
    archivo.close();
    return indices; // Retorna el puntero al arreglo de indices
}

template <typename Registro>
bool actualizarRegistro(const char* nombreArchivo, int indice, const Registro& datosNuevos) {

    fstream archivo(nombreArchivo, ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) return false;

    archivo.seekp(sizeof(ArchivoHeader) + (indice * sizeof(Registro)), ios::beg);
    archivo.write(reinterpret_cast<const char*>(&datosNuevos), sizeof(Registro));

    archivo.close();
    return true;
}

// Acceso Aleatorio y Calculo de Offsets
template<typename Registro>
int calcularPosicionRegistro(int indice) {
    // Se salta el Header, y luego multiplica el índice por el tamaño del struct productos para obtener la posicion del producto
    int posicionRegistro = sizeof(ArchivoHeader) + (indice * sizeof(Registro));
    return posicionRegistro;
}

// Creacion de un Registro (Append)
template<typename Registro>
bool guardarNuevoRegistro(const char* nombreArchivo, Registro& nuevoRegistro){
    
    ArchivoHeader header = leerHeader(nombreArchivo); // Leer el header para obtener metadatos
    nuevoRegistro.id = header.proximoID;
    nuevoRegistro.eliminado = false;
    obtenerFechaActual(nuevoRegistro.fechaRegistro);
    obtenerFechaActual(nuevoRegistro.fechaUltimaModificacion);
    
    // Abrir el archivo para escritura binaria
    fstream archivo(nombreArchivo, ios::in | ios::out | ios::binary);
    if (!archivo.is_open()){
        cout << ROJO << "Error: No se pudo abrir el archivo " << nombreArchivo << endl << RESET;
        return false;
    }
    // Calculo de la posicion de escritura
    int posicionEscritura = sizeof(ArchivoHeader) + (header.cantidadRegistros * sizeof(Registro));
    archivo.seekp(posicionEscritura, ios::beg);

    archivo.write(reinterpret_cast<const char*>(&nuevoRegistro), sizeof(Registro));
    
    if (archivo.fail()) {
    cout << ROJO << "Error crítico: No se pudo escribir en el archivo." << endl << RESET;
    archivo.close();
    return false;
    }

    // Actualizar contadores del Header
    header.cantidadRegistros++; // Nueva cantidad de registros
    header.proximoID++; // Actualiza al nuevo ID disponible
    header.registrosActivos++; // Nueva cantidad de registros no eliminados
    
    archivo.close(); 

    return actualizarHeader(nombreArchivo, header);
} 

// Funcion para validad si existe una entidad por medio del id (proveedor, cliente o producto)
template<typename Registro>
bool existeEntidad(const char* nombreArchivo, int id){
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()) return false;

    ArchivoHeader header;
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    
    Registro temp;
    // Recorrido del arreglo en busca del id
    for (int i = 0; i < header.cantidadRegistros; i++){
        archivo.read(reinterpret_cast<char*>(&temp), sizeof(Registro));
        if (!temp.eliminado && temp.id == id){
            archivo.close();
            return true; // Si lo encuenta retorna True (Si existe el proveedor)
        }
    }
    archivo.close();
    return false; // No existe
}

void mostrarDetalleTransaccion(const char* archivoDetalles, const char* archivoProductos, const Transaccion& t) {
    ifstream archivo(archivoDetalles, ios::binary);
    if (!archivo.is_open()) {
        cout << ROJO << "Error: no se pudo abrir el archivo de detalles." << endl << RESET;
        return;
    }

    DetalleTransaccion d;
    bool tieneDetalles = false;

    while (archivo.read(reinterpret_cast<char*>(&d), sizeof(DetalleTransaccion))) {
        if (d.idTransaccion == t.id) {
            if (!tieneDetalles) {
                encabezadoDetalleTransaccion(); // Solo imprime el encabezado si hay detalles
                tieneDetalles = true;
            }

            // Busca el nombre del producto
            int indP = buscarPorId<Producto>(archivoProductos, d.idProducto);
            char nombreProd[100] = "No encontrado";
            
            if (indP != -1) {
                Producto p = obtenerRegistroPorIndice<Producto>(archivoProductos, indP);
                strcpy(nombreProd, p.nombre);
            }

            // Calcula el total de la línea (cantidad * precio de ese momento)
            float totalLinea = d.cantidad * d.precioUnitario;

            mostrarLineaDetalle(t, nombreProd, d.idProducto, d.cantidad, totalLinea);
        }
    }
    if (!tieneDetalles) {
        cout << AMARILLO << "No se encontraron detalles para la transacción " << t.id << endl << RESET;
    } else {
        imprimirSeparador();
        cout << CYAN << right << setw(72) << "TOTAL GENERAL: $" << fixed << setprecision(2) << t.total << endl << RESET;
    }

    archivo.close();
}

// FUNCIONES CRUD - PRODUCTOS
// Crear Productos
void crearProducto(const char* archivoProductos, const char* archivoProveedores) {
    
    char decision;
    cout << "¿Desea registrar un nuevo producto? (S/N): ";
    cin >> decision;

    if (decision == 's' || decision == 'S') {
        ArchivoHeader header = leerHeader(archivoProveedores);
        if (header.registrosActivos == 0){
            cout << ROJO << "Error: para registrar un producto, debe registrar primero el proveedor." << endl << RESET;
            return;
        }

        Producto nuevoProducto;
        
        // Validaciones de entrada
        if (!solicitarTexto("Ingrese nombre del Producto", nuevoProducto.nombre, 100)) return;
        if (!solicitarTexto("Ingrese codigo del producto", nuevoProducto.codigo, 20)) return;
        
        if (codigoDuplicado(archivoProductos, nuevoProducto.codigo)) {
            cout << ROJO << "Error: El codigo ya existe." << endl << RESET;
            return;
        }

        // Solicitud de datos para el nuevo producto (Validacion de datos)
        if (!solicitarTexto("Ingrese descripcion del producto", nuevoProducto.descripcion, 200)) return;
        if (!solicitarFloat("Ingrese precio del producto", nuevoProducto.precio) || !floatesPositivo(nuevoProducto.precio)) return;
        if (!solicitarEntero("Ingrese Stock del producto", nuevoProducto.stock) || nuevoProducto.stock < 0) return;
        if (!solicitarEntero("Ingrese Stock Minimo de seguridad", nuevoProducto.stockMinimo) || nuevoProducto.stockMinimo < 0) return;
        if (!solicitarEntero("Ingrese ID del proveedor", nuevoProducto.idProveedor) || !intesPositivo(nuevoProducto.idProveedor)) return;
        if (!existeEntidad<Proveedor>(archivoProveedores, nuevoProducto.idProveedor)) {
            cout << endl << ROJO << "Error: El proveedor no existe. Debe crearlo primero" << endl << RESET;
            return;
        }

        // Resumen de nuevos datos
        imprimirSeparador(60, '=');
        cout << AZUL << NEGRITA << "      RESUMEN DE PRODUCTO" << endl << RESET;
        imprimirSeparador(60, '=');
        cout << CYAN << "Codigo: " << nuevoProducto.codigo << endl;
        cout << "Nombre: " << nuevoProducto.nombre << endl;
        cout << "Descripcion: " << nuevoProducto.descripcion << endl;
        cout << "Stock: " << nuevoProducto.stock << endl;
        cout << "Precio: " << nuevoProducto.precio << endl << RESET;
        imprimirSeparador(60, '=');
        
        cout << "¿Desea Guardar producto? (S/N): ";
        cin >> decision;

        if (decision == 's' || decision == 'S') {
            if (guardarNuevoRegistro<Producto>(archivoProductos, nuevoProducto)){
                cout << endl << VERDE << NEGRITA << "¡Producto guardado! ID asignado: " << nuevoProducto.id << endl << RESET;            
            }
            encabezadoProductos();
            mostrarDetalleProducto(nuevoProducto, archivoProveedores);
        } else {
            cout << ROJO << endl << "Operación cancelada. El Producto no fue guardado" << endl << RESET;
        }
    } else {
        cout << AMARILLO << endl << "Volviendo al menú." << endl << RESET;
    }
}

// Buscar Producto
void buscarProducto(const char* archivoProductos, const char* archivoProveedores) {
    
    ArchivoHeader header = leerHeader(archivoProductos);
    if (header.registrosActivos == 0){
        cout << ROJO << "No hay productos en el Sistema." << endl << RESET;
        return;
    }
    
    int seleccion;
    do {
        
        cout << AZUL << NEGRITA << endl << "===== MENU DE BUSQUEDA DE PRODUCTOS =====" << endl << RESET;
        cout << CYAN << "1. Buscar por ID" << endl;
        cout << "2. Buscar por nombre (coincidencia parcial)" << endl;
        cout << "3. Buscar por codigo (coincidencia parcial)" << endl;
        cout << "4. Listar por proveedor" << endl;
        cout << "0. Cancelar" << endl << RESET;

        if (!solicitarEntero("Seleccione una opción", seleccion)) {
            break; 
        }

        switch (seleccion) {
            case 1: { // Buscar por ID
                int i, id;
                if (solicitarEntero("Ingrese el ID del producto", id)) {
                    i = buscarPorId<Producto>(archivoProductos, id);                    
                    if (i != -1) {            
                        Producto producto = obtenerRegistroPorIndice<Producto>(archivoProductos, i);
                        cout << VERDE << NEGRITA << endl << "Producto encontrado:" << endl << RESET;
                        encabezadoProductos();
                        mostrarDetalleProducto(producto, archivoProveedores);
                        imprimirSeparador();
                    } else {
                        cout << ROJO << endl << "Error: El producto con ID " << id << " no existe." << endl << RESET;
                    }
                }
                break;
            }

            case 2: { // Buscar por nombre (coincidencia parcial)
                char nombreBusqueda[100];
                int numEncontrados = 0;

                if (solicitarTexto("Ingrese el nombre (o parte) del producto", nombreBusqueda, 100)) {
                    
                    int* indices = buscarRegistroPorNombre<Producto>(archivoProductos, nombreBusqueda, &numEncontrados);
                    
                    if (indices != nullptr) {
                        cout << VERDE << NEGRITA << "Se encontraron " << numEncontrados << " coincidencias:" << endl << RESET;
                        encabezadoProductos();
                        
                        for (int i = 0; i < numEncontrados; i++) {
                            Producto producto =obtenerRegistroPorIndice<Producto>(archivoProductos, indices[i]);
                            mostrarDetalleProducto(producto, archivoProveedores);
                        }
                        imprimirSeparador();
                        delete[] indices; // liberamos memoria
                        indices = nullptr;

                    } else {
                        cout << AMARILLO << endl << "No se encontraron productos que coincidan con: '" << nombreBusqueda << "'" << endl << RESET;
                    }
                }
                break;
            }

            case 3: { // Busqueda por codigo (coincidencia parcial)
                char codigoBusqueda[20];
                int numEncontrados = 0;

                if (solicitarTexto("Ingrese el codigo (o parte) a buscar", codigoBusqueda, 20)) {
                    
                    int* indices = buscarProductoPorCodigo(archivoProductos, codigoBusqueda, &numEncontrados);
                    if (indices != nullptr) {
                        cout << VERDE << NEGRITA << endl << "Se encontraron " << numEncontrados << " coincidencias por codigo:" << endl << RESET;
                        
                        encabezadoProductos();
                        for (int i = 0; i < numEncontrados; i++) {
                            Producto producto = obtenerRegistroPorIndice<Producto>(archivoProductos, indices[i]);
                            mostrarDetalleProducto(producto, archivoProveedores);
                        }
                        imprimirSeparador();

                        delete[] indices; 
                        indices = nullptr;
                    } else {
                        cout << AMARILLO << endl << "No existen productos con el codigo: '" << codigoBusqueda << "'" << endl << RESET;
                    }
                }
                break;
            }

            case 4: { // Listar por proveedor
                int idProv;
                if (solicitarEntero("Ingrese ID del proveedor", idProv)) {
                    int indProv = buscarPorId<Proveedor>(archivoProveedores, idProv);
                    if (indProv == -1){
                        cout << ROJO << "Error: el proveedor no existe" << endl << RESET;
                    }
                    
                    Proveedor prov = obtenerRegistroPorIndice<Proveedor>(archivoProveedores, indProv);
                    if (prov.cantidadProductos == 0){
                        cout << AMARILLO << "El proveedor no tiene productos asociados en su historial de compras." << endl << RESET;
                    }
                    
                    cout << VERDE << NEGRITA << "\n PRODUCTOS SUMINISTRADOS POR: " << prov.nombre << endl << RESET;
                    encabezadoProductos();
                    bool encontro = false;
                    
                    for (int i = 0; i < prov.cantidadProductos; i++){
                        int idProd = prov.productosIDs[i];
                        int indProd = buscarPorId<Producto>(archivoProductos, idProd);
                        if (indProd != -1){
                            Producto producto = obtenerRegistroPorIndice<Producto>(archivoProductos, indProd);
                            if (!producto.eliminado){
                                mostrarDetalleProducto(producto, archivoProveedores);
                            }
                        }
                    }
                    imprimirSeparador();
                    if (!encontro) {
                        cout << AMARILLO << "No se encontraron productos activos para este proveedor." << endl << RESET;
                    }
                }
                break;
            }

            case 0:
                cout << AMARILLO << "Cancelando busqueda" << endl << RESET;
                break;

            default:
                cout << ROJO << "Opción no valida." << endl << RESET;
                break;
        }
    } while (seleccion != 0);
}

//Actualizar Producto
void actualizarProducto(const char* archivoProductos, const char* archivoProveedores) {

    ArchivoHeader header = leerHeader(archivoProductos);
    if (header.registrosActivos == 0){
        cout << ROJO << "No hay productos en el Sistema." << endl << RESET;
        return;
    }
    
    int id, i;
    if (!solicitarEntero("Ingrese el ID del producto a buscar", id)) return;

    i = buscarPorId<Producto>(archivoProductos, id);
    if (i == -1) {
        cout << ROJO << endl << "Error: El producto con ID " << id << " no existe." << endl << RESET;
        return;
    }
    Producto producto = obtenerRegistroPorIndice<Producto>(archivoProductos, i);

    cout << AZUL << NEGRITA << "DATOS ACTUALES DEL PRODUCTO:" << endl << RESET;
    encabezadoProductos();
    mostrarDetalleProducto(producto, archivoProveedores);
    imprimirSeparador();
    cout << RESET;

    char nombre[100], codigo[20], descripcion[200];
    float precio;
    int stock, idProveedor, stockMinimo;
    int seleccion; 

    bool verStockMinimo = false, verNombre = false, verCodigo = false, verDescrip = false, verPrecio = false, verStock = false, verIdProveedor = false;
    
    // Menú de campos editables
    do {
        cout << AZUL << NEGRITA << endl << "¿Qué desea editar?" << endl << RESET;
        cout << CYAN << "1. Código" << endl;
        cout << "2. Nombre" << endl;
        cout << "3. Descripción" << endl;
        cout << "4. Proveedor" << endl;
        cout << "5. Precio" << endl;
        cout << "6. Stock" << endl;
        cout << "7. Stock Minimo (Alerta)" << endl; 
        cout << "8. Guardar cambios" << endl;
        cout << "0. Cancelar sin guardar" << endl << RESET;
        
        if (!solicitarEntero("Seleccione una opción", seleccion)) continue;

        switch (seleccion) {
            case 1:
                if (solicitarTexto("Ingrese nuevo código", codigo, 20)) {
                    if (codigoDuplicado(archivoProductos, codigo) && strcmp(codigo, producto.codigo) != 0) {
                        cout << ROJO << "Error: El código ya existe." << endl << RESET;
                    } else {
                        strncpy(producto.codigo, codigo, 19); 
                        producto.codigo[19] = '\0';
                        verCodigo = true;
                    }
                }
                break;
            case 2:
                if (solicitarTexto("Ingrese nuevo nombre", nombre, 100)){
                    strncpy(producto.nombre, nombre, 99);
                    producto.nombre[99]= '\0';
                    verNombre = true;
                }
                break;
            case 3:
                if (solicitarTexto("Ingrese nueva descripción", descripcion, 200)) {
                    strncpy(producto.descripcion, descripcion, 199);
                    producto.descripcion[199] = '\0';
                    verDescrip = true;
                }
                break;
            case 4:
                if (solicitarEntero("Ingrese ID del nuevo proveedor", idProveedor)) {
                    if (existeEntidad<Proveedor>(archivoProveedores, idProveedor)) {
                        producto.idProveedor = idProveedor;
                        verIdProveedor = true;
                    }
                    else {
                        cout << ROJO << endl << "Error: El proveedor no existe." << endl << RESET;
                    }
                }
                break;
            case 5:
                if (solicitarFloat("Ingrese nuevo precio", precio) && floatesPositivo(precio)) {
                    producto.precio = precio;
                    verPrecio = true;
                }
                break;
            case 6:
                if (solicitarEntero("Ingrese nuevo stock", stock) && intesPositivo(stock)) {
                    producto.stock = stock;
                    verStock = true;
                }
                break;
            case 7: {
                if (solicitarEntero("Ingrese nuevo stock minimo", stockMinimo) && intesPositivo(stockMinimo)) {
                producto.stockMinimo = stockMinimo;
                verStockMinimo = true;
                }
            break;
            }
            case 8: {
                if (!verNombre && !verCodigo && !verDescrip && !verPrecio && !verStock && !verIdProveedor) {
                    cout << AMARILLO  << "No se han realizado cambios para guardar." << endl << RESET;
                } else {
                    char decision;
                    cout << "¿Desea Guardar los cambios hechos al producto? (S/N): ";
                    cin >> decision;
                    if (decision == 's' || decision == 'S') {
                        if (actualizarRegistro<Producto>(archivoProductos, i, producto)){
                        cout << VERDE << NEGRITA << "¡Cambios aplicados exitosamente!" << endl << RESET;
                        } else {
                        cout << ROJO << "Error al intentar escribir en el archivo." << endl << RESET;
                        }
                    } 
                }
                seleccion = 0; 
                break;
            }

            case 0:
                cout << AMARILLO << "Operación cancelada. No se guardaron cambios." << endl << RESET;
                break;
            default:
                cout << ROJO << "Opción no válida." << endl << RESET;
                break;
        }
    } while (seleccion != 0);
}

// Actualizar Stock Manualmente
void actualizarStockProducto(const char* archivoProductos) {
    ArchivoHeader header = leerHeader(archivoProductos);
    if (header.registrosActivos == 0){
        cout << ROJO << "No hay productos en el Sistema." << endl << RESET;
        return;
    }

    int id, i;
    if (!solicitarEntero("Ingrese el ID del producto", id)){
        return;
    }

    i = buscarPorId<Producto>(archivoProductos, id);
    if (i == -1) {
        cout << endl << ROJO << "Error: El producto no existe." << endl << RESET;
        return;
    }
    Producto producto = obtenerRegistroPorIndice<Producto>(archivoProductos, i);
    
    int seleccion;
    char decision;
    int stockTemporal = producto.stock;

    do {
        cout << endl << "----------------------------" << endl;
        cout << AZUL << "Producto: " << producto.nombre << endl;
        cout << "Stock actual del producto: " << producto.stock << " | Nuevo stock: " << stockTemporal << endl << RESET;
        cout << "----------------------------" << endl;
        cout << CYAN << "1. Agregar unidades" << endl;
        cout << "2. Quitar unidades" << endl;
        cout << "3. Guardar cambios" << endl;
        cout << "0. Cancelar sin guardar" << endl << RESET;
        if (!solicitarEntero("Seleccione una opción", seleccion)){
            return;
        }
        
        switch (seleccion) {
            case 1: {
                int cantidad;
                if (solicitarEntero("¿Cuantas unidades ingresan?", cantidad)) {
                    if (intesPositivo(cantidad)) {
                        stockTemporal += cantidad; 
                        cout << CYAN << "Cantidad añadida al calculo." << endl << RESET;
                    } else {
                        cout << ROJO << "Error: La cantidad debe ser positiva." << endl << RESET;
                    }
                }
                break;
            }
            case 2: { // Quitar unidades
                int cantidad;
                if (solicitarEntero("¿Cuantas unidades salen?", cantidad)) {
                    if (intesPositivo(cantidad)) {
                        if (stockTemporal - cantidad >= 0) {
                            stockTemporal -= cantidad;
                            cout << CYAN << "Cantidad restada al calculo." << endl << RESET;
                        } else {
                            cout << ROJO << "Error: El stock resultante no puede ser negativo." << endl << RESET;
                        }
                    } else {
                        cout << ROJO << "Error: La cantidad debe ser positiva." << endl << RESET;
                    }
                }
                break;
            }
            case 3: { // Guardar cambios
                char decision;
                cout << "¿Desea confirmar y guardar el nuevo stock (" << stockTemporal << ")? (S/N): ";
                cin >> decision;
                if (decision == 's' || decision == 'S') {
                    producto.stock = stockTemporal;
                    actualizarRegistro<Producto>(archivoProductos, i, producto);
                    cout << VERDE << NEGRITA << "¡Cambios guardados con éxito!" << endl << RESET;
                    seleccion = 0; // Para salir del bucle tras guardar
                } else {
                    cout << ROJO << "Guardado cancelado. Puede seguir editando o salir con 0." << endl << RESET;
                }
                break;
            }

            case 0: { // Cancelar
                cout << ROJO << "Operación cancelada. No se realizaron cambios en el inventario." << endl << RESET;
                break;
            }

            default: {
                cout << AMARILLO << "Opción no valida. Intente de nuevo." << endl << RESET;
                break;
            }
        }
    } while (seleccion != 0);
}

// Listar Productos
void listarProductos(const char* archivoProductos, const char* archivoProveedores) {
    
    ArchivoHeader header = leerHeader(archivoProductos);
    if (header.registrosActivos ==0){
        cout << endl << ROJO << "No hay productos registrados en el sistema" << endl << RESET;
        return;
    }
    
    ifstream archivo(archivoProductos, ios::binary);
    if (!archivo.is_open()){
        cout << ROJO << "Error: No se puso abrir el archivo " << archivoProductos << endl << RESET;
        return;    
    }
    archivo.seekg(sizeof(ArchivoHeader), ios::beg);

    cout << AZUL << NEGRITA << endl << "          LISTADO GENERAL DE PRODUCTOS" << endl << RESET;
    encabezadoProductos();

    Producto producto;
    bool seMostroAlgo = false; 

    for (int i = 0; i < header.cantidadRegistros; i++) {
        if (archivo.read(reinterpret_cast<char*>(&producto), sizeof(Producto))) {
            if (!producto.eliminado) {    
                mostrarDetalleProducto(producto, archivoProveedores);
                seMostroAlgo = true; 
            }
        }
    }
    
    archivo.close();

    
    if (!seMostroAlgo) {
        cout << AMARILLO << " No existen productos activos para mostrar." << RESET << endl;
    }
    imprimirSeparador();
}

// Eliminar Producto
void eliminarProducto(const char* archivoProductos, const char* archivoProveedores, const char* archivoTransacciones, const char* archivoDetalles){

    ArchivoHeader header = leerHeader(archivoProductos);
    if (header.registrosActivos ==0){
        cout << ROJO << endl << "No hay productos registrados en el sistema" << endl << RESET;
        return;
    }

    int id, ind;

    if (!solicitarEntero("Ingrese el ID del producto que desea eliminar", id)){
        return;
    }

    // Buscamos el índice
    ind = buscarPorId<Producto>(archivoProductos, id);
    // Validamos que exista
    if (ind == -1) {
        cout << ROJO << endl << "Error: El producto con ID " << id << " no existe." << endl << RESET;
        return;
    }
    Producto producto = obtenerRegistroPorId<Producto>(archivoProductos, id);

    char decision;
    encabezadoProductos();
    mostrarDetalleProducto(producto, archivoProveedores);
    
    if(productoTieneTransacciones(archivoTransacciones, archivoDetalles, id)){
        cout << ROJO << "Este producto tiene transacciones registradas." << endl;
        cout << AMARILLO << "Eliminarlo podria afectar la integridad de los reportes históricos." << endl << RESET;
    }

    cout << "¿Seguro que desea eliminar el producto? (S/N): ";
    cin >> decision;

    if (decision == 's' || decision == 'S') { // Reajuste de posicion de productos
        eliminarRegistroLogico<Producto>(archivoProductos, id);    
        cout << VERDE << NEGRITA << "Producto eliminado exitosamente." << endl << RESET;
    } else {
        cout << ROJO << "Operación cancelada. No se realizaron cambios." << endl << RESET;
    }
}

// FUNCIONES CRUD - PROVEEDORES
void crearProveedor(const char* archivoProveedores){

    char decision;
    cout << "¿Desea registrar un nuevo proveedor? (S/N): ";
    cin >> decision;

    if (decision == 's' || decision == 'S') {
        ArchivoHeader header = leerHeader(archivoProveedores);
        
        Proveedor nuevo_proveedor;

        // Validaciones de entrada
        if (!solicitarTexto("Ingrese nombre del Proveedor", nuevo_proveedor.nombre, 100)) return;
        if (!solicitarTexto("Ingrese rif del Proveedor", nuevo_proveedor.rif, 20)) return;
        
        if (rifDuplicado(archivoProveedores, nuevo_proveedor.rif)) {
            cout << ROJO << "Error: El rif ya existe." << endl << RESET;
            return;
        } // cambiar cuando se actualize rifduplicado para ficheros bin
        
        if (!solicitarTexto("Ingrese el telefono del Proveedor", nuevo_proveedor.telefono, 20)) return;
        if (!solicitarTexto("Ingrese la direccion del Proveedor", nuevo_proveedor.direccion, 200)) return;
        if (!solicitarTexto("Ingrese el email del Proveedor", nuevo_proveedor.email, 100)) return;
        
        if (!validarEmail(nuevo_proveedor.email)) {
            cout << ROJO << "Error: El correo debe incluir un '@' y al menos un '.'" << endl << RESET;
            return;
        }

        // Resumen de nuevos datos
        imprimirSeparador(60, '=');
        cout << AZUL << NEGRITA << "      RESUMEN DE PROVEEDOR" << endl << RESET;
        imprimirSeparador(60, '=');
        cout << CYAN << "Nombre: " << nuevo_proveedor.nombre << endl;
        cout << "Telefono: " << nuevo_proveedor.telefono << endl;
        cout << "Email: " << nuevo_proveedor.email << endl;
        cout << "Rif: " << nuevo_proveedor.rif << endl;
        cout << "Dirección: " << nuevo_proveedor.direccion << endl << RESET;
        imprimirSeparador(60, '=');

        cout << "¿Desea Guardar proveedor? (S/N): ";
        cin >> decision;

        if (decision == 's' || decision == 'S') {
            if (guardarNuevoRegistro<Proveedor>(archivoProveedores, nuevo_proveedor)){
                cout << VERDE << NEGRITA << endl << "¡Proveedor guardado! ID asignado: " << nuevo_proveedor.id << endl << RESET;            
            }
            encabezadoProveedorCliente();
            mostrarDetalleProveedor(nuevo_proveedor);
        } else {
            cout << ROJO << endl << "Operación cancelada. El Proveedor no fue guardado" << endl << RESET;
        }
    } else {
        cout << AMARILLO << endl << "Volviendo al menú." << endl << RESET;
    }
}

// Buscar Proveedor
void buscarProveedor(const char* archivoProveedores){

	ArchivoHeader header = leerHeader(archivoProveedores);
        if (header.registrosActivos == 0){
            cout << ROJO << "No hay Proveedores registrados en el sistema." << endl << RESET;
            return;
    }

    int seleccion;
    do {
        cout << AZUL << NEGRITA << endl << "===== MENU DE BUSQUEDA DE PROVEEDORES =====" << endl;
        cout << CYAN << "1. Buscar por ID" << endl;
        cout << "2. Buscar por nombre (coincidencia parcial)" << endl;
        cout << "3. Buscar por rif" << endl;
        cout << "0. Cancelar" << endl << RESET;

        if (!solicitarEntero("Seleccione una opción", seleccion)) {
            break; 
        }

        switch (seleccion) {
            case 1: { // Buscar por ID
                int id, i;
                Proveedor registro_proveedor;
                if (solicitarEntero("Ingrese el ID del proveedor", id)) {                    
                    i = buscarPorId<Proveedor>(archivoProveedores, id);
                    if (i != -1) {
                        registro_proveedor = obtenerRegistroPorIndice<Proveedor>(archivoProveedores, i); 
                        cout << VERDE << NEGRITA << endl << "Proveedor encontrado:" << endl << RESET;
                        encabezadoProveedorCliente();
            			mostrarDetalleProveedor(registro_proveedor);
                        imprimirSeparador();
                    } else {
                        cout << endl << ROJO << "Error: El proveedor con ID " << id << " no existe." << endl << RESET;
                    }
                }
                break;
            }

            case 2: { // Buscar por nombre (coincidencia parcial)
                char nombreBusqueda[100];
                int numEncontrados = 0;

                if (solicitarTexto("Ingrese el nombre (o parte) del proveedor", nombreBusqueda, 100)) {
                    
                    int* indices = buscarRegistroPorNombre<Proveedor>(archivoProveedores, nombreBusqueda, &numEncontrados);
                    
                    if (indices != nullptr) {
                        cout << VERDE << NEGRITA << endl << "Se encontraron " << numEncontrados << " coincidencias:" << endl << RESET;
                        
                        encabezadoProveedorCliente();
                        for (int i = 0; i < numEncontrados; i++) {
                            Proveedor proveedor =obtenerRegistroPorIndice<Proveedor>(archivoProveedores, indices[i]);
                            mostrarDetalleProveedor(proveedor);
                        }
                        imprimirSeparador();
                        delete[] indices; // liberamos memoria
                        indices = nullptr;

                    } else {
                        cout << AMARILLO << endl << "No se encontraron proveedores que coincidan con: '" << nombreBusqueda << "'" << endl << RESET;
                    }
                }
                break;
            }

            case 3: { // Buscar por rif
                char rifBusqueda[20];
                int i;
                if (solicitarTexto("Ingrese el rif completo a buscar", rifBusqueda, 20)) {
                   i = buscarProveedorPorRif(archivoProveedores, rifBusqueda); 
                    if (i != -1) {
                        Proveedor proveedor = obtenerRegistroPorIndice<Proveedor>(archivoProveedores, i);
                        cout << endl << VERDE << NEGRITA << "Proveedor encontrado:" << endl << RESET;
                        encabezadoProveedorCliente();
                        mostrarDetalleProveedor(proveedor);
                        imprimirSeparador();
                    } else {
                        cout << ROJO << endl << "Error: El proveedor con rif " << rifBusqueda << " no existe." << endl << RESET;
                    }
                }
                break;            
            }

            case 0:
                cout << AMARILLO << "Cancelando busqueda" << endl<< RESET;
                break;

            default:
                cout << ROJO << "Opción no valida." << endl << RESET;
                break;
        }
    } while (seleccion != 0);

}

// Actualizar Proveedor
void actualizarProveedor(const char* archivoProveedores){
	
    ArchivoHeader header = leerHeader(archivoProveedores);
    if (header.registrosActivos == 0){
        cout << ROJO << "No hay proveedores en el Sistema." << endl  << RESET;
        return;
    }
    
    int id, i;
    if (!solicitarEntero("Ingrese el ID del proveedor", id)){
        return; 
    }
        
    i=buscarPorId<Proveedor>(archivoProveedores, id);                    
    if (i == -1) {
        cout << ROJO << endl << "Error: El proveedor con ID " << id << " no existe." << endl << RESET;
        return;
    }
    Proveedor registro_proveedor = obtenerRegistroPorIndice<Proveedor>(archivoProveedores, i);
    cout << endl << VERDE << NEGRITA << "Proveedor encontrado:" << endl << RESET;
    encabezadoProveedorCliente();
    mostrarDetalleProveedor(registro_proveedor);
    imprimirSeparador();

    int seleccion;
    bool cambiosRealizados = false;

    // Menú de campos editables
    do {
        cout << endl << AZUL << "¿Qué desea editar?" << endl;
        cout << CYAN << "1. Nombre" << endl;
        cout << "2. Rif" << endl;
        cout << "3. Telefono" << endl;
        cout << "4. Email" << endl;
        cout << "5. Direccion" << endl;
        cout << "6. Guardar cambios" << endl;
        cout << "0. Cancelar sin guardar" << endl  << RESET;
        
        if (!solicitarEntero("Seleccione una opción", seleccion)) continue;

        switch (seleccion) {
            case 1:
                if (solicitarTexto("Ingrese nuevo nombre", registro_proveedor.nombre, 100)) {
                    cambiosRealizados = true;
                }
                break;
            case 2:{
                char rifTemp[20];
                if (solicitarTexto("Ingrese nuevo RIF", rifTemp, 20)){
                    
					if (strcmp(rifTemp, registro_proveedor.rif) != 0 && rifDuplicado(archivoProveedores, rifTemp)) {
                        cout << ROJO << "Error: El rif ya pertenece a otro proveedor." << endl  << RESET; 
                    }
                    else {
                        strcpy(registro_proveedor.rif, rifTemp);
                        cambiosRealizados = true;
                    }
                }
                break;
            }
            case 3:
                if (solicitarTexto("Ingrese nuevo telefono", registro_proveedor.telefono, 20)) {
                    cambiosRealizados = true;
                }
                break;
            case 4:{
                char emailTemp[100];
                if (solicitarTexto("Ingrese email del proveedor", emailTemp, 100)) {
                    if (!validarEmail(emailTemp)) {
            		cout << ROJO << "Error: El correo debe incluir un '@' y al menos un '.'" << endl  << RESET;
        			}
                    else {
                        strcpy(registro_proveedor.email, emailTemp);
                        cambiosRealizados = true;
                    }
                }
                break;
            }
            case 5:
                if (solicitarTexto("Ingrese direccion del proveedor", registro_proveedor.direccion,200)) {
                    cambiosRealizados = true;
                }
                break;
    
            case 6: {
                if (!cambiosRealizados) {
                    cout << AMARILLO << "No se han realizado cambios para guardar." << endl << RESET;
                } else {
                    char decision;
                    cout << "¿Desea Guardar los cambios hechos al proveedor? (S/N): ";
                    cin >> decision;
                    if (decision == 's' || decision == 'S') {
                        if (actualizarRegistro<Proveedor>(archivoProveedores, i, registro_proveedor)){
                            cout << VERDE << NEGRITA << "¡Cambios aplicados exitosamente!" << endl << RESET;
                        }
                    }
                }
                seleccion = 0; 
                break;
            }
            case 0:
                cout << AMARILLO << "Operación cancelada. No se guardaron cambios." << endl << RESET;
                break;
            default:
                cout << ROJO << "Opción no válida." << endl << RESET;
                break;
        }
    } while (seleccion != 0);
}

// Listar Proveedores
void listarProveedores(const char* archivoProveedores){

    ArchivoHeader header = leerHeader(archivoProveedores);
    if (header.registrosActivos ==0){
        cout << ROJO << endl << "No hay proveedores registrados en el sistema" << endl << RESET;
        return;
    }
    
    ifstream archivo(archivoProveedores, ios::binary);
    if (!archivo.is_open()){
        cout << ROJO << "Error: No se puso abrir el archivo " << archivoProveedores << endl << RESET;
        return;    
    }
    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    
    imprimirSeparador();
    cout << AZUL << NEGRITA << endl << "          LISTADO GENERAL DE PROVEEDORES" << endl << RESET;
    encabezadoProveedorCliente(); 

    Proveedor proveedor;

    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.read(reinterpret_cast<char*>(&proveedor), sizeof(Proveedor));
        if (!proveedor.eliminado){    
            mostrarDetalleProveedor(proveedor); 
        }
    }
    archivo.close();
    imprimirSeparador();
}

// Eliminar Proveedor
void eliminarProveedor(const char* archivoProveedores, const char* archivoTransacciones){

    ArchivoHeader header = leerHeader(archivoProveedores);
    if (header.registrosActivos ==0){
        cout << endl << ROJO << "No hay proveedores registrados en el sistema" << endl << RESET;
        return;
    }

    int id, ind;
    if (!solicitarEntero("Ingrese el ID del proveedor que desea eliminar", id)){
        return;
    }

    // Buscamos el índice
    ind = buscarPorId<Proveedor>(archivoProveedores, id);
    // Validamos que exista
    if (ind == -1) {
        cout << ROJO << endl << "Error: El proveedor con ID " << id << " no existe." << endl << RESET;
        return;
    }
    Proveedor proveedor = obtenerRegistroPorIndice<Proveedor>(archivoProveedores, ind);

    char decision;
    encabezadoProveedorCliente();
    mostrarDetalleProveedor(proveedor);
    
    if(idTieneTransacciones(archivoProveedores, id)){
        cout << ROJO << "Este producto tiene transacciones registradas." << endl;
        cout << AMARILLO << "Eliminarlo podria afectar la integridad de los reportes históricos." << endl << RESET;
    } 

    cout << "¿Seguro que desea eliminar el proveedor? (S/N): ";
    cin >> decision;

    if (decision == 's' || decision == 'S') { // Reajuste de posicion de productos
        eliminarRegistroLogico<Proveedor>(archivoProveedores, id);    
        cout << VERDE << NEGRITA << "Proveedores eliminado exitosamente." << endl << RESET;
    } else {
        cout << ROJO << "Operación cancelada. No se realizaron cambios" << endl << RESET;
    }
}

// FUNCIONES CRUD - CLIENTES
// Crear Cliente
void crearCliente(const char* archivoClientes){

    char decision;
    cout << "¿Desea registrar un nuevo cliente? (S/N): ";
    cin >> decision;

    if (decision == 's' || decision == 'S') {
       Cliente nuevo_cliente;

        // Validaciones de entrada
        if (!solicitarTexto("Ingrese nombre del Cliente", nuevo_cliente.nombre, 100)) return;
        if (!solicitarTexto("Ingrese cedula del Cliente", nuevo_cliente.cedula, 20)) return;
        
        if (cedulaDuplicada(archivoClientes, nuevo_cliente.cedula)) {
            cout << ROJO << "Error: La cedula ya existe." << endl << RESET;
            return;
        }
        
        if (!solicitarTexto("Ingrese el telefono del Cliente", nuevo_cliente.telefono, 20)) return;
        if (!solicitarTexto("Ingrese la direccion del Cliente", nuevo_cliente.direccion, 200)) return;
        if (!solicitarTexto("Ingrese el email del Cliente", nuevo_cliente.email, 100)) return;
        
        if (!validarEmail(nuevo_cliente.email)) {
            cout << ROJO << "Error: El correo debe incluir un '@' y al menos un '.'" << endl << RESET;
            return;
        }

        // Resumen de nuevos datos
        imprimirSeparador(60, '=');
        cout << AZUL << NEGRITA << "      RESUMEN DE CLIENTE" << endl << RESET;
        imprimirSeparador(60, '=');
        cout << CYAN << "Nombre: " << nuevo_cliente.nombre << endl;
        cout << "Telefono: " << nuevo_cliente.telefono << endl;
        cout << "Email: " << nuevo_cliente.email << endl;
        cout << "Cedula:" << nuevo_cliente.cedula << endl;
        cout << "Dirección:" << nuevo_cliente.direccion << endl << RESET;
        imprimirSeparador(60, '=');

        cout << "¿Desea Guardar al cliente? (S/N): ";
        cin >> decision;

        if (decision == 's' || decision == 'S') {
            if (guardarNuevoRegistro<Cliente>(archivoClientes, nuevo_cliente)){
                cout << endl << VERDE << NEGRITA << "¡Cliente guardado! ID asignado: " << nuevo_cliente.id << endl << RESET;
            }
            encabezadoProveedorCliente();
            mostrarDetalleCliente(nuevo_cliente);

        } else {
            cout << ROJO << "Operación cancelada. El cliente nol fue guardado." << endl << RESET;
        }
    } else {
        cout << AMARILLO << endl << "Volviendo al menú." << endl << RESET;
    }
}

// Buscar Cliente
void buscarCliente(const char* archivoClientes){

    ArchivoHeader header = leerHeader(archivoClientes);
    if (header.registrosActivos == 0){
            cout << ROJO << "No hay clientes registrados en el sistema." << endl << RESET;
            return;
    }

    int seleccion;
    do {
        cout << AZUL << NEGRITA << "===== MENÚ DE BUSQUEDA DE CLIENTES =====" << endl << RESET;
        cout << CYAN << "1. Buscar por ID" << endl;
        cout << "2. Buscar por nombre (coincidencia parcial)" << endl;
        cout << "3. Buscar por cedula" << endl;
        cout << "0. Cancelar" << endl << RESET;

        if (!solicitarEntero("Seleccione una opción", seleccion)) {
            break; 
        }

        switch (seleccion) {
        	case 1:{ 
	            int id, i;
	            Cliente registro_cliente;
	            if (solicitarEntero("Ingrese el ID del cliente", id)) {
	                i = buscarPorId<Cliente>(archivoClientes, id); 
	                if (i != -1) {
                        registro_cliente = obtenerRegistroPorId<Cliente>(archivoClientes, id);
	                    cout << endl << VERDE << NEGRITA << "Cliente encontrado:" << endl << RESET;
	                    encabezadoProductos();
	            		mostrarDetalleCliente(registro_cliente);
	                    imprimirSeparador();
	                } else {
	                    cout << ROJO << endl << "Error: El cliente con ID " << id << " no existe." << endl << RESET;
	                }
	            }
	            break;
            }

            case 2: { // Buscar por nombre (coincidencia parcial)
                char nombreBusqueda[100];
                int numEncontrados = 0;

                if (solicitarTexto("Ingrese el nombre (o parte) del cliente", nombreBusqueda, 100)) {
                    
                    int* indices = buscarRegistroPorNombre<Cliente>(archivoClientes, nombreBusqueda, &numEncontrados);
                    if (indices != nullptr) {
                        cout << endl << VERDE << NEGRITA << "Se encontraron " << numEncontrados << " coincidencias:" << endl << RESET;
                        
                        encabezadoProveedorCliente();
                        for (int i = 0; i < numEncontrados; i++) {
                            Cliente cliente = obtenerRegistroPorIndice<Cliente>(archivoClientes, indices[i]);
                            mostrarDetalleCliente(cliente);
                        }
                        imprimirSeparador();
                        delete[] indices; // liberamos memoria
                        indices = nullptr;

                    } else {
                        cout << AMARILLO << endl << "No se encontraron clientes que coincidan con: '" << nombreBusqueda << "'" << endl << RESET;
                    }
                }
                break;
            }

            case 3: { // Buscar por cedula
                char cedulaBusqueda[20];
                int i;
                if (solicitarTexto("Ingrese la cedula completa a buscar", cedulaBusqueda, 20)) {
                 
                   i = buscarClientePorCedula(archivoClientes, cedulaBusqueda); 
                   if (i != -1) {            
                        Cliente cliente = obtenerRegistroPorIndice<Cliente>(archivoClientes, i);
                        cout << VERDE << NEGRITA << endl << "Cliente encontrado:" << endl << RESET;
                        encabezadoProveedorCliente();
                        mostrarDetalleCliente(cliente);
                        imprimirSeparador();
                    } else {
                        cout << ROJO << "Error: El cliente con cedula " << cedulaBusqueda << " no existe." << endl << RESET;
                    }
                }
                break;        
            }

            case 0:{
                cout << AMARILLO << "Cancelando busqueda" << endl << RESET;
                break;
            }
            default:{
                cout << ROJO << "Opción no valida." << endl << RESET;
                break;
            }
        }
    } while (seleccion != 0);
}


// Actualizar Cliente
void actualizarCliente(const char* archivoClientes){

	ArchivoHeader header = leerHeader(archivoClientes);
    if (header.registrosActivos == 0){
            cout << ROJO << "No hay clientes en el sistema." << endl << RESET;
            return;
    }
    
    int id, i;
    Cliente registro_cliente;
	if (solicitarEntero("Ingrese el ID del cliente", id)) {
	    i = buscarPorId<Cliente>(archivoClientes, id); 
	    if (i != -1) {
            registro_cliente = obtenerRegistroPorIndice<Cliente>(archivoClientes, i);
			cout << endl << AZUL << NEGRITA << "DATOS ACTUALES DEL CLIENTE:" << endl << RESET;
	        cout << endl << VERDE << NEGRITA << "Cliente encontrado:" << endl << RESET;
	        encabezadoProveedorCliente();
	        mostrarDetalleCliente(registro_cliente);
	        imprimirSeparador();
	    } else {
	        cout << endl << ROJO << "Error: El cliente con ID " << id << " no existe." << endl << RESET;
	        return;
        } 
    } else {
        return; // Salir si no ingresó un ID válido
    }

    int seleccion;
    bool cambiosRealizados = false;

    // Menú de campos editables
    do {
        cout << AZUL << endl << "¿Qué desea editar?" << endl;
        cout << CYAN << "1. Nombre" << endl;
        cout << "2. Cedula " << endl;
        cout << "3. Telefono" << endl;
        cout << "4. Email" << endl;
        cout << "5. Direccion" << endl;
        cout << "6. Guardar cambios" << endl;
        cout << "0. Cancelar sin guardar" << endl << RESET;
        
        if (!solicitarEntero("Seleccione una opción", seleccion)) continue;

        switch (seleccion) {
            case 1:
                if (solicitarTexto("Ingrese nuevo nombre", registro_cliente.nombre, 100)) {
                    cambiosRealizados = true;
                }
                break;
            case 2:{
                char cedulaTemp[20];
                if (solicitarTexto("Ingrese nueva Cedula", cedulaTemp, 20)){
					if (strcmp(cedulaTemp, registro_cliente.cedula) != 0 && buscarClientePorCedula(archivoClientes, cedulaTemp)) {
                        cout << ROJO << "Error: El cedula ya existe." << endl << RESET; 
                    }
                    else {
                        strcpy(registro_cliente.cedula, cedulaTemp);
                        cambiosRealizados = true;
                    }
                }
                break;
            }
            case 3:
                if (solicitarTexto("Ingrese nuevo telefono", registro_cliente.telefono, 20)) {
                    cambiosRealizados = true;
                }
                break;
            case 4:
                char emailTemp[100];
                if (solicitarTexto("Ingrese el nuevo email", emailTemp, 100)) {
                    if (!validarEmail(emailTemp)) {
            		cout << ROJO << "Error: El correo debe incluir un '@' y al menos un '.'" << endl << RESET;
        			}
                    else {
                        strcpy(registro_cliente.email, emailTemp);
                        cambiosRealizados = true;
                    }
                }
                break;
            case 5:
                if (solicitarTexto("Ingrese la nueva direccion", registro_cliente.direccion,200)) {
                    cambiosRealizados = true;
                }
                break;
    
            case 6: {
                if (!cambiosRealizados) {
                    cout << AMARILLO << "No se han realizado cambios para guardar." << endl << RESET;
                } else {
                    char decision;
                    cout << "¿Desea Guardar los cambios hechos al cliente? (S/N): ";
                    cin >> decision;
                    if (decision == 's' || decision == 'S') {
                        if(actualizarRegistro<Cliente>(archivoClientes, i, registro_cliente)){
                            cout << VERDE << NEGRITA << "¡Cambios aplicados exitosamente!" << endl << RESET;
                            seleccion = 0; 
                        }
                    }
                }
                break;
            }
            case 0:
                cout << ROJO << "Operación cancelada. No se guardaron cambios." << endl << RESET;
                break;
            default: 
                cout << AMARILLO << "Opción no válida." << endl << RESET;
                break;
        }
    } while (seleccion != 0);
}

// Listar Clientes
void listarClientes(const char* archivoClientes){

    ArchivoHeader header = leerHeader(archivoClientes);
    if (header.registrosActivos ==0){
        cout << endl << ROJO << "No hay clientes registrados en el sistema" << endl << RESET;
        return;
    }
    
    ifstream archivo(archivoClientes, ios::binary);
    if (!archivo.is_open()){
        cout << ROJO << "Error: No se pudo abrir el archivo " << archivoClientes << endl << RESET;
        return;    
    }
    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    
    imprimirSeparador();
    cout << endl << AZUL << NEGRITA << "          LISTADO GENERAL DE CLIENTES" << endl << RESET;
    encabezadoProveedorCliente(); 

    Cliente cliente;

    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.read(reinterpret_cast<char*>(&cliente), sizeof(Cliente));
        if (!cliente.eliminado){    
            mostrarDetalleCliente(cliente); 
        }
    }
    archivo.close();
    imprimirSeparador();
}

// Eliminar Cliente
void eliminarCliente(const char* archivoClientes, const char* archivoTransacciones){

    ArchivoHeader header = leerHeader(archivoClientes);
    if (header.registrosActivos ==0){
        cout << endl << ROJO << "No hay clientes registrados en el sistema" << endl << RESET;
        return;
    }

    int id, ind;
    if (!solicitarEntero("Ingrese el ID del cliente que desea eliminar", id)){
        return;
    }

    // Buscamos el índice
    ind = buscarPorId<Cliente>(archivoClientes, id);
    // Validamos que exista
    if (ind == -1) {
        cout << ROJO << endl << "Error: El cliente con ID " << id << " no existe." << endl << RESET;
        return;
    }
    Cliente cliente = obtenerRegistroPorIndice<Cliente>(archivoClientes, ind);

    char decision;
    encabezadoProveedorCliente();
    mostrarDetalleCliente(cliente);
    
    if(idTieneTransacciones(archivoTransacciones, id)){
        cout << ROJO << "Este cliente tiene transacciones registradas." << endl;
        cout << AMARILLO << "Eliminarlo podria afectar la integridad de los reportes históricos." << endl << RESET;
    } // esto para clientes

    cout << "¿Seguro que desea eliminar el cliente? (S/N): ";
    cin >> decision;

    if (decision == 's' || decision == 'S') { // Reajuste de posicion de productos
        eliminarRegistroLogico<Cliente>(archivoClientes, id);    
        cout << VERDE << NEGRITA << "Cliente eliminado exitosamente." << endl << RESET;
    } else {
        cout << ROJO << "Operación cancelada. No se realizarion cambios." << endl << RESET;
    }

}

// FUNCIONES DE TRANSACCIONES

// Registrar Compra (a Proveedor)
void registrarCompra(const char* archivoTransacciones, const char* archivoDetalles, const char* archivoProductos, const char* archivoProveedores){

    ArchivoHeader header = leerHeader(archivoProveedores);
    if (header.registrosActivos ==0){
        cout << ROJO << endl << "No hay proveedores registrados en el sistema" << endl << RESET;
        return;
    }

    header = leerHeader(archivoProductos);
    if (header.registrosActivos ==0){
        cout << endl << ROJO << "No hay productos registrados en el sistema" << endl << RESET;
        return;
    }

    int proveedorId;
    cout << endl << AZUL << NEGRITA << "===== REGISTRAR COMPRA A PROVEEDOR =====" << endl << RESET;

    // Validar existencia del proveedor
    if (!solicitarEntero("Ingrese ID del proveedor", proveedorId)){
        return;
    } int indProveedor = buscarPorId<Proveedor>(archivoProveedores, proveedorId);
    if (indProveedor == -1){
        cout << endl << ROJO << "Error: El proveedor no existe." << endl << RESET;
        return;
    }

    // Carrito de compras temp
    DetalleTransaccion carrito[100];
    int cantItems = 0;
    float totalCompra = 0;
    char continuar;

    do
    {
        int productoId, cantidad;
        float costo;

        // Validar existencia del producto
        if (!solicitarEntero("Ingrese ID del producto", productoId)){
            break;
        }
        int indProducto = buscarPorId<Producto>(archivoProductos, productoId);
        if (indProducto == -1){
            cout << endl << ROJO << "Error: El producto no existe." << RESET;
            continue;
        } else {
            if (!solicitarEntero("Ingrese cantidad", cantidad)){
            return;
            }
            if (!solicitarFloat("Ingrese Costo Unitario", costo)){
            return;
            }
            carrito[cantItems].idProducto = productoId;
            carrito[cantItems].cantidad = cantidad;
            carrito[cantItems].precioUnitario = costo;

            totalCompra += cantidad*costo;
            cantItems++;
            cout << VERDE << NEGRITA << "Producto agregado al carrito." << endl << RESET;
        }
        if (cantItems < 100) {
            cout << "¿Desea agregar otro producto? (S/N y presione ENTER): ";
            cin >> continuar;
        } else {
            cout << AMARILLO << "Carrito lleno. No se pueden agregar mas productos al carrito." << endl << RESET;
            break;
        }

    } while ((continuar == 's' || continuar == 'S') && cantItems < 100);
    
    if (cantItems == 0){
        cout << ROJO << "Compra cancelada: No se agregaron productos al carrito" << endl << RESET;
        return;
    }
    
    cout << endl;
    imprimirSeparador(60, '=');
    cout << AZUL << NEGRITA << "      RESUMEN DE COMPRA" << endl << RESET;
    imprimirSeparador(60, '-');
    cout << CYAN << left << setw(20) << "PRODUCTO" << setw(8)  << "CANT." << setw(12) << "SUBTOTAL" << endl;
    for (int i = 0; i < cantItems; i++)
    {
        int indProd = buscarPorId<Producto>(archivoProductos, carrito[i].idProducto);
        Producto producto = obtenerRegistroPorIndice<Producto>(archivoProductos, indProd);
        cout << left << setw(20) << producto.nombre << setw(8)  << carrito[i].cantidad << setw(12) << carrito[i].precioUnitario*carrito[i].cantidad << endl;
    }
    cout << "TOTAL DE COMPRA:   $" << fixed << setprecision(2) << totalCompra << endl << RESET;
    imprimirSeparador(60, '=');

    char confirmar;
    cout << "¿Desea confirmar y guardar esta Compra? (S/N y presione ENTER para continuar) ";
    cin >> confirmar;

    if (confirmar == 'S' || confirmar == 's'){
        Transaccion trans;
        trans.idRelacionado = proveedorId;
        strcpy(trans.tipo, "COMPRA");
        trans.cantidadItemsDiferentes = cantItems;
        trans.total = totalCompra;
        solicitarTexto("Ingrese descripcion de la compra", trans.descripcion, 200);
        
        if (guardarNuevoRegistro<Transaccion>(archivoTransacciones, trans)){
            
            for (int i = 0; i < cantItems; i++){
                guardarDetalle(archivoDetalles, trans.id, carrito[i]);
                
                int indProducto = buscarPorId<Producto>(archivoProductos, carrito[i].idProducto);
                Producto producto = obtenerRegistroPorIndice<Producto>(archivoProductos, indProducto);
                producto.stock += carrito[i].cantidad;                                 // Calcula nuevo stock
                actualizarRegistro<Producto>(archivoProductos, indProducto, producto); // Actualiza stock en el archivo
            }
            Proveedor prov = obtenerRegistroPorIndice<Proveedor>(archivoProveedores, indProveedor);
            prov.totalCompras += totalCompra;       // Acumula el gasto/inversion

            if (prov.cantidadTransacciones < 100){
                prov.transaccionesIds[prov.cantidadTransacciones] = trans.id;
                prov.cantidadTransacciones++;
            }
            
            for (int i = 0; i < cantItems; i++) {
                int idProdActual = carrito[i].idProducto;
                bool yaExiste = false;

                // Verifica si el producto ya estaba en la lista del proveedor 
                for (int j = 0; j < prov.cantidadProductos; j++) {
                    if (prov.productosIDs[j] == idProdActual) {
                        yaExiste = true;
                        break;
                    }
                }
                // Si no esta en la lista del proveedor, se agrega
                if (!yaExiste && prov.cantidadProductos < 100) {
                    prov.productosIDs[prov.cantidadProductos] = idProdActual;
                    prov.cantidadProductos++;
                }
            }
            
            actualizarRegistro<Proveedor>(archivoProveedores, indProveedor, prov);

            cout << endl << VERDE << NEGRITA << "Compra registrada con éxito! ID Ticket: " << trans.id << endl << RESET;
        } else {
            cout << endl << ROJO << "Compra cancelada." << endl << RESET;
        }
    }
}

// Registrar Venta (a Cliente)
void registrarVenta(const char* archivoTransacciones, const char* archivoDetalles, const char* archivoProductos, const char* archivoProveedores, const char* archivoClientes){

    ArchivoHeader header = leerHeader(archivoClientes);
    if (header.registrosActivos ==0){
        cout << endl << ROJO << "No hay clientes registrados en el sistema" << endl << RESET;
        return;
    }

    header = leerHeader(archivoProductos);
    if (header.registrosActivos ==0){
        cout << endl << ROJO << "No hay productos registrados en el sistema" << endl << RESET;
        return;
    }

    int clienteId;
    cout << endl << AZUL << NEGRITA << "===== REGISTRAR VENTA A CLIENTES =====" << endl << RESET;

    // Validar existencia del cliente
    if (!solicitarEntero("Ingrese ID del cliente", clienteId)){
        return;
    } int indCliente = buscarPorId<Cliente>(archivoClientes, clienteId);
    if (indCliente == -1){
        cout << endl << ROJO <<  "Error: El Cliente no existe." << endl << RESET;
        return;
    }

    // Carrito de compras temp
    DetalleTransaccion carrito[100];
    int cantItems = 0;
    float totalVenta = 0;
    char continuar;

    do
    {
        int productoId, cantidad;
        float precio;

        // Validar existencia del producto
        if (!solicitarEntero("Ingrese ID del producto", productoId)){
            break;
        }
        int indProducto = buscarPorId<Producto>(archivoProductos, productoId);
        if (indProducto == -1){
            cout << endl << ROJO << "Error: El producto no existe." << endl << RESET;
            continue;
        } else
        {
            Producto prod =obtenerRegistroPorIndice<Producto>(archivoProductos, indProducto);
            cout << AZUL << "Producto:" << prod.nombre << " | Stock disponible: " << prod.stock << endl << RESET;

            if (!solicitarEntero("Ingrese cantidad a vender", cantidad)){
                break;
            } if (cantidad > prod.stock)
            {
                cout << ROJO << "Error: Stock insuficiente. Cantidad disponible: " << prod.stock << " unidades." << endl << RESET;
                continue;
            } else if (cantidad <= 0) {
                cout << ROJO << "Error: La cantidad debe ser mayor a cero." << endl << RESET;
                continue;
            } if (!solicitarFloat("Ingrese Precio Unitario", precio)){
                break;
            }
            carrito[cantItems].idProducto = productoId;
            carrito[cantItems].cantidad = cantidad;
            carrito[cantItems].precioUnitario = precio;
            
            totalVenta += cantidad*precio;
            cantItems++;
            cout << "Producto guardado al carrito." << endl;
        }
        if (cantItems< 100){
            cout << "¿Desea agregar otro producto? (S/N y presione ENTER para continuar): ";
            cin >> continuar;
        } else{
            cout << ROJO << "Carrito lleno. No se pueden agregar mas productos" << endl << RESET;   
            break;     
        }

    } while ((continuar == 's' || continuar == 'S') && cantItems < 100);
    
    if (cantItems == 0){
        cout << ROJO << "Venta cancelada: No se agregaron productos al carrito" << endl << RESET;
        return;
    } 
    
    cout << endl;
    imprimirSeparador(60, '=');
    cout << AZUL << NEGRITA << "      RESUMEN DE VENTA" << endl;
    imprimirSeparador(60, '-');
    cout << AZUL << left << setw(20) << "PRODUCTO" << setw(8)  << "CANT." << setw(12) << "SUBTOTAL" << endl;
    for (int i = 0; i < cantItems; i++)
    {
        int indProd = buscarPorId<Producto>(archivoProductos, carrito[i].idProducto);
        Producto producto = obtenerRegistroPorIndice<Producto>(archivoProductos, indProd);
        cout << left << setw(20) << producto.nombre << setw(8)  << carrito[i].cantidad << setw(12) << carrito[i].precioUnitario*carrito[i].cantidad << endl;
    }
    cout << "TOTAL DE VENTA:   $" << fixed << setprecision(2) << totalVenta << endl << RESET;
    imprimirSeparador(60, '=');

    char confirmar;
    cout << "¿Desea confirmar y guardar esta Compra? (S/N y presione ENTER para continuar) ";
    cin >> confirmar;

    if (confirmar == 'S' || confirmar == 's'){
        Transaccion trans;
        trans.idRelacionado = clienteId;
        strcpy(trans.tipo, "VENTA");
        trans.cantidadItemsDiferentes = cantItems;
        trans.total = totalVenta;
        solicitarTexto("Ingrese descripcion de la venta", trans.descripcion, 200);
        
        if (guardarNuevoRegistro<Transaccion>(archivoTransacciones, trans)){

            for (int i = 0; i < cantItems; i++){
                guardarDetalle(archivoDetalles, trans.id, carrito[i]);
                
                int indProducto = buscarPorId<Producto>(archivoProductos, carrito[i].idProducto);
                Producto producto = obtenerRegistroPorIndice<Producto>(archivoProductos, indProducto);
                producto.stock -= carrito[i].cantidad; // Actualizar Stock
                producto.totalVendido += carrito[i].cantidad;
                actualizarRegistro<Producto>(archivoProductos, indProducto, producto);
            }
            Cliente client = obtenerRegistroPorIndice<Cliente>(archivoClientes, indCliente);
            client.totalCompras += totalVenta;      // Sumar gasto del cliente (Venta)

            if (client.cantidadTransacciones<100){
                client.transaccionesIds[client.cantidadTransacciones] = trans.id;
                client.cantidadTransacciones++;
            }
            actualizarRegistro<Cliente>(archivoClientes, indCliente, client);

            cout << VERDE << NEGRITA << endl << "Venta registrada con éxito! ID Ticket " << trans.id << endl << RESET;
        } else {
            cout << endl << ROJO << "Venta cancelada." << endl << RESET;
        }
    }
}

// Buscar Transacciones
void buscarTransacciones(const char* archivoTransacciones, const char* archivoDetalles, const char* archivoProductos, const char* archivoClientes, const char* archivoProveedores){
    
    int op;
    Transaccion trans;
    bool encontro;

    cout << AZUL << endl << NEGRITA << "===== MENÚ DE BUSQUEDA DE TRANSACCIONES =====" << endl << RESET;
    cout << CYAN << "Seleccione opción de busqueda: " << endl 
    << "1. Buscar por ID de Transacción" << endl 
    << "2. Buscar por ID de Producto" << endl
    << "3. Buscar por ID de Cliente" << endl
    << "4. Buscar por ID de Proveedor" << endl
    << "5. Buscar por fecha exacta" << endl
    << "6. Buscar por tipo de transación (COMPRA/VENTA)" << endl
    << "0. Cancelar" << endl << RESET;
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
	        int i = buscarPorId<Transaccion>(archivoTransacciones, TransID); // Extraer ID
	        if (i == -1){ // Validación de existencia
	            cout << ROJO << "Error: La transacción no existe." << endl << RESET;
	            break;
	        } 
            trans = obtenerRegistroPorIndice<Transaccion>(archivoTransacciones, i);
	        if (!trans.eliminado)
            {
                cout << endl << VERDE << NEGRITA << "Transacción encontrada" << endl << AZUL <<  "DATOS DE LA TRANSACCIÓN" << endl << RESET;
                encabezadoTransacciones(); 
                mostrarDetalleGeneralTransaccion(trans);
                
                cout << endl << AZUL << NEGRITA << "DETALLE DE ARTICULOS:" << endl << RESET;
	            mostrarDetalleTransaccion(archivoDetalles, archivoProductos, trans);
                imprimirSeparador();
            } else {
                cout << AMARILLO << "La transacción fue anulada." << endl << RESET;
            }
	        break;
    	}
    	
    case 2: {// Busqueda por ID del Producto
	        int prodID;
	
	        if (!solicitarEntero("Ingrese ID del producto", prodID)){
	        return;
	        }
	        if (!existeEntidad<Producto>(archivoProductos, prodID)){ // Validacion de existencia
	            cout << endl << ROJO << "Error: El producto no existe." << endl << RESET;
	            break;
	        }
            
            encontro = false;
	        encabezadoTransacciones();

	        ifstream archivo(archivoTransacciones, ios::binary);
            archivo.seekg(sizeof(ArchivoHeader), ios::beg);

            while (archivo.read(reinterpret_cast<char*>(&trans), sizeof(Transaccion))){
                if (!trans.eliminado){
                    if (verificarProductoEnTransaccion(archivoDetalles, trans.id, prodID)) {
                        mostrarDetalleGeneralTransaccion(trans);
                        encontro = true;
                    }           
                }
            }
            
            archivo.close();
            if(!encontro){
                cout << AMARILLO << "No hay transacciones asociadas a este producto." << endl << RESET;
            }
	        break;
		}
    case 3: { // Busqueda por Cliente
	
        int clienteID;
        if (!solicitarEntero("Ingrese ID del cliente", clienteID)){
            break;
        }
        if (!existeEntidad<Cliente>(archivoClientes, clienteID)){ // Validación de existencia del cliente
            cout << ROJO << "Error: El cliente no existe." << endl << RESET;
            break; 
        }
        int indClient = buscarPorId<Cliente>(archivoClientes, clienteID);
        Cliente client = obtenerRegistroPorIndice<Cliente>(archivoClientes, indClient);
    
        if (client.cantidadTransacciones == 0) {
            cout << AMARILLO << "El cliente no tiene ventas registradas." << endl << RESET;
            break;
        }

        encontro = false;
        encabezadoTransacciones();
        for (int j = 0; j < client.cantidadTransacciones; j++) {
            int idTrans = client.transaccionesIds[j];
            int indTrans = buscarPorId<Transaccion>(archivoTransacciones, idTrans);
            
            if (indTrans != -1) {
                Transaccion trans = obtenerRegistroPorIndice<Transaccion>(archivoTransacciones, indTrans);
                if (!trans.eliminado) {
                    mostrarDetalleGeneralTransaccion(trans);
                    encontro = true;
                }
            }
        }
        imprimirSeparador();
        if (!encontro){ // No encontro el ID en las transacciones
            cout << AMARILLO << "No hay ventas registradas para este cliente." << endl << RESET; 
        }
    break;
	}
	
    case 4: { // Busqueda por ID de proveedor
	        int proveedorID;
	
	        if (!solicitarEntero("Ingrese ID del proveedor", proveedorID)){
	            break;
	        }
	        if (!existeEntidad<Proveedor>(archivoProveedores, proveedorID)){
	            cout << endl << ROJO << "Error: El proveedor no existe." << endl << RESET;
	            break;
	        }
            int indProv = buscarPorId<Proveedor>(archivoProveedores, proveedorID);
	        Proveedor prov = obtenerRegistroPorIndice<Proveedor>(archivoProveedores, indProv);
    
            if (prov.cantidadTransacciones == 0) {
                cout << AMARILLO << "El proveedor no tiene compras registradas." << endl << RESET;
                break;
            }

            encontro = false;
            encabezadoTransacciones();
            for (int j = 0; j < prov.cantidadTransacciones; j++) {
                int idTrans = prov.transaccionesIds[j];
                int indTrans = buscarPorId<Transaccion>(archivoTransacciones, idTrans);
                
                if (indTrans != -1) {
                    Transaccion trans = obtenerRegistroPorIndice<Transaccion>(archivoTransacciones, indTrans);
                    if (!trans.eliminado) {
                        mostrarDetalleGeneralTransaccion(trans);
                        encontro = true;
                    }
                    
                }
            }
            imprimirSeparador();
	        if (!encontro){
	            cout << AMARILLO << "No hay compras registradas para este proveedor." << endl << RESET;
	        }
	    break;
	}
    case 5: { // Busqueda por fecha
	        char fecha[11];
            encontro = false;
	        if (!solicitarTexto("Ingrese fecha de la Transacción (YYYY-MM-DD)", fecha, 11)){
	            break;;
	        }
	        if(!validarFecha(fecha)){
	            cout << ROJO << "Error: Formato de fecha no valido (Use YYYY-MM-DD)" << endl << RESET;
	            break;
	        }
            
            
            ifstream archivo(archivoTransacciones, ios::binary);
            if (!archivo.is_open()){
                cout << ROJO << "Error al abrir el archivo de transacciones." << endl << RESET;
                break;
            }
            encabezadoTransacciones();
            archivo.seekg(sizeof(ArchivoHeader), ios::beg);

            while (archivo.read(reinterpret_cast<char*>(&trans), sizeof(Transaccion))){
                if (!trans.eliminado && strcmp(trans.fechaRegistro, fecha) == 0){
                    mostrarDetalleGeneralTransaccion(trans);
                    encontro = true;
                }
            }
            archivo.close();
	        if (!encontro){
	            cout << AMARILLO << "No hay transacciones registradas en la fecha especificada." << endl << RESET;
	        }
	    break;
	}
    case 6: {// busqueda por tipo de transaccion

        int tipo;
        cout << AZUL << NEGRITA << "   TIPOS DE TRANSACCIÓN" << endl 
             << CYAN << "1. Compras (A Proveedor)" << endl
             << "2. Ventas (A Cliente)" << endl
             << "0. Cancelar" << endl
             << endl << RESET;
        if (!solicitarEntero("Ingrese tipo de transacción", tipo)){
            break;
        }
        if (tipo < 1 || tipo > 2){
            cout << ROJO << "Opción Invalida" << endl << RESET;
            break;
        }
        
        encontro = false;
        encabezadoTransacciones();
        ifstream archivo(archivoTransacciones, ios::binary);
        archivo.seekg(sizeof(ArchivoHeader), ios::beg);

        while (archivo.read(reinterpret_cast<char*>(&trans), sizeof(Transaccion))){
            bool match = (tipo == 1) ? (strcmp(trans.tipo, "COMPRA") == 0 || strcmp(trans.tipo, "C") == 0)
                                     : (strcmp(trans.tipo, "VENTA") == 0 || strcmp(trans.tipo, "V") == 0);
            if (!trans.eliminado && match){
                mostrarDetalleGeneralTransaccion(trans);
                encontro = true;
            }
        }
        archivo.close();
        imprimirSeparador();
        if (!encontro){
            cout << AMARILLO << "No hay transacciones de este tipo" << endl << RESET;
        }
        break;
	}
    default:
        break;
    }
}

// Listar Transacciones
void listarTransacciones(const char* archivoTransacciones){

    ArchivoHeader header = leerHeader(archivoTransacciones);
    if (header.registrosActivos ==0){
        cout << endl << ROJO << "No hay transacciones activas registradas en el sistema" << endl << RESET;
        return;
    }
    ifstream archivo(archivoTransacciones, ios::binary);
    if (!archivo.is_open()){
        cout << ROJO << "Error: No se pudo abrir el archivo de transacciones." << endl << RESET;
        return;    
    }
    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    
    imprimirSeparador(60, '=');
    cout << AZUL << NEGRITA << "LISTADO GENERAL DE TRANSACCIONES" << endl << RESET;
    encabezadoTransacciones();

    Transaccion trans;
    for (int i = 0; i < header.cantidadRegistros; i++){
        if(!archivo.read(reinterpret_cast<char*>(&trans), sizeof(Transaccion))) break;
        if (!trans.eliminado){
            mostrarDetalleGeneralTransaccion(trans);
        }    
    }
    archivo.close();
    imprimirSeparador();
}

// Cancelar/Anular Transacción
void cancelarTransaccion(const char* archivoTransacciones, const char* archivoDetalles, const char* archivoProductos, const char* archivoProveedores, const char* archivoClientes){
    
    ArchivoHeader header = leerHeader(archivoTransacciones);
    if (header.registrosActivos ==0){
        cout << endl << ROJO << "No hay transacciones activas registradas en el sistema" << endl << RESET;
        return;
    }

    int transID;
    cout << endl << AZUL << NEGRITA << "===== CANCELAR TRANSACCION =====" << endl << RESET;
    if (!solicitarEntero("Ingrese ID de la transacción a cancelar", transID)){
        return;
    }
    int indiceTrans = buscarPorId<Transaccion>(archivoTransacciones, transID);
    if (indiceTrans == -1){
        cout << ROJO << "Error: Transacción no encontrada." << endl << RESET;
        return;
    }
    Transaccion trans = obtenerRegistroPorIndice<Transaccion>(archivoTransacciones, indiceTrans);

    if (trans.eliminado) {
        cout << endl << ROJO << "Error: Esta transacción ya se encuentra anulada." << endl << RESET;
        return;
    }
    
    cout << endl << AZUL << NEGRITA << "DATOS DE LA TRANSACCIÓN A ANULAR:" << endl << RESET;
    mostrarDetalleTransaccion(archivoDetalles, archivoProductos, trans);
    imprimirSeparador();

    char confirmar;
    cout << "¿Esta seguro de que desea ANULAR esta transacción? (S/N) ";
    cin >> confirmar;

    if (confirmar == 'S' || confirmar == 's'){
        if (eliminarRegistroLogico<Transaccion>(archivoTransacciones, indiceTrans)) {
            
            // Revertir el Stock (Recorrer todos los detalles de esta transacción)
            ifstream fDetalles(archivoDetalles, ios::binary);
            if (fDetalles.is_open()) {
                DetalleTransaccion d;
                while (fDetalles.read(reinterpret_cast<char*>(&d), sizeof(DetalleTransaccion))) {
                    if (d.idTransaccion == transID) {
                        // Busca el producto para ajustar su stock
                        int indProd = buscarPorId<Producto>(archivoProductos, d.idProducto);
                        if (indProd != -1) {
                            Producto p = obtenerRegistroPorIndice<Producto>(archivoProductos, indProd);
                            
                            if (strcmp(trans.tipo, "VENTA") == 0) {
                                p.stock += d.cantidad;
                                p.totalVendido -= d.cantidad; // Se resta lo vendido
                            } else {
                                p.stock -= d.cantidad;
                            }
                            actualizarRegistro<Producto>(archivoProductos, indProd, p);
                        }
                    }
                }
                fDetalles.close();
            }
            if (strcmp(trans.tipo, "VENTA") == 0 || strcmp(trans.tipo, "V") == 0){
                int indClient = buscarPorId<Cliente>(archivoClientes, trans.idRelacionado);
                if (indClient != -1){
                    Cliente client = obtenerRegistroPorIndice<Cliente>(archivoClientes, indClient);
                    client.totalCompras-=trans.total;
                    actualizarRegistro<Cliente>(archivoClientes, indClient, client);
                }
            } else {
                int indProv = buscarPorId<Proveedor>(archivoProveedores, trans.idRelacionado);
                if (indProv != -1){
                    Proveedor prov = obtenerRegistroPorIndice<Proveedor>(archivoProveedores, indProv);
                    prov.totalCompras -= trans.total; // Restamos la inversión anulada al proveedor
                    actualizarRegistro<Proveedor>(archivoProveedores, indProv, prov);
                }
            }
            
            cout << endl << VERDE << NEGRITA << "Transacción anulada con éxito y stock revertido." << endl << RESET;
        }
    } else {
        cout << endl << ROJO << "Operación cancelada. No se realizaron cambios en la transacción." <<  endl << RESET;
    }
}

// FUNCIONES DE MANTENIMIENTO E INTEGRIDAD 
void verificarIntegridadReferencial(const char* archivoProductos, const char* archivoTransacciones, const char* archivoDetalles, const char* archivoProveedores, const char* archivoClientes) {
    cout << AZUL << NEGRITA << "\n--- DIAGNÓSTICO DE INTEGRIDAD REFERENCIAL ---" << endl << RESET;
    int errores = 0;

    // Productos -> Valida ID Proveedores
    ifstream archProd(archivoProductos, ios::binary);
    if (archProd.is_open()) {
        archProd.seekg(sizeof(ArchivoHeader), ios::beg); // SALTA EL HEADER
        Producto p;
        while (archProd.read((char*)&p, sizeof(Producto))) {
            if (!p.eliminado) {
                if (!existeEntidad<Proveedor>(archivoProveedores, p.idProveedor)) {
                    cout << ROJO << "[ERROR] Producto ID " << p.id << " (" << p.nombre << ") refiere a Proveedor ID " << p.idProveedor << " inexistente." << endl << RESET;
                    errores++;
                }
            }
        }
        archProd.close();
    } else {
        cout << ROJO << "Error no se pudo abrir el archivo de Productos." << endl << RESET; 
    }
    
    // Detalles -> Valida ID Productos y ID Transacciones
    ifstream archDet(archivoDetalles, ios::binary);
    if (archDet.is_open()) {
        
        DetalleTransaccion d;
        while (archDet.read((char*)&d, sizeof(DetalleTransaccion))) {
            if (!existeEntidad<Producto>(archivoProductos, d.idProducto)) {
                cout << ROJO << "[ERROR] Detalle refiere a Producto ID " << d.idProducto << " inexistente." << endl << RESET;
                errores++;
            }
            if (!existeEntidad<Transaccion>(archivoTransacciones, d.idTransaccion)) {
                cout << ROJO << "[ERROR] Detalle refiere a Transacción ID " << d.idTransaccion << " inexistente." << endl << RESET;
                errores++;
            }
        }
        archDet.close();
    } else {
        cout << ROJO << "Error no se pudo abrir el archivo de Detalles transaccionales." << endl << RESET; 
    }

    // Clientes -> Valida ID transacción
    ifstream archCli(archivoClientes, ios::binary);
    if (archCli.is_open()) {
        archCli.seekg(sizeof(ArchivoHeader), ios::beg);
        Cliente c;
        while (archCli.read((char*)&c, sizeof(Cliente))) {
            if (!c.eliminado) {
                for (int j = 0; j < c.cantidadTransacciones; j++) {
                    int idBusq = c.transaccionesIds[j];
                    int indTrans = buscarPorId<Transaccion>(archivoTransacciones, idBusq);

                    if (indTrans == -1) {
                        cout << ROJO << "[ERROR] Cliente ID " << c.id << " refiere a Transacción ID " << idBusq << " inexistente." << endl << RESET;
                        errores++;
                    } else { // Valida que la transacción asociada sea una "Venta"
                        Transaccion t = obtenerRegistroPorIndice<Transaccion>(archivoTransacciones, indTrans);
                        if (strcmp(t.tipo, "VENTA") != 0) {
                            cout << ROJO << "[ERROR] Cliente ID " << c.id << " tiene en su histórial la Transacción ID " << idBusq << " que NO es una VENTA." << endl << RESET;
                            errores++;
                        }
                    }
                }
            }
        }
        archCli.close();
    } else {
        cout << ROJO << "Error no se pudo abrir el archivo de Clientes." << endl << RESET; 
    }

    // Proveedores -> Validar ID transacción(Compras) y ID Productos
    ifstream archProv(archivoProveedores, ios::binary);
    if (archProv.is_open()) {
        archProv.seekg(sizeof(ArchivoHeader), ios::beg);
        Proveedor prov;
        
        while (archProv.read((char*)&prov, sizeof(Proveedor))) {
            if (!prov.eliminado) {
                // Validación del historial de transacciones
                for (int j = 0; j < prov.cantidadTransacciones; j++) {
                    int idBusq = prov.transaccionesIds[j];
                    int indTrans = buscarPorId<Transaccion>(archivoTransacciones, idBusq);

                    if (indTrans == -1) {
                        cout << ROJO << "[ERROR] Proveedor ID " << prov.id << " refiere a Transacción ID " << idBusq << " inexistente." << endl << RESET;
                        errores++;
                    } else {
                        // Valida que la transacción sea una COMPRA
                        Transaccion t = obtenerRegistroPorIndice<Transaccion>(archivoTransacciones, indTrans);
                        if (strcmp(t.tipo, "COMPRA") != 0) {
                            cout << ROJO << "[ERROR] Proveedor ID " << prov.id << " tiene en su historial la Transacción ID " 
                                 << idBusq << " que es una " << t.tipo << " (deberia ser COMPRA)." << endl << RESET;
                            errores++;
                        }
                    }
                }
                // Valida la lista de productos suministrados
                for (int k = 0; k < prov.cantidadProductos; k++) {
                    int idProdBusq = prov.productosIDs[k];
                    if (!existeEntidad<Producto>(archivoProductos, idProdBusq)) {
                        cout << ROJO << "[ERROR] Proveedor ID " << prov.id << " ofrece el Producto ID " 
                             << idProdBusq << " que no existe en el inventario." << endl << RESET;
                        errores++;
                    }
                }
            }
        }
        archProv.close();
    } else {
        cout << ROJO << "Error: No se pudo abrir el archivo de Proveedores." << endl; 
    }

    cout << AMARILLO << NEGRITA << "\nREPORTE FINAL: Se encontraron " << errores << " inconsistencias." << endl << RESET;
}

// --- RESPALDO DE DATOS (BACKUP) ---

void copiarArchivo(const char* origen, const char* destino) {
    ifstream src(origen, ios::binary);
    if (!src.is_open()) {
        cout << ROJO << "Error: no se pudo abrir el archivo." << endl << RESET;
        return;
    }
    ofstream dst(destino, ios::binary);
    if (!dst.is_open()) {
        cout << ROJO << "Error: no se pudo abrir el archivo." << endl << RESET;
        return;
    }
    dst << src.rdbuf(); // Copia archivos
    cout << RESET;
    src.close();
    dst.close();
}

void crearBackup(const char* archProd, const char* archProv, const char* archTrans, const char* archDet, const char* archCli) {
    char fecha[20];
    obtenerFechaActual(fecha); 

    char nombreCarpeta[100];
    snprintf(nombreCarpeta, sizeof(nombreCarpeta), "BACKUP_%s", fecha);

    if (_mkdir(nombreCarpeta) == 0) {
        cout << VERDE << NEGRITA << "\nCarpeta de respaldo creada: " << nombreCarpeta << RESET << endl << RESET;
    } else {
        cout << AMARILLO << "\nActualizando respaldo en carpeta: " << nombreCarpeta << RESET << endl << RESET;
    }

    const char* archivos[] = { archProd, archProv, archCli, archTrans, archDet };
    char rutaDestino[256];

    for (int i = 0; i < 5; i++) {
        // Construye la ruta interna: "BACKUP_19-03-2026/productos.bin"
        snprintf(rutaDestino, sizeof(rutaDestino), "%s/%s", nombreCarpeta, archivos[i]);
        
        copiarArchivo(archivos[i], rutaDestino);
        cout << "  " << VERDE << NEGRITA << "[OK]" << RESET << " " << archivos[i] << " guardado." << endl << RESET;
    }

    cout << VERDE << NEGRITA << "\n¡Respaldo de seguridad finalizado!" << RESET << endl;
}

// --- REPORTES ANALÍTICOS ---

void reporteStockCritico(const char* archivoProductos) {
    ifstream arch(archivoProductos, ios::binary);
    if (!arch.is_open()){ 
        cout << ROJO << "Error al abrir el archivo." << endl << RESET;
        return;
    }

    ArchivoHeader header = leerHeader(archivoProductos);
    arch.seekg(sizeof(ArchivoHeader), ios::beg);

    Producto p;
    imprimirSeparador(60, '=');
    cout << ROJO << NEGRITA << "      ALERTA: PRODUCTOS POR DEBAJO DEL MINIMO" << endl << RESET;
    imprimirSeparador(60, '=');
    cout << AMARILLO << left 
        << setw(10) << "ID" 
        << setw(20) << "NOMBRE" 
        << setw(10) << "STOCK" 
        << setw(10) << "MINIMO" << endl << RESET;
    imprimirSeparador(50, '-');

    bool huboAlertas = false;

    while (arch.read((char*)&p, sizeof(Producto))) {
        if (!p.eliminado && p.stock <= p.stockMinimo) {
            huboAlertas = true;
            
            if (p.stock == 0) {
                cout << ROJO << NEGRITA; // Rojo brillante si no hay nada
            } else {
                cout << AMARILLO; // Amarillo si es solo advertencia
            }

            cout << left << setw(8) << p.id 
                 << setw(25) << p.nombre 
                 << setw(12) << p.stock 
                 << setw(12) << p.stockMinimo;
            
            if (p.stock == 0) {
                cout << " [AGOTADO]";
            }
            
            cout << RESET << endl;
        }
    }

    if (!huboAlertas) {
        cout << VERDE << NEGRITA << "No hay productos en estado crítico. ¡Todo en orden!" << RESET << endl << RESET;
    }
    imprimirSeparador();
    arch.close();
}

void historialCliente(const char* archivoClientes, const char* archivoTransacciones, const char* archivoDetalles, const char* archivoProductos) {
    int idBusq;
    if (!solicitarEntero("Ingrese ID del cliente para ver su historial", idBusq)) return;

    int indClient = buscarPorId<Cliente>(archivoClientes, idBusq);
    if (indClient == -1) {
        cout << ROJO << "Error: El cliente no existe." << endl << RESET;
        return;
    }
    
    Cliente client = obtenerRegistroPorIndice<Cliente>(archivoClientes, indClient);
    
    imprimirSeparador(60, '=');
    cout << AZUL << NEGRITA << "\n--- HISTORIAL DEL CLIENTE " << client.nombre << " ---" << endl << RESET;
    cout << CYAN << "Cedula/RIF: " << client.cedula << " | Total Compras: $" << client.totalCompras << endl << RESET;
    imprimirSeparador(60, '-');

    if (client.cantidadTransacciones == 0) {
        cout << AMARILLO << "No registra movimientos financieros." << endl << RESET;
        return;
    }

    for (int j = 0; j < client.cantidadTransacciones; j++) {
        int idTrans = client.transaccionesIds[j];
        int indTrans = buscarPorId<Transaccion>(archivoTransacciones, idTrans);
            
        if (indTrans != -1) {
            Transaccion trans = obtenerRegistroPorIndice<Transaccion>(archivoTransacciones, indTrans);
            if (!trans.eliminado) {
                cout << CYAN << "\n Transacción ID " << trans.id << " | Fecha: " << trans.fechaRegistro << " | Total: $" << trans.total << endl << RESET;
                mostrarDetalleTransaccion(archivoDetalles, archivoProductos, trans);
            } else {
                cout << AMARILLO << "\n Transacción ID" << trans.id << " [ANULADA]" << endl << RESET;
            }
        }
    }
}

// FUNCIONES AUXILIARES

void guardarDetalle(const char* archivoDetalles, int idTransaccion, const DetalleTransaccion &detalle) {
    // Asignamos el ID de la cabecera al detalle para vincularlos
    DetalleTransaccion nuevoDetalle = detalle;
    nuevoDetalle.idTransaccion = idTransaccion;

    ofstream archivo(archivoDetalles, ios::binary | ios::app);
    if (archivo.is_open()) {
        archivo.write(reinterpret_cast<const char*>(&nuevoDetalle), sizeof(DetalleTransaccion));
        archivo.close();
    } else {
        cout << ROJO << "Error crítico: No se guardaron los detalles en el archivo." << endl << RESET;
    }
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

bool productoTieneTransacciones(const char* archivoTransacciones, const char* archivoDetalles, int idProducto) {
    ArchivoHeader header = leerHeader(archivoTransacciones);
    if (header.cantidadRegistros == 0){
        return false;
    }
    ifstream archivo(archivoDetalles, ios::binary);
    if (!archivo.is_open()){
        return false;
    }
    
    DetalleTransaccion detalle;

    while (archivo.read(reinterpret_cast<char*>(&detalle), sizeof(DetalleTransaccion))) {
        if (detalle.idProducto == idProducto) {
            archivo.close();
            return true; 
        }
    }
    archivo.close();
    return false;
}

bool verificarProductoEnTransaccion(const char* archivoDetalles, int idTrans, int idProd) {
    ifstream archivo(archivoDetalles, ios::binary);
    DetalleTransaccion trans;
    while(archivo.read(reinterpret_cast<char*>(&trans), sizeof(DetalleTransaccion))) {
        if(trans.idTransaccion == idTrans && trans.idProducto == idProd) {
            archivo.close();
            return true;
        }
    }
    archivo.close();
    return false;
}

bool idTieneTransacciones(const char* archivoTransacciones, int idRelacionado){
    ArchivoHeader header = leerHeader(archivoTransacciones);
    if (header.cantidadRegistros == 0){
        return false;
    }
    ifstream archivo(archivoTransacciones, ios::binary);
    if (!archivo.is_open()){
        cout << ROJO << "Error: No se pudo abrir el archivo." << endl<< RESET;
        return false;
    }
    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    Transaccion trans;
    
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.read(reinterpret_cast<char*>(&trans), sizeof(Transaccion));
        
        if (!trans.eliminado) {
            if (trans.idRelacionado == idRelacionado) {
                archivo.close();
                return true; // Si encuentra al menos una relacion
            }
        }
    }
    archivo.close();
    return false;
}

bool codigoDuplicado(const char* nombreArchivo, const char* codigo){
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()){
        cout << ROJO << "Error: No se pudo abrir el archivo " << nombreArchivo << endl<< RESET;
        return false;
    }
    ArchivoHeader header = leerHeader(nombreArchivo);
    archivo.seekg(sizeof(ArchivoHeader), ios::beg);

    Producto producto;

    // Recorrido del registro en busca del codigo
    for (int i = 0; i < header.cantidadRegistros; i++){
        archivo.read(reinterpret_cast<char*>(&producto), sizeof(Producto));
        if (!producto.eliminado && strcmp(producto.codigo, codigo) == 0){
            archivo.close();
            return true; // Si lo encuenta retorna True (Si existe el codigo)
        }
    }
    archivo.close();
    return false; // No esta duplicado
}


bool rifDuplicado(const char* nombreArchivo, const char* rif){
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()){
        cout << ROJO << "Error: No se pudo abrir el archivo " << nombreArchivo << endl<< RESET;
        return false;
    }
    ArchivoHeader header = leerHeader(nombreArchivo);
    archivo.seekg(sizeof(ArchivoHeader), ios::beg);

    if (header.cantidadRegistros == 0) {
        archivo.close();
        return false;
    }

    Proveedor proveedor;

    // Recorrido del registro en busca del rif
    for (int i = 0; i < header.cantidadRegistros; i++){
        archivo.read(reinterpret_cast<char*>(&proveedor), sizeof(Proveedor));
        if (!proveedor.eliminado && strcmp(proveedor.rif, rif) == 0){
            archivo.close();
            return true; // Si lo encuenta retorna True (Si existe el codigo)
        }
    }
    archivo.close();
    return false; // No esta duplicado
}

bool cedulaDuplicada(const char* nombreArchivo, const char* cedula){
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()){
        cout << ROJO << "Error: No se pudo abrir el archivo " << nombreArchivo << endl << RESET;
        return false;
    }
    ArchivoHeader header = leerHeader(nombreArchivo);
    archivo.seekg(sizeof(ArchivoHeader), ios::beg);

    if (header.cantidadRegistros == 0) {
        archivo.close();
        return false;
    }

    Cliente cliente;

    // Recorrido del registro en busca del rif
    for (int i = 0; i < header.cantidadRegistros; i++){
        archivo.read(reinterpret_cast<char*>(&cliente), sizeof(Cliente));
        if (!cliente.eliminado && strcmp(cliente.cedula, cedula) == 0){
            archivo.close();
            return true; // Si lo encuenta retorna True (Si existe el codigo)
        }
    }
    archivo.close();
    return false; // No esta duplicado
}


// BUSQUEDAS 

int* buscarProductoPorCodigo(const char* nombreArchivo, const char* consulta, int* numResultados){
    ArchivoHeader header = leerHeader(nombreArchivo);
    // Inicializacion del contador de resultados encontrados
    *numResultados = 0;

    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()){
        cout << ROJO << "Error abriendo archivo de busqueda." << endl << RESET;
        return nullptr;
    }

    Producto registro;
    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    // Recorrido para saber tamaño del arreglo de productos encontrados
    for (int i = 0; i < header.cantidadRegistros; i++){ // Verificar si hay coincidencias
        archivo.read(reinterpret_cast<char*>(&registro), sizeof(Producto));

        // Logica de comparacion
        if(!registro.eliminado){
                if(contieneSubstring(registro.codigo, consulta)){
                    (*numResultados)++;
                }
        }
    }
    if (*numResultados == 0){ // Si no hay coincidencias retorna nullptr
        archivo.close();
        return nullptr;
    }
    // Reservar memoria para arreglo de productos encontrados
    int* indices = new int[*numResultados];
    int posicion = 0;

    archivo.clear();
    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    // Llenar arreglo con indices reales de los productos encontrados
    for (int i = 0; i < header.cantidadRegistros; i++){ // Verificar si hay coincidencias
        archivo.read(reinterpret_cast<char*>(&registro), sizeof(Producto));

        if(!registro.eliminado){
            if(contieneSubstring(registro.codigo, consulta)){
                indices[posicion]= i;
                posicion++;
            }
        }
    }
    archivo.close();
    return indices; // Retorna el puntero al arreglo de indices
}

int buscarProveedorPorRif(const char* nombreArchivo, const char* rif){
    ArchivoHeader header = leerHeader(nombreArchivo);
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()){
        cout << ROJO << "Error abriendo archivo de busqueda." << endl << RESET;
        return -1;
    }
    
    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    Proveedor temp;

    // Recorrido del arreglo en busca del cliente
    for (int i = 0; i < header.cantidadRegistros; i++){
        archivo.read(reinterpret_cast<char*>(&temp), sizeof(Proveedor));
        if (!temp.eliminado && strcmp(temp.rif, rif) == 0) {
            archivo.close();
            return i; // Si lo encuentra retorna el indice
        }
    }
    archivo.close();
    return -1; // Si llega hasta aqui, No existe
}

int buscarClientePorCedula(const char* nombreArchivo, const char* cedula){
    ArchivoHeader header = leerHeader(nombreArchivo);
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()){
        cout << ROJO << "Error abriendo archivo de busqueda." << endl << RESET;
        return -1;
    }
    
    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    Cliente temp;

    // Recorrido del arreglo en busca del cliente
    for (int i = 0; i < header.cantidadRegistros; i++){
        archivo.read(reinterpret_cast<char*>(&temp), sizeof(Cliente));
        if (!temp.eliminado && strcmp(temp.cedula, cedula) == 0) {
            archivo.close();
            return i; // Si lo encuentra retorna el indice
        }
    }
    archivo.close();
    return -1; // Si llega hasta aqui, No existe
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

string truncarTexto(string texto, int limite) {
    if (texto.length() > limite) {
        // Corta el texto y deja espacio para los 3 puntos
        return texto.substr(0, limite - 3) + "...";
    }
    return texto;
}

void mostrarLineaDetalle(const Transaccion& t, const char* nombreProd, int idProducto, int cantidad, float total) {
    cout << CYAN << left << setw(10) << t.id 
         << setw(12) << t.fechaRegistro
         << setw(6)  << t.tipo           // COMPRA O VENTA
         << setw(10) << t.idRelacionado    // ID de Cliente o Proveedor
         << setw(20) << (strlen(nombreProd) > 19 ? string(nombreProd).substr(0, 17) + ".." : nombreProd) 
         << setw(8)  << cantidad
         << "$" << fixed << setprecision(2) << setw(11) << total
         << setw(15) << t.fechaUltimaModificacion << endl << RESET; 
}

void mostrarDetalleGeneralTransaccion(const Transaccion &trans){
    
    string descCorta = truncarTexto(trans.descripcion, 30);
    
    cout << CYAN << left 
         << setw(6)  << trans.id 
         << setw(12) << trans.fechaRegistro
         << setw(10) << (strcmp(trans.tipo, "VENTA") == 0 || strcmp(trans.tipo, "VENTA") == 0 ? "VENTA" : "COMPRA")
         << setw(10) << trans.idRelacionado 
         << setw(35) << descCorta
         << "$" << fixed << setprecision(2) << setw(11) << trans.total 
         << setw(15) << trans.fechaUltimaModificacion << endl << RESET;
}

void mostrarDetalleProducto(const Producto& producto, const char* archivoProveedores) {
    
    Proveedor prov = obtenerRegistroPorId<Proveedor>(archivoProveedores, producto.idProveedor);
    
    string descCorta = truncarTexto(producto.descripcion, 30);
    string provCorto = truncarTexto(prov.nombre, 20);

    cout << CYAN << left 
         << setw(8) << producto.id
         << setw(15) << producto.codigo
         << setw(20) << producto.nombre
         << setw(30) << descCorta
         << setw(8) << producto.stock;
         if (producto.stock <= producto.stockMinimo) {
            cout << ROJO << setw(15) << " [¡CRITICO!]"; 
        } else {
            cout << setw(15) << " ";
        } cout
         << setw(12) << fixed << setprecision(2) << producto.precio
         << setw(10) << producto.idProveedor
         << setw(20) << provCorto
         << setw(15) << producto.fechaRegistro 
         << setw(15) << producto.fechaUltimaModificacion << endl << RESET;
         
}

void mostrarDetalleProveedor(const Proveedor& proveedor) {
   
    string nomCorto = truncarTexto(proveedor.nombre, 20);
    string dirCorto = truncarTexto(proveedor.direccion, 20);

    cout << CYAN << left 
         << setw(8)  << proveedor.id
         << setw(15) << proveedor.rif
         << setw(20) << nomCorto
         << setw(20) << proveedor.telefono
         << setw(25) << fixed << setprecision(2) << dirCorto
         << setw(30) << proveedor.email
         << setw(15) << proveedor.fechaRegistro 
         << setw(15) << proveedor.fechaUltimaModificacion << endl << RESET;
}

void mostrarDetalleCliente(const Cliente& cliente) {

    string dirCorto = truncarTexto(cliente.direccion, 20);

    cout << CYAN << left 
         << setw(8)  << cliente.id
         << setw(15) << cliente.cedula
         << setw(20) << cliente.nombre
         << setw(20) << cliente.telefono
         << setw(25) << fixed << setprecision(2) << dirCorto
         << setw(30) << cliente.email
         << setw(15) << cliente.fechaRegistro 
         << setw(15) << cliente.fechaUltimaModificacion << endl << RESET;
}

void encabezadoDetalleTransaccion() {
    cout << endl;
    imprimirSeparador();
    cout << AZUL << NEGRITA << left 
         << setw(10) << "ID TRANS" 
         << setw(12) << "F.REGISTRO"
         << setw(6)  << "TIPO" 
         << setw(10) << "ENTIDAD" 
         << setw(20) << "PRODUCTO"
         << setw(8)  << "CANT."
         << setw(12) << "TOTAL" 
         << setw(12) << "F.MODIF" << endl << RESET;
    imprimirSeparador();
}

void encabezadoTransacciones(){
    cout << endl;
    imprimirSeparador();
    cout << AZUL << NEGRITA << left 
         << setw(6)  << "ID" 
         << setw(12) << "FECHA" 
         << setw(10) << "TIPO" 
         << setw(10) << "ENTIDAD" 
         << setw(35) << "DESCRIPCION" 
         << setw(12) << "TOTAL" 
         << setw(15) << "ULT. MODIF." << endl << RESET;
         imprimirSeparador();
}

void encabezadoProveedorCliente(){
    cout << endl;
    imprimirSeparador();

    cout << AZUL << NEGRITA << left 
         << setw(8) << "ID" 
         << setw(15) << "C.I/RIF" 
         << setw(20) << "NOMBRE"
         << setw(20) << "TLF" 
         << setw(25) << "DIRECCION" 
         << setw(30) << "EMAIL"
         << setw(15) << "F.REGISTRO"
         << setw(15) << "ULT. MODIF." << endl << RESET;

    imprimirSeparador();
}


void encabezadoProductos() {
    cout << endl;
    imprimirSeparador();
    
    cout << AZUL << NEGRITA << left 
         << setw(8)  << "ID PROD" 
         << setw(15) << "CODIGO" 
         << setw(20) << "NOMBRE" 
         << setw(30) << "DESCRIPCION"
         << setw(8) << "STOCK"
         << setw(15) << "ESTADO" 
         << setw(12) << "PRECIO" 
         << setw(10) << "ID PROV." 
         << setw(20) << "PROVEEDOR"
         << setw(15) << "F.REGISTRO" 
         << setw(15) << "ULT. MODIF" << endl << RESET; 
         
    imprimirSeparador();
}

void imprimirSeparador(int ancho, char simbolo){
    cout << NEGRITA << setfill(simbolo) << setw(ancho) << "" << setfill(' ') << endl << RESET;
}

bool floatesPositivo(float valor){
    if (valor <= 0){
        cout << ROJO << "Error: El valor debe ser mayor a cero." << endl << RESET;
        return false;
    }
    return true;
}

bool intesPositivo(int valor){
    if (valor <= 0){
        cout << ROJO << "Error: El valor debe ser mayor a cero." << endl << RESET;
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
        cout << AMARILLO << "Operación cancelada." << endl << RESET;
        imprimirSeparador();
        return false; 
    }

    strncpy(destino, temp, largo);
    return true;
}

bool solicitarEntero(const char* prompt, int& valor) {
    string entrada;
    cout << prompt << " (o Ingresa 's' y Enter para salir): ";
    cin >> entrada;

    if (entrada == "s" || entrada == "S") {
        cout << AMARILLO << "Entrada cancelada." << endl << RESET;
        return false;
    }

    // Intentar convertir a numero
    try {
        valor = stoi(entrada);
        return true;
    } catch (...) {
        cout << ROJO << "Error: Debe ingresar un numero valido." << endl << RESET;
        return false; // 
    }
}

bool solicitarFloat(const char* prompt, float& valor) {
    string entrada;
    cout << prompt << " (o Ingresa 's' y Enter para salir): ";
    cin >> entrada;

    if (entrada == "s" || entrada == "S") return false;

    try {
        valor = stof(entrada);
        return true;
    } catch (...) {
        cout << ROJO << "Error: Numero decimal invalido." << endl << RESET;
        return false;
    }
}

int main(){

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    Tienda* Negocio = new Tienda;
    inicializarTienda(Negocio, "JSport Maracaibo", "J-12345678-9");

    int op;
    do {
        system("cls");
        imprimirSeparador(60,'=');
        cout << AZUL << NEGRITA << "             SISTEMA DE GESTIÓN DE INVENTARIO" << endl << RESET
         << AZUL << "               Tienda: " << Negocio->nombre << endl << RESET;
        imprimirSeparador(60,'=');
        
        cout << CYAN 
             << "1. Gestión de Productos" << endl
             << "2. Gestión de Proveedores" << endl
             << "3. Gestión de Clientes" << endl
             << "4. Gestión de Transacciones" << endl
             << "5. Gestión de Reportes y Seguridad" << endl
             << "6. Salir" << endl << RESET;
        while (!solicitarEntero("Seleccione una opción", op)){
            cout << endl;
        }
        imprimirSeparador(60, '=');
        if (!intesPositivo(op)){
            break;
        }

        switch (op)
            {
            case 1: { // Gestión de Productos
            
            int opProd;
            do {
                imprimirSeparador(60,'=');
                cout << AZUL << NEGRITA << "             SISTEMA DE GESTIÓN DE PRODUCTOS" << endl << RESET;
                imprimirSeparador(60,'=');
                cout << CYAN
                    << "1. Registrar nuevo Producto" << endl
                    << "2. Buscar Producto" << endl
                    << "3. Actualizar Producto" << endl
                    << "4. Actualizar Stock manualmente" << endl
                    << "5. Listar todos los Productos" << endl
                    << "6. Eliminar Producto" << endl
                    << "0. Volver al menú principal" << endl << RESET;
                if (!solicitarEntero("Seleccione una opción", opProd)){
                    break;
                }
                imprimirSeparador(60, '=');
                if (!intesPositivo(opProd)){
                    break;
                }
                
                switch (opProd) {
                    case 1:{ // Registrar nuevo productos
                        crearProducto(ARCHIVO_PRODUCTOS, ARCHIVO_PROVEEDORES);
                        break;
                    }
                    case 2:{ // Buscar producto
                        buscarProducto(ARCHIVO_PRODUCTOS, ARCHIVO_PROVEEDORES);
                        break;
                    }
                    case 3:{ // Actualizar Producto
                        actualizarProducto(ARCHIVO_PRODUCTOS, ARCHIVO_PROVEEDORES);
                        break;
                    }
                    case 4: { // Actualizar Stock manualmente
                        actualizarStockProducto(ARCHIVO_PRODUCTOS);
                        break;
                    }
                    case 5: { // Listar todos los productos
                        listarProductos(ARCHIVO_PRODUCTOS, ARCHIVO_PROVEEDORES);
                        break;
                    }
                    case 6: { // Eliminar prododucto
                        eliminarProducto(ARCHIVO_PRODUCTOS, ARCHIVO_PROVEEDORES, ARCHIVO_TRANSACCIONES, ARCHIVO_DETALLES);
                        break;
                    }
                    case 0: { // Volver al menu principal
                        cout << AMARILLO << "Volviendo al menú principal..." << endl << RESET;
                        break;
                    }
                    default:
                        cout << ROJO << "Opción Invalida." << endl << RESET;
                        break;
                }
            } while (opProd != 0);

            break;
            }
            case 2: { // Gestión de Proveedores
                
                int opProv;
                do {
                    imprimirSeparador(60,'=');
                    cout << AZUL << NEGRITA << "             SISTEMA DE GESTIÓN DE PROVEEDORES" << endl << RESET;
                    imprimirSeparador(60,'=');
                    cout << CYAN  
                        << "1. Registrar nuevo Proveedor" << endl
                        << "2. Buscar Proveedor" << endl
                        << "3. Actualizar Proveedor" << endl
                        << "4. Listar todos los Proveedores" << endl
                        << "5. Eliminar Proveedor" << endl
                        << "0. Volver al menú principal" << endl << RESET;
                    if (!solicitarEntero("Seleccione una opción", opProv)){
                        break;
                    }
                    imprimirSeparador(60, '=');
                    if (!intesPositivo(opProv)){
                        break;
                    }
                    
                    switch (opProv) {
                        case 1:{ // Registrar nuevo proveedor
                            crearProveedor(ARCHIVO_PROVEEDORES);
                            break;
                        }
                        case 2:{ // Buscar proveedor
                            buscarProveedor(ARCHIVO_PROVEEDORES);
                            break;
                        }
                        case 3:{ // Actualizar proveedor
                            actualizarProveedor(ARCHIVO_PROVEEDORES);
                            break;
                        }
                        case 4: { // Listar todos los proveedores
                            listarProveedores(ARCHIVO_PROVEEDORES);
                            break;
                        }
                        case 5: { // Eliminar proveedor
                            eliminarProveedor(ARCHIVO_PROVEEDORES, ARCHIVO_TRANSACCIONES);
                            break;
                        }
                        case 0: { // Volver al menu principal
                            cout << AMARILLO << "Volviendo al menú principal..." << RESET;
                            break;
                        }
                        default:
                            cout << ROJO << "Opción Invalida." << RESET;
                            break;
                    }
                } while (opProv != 0);

                break;
            }
            case 3:{ // Gestión de Clientes
                
                int opCliente;
                do {
                    imprimirSeparador(60,'=');
                    cout << AZUL << NEGRITA << "             SISTEMA DE GESTIÓN DE CLIENTES" << endl << RESET;
                    imprimirSeparador(60,'=');  
                    cout << CYAN 
                        << "1. Registrar nuevo Cliente" << endl
                        << "2. Buscar Cliente" << endl
                        << "3. Actualizar Cliente" << endl
                        << "4. Listar todos los Cliente" << endl
                        << "5. Eliminar Cliente" << endl
                        << "0. Volver al menú principal" << endl << RESET;
                    if (!solicitarEntero("Seleccione una opción", opCliente)){
                        break;
                    }
                    imprimirSeparador(60, '=');
                    if (!intesPositivo(opCliente)){
                        break;;
                    }
                    cin.ignore();

                    switch (opCliente) {
                        case 1:{ // Registrar nuevo Cliente
                            crearCliente(ARCHIVO_CLIENTES);
                            break;
                        }
                        case 2:{ // Buscar Cliente
                            buscarCliente(ARCHIVO_CLIENTES);
                            break;
                        }
                        case 3:{ // Actualizar Cliente
                            actualizarCliente(ARCHIVO_CLIENTES);
                            break;
                        }
                        case 4: { // Listar todos los Cliente
                            listarClientes(ARCHIVO_CLIENTES);
                            break;
                        }
                        case 5: { // Eliminar Cliente
                            eliminarCliente(ARCHIVO_CLIENTES, ARCHIVO_TRANSACCIONES);
                            break;
                        }
                        case 0: { // Volver al menu principal
                            cout << AMARILLO << "Volviendo al menú principal..." << endl << RESET;
                            break;
                        }
                        default:
                            cout << ROJO << "Opción Invalida." << endl << RESET;
                            break;
                    }
                } while (opCliente != 0);

                break;
            }
            case 4:{ // Gestión de Transacciones
                
                int opTrans;
                do {
                    imprimirSeparador(60,'=');
                    cout << AZUL << NEGRITA << "             SISTEMA DE GESTIÓN DE TRANSACCIONES" << endl << RESET;
                    imprimirSeparador(60,'=');
                    cout << CYAN
                        << "1. Registrar Compra a Proveedor" << endl
                        << "2. Registrar Venta a Cliente" << endl
                        << "3. Buscar Transacciones" << endl
                        << "4. Listar Transacciones" << endl
                        << "5. Cancelar Transacción" << endl
                        << "0. Volver al menú principal" << endl << RESET;
                    if (!solicitarEntero("Seleccione una opción", opTrans)){
                        break;
                    }
                    imprimirSeparador(60, '=');
                    if (!intesPositivo(opTrans)){
                        break;
                    }
                    
                    switch (opTrans)
                    {
                    case 1: { // Registrar compra a proveedor
                        registrarCompra(ARCHIVO_TRANSACCIONES, ARCHIVO_DETALLES, ARCHIVO_PRODUCTOS, ARCHIVO_PROVEEDORES);      
                        break;
                    }
                    case 2: { // Registrar venta a cliente
                        registrarVenta(ARCHIVO_TRANSACCIONES, ARCHIVO_DETALLES, ARCHIVO_PRODUCTOS, ARCHIVO_PROVEEDORES, ARCHIVO_CLIENTES);
                        break;
                    }
                    case 3: { // Buscar transacciones
                        buscarTransacciones(ARCHIVO_TRANSACCIONES, ARCHIVO_DETALLES, ARCHIVO_PRODUCTOS, ARCHIVO_CLIENTES, ARCHIVO_PROVEEDORES);
                        break;    
                    }
                    case 4: { // Listar transacciones
                        listarTransacciones(ARCHIVO_TRANSACCIONES);
                        break;
                    }
                    case 5: { // Cancelar transaccion
                        cancelarTransaccion(ARCHIVO_TRANSACCIONES, ARCHIVO_DETALLES, ARCHIVO_PRODUCTOS, ARCHIVO_PROVEEDORES, ARCHIVO_CLIENTES);
                        break;
                    }
                    case 0: { // Volver al menu principal
                        cout << AMARILLO << "Volviendo al menú principal..." << endl << RESET;
                        break;
                    }
                    default:
                        cout << ROJO << "Opción Invalida." << endl << RESET;
                        break;
                    }
                } while (opTrans != 0);
                break;
            }
            case 5: { 
                int opRep;
                do {
                    imprimirSeparador(60, '=');
                    cout << AZUL << NEGRITA << "             SISTEMA DE GESTIÓN DE REPORTES Y SEGURIDAD" << endl << RESET;
                    imprimirSeparador(60, '=');
                    cout << CYAN
                     << "1. Diagnóstico de Intergridad y Referencial" << endl
                     << "2. Reporte de Stock Minimo" << endl
                     << "3. Crear respaldo (BACKUP) de los archivos de inventario" << endl
                     << "4. Estadisticas de ventas totales por Cliente" << endl 
                     << "0. Volver al menú principal"<< endl << RESET;
                    if (!solicitarEntero("Seleccione una opción", opRep)){
                    break;
                    }
                    imprimirSeparador(60, '=');
                    if (!intesPositivo(opRep)){
                        break;
                    }

                    switch (opRep)
                    {
                    case 1:{ // Diagnóstico de Intergridad y Referencial
                        verificarIntegridadReferencial(ARCHIVO_PRODUCTOS, ARCHIVO_TRANSACCIONES, ARCHIVO_DETALLES, ARCHIVO_PROVEEDORES, ARCHIVO_CLIENTES);
                        break;
                    }
                    case 2:{ // Reporte de Stock Minimo
                        reporteStockCritico(ARCHIVO_PRODUCTOS);
                        break;
                    }
                    case 3:{ // Crear respaldo (BACKUP)
                        crearBackup(ARCHIVO_PRODUCTOS, ARCHIVO_PROVEEDORES, ARCHIVO_TRANSACCIONES, ARCHIVO_DETALLES, ARCHIVO_CLIENTES);
                        break;
                    }
                    case 4:{ // Ventas totales por Cliente
                        historialCliente(ARCHIVO_CLIENTES, ARCHIVO_TRANSACCIONES, ARCHIVO_DETALLES, ARCHIVO_PRODUCTOS);                      
                        break;
                    }
                    case 0:{ // Volver al menu principal
                        cout << AMARILLO << "Volviendo al menú principal..." << endl << RESET;
                        break;
                    }
                    default:
                        cout << ROJO << "Opción Invalida." << endl << RESET;
                        break;
                    }

                } while (opRep != 0);
                break;
            }
            case 6:{ // Salir
                cout << AMARILLO << "Saliendo del programa..." << endl << RESET;
                break;
            }
            default:
                cout << ROJO << "Opción Invalida." << endl << RESET;
                break;
            }
    } while (op != 6);

    delete Negocio;
    Negocio = nullptr;

    return 0;
}