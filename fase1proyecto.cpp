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

// Inicializaci√≥n y Liberaci√≥n
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
int buscarClientePorId(Tienda* tienda, int id);
int buscarTransaccionPorId(Tienda* tienda, int id);
int* buscarProductosPorNombre(Tienda* tienda, const char* nombre, int* numResultados);
int* buscarProductosPorCodigo(Tienda* tienda, const char* codigo, int* numResultados);

// UTILIDADES
void obtenerFechaActual(char* buffer);
void convertirAMinusculas(char* cadena);
bool contieneSubstring(const char* texto, const char* busqueda);
void mostrarDetalleTransaccion(Tienda* tienda, int ind);
void imprimirSeparador(int ancho = 91, char simbolo = '-');
void encabezadoTabla();
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

// Estructura Transacci√≥n (puede separarse por transaccion)
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
    
    // Arrays din√°micos de entidades
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

// Funcion de Inicializaci√≥n
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

// Funcion de Liberaci√≥n
void liberarTienda(Tienda* tienda){

    // Liberar arreglos din√°micos para evitar fugas de memoria
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

	char decision;
	cout << "øDesea registrar un nuevo producto? (S/N)" << endl;
	cin >> decision;
	
	if ((decision == 's') || (decision == 'S')) {
	
		int i = tienda->siguienteIdProducto;
		char* nombre = new char[100];
		char* codigo = new char[20];
		char* descripcion = new char[200];
		float precio;
		int stock;
		int idProveedor;
		
		cout << "Ingrese el nombre del producto. (Puede cancelar la crecion en cualquier momento si ingresa 'CANCELAR' o '0')" << endl;
		cin.ignore();
		cin.getline(nombre, 100);
		
		cout << "Ingrese el codigo del producto. (Puede cancelar la crecion en cualquier momento si ingresa 'CANCELAR' o '0')" << endl;
		cin.getline(codigo, 20);
		codigoDuplicado(tienda, codigo);
		
		cout << "Ingrese la descripcion del producto. (Puede cancelar la crecion en cualquier momento si ingresa 'CANCELAR' o '0')" << endl;
		cin.getline(descripcion, 200);
		
		do {
		
			cout << "Ingrese el precio del producto. (Puede cancelar la crecion en cualquier momento si ingresa '0')" << endl;
			cin >> precio;
			
		}while (precio <= 0);
		
		do{
		cout << "Ingrese el stock del producto. (Puede cancelar la crecion en cualquier momento si ingresa '0')" << endl;
		cin >> stock;
		}while (precio < 0);
		
		cout << "Ingrese la ID del proveedor. (Puede cancelar la crecion en cualquier momento si ingresa '0')" << endl;
		cin >> idProveedor;
		existeProveedor(tienda, idProveedor);
		
		cout << "A continuacion se le mostrara los datos ingresados" << endl;
		cout << "Nombre: " << nombre << endl;
		cout << "Codigo: " << codigo << endl;
		cout << "Descripcion: " << descripcion << endl;
		cout << "Precio: " << precio << endl;
		cout << "Stock: " << stock << endl;
		cout << "ID del proveedor: " << idProveedor << endl;
		cout << "Desea Guardar producto? (S/N)" << endl;
		cin >> decision;
	
		if ((decision == 's') || (decision == 'S')) {
		
			strncpy(tienda->productos[i].nombre, nombre, 100);
			strncpy(tienda->productos[i].codigo, codigo, 20);
			strncpy(tienda->productos[i].descripcion, descripcion, 200);
			tienda->productos[i].idProveedor = idProveedor;
			tienda->productos[i].precio = precio;
			tienda->productos[i].stock = stock;
			tienda->productos[i].id = tienda->siguienteIdProducto;
			tienda->siguienteIdProducto++;
			
			cout << "El producto fue creado con exito, volviendo al menu principal" << endl;
		} else {
			
			cout << "No se deseo guardar el producto hecho, eliminando producto y volviendo al menu principal" << endl;
			
		}
		
		delete [] nombre;
		delete [] codigo;
		delete [] descripcion;
		
	} else {
		
		cout << "No se va a crear un prodcuto nuevo, volviendo al menu" << endl;
		
	}

}

