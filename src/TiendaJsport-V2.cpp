#include <iostream>
#include <iomanip>
#include <cstring>
#include <ctime>
#include <limits>
#include <fstream>

using namespace std;

struct Producto;
struct Proveedor;
struct Cliente;
struct DetalleVenta;
struct Transaccion;
struct Tienda;
struct ArchivoHeader;


// PROTOTIPO DE FUNCIONES

// Inicialización y Liberación
void inicializarTienda(Tienda* tienda, const char* nombre, const char* rif);
void liberarTienda(Tienda* tienda);
bool inicializarArchivo(const char* nombreArchivo);
bool actualizarHeader(const char* nombreArchivo, ArchivoHeader header);

// FUNCIONES AUXILIARES
int calcularPosicionRegistro(int indice);
bool guardarNuevoRegistro(const char* nombreArchivo, Registro& nuevoRegistro);

// VALIDACIONES
bool validarEmail(const char* email);
bool validarFecha(const char* fecha);
bool existeEntidad(const char* nombreArchivo, int id);
bool codigoDuplicado(const char* nombreArchivo, const char* codigo);
bool rifDuplicado(Tienda* tienda, const char* rif);

// BUSQUEDAS
int buscarProveedorPorRif(Tienda* tienda, const char* rif);
int buscarPorId(const char* nombreArchivo, int id);
int* buscarProductosPorNombre(Tienda* tienda, const char* nombre, int* numResultados);
int* buscarProductosPorCodigo(Tienda* tienda, const char* codigo, int* numResultados);
int* buscarProveedorPorNombre(Tienda* tienda, const char* nombre, int* numResultados);
int* buscarClientePorNombre(Tienda* tienda, const char* nombre, int* numResultados);
int buscarClientePorCedula(Tienda* tienda, const char* cedula);

// UTILIDADES
void obtenerFechaActual(char* buffer);
void convertirAMinusculas(char* cadena);
bool contieneSubstring(const char* texto, const char* busqueda);
void mostrarDetalleTransaccion(const Transaccion& transaccion, Producto& producto);
void mostrarDetalleProducto(const Producto& producto, const char* archivoProveedores);
void mostrarDetalleProveedor(const Proveedor& proveedor);
void mostrarDetalleCliente(const Cliente& cliente);
void imprimirSeparador(int ancho = 130, char simbolo = '=');
void encabezadoTransacciones();
void encabezadoProductos();
void encabezadoProveedorCliente();
bool floatesPositivo(float valor);
bool IntesPositivo(int valor);
bool solicitarTexto(const char* prompt, char* destino, int largo);
bool solicitarFloat(const char* prompt, float& valor);
bool solicitarEntero(const char* prompt, int& valor);

// FUNCIONES CRUD
void crearProducto(const char* archivoProductos, const char* archivoProveedores);
void buscarProducto(Tienda* tienda);
void actualizarProducto(Tienda* tienda);
void actualizarStockProducto(Tienda* tienda);
void listarProductos(const char* archivoProductos);



void crearProveedor(Tienda* tienda);
void buscarProveedor(Tienda* tienda);
void actualizarProveedor(Tienda* tienda);
void listarProveedores(Tienda* tienda);

void crearCliente(Tienda* tienda);
void buscarCliente(Tienda* tienda);
void actualizarCliente(Tienda* tienda);
void listarClientes(Tienda* tienda);


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
    // Estadisticas del registro
    int stockMinimo;                 
    int totalVendido;                
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
	int productosIDs[100];     // IDs de los productos bajo el proveedor
    
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
    
    bool eliminado; // Para BORRADO LOGICO
    char fechaUltimaModificacion[11];
};

struct DetalleVenta {
    int idProducto;
    int cantidad;
    float precioUnitario; 
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
    char descripcion[200];     // Notas adicionales (opcional)
    char fechaRegistro[11];
    DetalleVenta detalles[50];  // Arreglo de productos en la transaccion
    int cantidadItemsDiferentes;

    bool eliminado;      // Para BORRADO LOGICO
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
    int cantidadRegistros;      // Total hist�rico de registros
    int proximoID;              // Siguiente ID a asignar (Autoincremental)
    int registrosActivos;       // Registros que no est�n marcados como eliminados
    int version;                // Control de versi�n del archivo
};

// Funcion de Inicialización
void inicializarTienda(Tienda* tienda, const char* nombre, const char* rif){
    
    // Informacion de la tienda 
    strncpy(tienda->nombre, nombre, 99);
    tienda->nombre[99] = '\0';

    strncpy(tienda->rif, rif, 19);
    tienda->rif[19] = '\0';

    // Inicializar IDs Autoincrementales
    tienda->siguienteIdTransaccion = 1;
    tienda->siguienteIdCliente = 1;
    tienda->siguienteIdProducto = 1;
    tienda->siguienteIdProveedor = 1;

}

