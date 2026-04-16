# Makefile for ProyectoTienda_v3
# Compilación multi-archivo del sistema de inventario Tienda JSport

# Configuración del compilador
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -O2
TARGET = TiendaJsportV3

# Directorios
SRCDIR = .
OBJDIR = obj
DATADIR = datos

# Archivos fuente por módulo
SOURCES = main.cpp \
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
          persistencia/GestorArchivos.cpp \
          persistencia/Constantes.cpp \
          utilidades/Validaciones.cpp \
          utilidades/Formatos.cpp \
          interfaz/Interfaz.cpp \
          reportes/EstadisticasVentas.cpp \
          mantenimiento/Backup.cpp \
          mantenimiento/IntegridadReferencial.cpp

# Archivos objeto
OBJECTS = $(SOURCES:.cpp=.obj)

# Regla principal
all: directories $(TARGET)

# Crear directorios necesarios
directories:
	@if not exist $(OBJDIR) mkdir $(OBJDIR)
	@if not exist $(DATADIR) mkdir $(DATADIR)

# Compilación del ejecutable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "=== Compilación exitosa ==="
	@echo "Ejecutable creado: $(TARGET)"
	@echo "Para ejecutar: ./$(TARGET)"

# Regla de compilación para archivos .cpp
%.obj: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpieza
clean:
	@if exist $(TARGET) del $(TARGET)
	@if exist $(OBJDIR) rmdir /s /q $(OBJDIR)
	@echo "=== Limpieza completada ==="

# Ejecutar el programa
run: $(TARGET)
	./$(TARGET)

# Debug
debug: CXXFLAGS += -DDEBUG -g3
debug: $(TARGET)

# Release
release: CXXFLAGS += -DNDEBUG -O3
release: clean $(TARGET)

# Ayuda
help:
	@echo "Comandos disponibles:"
	@echo "  make         - Compilar el proyecto"
	@echo "  make clean   - Limpiar archivos compilados"
	@echo "  make run     - Compilar y ejecutar"
	@echo "  make debug   - Compilar con símbolos de depuración"
	@echo "  make release - Compilar versión optimizada"
	@echo "  make help    - Mostrar esta ayuda"

# Dependencias específicas
main.obj: main.cpp persistencia/GestorArchivos.hpp tienda/Tienda.hpp interfaz/Interfaz.hpp
tienda/Tienda.obj: tienda/Tienda.cpp tienda/Tienda.hpp utilidades/Formatos.hpp
productos/Producto.obj: productos/Producto.cpp productos/Producto.hpp utilidades/Validaciones.hpp utilidades/Formatos.hpp
productos/operacionesProductos.obj: productos/operacionesProductos.cpp productos/operacionesProductos.hpp productos/Producto.hpp
proveedores/Proveedor.obj: proveedores/Proveedor.cpp proveedores/Proveedor.hpp utilidades/Validaciones.hpp utilidades/Formatos.hpp
proveedores/operacionesproveedor.obj: proveedores/operacionesproveedor.cpp proveedores/operacionesproveedor.hpp proveedores/Proveedor.hpp
clientes/Cliente.obj: clientes/Cliente.cpp clientes/Cliente.hpp utilidades/Validaciones.hpp utilidades/Formatos.hpp
clientes/operacionescliente.obj: clientes/operacionescliente.cpp clientes/operacionescliente.hpp clientes/Cliente.hpp
transacciones/Transaccion.obj: transacciones/Transaccion.cpp transacciones/Transaccion.hpp transacciones/DetalleTransaccion.hpp utilidades/Validaciones.hpp utilidades/Formatos.hpp
transacciones/DetalleTransaccion.obj: transacciones/DetalleTransaccion.cpp transacciones/DetalleTransaccion.hpp utilidades/Validaciones.hpp utilidades/Formatos.hpp
transacciones/operacionesTransacciones.obj: transacciones/operacionesTransacciones.cpp transacciones/operacionesTransacciones.hpp transacciones/Transaccion.hpp transacciones/DetalleTransaccion.hpp
persistencia/GestorArchivos.obj: persistencia/GestorArchivos.cpp persistencia/GestorArchivos.hpp persistencia/Constantes.hpp utilidades/Formatos.hpp
persistencia/Constantes.obj: persistencia/Constantes.cpp persistencia/Constantes.hpp
utilidades/Validaciones.obj: utilidades/Validaciones.cpp utilidades/Validaciones.hpp
utilidades/Formatos.obj: utilidades/Formatos.cpp utilidades/Formatos.hpp persistencia/Constantes.hpp
interfaz/Interfaz.obj: interfaz/Interfaz.cpp interfaz/Interfaz.hpp productos/operacionesProductos.hpp proveedores/operacionesproveedor.hpp clientes/operacionescliente.hpp transacciones/operacionesTransacciones.hpp persistencia/GestorArchivos.hpp utilidades/Formatos.hpp
reportes/EstadisticasVentas.obj: reportes/EstadisticasVentas.cpp reportes/EstadisticasVentas.hpp
mantenimiento/Backup.obj: mantenimiento/Backup.cpp mantenimiento/Backup.hpp persistencia/GestorArchivos.hpp
mantenimiento/IntegridadReferencial.obj: mantenimiento/IntegridadReferencial.cpp mantenimiento/IntegridadReferencial.hpp persistencia/GestorArchivos.hpp

.PHONY: all clean run debug release help directories
