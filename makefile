SRC_DIR = src
BUILD_DIR = bin/debug
CC = g++
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_NAME = oneLiner.exe
INCLUDE_PATHS = -Iinclude
LIBRARY_PATHS = -Llib/SDL2
COMPILER_FLAGS = -std=c++17 -Wall -O0 -g
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2
#idk why, but LIBRARY_PATHS break the build
#they go after compiler flags
#linker flags go after src files

all:
	$(CC) $(COMPILER_FLAGS) $(SRC_FILES) $(LINKER_FLAGS) $(INCLUDE_PATHS) -o $(BUILD_DIR)/$(OBJ_NAME)

run:
	$(all)
	./$(BUILD_DIR)/$(OBJ_NAME)