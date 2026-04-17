# Makefile actualizado para ProyectoTienda_v3

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -O2
TARGET = TiendaJsportV3

SRCDIR = .
OBJDIR = obj
DATADIR = datos

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

OBJECTS = $(patsubst %.cpp,$(OBJDIR)/%.obj,$(SOURCES))

all: directories $(TARGET)

# Solo crea las carpetas base
directories:
	@if not exist $(OBJDIR) mkdir $(OBJDIR)
	@if not exist $(DATADIR) mkdir $(DATADIR)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "=== Compilación exitosa ==="

# REGLA CLAVE: Crea la subcarpeta necesaria antes de compilar el .obj
$(OBJDIR)/%.obj: %.cpp
	@if not exist $(dir $@) mkdir $(subst /,\,$(dir $@))
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@if exist $(TARGET).exe del $(TARGET).exe
	@if exist $(OBJDIR) rmdir /s /q $(OBJDIR)
	@echo "=== Limpieza completada ==="

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run directories