#include "../include/producto.h"
#include "../src\producto.cpp"
#include <iostream>
#include <fstream>
#include <limits>

using namespace std;

// Definición de colores ANSI
#define RESET   "\033[00m"
#define ROJO    "\033[31m"
#define VERDE   "\033[32m"
#define AMARILLO "\033[33m"
#define AZUL    "\033[34m"
#define CYAN    "\033[36m"
#define NEGRITA "\033[1m"

void mostrarMenuProductos() {
    cout << AZUL << NEGRITA << "\n===== SISTEMA DE GESTIÓN DE PRODUCTOS =====" << endl << RESET;
    cout << CYAN << "1. Crear nuevo producto" << endl;
    cout << "2. Buscar producto" << endl;
    cout << "3. Listar todos los productos" << endl;
    cout << "4. Actualizar producto" << endl;
    cout << "5. Actualizar stock" << endl;
    cout << "6. Eliminar producto" << endl;
    cout << "0. Salir" << endl << RESET;
    cout << AZUL << "=========================================" << endl << RESET;
}

int main() {
    cout << VERDE << NEGRITA << "=== TIENDA JSPORT - MÓDULO DE PRODUCTOS ===" << endl << RESET;
    
    // Inicializar el gestor de productos
    ProductoManager gestorProductos;
    
    // Inicializar archivos
    if (!gestorProductos.inicializarArchivo()) {
        cout << ROJO << "Error: No se pudo inicializar el sistema de archivos." << endl << RESET;
        return 1;
    }
    
    // Crear archivo de proveedores dummy para pruebas
    {
        ofstream proveedoresFile("proveedores.bin", ios::binary);
    if (proveedoresFile.is_open()) {
        struct Header {
            int cantidadRegistros;
            int proximoID;
            int registrosActivos;
            int version;
        } header = {1, 2, 1, 1};
        
        struct ProveedorDummy {
            int id;
            char nombre[100];
            char rif[20];
            char telefono[20];
            char email[100];
            char direccion[200];
            char fechaRegistro[11];
            int cantidadProductos;
            int productosIDs[100];
            float totalCompras;
            int transaccionesIds[100];
            int cantidadTransacciones;
            bool eliminado;
            char fechaUltimaModificacion[11];
        } proveedor = {1, "Proveedor Demo", "J-123456789", "0414-1234567", "demo@proveedor.com", 
                     "Dirección demo", "01/01/2024", 0, {}, 0.0f, {}, 0, false, "01/01/2024"};
        
        proveedoresFile.write(reinterpret_cast<const char*>(&header), sizeof(Header));
        proveedoresFile.write(reinterpret_cast<const char*>(&proveedor), sizeof(ProveedorDummy));
        proveedoresFile.close();
    }
    }
    
    int opcion;
    do {
        mostrarMenuProductos();
        cout << CYAN << "Seleccione una opción: " << RESET;
        cin >> opcion;
        
        // Limpiar el buffer de entrada
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (opcion) {
            case 1:
                gestorProductos.crearProducto();
                break;
                
            case 2:
                gestorProductos.buscarProducto();
                break;
                
            case 3:
                gestorProductos.listarProductos();
                break;
                
            case 4:
                gestorProductos.actualizarProducto();
                break;
                
            case 5:
                gestorProductos.actualizarStock();
                break;
                
            case 6:
                gestorProductos.eliminarProducto();
                break;
                
            case 0:
                cout << VERDE << "Saliendo del sistema de productos..." << endl << RESET;
                break;
                
            default:
                cout << ROJO << "Opción no válida. Por favor, intente nuevamente." << endl << RESET;
                break;
        }
        
        if (opcion != 0) {
            cout << "\nPresione Enter para continuar...";
            cin.get();
        }
        
    } while (opcion != 0);
    
    cout << VERDE << "¡Gracias por usar el sistema de productos de Tienda JSPORT!" << endl << RESET;
    return 0;
}