// Buscar Producto
void buscarProducto(Tienda* tienda){

	int seleccion;
	char decision;
	
	do {
	
	cout << "Menu de Busqueda" << endl;
	cout << "1. Buscar por ID" << endl;
	cout << "2. Buscar por nombre (coincidencia parcial)" << endl;
	cout << "3. Buscar por codigo (coincidencia parcial)" << endl;
	cout << "4. Listar por proveedor" << endl;
	cout << "0. Cancelar" << endl;
	
		if (seleccion == 1){
			
			do {
				
				int id;
				int i;
				cout << "Eligo buscar por id, por favor ingrese la ID del producto a buscar" << endl;
				cin >> id;
			
				i =	buscarProductoPorId(tienda, id);
					
					if (buscarProductoPorId){
						
						
						cout << "Se encontro el producto de ID " << id << endl;
						cout << "Nombre: " << tienda->productos[i].nombre << endl;
						cout << "Codigo: " << tienda->productos[i].codigo << endl;
						cout << "Descripcion: " << tienda->productos[i].descripcion << endl;
						cout << "Precio: " << tienda->productos[i].precio << endl;
						cout << "Stock: " << tienda->productos[i].stock << endl;
						cout << "ID del proveedor: " << tienda->productos[i].idProveedor << endl;
						
					}
					
				
				cout << "Desea buscar otro producto por ID (S/N)" << endl;
				cin >> decision;
			} while((decision == 's') || (decision == 'S'));
			
		} else if (seleccion == 2){
			
			char* nombre = new char[100];
			int* numResultados;
			cout << "Eligo buscar producto por nombre, por favor ingrese el del producto a buscar" << endl;
			cin.ignore();
			cin.getline(nombre, 100);
			buscarProductosPorNombre(tienda, nombre, numResultados);
			
		} else if (seleccion == 3){
			
			char* codigo = new char[20];
			int* numResultados;
			cout << "Eligo buscar producto por codigo, por favor ingrese el codigo del producto a buscar" << endl;
			cin.ignore();
			cin.getline(codigo, 20);
			buscarProductosPorCodigo(tienda, codigo, numResultados);
			
		} else if (seleccion == 4){
			
			int id;
			cout << "Eligo listar productos por id del Proveedor, por favor ingrese la ID del proveedor" << endl;
			cin >> id;
				
				for(int i = 0; i < tienda->capacidadProductos; i++){
				
						cout <<	"+------------------------------------------------------------+" << endl;
						cout <<	"¶                    LISTA DE PRODUCTOS                      ¶" << endl;
						cout <<	"¶------------------------------------------------------------¶" << endl;
						cout <<	"¶ NOMBRE     ¶ CODIGO       ¶     PRECIO    ¶      STOCK     ¶" << endl;
						cout <<	"¶-----+---------------------+--------------+------+----------¶" << endl;
					if (tienda->productos[i].idProveedor == id){

								cout <<	left << setw(10) << tienda->productos[i].nombre <<	left << setw(5) << tienda->productos[i].codigo << left << setw(15) << tienda->productos[i].precio << left << setw(15) << tienda->productos[i].stock << endl;
							
					}
						cout <<	"+------------------------------------------------------------+" << endl;
				}
		}
	
	} while(seleccion != 0);

}

