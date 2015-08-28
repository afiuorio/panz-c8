OBJS = main.c opcode.c c8-cpu.c

ifeq ($(OS),Windows_NT)
CC = gcc
INCLUDE_PATHS = -IC:\mingw_dev_lib\include\SDL2
LIBRARY_PATHS = -LC:\mingw_dev_lib\lib
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2
else
CC = clang
INCLUDE_PATHS =
LIBRARY_PATHS =
LINKER_FLAGS = -framework SDL2
endif

COMPILER_FLAGS = -std=c99 -O2
OBJ_NAME = panc-8

all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
