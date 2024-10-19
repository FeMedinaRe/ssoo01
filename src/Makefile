# Variables
CXX = g++
CXXFLAGS = -std=c++11 -pthread -Wall
TARGET = carrera

# Compilación
all: $(TARGET)

$(TARGET): carrera.cpp
	$(CXX) $(CXXFLAGS) carrera.cpp -o $(TARGET)

# Limpiar archivos binarios
clean:
	rm -f $(TARGET)