// Funcion de Liberación
void liberarTienda(Tienda* tienda){
    // Reiniciar IDs Autoincrementales
    tienda->siguienteIdTransaccion = 0;
    tienda->siguienteIdCliente = 0;
    tienda->siguienteIdProducto = 0;
    tienda->siguienteIdProveedor = 0;

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
        cout << "Error: No se pudo crear el archivo " << nombreArchivo << endl;
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
        cout << "Error: No se pudo abrir el archivo " << nombreArchivo << endl;
        return false;
    }

    // Nos posicionamos al byte 0 (principio del archivo)
    archivo.seekp(0, ios::beg);
    // Sobrescribimoscon los nuevos datos del header
    archivo.write(reinterpret_cast<const char*>(&header), sizeof(ArchivoHeader));
    
    bool resultado = archivo.good();
    archivo.close();
    return resultado;
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
        archivo.seekg(posicion ios::beg);
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
        cout << "Error: No se pudo abrir el archivo " << nombreArchivo << endl;
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

// 4. GESTI�N DE RELACIONES Y TRANSACCIONES

// 4.1 Registrar una Compra/Venta
//El proceso de registrar una transacci�n ahora involucra m�ltiples accesos a disco para mantener la coherencia.

//**Pasos algor�tmicos obligatorios para una Venta:**

//1. Buscar y leer el Producto y el Cliente desde sus respectivos archivos.
//2. Validar reglas de negocio (ej. �Hay stock suficiente? �El cliente existe?).
//3. Si es v�lido, instanciar un registro Transaccion.
//4. Guardar la Transaccion al final de `transacciones.bin`.
//5. Modificar el Producto en memoria (restar stock, sumar estad�sticas, a�adir el ID de la transacci�n a su arreglo de historial) y actualizar su registro en el archivo binario.
//6. Modificar el Cliente en memoria (sumar gastos, a�adir ID de transacci�n) y actualizar su registro en el archivo binario.

// 5. MANTENIMIENTO E INTEGRIDAD (NUEVAS FUNCIONALIDADES)

//5.1 Integridad Referencial
//Debes programar un m�dulo de diagn�stico `verificarIntegridadReferencial()` que detecte "referencias rotas".

//�Qu� debe hacer el algoritmo?

//- Recorrer `productos.bin`. Por cada producto activo, extraer su `idProveedor`.
//- Buscar en `proveedores.bin` si ese ID existe y no est� eliminado. Si no existe, registrar el error.
//- Repetir la l�gica cruzada para Transacciones (verificar que el `idProducto` y el `idCliente/Proveedor` a�n existan).
//- Imprimir un reporte de salud de la base de datos al finalizar.

//5.2 Respaldo de Datos (Backup)
//Implementar una funci�n `crearBackup()`:

//- Debe crear una carpeta (o usar un prefijo en el nombre) con la fecha y hora actual.
//- Debe copiar byte a byte los 5 archivos .bin operativos a este nuevo destino seguro.

//5.3 Reportes Anal�ticos (Lectura en Lote)
//Se requiere un submen� para leer datos y generar estad�sticas:

//- **Productos con stock cr�tico:** Recorrer productos y filtrar los que tengan `stock <= stockMinimo`.
//- **Historial de Cliente:** Dado un ID de cliente, imprimir sus datos b�sicos, buscar todas las transacciones asociadas a su arreglo `comprasIDs[]` e imprimir el detalle recuperando el nombre del producto involucrado.

// 6. EXPERIENCIA DE USUARIO

// 6.1. IMPRESI�N DE DATOS
//- Todos los datos que se muestren en el sistema deben estar correctamente formateados usando tablas, colores, caracteres, etc


// FUNCIONES CRUD - PRODUCTOS

