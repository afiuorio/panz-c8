SOURCES = $(wildcard src/*.c)
OBJECTS=$(SOURCES:.c=.o)
COMPILER_FLAGS = -c -O2 -std=c99
EXECUTABLE = panz-c8

CC = clang
INCLUDE_PATHS =
LIBRARY_PATHS =
LINKER_FLAGS = -framework SDL2

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LIBRARY_PATHS) $(LINKER_FLAGS) $(OBJECTS) -o $@
.c.o:
	$(CC) $(INCLUDE_PATHS) $(COMPILER_FLAGS) $< -o $@

clean:
	@rm src/*.o
