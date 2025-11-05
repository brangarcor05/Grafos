# Compilador y flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -O2

# Archivos fuente y objeto
SOURCES = main.cpp Grafo.cxx
OBJECTS = $(SOURCES:.cpp=.o)
OBJECTS := $(OBJECTS:.cxx=.o)

# Nombre del ejecutable
TARGET = main

# Regla por defecto
all: $(TARGET)

# Compilar el ejecutable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Compilar archivos .cpp a .o
%.o: %.cpp Grafo.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compilar archivos .cxx a .o
%.o: %.cxx Grafo.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpiar archivos compilados
clean:
	rm -f $(OBJECTS) $(TARGET)

# Limpiar y recompilar
rebuild: clean all

# Ejecutar el programa
run: $(TARGET)
	./$(TARGET)

# Reglas que no son archivos
.PHONY: all clean rebuild run