// Crear Productos
void crearProducto(const char* archivoProductos, const char* archivoProveedores) {
    
    char decision;
    cout << "¿Desea registrar un nuevo producto? (S/N): ";
    cin >> decision;

    if (decision == 's' || decision == 'S') {
        ArchivoHeader header = leerHeader(archivoProveedores);
        if (header.registrosActivos == 0){
            cout << "Error: para registrar un producto, debe registrar primero el proveedor." << endl;
            return;
        }

        Producto nuevoProducto;
        
        // Validaciones de entrada
        if (!solicitarTexto("Ingrese nombre del Producto", nuevoProducto.nombre, 100)) return;
        if (!solicitarTexto("Ingrese codigo del producto", nuevoProducto.codigo, 20)) return;
        
        if (codigoDuplicado(archivoProductos, nuevoProducto.codigo)) {
            cout << "Error: El codigo ya existe." << endl;
            return;
        }

        // Solicitud de datos para el nuevo producto (Validacion de datos)
        if (!solicitarTexto("Ingrese descripcion del producto", nuevoProducto.descripcion, 200)) return;
        if (!solicitarFloat("Ingrese precio del producto", nuevoProducto.precio) || !floatesPositivo(nuevoProducto.precio)) return;
        if (!solicitarEntero("Ingrese Stock del producto", nuevoProducto.stock) || nuevoProducto.stock < 0) return;
        if (!solicitarEntero("Ingrese ID del proveedor", nuevoProducto.idProveedor) || !IntesPositivo(nuevoProducto.idProveedor)) return;
        if (!existeEntidad<Proveedor>(archivoProveedores, nuevoProducto.idProveedor)) {
            cout << endl << "Error: El proveedor no existe. Debe crearlo primero" << endl;
            return;
        }

        // Resumen de nuevos datos
        imprimirSeparador(60, '=');
        cout << "      RESUMEN DE PRODUCTO" << endl;
        imprimirSeparador(60, '=');
        cout << "Codigo: " << nuevoProducto.codigo << endl;
        cout << "Nombre: " << nuevoProducto.nombre << endl;
        cout << "Descripcion: " << nuevoProducto.descripcion << endl;
        cout << "Stock: " << nuevoProducto.stock << endl;
        cout << "Precio: " << nuevoProducto.precio << endl;
        imprimirSeparador(60, '=');
        
        cout << "¿Desea Guardar producto? (S/N): ";
        cin >> decision;

        if (decision == 's' || decision == 'S') {
            if (guardarNuevoRegistro<Producto>(archivoProductos, nuevoProducto)){
                cout << endl << "¡Producto guardado! ID asignado: " << nuevoProducto.id << endl;            
            }
            encabezadoProductos();
            mostrarDetalleProducto(nuevoProducto, archivoProveedores);
        } else {
            cout << endl << "Operacion cancelada. El Producto no fue guardado" << endl;
        }
    } else {
        cout << endl << "Volviendo al menu." << endl;
    }
}

// Buscar Producto
void buscarProducto(Tienda* tienda) {

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
                        imprimirSeparador();
                    } else {
                        cout << endl << "Error: El producto con ID " << id << " no existe." << endl;
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
                        imprimirSeparador();
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
                        imprimirSeparador();

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
                    imprimirSeparador();
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
        cout << "Error al crear tienda" << endl;
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
        cout << endl << "Error: El producto con ID " << id << " no existe." << endl;
        return;
    }

    cout << endl << "DATOS ACTUALES DEL PRODUCTO:" << endl;
    encabezadoProductos();
    mostrarDetalleProducto(tienda, i);
    imprimirSeparador();

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
                        cout << endl << "Error: El proveedor no existe." << endl;
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
        cout << "Error al crear tienda" << endl;
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
        cout << endl << "Error: El producto no existe." << endl;
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
void listarProductos(const char* archivoProductos, const char* archivoProveedores) {
    
    ArchivoHeader header = leerHeader(archivoProductos);
    if (header.registrosActivos ==0){
        cout << endl << "No hay productos registrados en el sistema" << endl;
        return;
    }
    
    ifstream archivo(archivoProductos, ios::binary);
    if (!archivo.is_open()){
        cout << "Error: No se puso abrir el archivo " << archivoProductos << endl;
        return;    
    }
    archivo.seekg(sizeof(ArchivoHeader), ios::beg);

    cout << endl << "          LISTADO GENERAL DE PRODUCTOS" << endl;
    encabezadoProductos();

    Producto producto;

    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.read(reinterpret_cast<char*>(&producto), sizeof(Producto));
        if (!producto.eliminado){    
            mostrarDetalleProducto(producto, archivoProveedores);
        }
    }
    archivo.close();
    imprimirSeparador();
}

// Eliminar Producto
void eliminarProducto(const char* archivoProductos, const char* archivoProveedores){

    ArchivoHeader header = leerHeader(archivoProductos);
    if (header.registrosActivos ==0){
        cout << endl << "No hay productos registrados en el sistema" << endl;
        return;
    }

    int id, ind;
    cout << "Por favor ingrese la ID del producto que desea eliminar: ";
    cin >> id;

    // Buscamos el índice
    ind = buscarPorId(archivoProductos, id);
    // Validamos que exista
    if (ind == -1) {
        cout << endl << "Error: El producto con ID " << id << " no existe." << endl;
        return;
    }
    Producto producto = obtenerRegistroPorId<Producto>(archivoProductos, id);

    char decision;
    encabezadoProductos();
    mostrarDetalleProducto(producto, archivoProveedores);
    
    if(productoTieneTransacciones){
        cout << "Este producto tiene transacciones registradas." << endl;
        cout << "Eliminarlo podria afectar la integridad de los reportes historicos." << endl;
    }

    cout << "¿Seguro que desea eliminar el producto? (S/N): ";
    cin >> decision;

    if (decision == 's' || decision == 'S') { // Reajuste de posicion de productos
        eliminarRegistroLogico<Producto>(archivoProductos, id);    
        cout << "Producto eliminado exitosamente." << endl;
    } else {
        cout << "Operacion cancelada." << endl;
    }
}

