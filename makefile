SOURCES = $(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
COMPILER_FLAGS = -c -O2 -std=c99
EXECUTABLE = panc-8

CC = clang
INCLUDE_PATHS = -I/Library/Frameworks/SDL2.framework/Headers
LIBRARY_PATHS = -F/Library/Frameworks
LINKER_FLAGS = -framework SDL2

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LIBRARY_PATHS) $(LINKER_FLAGS) $(OBJECTS) -o $@
.c.o:
	$(CC) $(INCLUDE_PATHS) $(COMPILER_FLAGS) $< -o $@

clean:
	@rm *.o