//Actualizar Producto
void actualizarProducto(Tienda* tienda){
		
	int id;
	int i;
	cout << "Eligo buscar por id, por favor ingrese la ID del producto a buscar" << endl;
	cin >> id;
			
	i = buscarProductoPorId(tienda, id);
	int seleccion;
	char decision;
	
	if (buscarProductoPorId){
		
		char* nombre = new char[100];
		char* codigo = new char[20];
		char* descripcion = new char[200];
		float precio;
		int stock;
		int idProveedor;
		bool VerCodigo;
		bool VerNombre;
		bool VerDescrip;
		bool VerPrecio;
		bool VerStock;
		bool VerIdProveedor;
		
		do {
					
			cout << "øQuÈ desea editar?" << endl;
			cout << "1. CÛdigo" << endl;
			cout << "2. Nombre" << endl;
			cout << "3. DescripciÛn" << endl;
			cout << "4. Proveedor" << endl;
			cout << "5. Precio" << endl;
			cout << "6. Stock" << endl;
			cout << "7. Guardar cambios" << endl;
			cout << "0. Cancelar sin guardar" << endl;
			
			if (seleccion == 1 ){
				
				cout << "Ingrese el nuevo codigo del producto" << endl;
				cin.ignore();
				cin.getline(codigo, 20);
				codigoDuplicado(tienda, codigo);
				VerCodigo = true;
				
				} else if (seleccion == 2 ){
				
				cout << "Ingrese el nuevo nombre del producto" << endl;
				cin.ignore();
				cin.getline(nombre, 100); 
				VerNombre = true;
				
				} else if (seleccion == 3 ){
				
				cout << "Ingrese la nueva descripccion del producto" << endl;
				cin.ignore();
				cin.getline(descripcion, 200);
				VerDescrip = true;
				
				} else if (seleccion == 4 ){
				
				cout << "Ingrese la ID del nuevo proveedor del producto" << endl;
				cin >> idProveedor;
				existeProveedor(tienda, idProveedor);
				VerIdProveedor = true;
				
				} else if (seleccion == 5 ){
				
				cout << "Ingrese el nuevo precio del producto" << endl;
				cin >> precio;
				VerPrecio = true;
				
				} else if (seleccion == 6 ){
				
				cout << "Ingrese el nuevo stock del producto" << endl;
				cin >> stock;
				VerStock = true;
				
				} else if (seleccion == 7 ){
					
					
				cout << "Desea Guardar los cambios hechos al producto? (S/N)" << endl;
				cin >> decision;
			
				if ((decision == 's') || (decision == 'S')){
						
						if (VerNombre == true){
							strncpy(tienda->productos[i].nombre, nombre, 100);
						}
						if (VerCodigo == true){
						strncpy(tienda->productos[i].codigo, codigo, 20);
						}
						if (VerDescrip == true){
						
						strncpy(tienda->productos[i].descripcion, descripcion, 200);
						}
						if (VerIdProveedor == true){
						
						tienda->productos[i].idProveedor = idProveedor;
						}
						if (VerPrecio == true){
						
						tienda->productos[i].precio = precio;
						}
						if (VerStock == true){
						
						tienda->productos[i].stock = stock;
						}
				}
				
			}
		
		}while(seleccion != 0);
						
	}
		
	
}

// Actualizar Stock Manualmente
void actualizarStockProducto(Tienda* tienda){
	
	int id;
	int i;
	cout << "Por favor ingrese la ID del producto que se va a actualizar" << endl;
	cin >> id;
			
	i = buscarProductoPorId(tienda, id);
	int seleccion;
	char decision;
	
	if (buscarProductoPorId){
		
		int stock = tienda->productos[i].stock;
		do {
		
			cout << "Stock actual:" << tienda->productos[i].stock;
			cout << "Nuevo Stock:" << stock;
			cout << "Que desea hacer?" << endl;
			cout << "1. Agregar stock" << endl;
			cout << "2. Quitar stock" << endl;
			cout << "3. Guardar cambios" << endl;
			cout << "0. Cancelar sin guardar" << endl;
			cin >> seleccion;
			
			if (seleccion == 1) {
				
				stock++;
				
			} else if (seleccion == 2) {
				
				stock--;
				
			} else if (seleccion == 3){
				
				cout << "Seguro que quiere guardar el nuevo stock?(S/N)" << endl;
				cin >> decision;
				
				if ((decision == 's') || (decision == 'S')){
				
					tienda->productos[i].stock = stock;
				
				}
			}
		
		}while(seleccion !=0);
	}
}