// FUNCIONES CRUD - PROVEEDORES

// Crear Proveedores
void crearProveedor(Tienda* tienda){

	if (tienda == nullptr){ // Validacion de seguridad
        cout << "Error al crear tienda" << endl;
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
        if (!solicitarTexto("Ingrese el email del Proveedor", email, 100)) return;
        
        if (!validarEmail(email)) {
            cout << "Error: El correo debe incluir un '@' y al menos un '.'" << endl;
            return;
        }


        // Resumen de nuevos datos
        imprimirSeparador(60, '=');
        cout << "      RESUMEN DE PROVEEDOR" << endl;
        imprimirSeparador(60, '=');
        cout << "Nombre: " << nombre << endl;
        cout << "Telefono: " << telefono << endl;
        cout << "Email: " << email << endl;
        cout << "Rif: " << rif << endl;
        cout << "Dirección: " << direccion << endl;
        imprimirSeparador(60, '=');

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
        cout << "Error al crear tienda" << endl;
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
                        encabezadoProveedorCliente();
                        mostrarDetalleProveedor(tienda, i);
                        imprimirSeparador();
                    } else {
                        cout << endl << "Error: El proveedor con ID " << id << " no existe." << endl;
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
                        
                        encabezadoProveedorCliente();
                        for (int i = 0; i < numEncontrados; i++) {
                            int indiceReal = indices[i];
                            mostrarDetalleProveedor(tienda, indiceReal);
                        }
                        imprimirSeparador();
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
                        encabezadoProveedorCliente();
                        mostrarDetalleProveedor(tienda, i);
                        imprimirSeparador();
                    } else {
                        cout << endl << "Error: El proveedor con rif " << rifBusqueda << " no existe." << endl;
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
        cout << "Error al crear tienda" << endl;
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
        cout << endl << "Error: El proveedor con ID " << id << " no existe." << endl;
        return;
    }

    cout << endl << "DATOS ACTUALES DEL PROVEEDOR:" << endl;
    encabezadoProveedorCliente();
    mostrarDetalleProveedor(tienda, i);
    imprimirSeparador();

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
        cout << "Error al crear tienda" << endl;
        return;
    }
    if (tienda->numProveedores== 0){
        cout << endl << "No hay Proveedores registrados en el sistema." << endl;
        return;
    }

    cout << endl << "          LISTADO GENERAL DE PROVEEDOR" << endl;
    encabezadoProveedorCliente();
    for (int i = 0; i < tienda->numProveedores; i++) {
        mostrarDetalleProveedor(tienda, i);
    }
    imprimirSeparador();

}

