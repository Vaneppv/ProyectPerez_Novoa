# Sistema de Gestión de Inventario y Ventas para la Tienda Jsport 🎽🏀

## 📝 Descripción del Proyecto
**Tienda Jsport** es una aplicación de consola desarrollada en **C++** diseñada para la gestión integral de un negocio. El sistema permite el control de inventario, administración de proveedores y clientes, y el registro detallado de transacciones (compras y ventas). 

El proyecto destaca por el uso de **memoria dinámica**, permitiendo que el sistema crezca de forma flexible mediante el redimensionamiento automático de sus estructuras de datos a medida que se añaden nuevos registros.

---

## 🛠 Estructura del Código
El código está organizado de manera modular bajo una estructura principal denominada `Tienda`, que actúa como el núcleo del sistema.

---

## 👥 Autores
- **Vanessa Perez & Luis Novoa** - Estudiantes de Programación 2.

### Componentes Principales:
* **Estructuras (Structs):**
    * `Producto`: Información técnica, stock y precio.
    * `Proveedor` / `Cliente`: Datos de contacto e identificación.
    * `Transaccion`: Registro histórico de movimientos (Entradas/Salidas).
* **Gestión de Memoria:** Implementa funciones de redimensionamiento (`redimensionar...`) para manejar arreglos dinámicos de punteros.
* **Módulos de Lógica:**
    * Validaciones de datos (Email, RIF, Fechas).
    * Búsquedas especializadas por ID, nombre o código.
    * Cálculos automáticos (Totales).

---

## 🚀 Instrucciones de Compilación

Para compilar este proyecto, asegúrate de tener instalado un compilador de C++ (como **GCC/G++**).

1. Abre una terminal en la carpeta donde se encuentra el archivo `TiendaJsport.cpp`.
2. Ejecuta el siguiente comando:

```Bash
g++ TiendaJsport.cpp -o TiendaJsport
```
## 💻 Instrucciones de Ejecución
Una vez compilado, puedes ejecutar el programa directamente desde la terminal:

En Windows:

```Bash
.\TiendaJsport.exe
```
En Linux / macOS:

```Bash
./TiendaJsport
```
Nota: Antes de Iniciar el programa debes Ingresar el Nombre de la Empresa y el Rif.

## ✨ Funcionalidades Implementadas
El sistema cuenta con un menú interactivo dividido en los siguientes módulos:

### 1. Gestión de Productos
* Registro de nuevos productos con validación de código duplicado.

* Búsqueda avanzada (por ID, código o nombre).

* Modificación de datos y actualización de stock.

* Eliminación física con validación de integridad.

### 2. Gestión de Proveedores y Clientes
* Registro de datos con validaciones de RIF y formato de Email.

* Listados detallados en formato de tabla para una lectura clara.

* Control de relaciones comerciales y existencia previa.

### 3. Sistema de Transacciones
* **Compras:** Abastecimiento de inventario a través de proveedores registrados.

* **Ventas:** Facturación a clientes.

* **Historial:** Consulta de movimientos filtrados por cliente o proveedor.

* **Cancelación:** Función para revertir transacciones y restaurar el stock automáticamente.

### 4. Seguridad y Validación
* Protección contra entradas de datos inválidas (manejo de buffer para evitar bucles infinitos).

* Validación estricta de fechas en formato YYYY-MM-DD.

* Control de punteros nulos para evitar cierres inesperados del programa.


---

**¿Necesitas que te ayude a generar algún otro documento para tu proyecto, como un manual de usuario rápido o comentarios detallados para alguna función específica?**