// Listar Productos
void listarProductos(Tienda* tienda){

		for(int i = 0; i < tienda->capacidadProductos; i++){
		
			
			cout <<	"+------------------------------------------------------------+" << endl;
			cout <<	"¶                    LISTA DE PRODUCTOS                      ¶" << endl;
			cout <<	"¶------------------------------------------------------------¶" << endl;
			cout <<	"¶ NOMBRE     ¶ CODIGO       ¶     PRECIO    ¶      STOCK     ¶" << endl;
			cout <<	"¶-----+---------------------+--------------+------+----------¶" << endl;
			
					cout <<	left << setw(10) << tienda->productos[i].nombre <<	left << setw(5) << tienda->productos[i].codigo << left << setw(15) << tienda->productos[i].precio << left << setw(15) << tienda->productos[i].stock << endl;
			cout <<	"+------------------------------------------------------------+" << endl;
			
	}

}

// Eliminar Producto
void eliminarProducto(Tienda* tienda){

	int id;
	int i;
	cout << "Por favor ingrese la ID del producto que se va a eliminar" << endl;
	cin >> id;
			
	i = buscarProductoPorId(tienda, id);
	for (int j = i; j < tienda->capacidadProductos - 1; ++j) {
                tienda->productos[j] = tienda->productos[j + 1];
    }

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
    if (tienda->numTransacciones >= tienda->capacidadTransacciones){ // Verificacion del tama√±o del arreglo
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
    
    if (!solicitarTexto("Ingrese descripci√≥n", tienda->transacciones[i].descripcion, 200)){
        strcpy(tienda->transacciones[i].descripcion, "Sin descripci√≥n");
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
    cout << "¬øDesea confirmar y guardar esta Compra? (S/N) ";
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
        return;
    }
    if (tienda->numTransacciones >= tienda->capacidadTransacciones){ // Verificacion del tama√±o del arreglo
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

    if (!solicitarTexto("Ingrese descripci√≥n", tienda->transacciones[i].descripcion, 200)){
        strcpy(tienda->transacciones[i].descripcion, "Sin descripci√≥n");
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
    cout << "¬øDesea confirmar y guardar esta Venta? (S/N) ";
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
        return;
    }
    
    int op;
    bool encontro;

    cout << endl << "===== Buscar Transacciones =====" << endl;
    cout << "Seleccione opcion de busqueda: " << endl 
    << "1. Buscar por ID de Transacci√≥n" << endl 
    << "2. Buscar por ID de Producto" << endl
    << "3. Buscar por ID de Cliente" << endl
    << "4. Buscar por ID de Proveedor" << endl
    << "5. Buscar por fecha exacta" << endl
    << "6. Buscar por tipo de transaci√≥n (COMPRA/VENTA)" << endl
    << "0. Cancelar" << endl;
    cin >> op;
    
    if (op == 0){
        return;
    }
    
    switch (op){
    case 1: // Busqueda por ID de transacci√≥n
        int TransID;
        if (!solicitarEntero("Ingrese ID de la transacci√≥n", TransID)){
        return;
        }
        int i = buscarTransaccionPorId(tienda, TransID); // Extraer ID
        if (i == -1){ // Validaci√≥n de existencia
            cout << "Error: La transaccion no existe.";
            break;
        }

        encabezadoTabla();
        mostrarDetalleTransaccion(tienda, i);
        imprimirSeparador(91, '=');
        break;
    
    case 2: // Busqueda por ID del Producto
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
            encontro = true; // Actualizaci√≥n de la variable booleana
            }
        }
        imprimirSeparador(91, '=');
        if (encontro == false){ // No encontro el ID en las transacciones
            cout << "No hay transacciones para este producto." << endl;
        }
        break;

    case 3:
        int clienteID;

        if (!solicitarEntero("Ingrese ID del cliente", clienteID)){
        return;
        }
        if (existeCliente(tienda, clienteID) == false){ // Validaci√≥n de existencia del cliente
            cout << "Error: El cliente no existe.";
            break; 
        }
        encontro = false;
        encabezadoTabla();

        for (int i = 0; i < tienda->numTransacciones; i++){
            if (tienda->transacciones[i].idRelacionado == clienteID){ // Si encuentra el ID del producto lo muestra
                mostrarDetalleTransaccion(tienda, i);
                encontro = true; // Actualizaci√≥n de la variable boolena
            }
        }
        imprimirSeparador(91, '=');
        if (encontro == false){ // No encontro el ID en las transacciones
            cout << "No hay transacciones registradas para este cliente." << endl; 
        }
    break;

    case 4: // Busqueda por ID de proveedor
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

    case 5: // Busqueda por fecha
        char fecha[11];

        if (!solicitarTexto("Ingrese fecha de la Transacci√≥n (YYYY-MM-DD)", fecha, 11)){
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

    case 6: // busqueda por tipo de transaccion

        int tipo;
        cout << "1. Compra (A Proveedor)"
             << "2. Venta (A Cliente)" 
             << "0. Cancelar"
             << endl;
        if (!solicitarEntero("Ingrese tipo de transacci√≥n", tipo)){
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

    default:
        break;
    }
}

// Listar Transacciones
void listarTransacciones(Tienda* tienda){
    if (tienda == nullptr){ // Validacion de seguridad
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

// Cancelar/Anular Transacci√≥n
void cancelarTransaccion(Tienda* tienda){
    if (tienda == nullptr || tienda->numTransacciones <= 0){ // Validacion de seguridad
        cout << "No hay transacciones para cancelar.";
        return;
    }
    int TransID;
    cout << endl << "===== CANCELAR TRANSACCI√ìN =====" << endl;
    if (!solicitarEntero("Ingrese ID de la transacci√≥n a cancelar", TransID)){
        return;
    }
    int IndiceTrans = buscarTransaccionPorId(tienda, TransID);
    if (IndiceTrans == -1)
    {
        cout << "Error: Transacci√≥n no encontrada.";
        return;
    }
    encabezadoTabla();
    mostrarDetalleTransaccion(tienda, IndiceTrans);
    imprimirSeparador(91, '=');

    char confirmar;
    cout << "¬øDesea confirmar y anular esta transacci√≥n? (S/N) ";
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

        // Eliminar datos de la transacci√≥n moviendo los elementos del arreglo
        for (int j = IndiceTrans; j < tienda->numTransacciones-1; j++){
            tienda->transacciones[j] = tienda->transacciones[j + 1];
        }
        tienda->numTransacciones--;

        cout << endl << "Transacci√≥n anulada y stock actualizado" << endl;
    } else {
        cout << endl << "Operaci√≥n cancelada. No se realizaron cambios en la transacci√≥n." <<  endl;
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

    if (strlen(fecha) != 10) return false; // Validar el tama√±o de caracteres (YYYY-MM-DD)
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

int buscarTransaccionPorId(Tienda* tienda, int id){
    if (tienda == nullptr){ 
        return -1;
    }
    // Recorrido del arreglo en busca del producto
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
    // Recorrido para saber tama√±o del arreglo de productos encontrados
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

int* buscarProductosPorCodigo(Tienda* tienda, const char* codigo, int* numResultados){
    if (tienda == nullptr){ // Validacion de seguridad
        return nullptr;
    }
    // Inicializacion del contador de resultados encontrados
    *numResultados = 0;
    // Recorrido para saber tama√±o del arreglo de productos encontrados
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
         << fixed << setprecision(2) << tienda->transacciones[ind].total << endl;
    
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
    cout << prompt << " (o Enter para cancelar): ";
    cin.ignore(cin.rdbuf()->in_avail(), '\n'); // Limpia restos del buffer
    
    char temp[200];
    cin.getline(temp, 200);

    if (strlen(temp) == 0) {
        cout << "Operacion cancelada por el usuario." << endl;
        return false; 
    }

    strncpy(destino, temp, largo);
    return true;
}

bool solicitarEntero(const char* prompt, int& valor) {
    string entrada;
    cout << prompt << " (Escribe 's' para salir): ";
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
    cout << prompt << " (Escribe 's' para salir): ";
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


    return 0;
}