// Eliminar Proveedor
void eliminarProveedor(Tienda* tienda){

	if (tienda == nullptr){ // Validacion de seguridad
        cout << "Error al crear tienda" << endl;
        return;
    }
    if (tienda->numProveedores == 0){
        cout << endl << "No hay Proveedores registrados en el sistema." << endl;
        return;
    }

    int id, ind;
    cout << "Por favor ingrese la ID del proveedor que se va a eliminar: ";
    cin >> id;

    // Buscamos el índice
    ind = buscarProveedorPorId(tienda, id);

    // Validamos que exista
    if (ind == -1) {
        cout << endl << "Error: El proveedor con ID " << id << " no existe." << endl;
        return;
    }

    for (int i = 0; i < tienda->numProductos; i++)
    {
        if (tienda->productos[i].idProveedor == id){
            cout << "Error: El proveedor no puede ser eliminado" << endl
                 << "El proveedor esta asociado a productos en el sistema." << endl;
                return;
        }
    }

    char decision;
    cout << "¿Seguro que desea eliminar '" << tienda->proveedores[ind].nombre << "'? (S/N): ";
    cin >> decision;

    if (decision == 's' || decision == 'S') { // Reajuste de posicion de productos
        for (int j = ind; j < tienda->numProveedores - 1; ++j) {
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
        cout << "Error al crear tienda" << endl;
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
        imprimirSeparador(60, '=');
        cout << "      RESUMEN DE CLIENTE" << endl;
        imprimirSeparador(60, '=');
        cout << "Nombre: " << nombre << endl;
        cout << "Telefono: " << telefono << endl;
        cout << "Email: " << email << endl;
        cout << "Cedula:" << cedula << endl;
        cout << "Dirección:" << direccion << endl;
        imprimirSeparador(60, '=');

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
        cout << "Error al crear tienda" << endl;
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
                        encabezadoProveedorCliente();
                        mostrarDetalleCliente(tienda, i);
                        imprimirSeparador();
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
                        
                        encabezadoProveedorCliente();
                        for (int i = 0; i < numEncontrados; i++) {
                            int indiceReal = indices[i];
                            mostrarDetalleCliente(tienda, indiceReal);
                        }
                        imprimirSeparador();
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
                        cout << endl << "Cliente encontrado:" << endl;
                        encabezadoProveedorCliente();
                        mostrarDetalleCliente(tienda, i);
                        imprimirSeparador();
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
        cout << "Error al crear tienda" << endl;
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
    encabezadoProveedorCliente();
    mostrarDetalleCliente(tienda, i);
    imprimirSeparador();

    char nombre[100], cedula[20], telefono[20], direccion[200], email[100];
    int seleccion; 

    bool VerNombre = false, VerTlf = false, VerEmail = false;
    bool VerCedula = false, VerDir = false;

    // Menú de campos editables
    do {
        cout << endl << "¿Qué desea editar?" << endl;
        cout << "1. Nombre" << endl;
        cout << "2. Cedula " << endl;
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
        cout << "Error al crear tienda" << endl;
        return;
    }
    if (tienda->numClientes == 0){
        cout << endl << "No hay Clientes registrados en el sistema." << endl;
        return;
    }

    cout << endl << "          LISTADO GENERAL DE CLIENTES" << endl;
    encabezadoProveedorCliente();
    for (int i = 0; i < tienda->numClientes; i++) {
        mostrarDetalleCliente(tienda, i);
    }
    imprimirSeparador();

}

// Eliminar Cliente
void eliminarCliente(Tienda* tienda){

	if (tienda == nullptr){ // Validacion de seguridad
        cout << "Error al crear tienda" << endl;
        return;
    }
    if (tienda->numClientes == 0){
        cout << endl << "No hay Clientes registrados en el sistema." << endl;
        return;
    }

    int id, i;
    cout << "Por favor ingrese el ID del cliente que se va a eliminar: ";
    cin >> id;

    // Buscamos el índice
    i = buscarClientePorId(tienda, id);

    // Validamos que exista
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
        cout << "Error al crear tienda" << endl;
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
        cout << endl << "Error: El producto no existe.";
        return;
    }
    
    if (!solicitarEntero("Ingrese ID del proveedor", ProveedorID)){
        return;
    }
    if (existeProveedor(tienda, ProveedorID) == false){
        cout << endl << "Error: El proveedor no existe." << endl;
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
    imprimirSeparador(60, '=');
    cout << "      RESUMEN DE COMPRA" << endl;
    imprimirSeparador(60, '-');
    cout << "Cantidad: " << tienda->transacciones[i].cantidad << endl;
    cout << "Producto: " << tienda->productos[IndiceProducto].nombre << endl;
    cout << "Total a pagar: " << tienda->transacciones[i].total << endl;
    imprimirSeparador(60, '=');

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
    cout << tienda->productos[IndiceProducto].nombre << ": " << tienda->productos[IndiceProducto].stock << endl;
    } else {
        cout << endl << "Compra cancelada.";
    }   
}

// Registrar Venta (a Cliente)
void registrarVenta(Tienda* tienda){
    if (tienda == nullptr){ // Validacion de seguridad
        cout << "Error al crear tienda" << endl;
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
        cout << endl << "Error: El producto no existe.";
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
    imprimirSeparador(60, '=');
    cout << "      RESUMEN DE VENTA" << endl;
    imprimirSeparador(60, '-');
    cout << "Cantidad: " << tienda->transacciones[i].cantidad << endl;
    cout << "Producto: " << tienda->productos[IndiceProducto].nombre << endl;
    cout << "Total a pagar: " << tienda->transacciones[i].total << endl;
    imprimirSeparador(60, '=');

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
    cout << tienda->productos[IndiceProducto].nombre << ": " << tienda->productos[IndiceProducto].stock << endl;
    } else{
        cout << endl << "Venta cancelada.";
    }   
}

// Buscar Transacciones
void buscarTransacciones(Tienda* tienda){
    if (tienda == nullptr){ // Validacion de seguridad
        cout << "Error al crear tienda" << endl;
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
		
	        encabezadoTransacciones();
	        mostrarDetalleTransaccion(tienda, i);
	        imprimirSeparador();
	        break;
    	}
    	
    case 2: {// Busqueda por ID del Producto
	        int prodID;
	
	        if (!solicitarEntero("Ingrese ID del producto", prodID)){
	        return;
	        }
	        if (existeProducto(tienda, prodID) == false){ // Validacion de existencia
	            cout << endl << "Error: El producto no existe.";
	            break;
	        }
	        encontro = false;
	        encabezadoTransacciones();
	        
	        for (int i = 0; i < tienda->numTransacciones; i++){
	            if (tienda->transacciones[i].idProducto == prodID){ // Si encuentra el ID del producto lo muestra
	            mostrarDetalleTransaccion(tienda, i);
	            encontro = true; // Actualización de la variable booleana
	            }
	        }
	        imprimirSeparador();
	        if (encontro == false){ // No encontro el ID en las transacciones
	            cout << "No hay transacciones para este producto." << endl;
	        }
	        break;
		}
    case 3: {
	
        int clienteID;

        if (!solicitarEntero("Ingrese ID del cliente", clienteID)){
            break;
        }
        if (existeCliente(tienda, clienteID) == false){ // Validación de existencia del cliente
            cout << "Error: El cliente no existe.";
            break; 
        }
        encontro = false;
        encabezadoTransacciones();

        for (int i = 0; i < tienda->numTransacciones; i++){
            if (tienda->transacciones[i].idRelacionado == clienteID){ // Si encuentra el ID del producto lo muestra
                if (strcmp(tienda->transacciones[i].tipo, "VENTA") == 0){
                    mostrarDetalleTransaccion(tienda, i);
                    encontro = true; // Actualización de la variable boolena
                    }
            }
        }
        imprimirSeparador();
        if (encontro == false){ // No encontro el ID en las transacciones
            cout << "No hay transacciones registradas para este cliente." << endl; 
        }
    break;
	}
	
    case 4: { // Busqueda por ID de proveedor
	        int proveedorID;
	
	        if (!solicitarEntero("Ingrese ID del proveedor", proveedorID)){
	            break;
	        }
	        if (existeProveedor(tienda, proveedorID) == false){
	            cout << endl << "Error: El proveedor no existe.";
	            break;
	        }
	
	        encabezadoTransacciones();
	        encontro = false;
	
	        for (int i = 0; i < tienda->numTransacciones; i++){
	            if (tienda->transacciones[i].idRelacionado == proveedorID){
	                if (strcmp(tienda->transacciones[i].tipo, "COMPRA") == 0){
                    mostrarDetalleTransaccion(tienda, i);
                    encontro = true; // Actualización de la variable boolena
                    }
	            }
	        }
	        imprimirSeparador();
	        if (encontro == false){
	            cout << "No hay transacciones registradas para este proveedor.";
	        }
	    break;
	}
    case 5: { // Busqueda por fecha
	        char fecha[11];
	
	        if (!solicitarTexto("Ingrese fecha de la Transacción (YYYY-MM-DD)", fecha, 11)){
	            break;;
	        }
	        if(validarFecha(fecha) == false){
	            cout << "Error: Formato de fecha no valido (Use YYYY-MM-DD)";
	            break;
	        }
	
	        encontro = false; 
	        encabezadoTransacciones();
	
	        for (int i = 0; i < tienda->numTransacciones; i++){
	            if (strcmp(tienda->transacciones[i].fecha, fecha) == 0){
	                mostrarDetalleTransaccion(tienda, i);
	                encontro = true;
	            }
	        }
	        imprimirSeparador();
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
        encabezadoTransacciones();

        for (int i = 0; i < tienda->numTransacciones; i++){
            if (strcmp(tienda->transacciones[i].tipo, tipoTransaccion) == 0){
                mostrarDetalleTransaccion(tienda, i);
                encontro = true;
            }
        }
        imprimirSeparador();
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
        cout << "Error al crear tienda" << endl;
        return;
    }

    if (tienda->numTransacciones == 0){
        cout << endl << "No hay transacciones registradas en el sistema." << endl;
        return;
    }
    
    cout << "LISTADO GENERAL DE TRANSACCIONES" << endl; 
    encabezadoTransacciones();
    for (int i = 0; i < tienda->numTransacciones; i++){
        mostrarDetalleTransaccion(tienda, i);
    }
    imprimirSeparador();
}

// Cancelar/Anular Transacción
void cancelarTransaccion(Tienda* tienda){
    if (tienda == nullptr){ // Validacion de seguridad
        cout << "Error al crear tienda" << endl;
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
    encabezadoTransacciones();
    mostrarDetalleTransaccion(tienda, IndiceTrans);
    imprimirSeparador();

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
    if (!archivo.is_open())
    {
        cout << "Error: no se pudo abrir el archivo " << nombreArchivo << endl; 
        return false;
    }
    // Calculo de la posicion de escritura
    int posicionEscritura = sizeof(ArchivoHeader) + (header.cantidadRegistros * sizeof(Registro));
    archivo.seekp(posicionEscritura, ios::beg);

    archivo.write(reinterpret_cast<const char*>(&nuevoRegistro), sizeof(Registro));
    
    if (archivo.fail()) {
    cout << "Error crítico: No se pudo escribir en el archivo." << endl;
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

// Actualizacion (Update)
template <typename Registro>
bool actualizarRegistro(const char* nombreArchivo, int id, const Registro& datosNuevos) {
    int indice = buscarPorId<Registro>(nombreArchivo, id);
    if (indice == -1){
        cout << "Error: Producto no encontrado. " << endl;
        return false;
    }

    fstream archivo(nombreArchivo, ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) return false;

    archivo.seekp(sizeof(ArchivoHeader) + (indice * sizeof(Registro)), ios::beg);
    archivo.write(reinterpret_cast<const char*>(&datosNuevos), sizeof(Registro));

    archivo.close();
    return true;
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

bool productoTieneTransacciones(const char* archivoTransacciones, int idProducto) {
    ArchivoHeader header = leerHeader(archivoTransacciones);
    if (header.cantidadRegistros == 0){
        return false;
    }
    ifstream archivo(archivoTransacciones, ios::binary);
    if (!archivo.is_open()){
        cout << "Error: No se pudo abrir el archivo." << endl;
        return false;
    }
    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    Transaccion trans;

    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.read(reinterpret_cast<char*>(&trans), sizeof(Transaccion));
        
        if (!trans.eliminado) {
            // Revisa el arreglo de productos dentro de cada transaccion
            for (int j = 0; j < trans.cantidadItemsDiferentes; j++) {
                if (trans.detalles[j].idProducto == idProducto) {
                    archivo.close();
                    return true; // Si encuentra al menos una relacion
                }
            }
        }
    }

    archivo.close();
    return false;
}

bool codigoDuplicado(const char* nombreArchivo, const char* codigo){
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()){
        cout << "Error: No se pudo abrir el archivo " << nombreArchivo << endl;
    }
    ArchivoHeader header;
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));

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
template <typename Registro> // Template para tomar cualquier registro (productos, cliente, proveedor o transacciones)
int buscarPorId(const char* nombreArchivo, int id){
    
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open())
    {
        cout << "Error: No se pudo abrir el archivo" << endl; 
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

int buscarProveedorPorRif(Tienda* tienda, const char* rif){
    if (tienda == nullptr){ // Validacion de seguridad
        return -1;
    }
    // Recorrido del arreglo en busca del proveedor
    for (int i = 0; i < tienda->numProveedores; i++){
        if (strstr(tienda->proveedores[i].rif, rif) != nullptr){
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
        if (strstr(tienda->clientes[i].cedula, cedula) != nullptr){
            return i; // Si lo encuentra retorna el indice
        }
    }
    return -1; // Si llega hasta aqui, No existe
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

void mostrarDetalleTransaccion(const Transaccion& transaccion, Producto& producto){
    
    int indprod = buscarPorId(producto, id);
    char nombreProd[100];

    if (indprod != -1){
        strcpy(nombreProd, producto.nombre);
    } else {
        strcpy(nombreProd, "No encontrado");
    }
    
    cout << left << setw(10) << transaccion.id 
         << setw(12) << transaccion.fecha 
         << setw(10) << transaccion.tipo 
         << setw(12) << transaccion.idRelacionado 
         << setw(12) << transaccion.idProducto
         << setw(15) << nombreProd
         << setw(10) << transaccion.cantidad
         << "$" << fixed << setprecision(2) << transaccion.total << endl;
    
}

void mostrarDetalleProducto(const Producto& producto, const char* archivoProveedores) {
    
    Proveedor prov = obtenerRegistroPorId<Proveedor>(archivoProveedores, producto.idProveedor);
    
    cout << left 
         << setw(10) << producto.id
         << setw(15) << producto.codigo
         << setw(20) << producto.nombre
         << setw(20) << producto.descripcion
         << setw(10) << producto.stock
         << setw(12) << fixed << setprecision(2) << producto.precio
         << setw(12) << producto.idProveedor
         << setw(20) << prov.nombre
         << setw(12) << producto.idProveedor
         << setw(20) << prov.nombre
         << setw(15) << producto.fechaRegistro << endl;
}

void mostrarDetalleProveedor(const Proveedor& proveedor) {
   
    cout << left 
         << setw(8)  << proveedor.id
         << setw(15) << proveedor.rif
         << setw(20) << proveedor.nombre
         << setw(20) << proveedor.telefono
         << setw(20) << fixed << setprecision(2) << proveedor.direccion
         << setw(20) << proveedor.email
         << setw(15) << proveedor.fechaRegistro << endl;
}

void mostrarDetalleCliente(const Cliente& cliente) {
   
    cout << left 
         << setw(8)  << cliente.id
         << setw(15) << cliente.cedula
         << setw(20) << cliente.nombre
         << setw(20) << cliente.telefono
         << setw(20) << fixed << setprecision(2) << cliente.direccion
         << setw(20) << cliente.email
         << setw(15) << cliente.fechaRegistro << endl;
}

void encabezadoTransacciones(){
    cout << endl;
    imprimirSeparador();

    cout << left 
         << setw(10) << "ID Trans" 
         << setw(12) << "Fecha" 
         << setw(10) << "Tipo" 
         << setw(12) << "ID Relac." 
         << setw(12) << "ID Prod"
         << setw(15) << "Producto" 
         << setw(10) << "Cant" 
         << setw(10) << "Total" << endl;

    imprimirSeparador();
}

void encabezadoProveedorCliente(){
    cout << endl;
    imprimirSeparador();

    cout << left 
         << setw(8) << "ID" 
         << setw(15) << "RIF" 
         << setw(20) << "NOMBRE"
         << setw(20) << "TLF" 
         << setw(20) << "DIRECCION" 
         << setw(20) << "EMAIL"
         << setw(15) << "F.REGISTRO"<< endl;

    imprimirSeparador();
}


void encabezadoProductos() {
    cout << endl;
    imprimirSeparador();
    
    cout << left 
         << setw(10)  << "ID PROD" 
         << setw(15) << "CODIGO" 
         << setw(20) << "NOMBRE" 
         << setw(20) << "DESCRIPCION"
         << setw(10) << "STOCK" 
         << setw(12) << "PRECIO" 
         << setw(12) << "ID PROV." 
         << setw(20) << "PROVEEDOR"
         << setw(15) << "F.REGISTRO" << endl; 
         
    imprimirSeparador();
}

void imprimirSeparador(int ancho, char simbolo){
    
    cout << setfill(simbolo) << setw(ancho) << "" << setfill(' ') << endl;
}

bool floatesPositivo(float valor){
    if (valor <= 0){
        cout << "Error: El valor debe ser mayor a cero." << endl;
        return false;
    }
    return true;
}

bool intesPositivo(int valor){
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
    cout << prompt << " (o Ingresa 's' y Enter para salir): ";
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
        cout << "Error: Numero decimal invalido." << endl;
        return false;
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

        cout << "1. Gestión de Productos" << endl
             << "2. Gestión de Proveedores" << endl
             << "3. Gestión de Clientes" << endl
             << "4. Gestión de Transacciones" << endl
             << "5. Salir" << endl;
        while (!solicitarEntero("Seleccione una opción", op)){
            cout << endl;
        }
        imprimirSeparador(60, '=');
        if (!IntesPositivo(op)){
            break;
        }

        switch (op)
            {
            case 1: { // Gestión de Productos
            
            int opProd;
            do {
                imprimirSeparador(60,'=');
                cout << "             SISTEMA DE GESTION DE PRODUCTOS" << endl;
                imprimirSeparador(60,'=');
                cout<< "1. Registrar nuevo Productos" << endl
                    << "2. Buscar Producto" << endl
                    << "3. Actualizar Producto" << endl
                    << "4. Actualizar Stock manualmente" << endl
                    << "5. Listar todos los Productos" << endl
                    << "6. Eliminar Producto" << endl
                    << "0. Volver al menu principal" << endl;
                if (!solicitarEntero("Seleccione una opción", opProd)){
                    break;
                }
                imprimirSeparador(60, '=');
                if (!IntesPositivo(opProd)){
                    break;
                }
                
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
                
                int opProv;
                do {
                    imprimirSeparador(60,'=');
                    cout << "             SISTEMA DE GESTION DE PROVEEDORES" << endl;
                    imprimirSeparador(60,'=');
                    cout << "1. Registrar nuevo Proveedor" << endl
                        << "2. Buscar Proveedor" << endl
                        << "3. Actualizar Proveedor" << endl
                        << "4. Listar todos los Proveedores" << endl
                        << "5. Eliminar Proveedor" << endl
                        << "0. Volver al menu principal" << endl;
                    if (!solicitarEntero("Seleccione una opción", opProv)){
                        break;
                    }
                    imprimirSeparador(60, '=');
                    if (!IntesPositivo(opProv)){
                        break;
                    }
                    
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
                
                int opCliente;
                do {
                    imprimirSeparador(60,'=');
                    cout << "             SISTEMA DE GESTION DE CLIENTES" << endl;
                    imprimirSeparador(60,'=');  
                    cout << "1. Registrar nuevo Cliente" << endl
                        << "2. Buscar Cliente" << endl
                        << "3. Actualizar Cliente" << endl
                        << "4. Listar todos los Cliente" << endl
                        << "5. Eliminar Cliente" << endl
                        << "0. Volver al menu principal" << endl;
                    if (!solicitarEntero("Seleccione una opción", opCliente)){
                        break;
                    }
                    imprimirSeparador(60, '=');
                    if (!IntesPositivo(opCliente)){
                        break;;
                    }
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
                
                int opTrans;
                do {
                    imprimirSeparador(60,'=');
                    cout << "             SISTEMA DE GESTION DE TRANSACCIONES" << endl;
                    imprimirSeparador(60,'=');
                    cout << "1. Registrar Compra a Proveedor" << endl
                        << "2. Registrar Venta a Cliente" << endl
                        << "3. Buscar Transacciones" << endl
                        << "4. Listar Transacciones" << endl
                        << "5. Cancelar Transacción" << endl
                        << "0. Volver al menu principal" << endl;
                    if (!solicitarEntero("Seleccione una opción", opTrans)){
                        break;
                    }
                    imprimirSeparador(60, '=');
                    if (!IntesPositivo(opTrans)){
                        break;
                    }
                    
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
