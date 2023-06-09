CC = gcc
CFLAGS = -Wall -Wextra

# Lista de archivos fuente (.c) en la carpeta
SOURCES = $(wildcard *.c)

# Genera los nombres de los archivos objeto (.o) a partir de los archivos fuente
OBJECTS = $(SOURCES:.c=.o)

# Nombre del ejecutable final
TARGET = program

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: clean


