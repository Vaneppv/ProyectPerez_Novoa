![Status](https://img.shields.io/badge/Status-En%20Desarrollo-green)
![Language](https://img.shields.io/badge/Language-C%2B%2B-blue)
![Architecture](https://img.shields.io/badge/Architecture-Binary%20Files-orange)

# Tienda JSport - Sistema de Inventario V3.0

## Descripción

Sistema de gestión de inventario para la tienda deportiva JSport, desarrollado en C++ con Programación Orientada a Objetos (POO) y persistencia en archivos binarios.

**Versión 3.0:** Arquitectura modularizada con separación de responsabilidades, encapsulamiento y manejo de archivos binarios.

## Características Principales

### 🏗️ Arquitectura Modular
- **Separación por dominios:** Cada entidad tiene su propio módulo (productos, proveedores, clientes, transacciones)
- **Encapsulamiento:** Clases con atributos privados y métodos públicos con validaciones
- **Templates:** Operaciones genéricas de archivos que funcionan con cualquier entidad
- **Responsabilidad Única:** Cada clase tiene una responsabilidad bien definida

### 📦 Módulos del Sistema

```
ProyectoTienda_v3/
|-- tienda/              # Gestión de la tienda principal
|-- productos/           # Gestión de productos y operaciones
|-- proveedores/         # Gestión de proveedores y operaciones  
|-- clientes/            # Gestión de clientes y operaciones
|-- transacciones/       # Gestión de transacciones y detalles
|-- interfaz/            # Menús y navegación del sistema
|-- persistencia/        # Manejo de archivos binarios
|-- utilidades/          # Validaciones y formatos
|-- mantenimiento/       # Backup e integridad referencial
|-- reportes/            # Estadísticas y reportes de ventas
|-- obj/                 # Archivos objeto compilados
|-- datos/               # Archivos binarios (generados automáticamente)
|-- main.cpp             # Punto de entrada principal
`-- Makefile             # Sistema de compilación
```

### 🔧 Funcionalidades

#### Gestión de Productos
- Registro, búsqueda, actualización y eliminación de productos
- Control de stock con alertas de stock mínimo
- Validación de códigos únicos
- Asociación con proveedores

#### Gestión de Proveedores  
- Registro y gestión de proveedores
- Validación de RIF y datos de contacto
- Historial de productos suministrados
- Control de transacciones comerciales

#### Gestión de Clientes
- Registro y gestión de clientes
- Validación de cédula y datos de contacto
- Historial de compras
- Estadísticas de consumo

#### Gestión de Transacciones
- Registro de compras a proveedores
- Registro de ventas a clientes
- Control de detalles de transacción
- Cancelación y reversión de operaciones

#### Mantenimiento y Reportes
- Verificación de integridad referencial
- Creación de backups automáticos
- Reportes de stock crítico
- Estadísticas de ventas y compras
- Análisis de productos más vendidos
- Reportes por períodos de tiempo

## 📋 Requisitos del Sistema

### Software
- **Compilador:** GCC con soporte C++17 o superior
- **Sistema Operativo:** Windows, Linux o macOS
- **Terminal/Consola:** Para ejecución del programa

### Hardware
- **RAM:** Mínimo 512MB
- **Almacenamiento:** 50MB para el programa y datos
- **Procesador:** Cualquier procesador moderno

## 🚀 Instalación y Compilación

### 1. Descargar el Proyecto
```bash
# Clonar o descargar el proyecto
cd ProyectoTienda_v3
```

### 2. Compilar con Make (Recomendado)
```bash
# Compilar el proyecto
make

# Opciones adicionales
make run      # Compilar y ejecutar
make debug    # Compilar con símbolos de depuración
make release  # Compilar versión optimizada
make clean    # Limpiar archivos compilados
make help     # Ver todas las opciones
make rebuild  # Limpiar y recompilar todo
```

El Makefile maneja automáticamente:
- Creación del directorio `datos/` para archivos binarios
- Creación del directorio `obj/` para archivos objeto
- Compilación modular por directorios
- Enlace automático de librerías necesarias

### 3. Compilación Manual (Alternativa)
```bash
# Crear directorio de datos
mkdir datos

# Compilar todos los archivos fuente
g++ -std=c++17 -Wall -Wextra -g -O2 -o TiendaJsportV3 \
    main.cpp \
    tienda/Tienda.cpp \
    productos/Producto.cpp \
    productos/operacionesProductos.cpp \
    proveedores/Proveedor.cpp \
    proveedores/operacionesproveedor.cpp \
    clientes/Cliente.cpp \
    clientes/operacionescliente.cpp \
    transacciones/Transaccion.cpp \
    transacciones/DetalleTransaccion.cpp \
    transacciones/operacionesTransacciones.cpp \
    mantenimiento/Backup.cpp \
    mantenimiento/IntegridadReferencial.cpp \
    persistencia/GestorArchivos.cpp \
    persistencia/Constantes.cpp \
    utilidades/Validaciones.cpp \
    utilidades/Formatos.cpp \
    reportes/EstadisticasVentas.cpp \
    interfaz/Interfaz.cpp
```

### 4. Ejecutar el Programa
```bash
# En Windows
./TiendaJsportV3.exe

# En Linux/macOS
./TiendaJsportV3
```

## 📖 Manual de Usuario

### Inicio del Sistema

Al ejecutar el programa, verás una pantalla de bienvenida seguida del menú principal:

```
===========================================
    TIENDA JSPORT - SISTEMA DE INVENTARIO V3.0    
===========================================

MENÚ PRINCIPAL
============================================================
1. Gestión de Productos
2. Gestión de Proveedores  
3. Gestión de Clientes
4. Gestión de Transacciones
5. Mantenimiento del Sistema
6. Reportes y Estadísticas
0. Salir del Sistema
============================================================
```

### Gestión de Productos

#### Registrar un Nuevo Producto
1. Seleccionar "Gestión de Productos" → "Registrar Producto"
2. Ingresar los siguientes datos:
   - **Nombre:** Nombre descriptivo del producto
   - **Código:** Código único (ej: JS-001)
   - **Descripción:** Detalles del producto
   - **Precio:** Precio de venta (ej: 99.99)
   - **Stock:** Cantidad inicial en inventario
   - **Stock Mínimo:** Nivel para alertas (ej: 5)
   - **ID Proveedor:** Proveedor asociado

#### Buscar Productos
Opciones de búsqueda disponibles:
- **Por ID:** Búsqueda exacta por identificador
- **Por Nombre:** Búsqueda parcial por coincidencias
- **Por Código:** Búsqueda parcial por código
- **Por Proveedor:** Listar productos de un proveedor específico

#### Actualizar Productos
1. Buscar el producto por ID
2. Seleccionar el campo a modificar:
   - Código, Nombre, Descripción, Proveedor, Precio, Stock, Stock Mínimo
3. Confirmar los cambios

#### Control de Stock
- **Agregar Stock:** Incrementar inventario manualmente
- **Reducir Stock:** Decrementar inventario (valida disponibilidad)
- **Alertas Automáticas:** Sistema avisa cuando stock ≤ stock mínimo

### Gestión de Proveedores

#### Registrar un Nuevo Proveedor
1. Seleccionar "Gestión de Proveedores" → "Registrar Proveedor"
2. Ingresar los siguientes datos:
   - **Nombre:** Nombre completo del proveedor
   - **RIF:** Identificación fiscal (ej: J-12345678-9)
   - **Teléfono:** Número de contacto
   - **Email:** Correo electrónico
   - **Dirección:** Dirección física

#### Validaciones Automáticas
- **RIF:** Formato válido (J-, V-, E-, G- seguido de números)
- **Email:** Debe contener @ y dominio válido
- **Teléfono:** Formato telefónico válido
- **Unicidad:** RIF no debe duplicarse

### Gestión de Clientes

#### Registrar un Nuevo Cliente
1. Seleccionar "Gestión de Clientes" → "Registrar Cliente"
2. Ingresar los siguientes datos:
   - **Nombre:** Nombre completo del cliente
   - **Cédula:** Identificación personal (ej: V-12345678)
   - **Teléfono:** Número de contacto
   - **Email:** Correo electrónico
   - **Dirección:** Dirección de entrega

#### Historial de Compras
- El sistema mantiene un registro automático de todas las compras
- Estadísticas de consumo por cliente
- Transacciones asociadas automáticamente

### Gestión de Transacciones

#### Registrar Compra a Proveedor
1. Seleccionar "Gestión de Transacciones" → "Registrar Compra"
2. Seleccionar el proveedor
3. Agregar productos a la compra:
   - Buscar producto por ID o código
   - Ingresar cantidad comprada
   - Ingresar precio de compra unitario
4. Confirmar la transacción

#### Registrar Venta a Cliente
1. Seleccionar "Gestión de Transacciones" → "Registrar Venta"
2. Seleccionar el cliente
3. Agregar productos a la venta:
   - Buscar producto por ID o código
   - Ingresar cantidad vendida
   - Sistema valida stock disponible
4. Confirmar la transacción

#### Control de Inventario
- **Actualización Automática:** Stock se actualiza inmediatamente
- **Validaciones:** No permite vender más de lo disponible
- **Cancelación:** Posible revertir transacciones si hay stock disponible

### Mantenimiento del Sistema

#### Verificación de Integridad
- Verifica relaciones entre entidades
- Detecta registros huérfanos
- Valida consistencia de datos

#### Backup y Restauración
- **Backup Automático:** Copia de seguridad de todos los datos
- **Restauración:** Recuperar datos desde backup
- **Programación:** Se recomienda backup semanal

#### Limpieza de Archivos
- Elimina archivos temporales
- Optimiza espacio en disco
- Mantiene rendimiento del sistema

### Reportes y Estadísticas

#### Reporte de Stock Crítico
- Lista todos los productos con stock ≤ stock mínimo
- Alertas de reabastecimiento necesario
- Opciones de exportación

#### Estadísticas de Ventas
- Ventas totales por cliente
- Productos más vendidos
- Períodos de mayor actividad

#### Reportes de Proveedores
- Proveedores con más compras
- Montos totales por proveedor
- Frecuencia de compras

## 🔍 Estructura de Archivos

### Archivos de Datos (Generados Automáticamente)
```
datos/
|-- tienda.bin              # Información principal de la tienda
|-- productos.bin           # Catálogo de productos
|-- proveedores.bin         # Registro de proveedores
|-- clientes.bin            # Base de datos de clientes
|-- transacciones.bin       # Historial de transacciones
|-- detallesTransacciones.bin # Detalles de cada transacción
|-- backup.bin              # Copia de seguridad
`-- [otros archivos .bin]   # Archivos temporales y de sistema
```

### Formato de Archivos Binarios
- **Headers:** Metadatos al inicio de cada archivo
- **Registros:** Datos estructurados con tamaño fijo
- **Acceso Aleatorio:** Posicionamiento directo para búsquedas rápidas
- **Borrado Lógico:** Marcado de eliminación sin pérdida física

## 🛠️ Resolución de Problemas

### Problemas Comunes

#### Error de Compilación
```bash
# Verificar versión de C++
g++ --version  # Debe ser C++17 o superior

# Instalar dependencias si es necesario
# Ubuntu/Debian: sudo apt-get install build-essential
# Windows: Instalar MinGW o Visual Studio Build Tools
```

#### Error de Ejecución
```bash
# Verificar permisos en Linux/macOS
chmod +x TiendaJsportV3

# Crear directorio de datos manualmente
mkdir datos
```

#### Problemas con Archivos
- **Permisos:** Asegurar acceso de escritura en la carpeta
- **Espacio:** Verificar espacio disponible en disco
- **Corrupción:** Usar backup para restaurar datos

### Mensajes de Error Comunes

| Error | Causa | Solución |
|--------|-------|----------|
| "No se pudo crear el archivo" | Permisos insuficientes | Ejecutar como administrador o cambiar carpeta |
| "Error: El código ya existe" | Código duplicado | Usar un código diferente |
| "Stock no disponible" | Inventario insuficiente | Reabastecer producto o reducir cantidad |
| "El proveedor no existe" | ID inválido | Verificar ID o registrar proveedor |

## 📚 Referencia Técnica

### Principios de POO Aplicados

#### Encapsulamiento
```cpp
class Producto {
private:
    int id;
    char nombre[100];
    float precio;
public:
    int getId() const;           // Getter con const
    void setPrecio(float precio); // Setter con validación
};
```

#### Abstracción
```cpp
producto.registrarVenta(10);  // No necesitamos saber cómo se actualiza el stock
producto.estaBajoStockMinimo(); // Oculta la lógica de comparación
```

#### Templates para Persistencia
```cpp
template<typename T>
bool guardarRegistro(const char* archivo, T& registro);
// Funciona con Producto, Proveedor, Cliente, etc.
```

### Validaciones Implementadas

#### Formatos de Datos
- **Email:** usuario@dominio.extension
- **RIF:** J-12345678-9, V-12345678
- **Teléfono:** 0414-1234567, +584141234567
- **Fecha:** DD/MM/AAAA
- **Cédula:** V-12345678, E-12345678

#### Validaciones de Negocio
- **Stock:** No puede ser negativo
- **Precio:** Debe ser positivo
- **Códigos:** Únicos dentro de su dominio
- **Relaciones:** Integridad referencial

## 🤝 Contribución

### Estructura para Contribuir
1. **Crear rama:** `git checkout -b feature/nueva-funcionalidad`
2. **Desarrollar:** Seguir patrones de arquitectura existentes
3. **Probar:** Verificar compilación y funcionamiento
4. **Documentar:** Actualizar README y comentarios
5. **Integrar:** Pull request con descripción detallada

### Convenciones de Código
- **Nombres:** PascalCase para clases, camelCase para métodos
- **Comentarios:** Explicar lógica de negocio compleja
- **Validaciones:** Siempre validar entrada de usuario
- **Errores:** Manejar casos de error gracefully

## 📄 Licencia

Este proyecto es desarrollado como parte de la asignatura Programación 2 de la Universidad Rafael Urdaneta.

**Autores:** Luis Novoa & Ing. Vanessa Perez.
**Profesor:** Ing. Victor Kneider  
**Versión:** 3.0 - POO y Modularización  
**Año:** 2026

---

**Nota:** Esta versión representa una evolución significativa del proyecto original, aplicando principios modernos de diseño de software y buenas prácticas de programación.
