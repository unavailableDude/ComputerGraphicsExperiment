SRC_DIR = src
BUILD_DIR = bin/debug
CC = g++
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp) glad/src/glad.c $(wildcard imgui/*.cpp)
OBJ_NAME = oneLiner
INCLUDE_PATHS = -Iinclude -Iinclude/SDL2 -Iglad/include -Iinclude/imgui -Iglm
LIBRARY_PATHS = -Llib
LIBRARY_PATHS_LX = -Llib/SDL2 -ldl
COMPILER_FLAGS = -std=c++17 -Wall -O0 -g
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2
LINKER_FLAGS_LX = -lsdl2

#linker flags go after src files
lin:
	$(CC) $(COMPILER_FLAGS) $(SRC_FILES) $(LINKER_FLAGS_LX) $(INCLUDE_PATHS) $(LIBRARY_PATHS_LX) -o $(BUILD_DIR)/$(OBJ_NAME)

win:
	$(CC) $(COMPILER_FLAGS) $(SRC_FILES) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) -o $(BUILD_DIR)/$(OBJ_NAME).exe

run:
	./$(BUILD_DIR)/$(OBJ_NAME)