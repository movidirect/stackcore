# Copyright (C) 2023-2025 Jose R Arenas

# Compilers
CXX_LINUX = g++
CXX_WIN = x86_64-w64-mingw32-g++
WINDRES = x86_64-w64-mingw32-windres

# Base flags
IMGUI_DIR = imgui
BASE_CXXFLAGS = -Wall -Wextra -std=c++17 -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends -DNO_FONT_AWESOME -O2 -DNDEBUG -Wformat -Wformat-security -D_FORTIFY_SOURCE=2

# Linux specific
RAYLIB_CFLAGS_LINUX = $(shell pkg-config --cflags raylib 2>/dev/null || echo "-I/usr/local/include")
RAYLIB_LIBS_LINUX = $(shell pkg-config --libs raylib 2>/dev/null || echo "-lraylib")
CXXFLAGS_LINUX = $(BASE_CXXFLAGS) -fstack-protector-strong $(RAYLIB_CFLAGS_LINUX)
LDLIBS_LINUX = $(RAYLIB_LIBS_LINUX) -lGL -lm -lpthread -ldl -lrt -lX11
TARGET_LINUX = Output/stackcore

# Windows specific
LIB_DIR_WIN = lib/raylib
INCLUDE_DIR_WIN = include/raylib
CXXFLAGS_WIN = $(BASE_CXXFLAGS) -I$(INCLUDE_DIR_WIN) -L$(LIB_DIR_WIN) -static -static-libgcc -static-libstdc++
LDLIBS_WIN = -lraylib -lopengl32 -lgdi32 -lwinmm -mwindows -lpthread
TARGET_WIN = Output/stackcore.exe
WIN_RESOURCE_FILE = obj/windows/resource.res
WIN_RESOURCE_SRC = resource.rc

# Sources
DEPS = $(wildcard src/*.h)
SRC = $(wildcard src/*.cpp)
SRC += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SRC += $(IMGUI_DIR)/rlImGui.cpp

# Object files mapped to separate directories
OBJ_LINUX = $(SRC:%.cpp=obj/linux/%.o)
OBJ_WIN = $(SRC:%.cpp=obj/windows/%.o) $(WIN_RESOURCE_FILE)

# Default target
all: linux windows

linux: $(TARGET_LINUX)
windows: $(TARGET_WIN)

# Link Linux executable
$(TARGET_LINUX): $(OBJ_LINUX)
	@mkdir -p $(dir $@)
	$(CXX_LINUX) $(CXXFLAGS_LINUX) -o $@ $^ $(LDLIBS_LINUX)

# Link Windows executable
$(TARGET_WIN): $(OBJ_WIN)
	@mkdir -p $(dir $@)
	$(CXX_WIN) $(CXXFLAGS_WIN) -o $@ $^ $(LDLIBS_WIN)

# Compile Linux objects
obj/linux/%.o: %.cpp $(DEPS)
	@mkdir -p $(dir $@)
	$(CXX_LINUX) $(CXXFLAGS_LINUX) -c $< -o $@

# Compile Windows objects
obj/windows/%.o: %.cpp $(DEPS)
	@mkdir -p $(dir $@)
	$(CXX_WIN) $(CXXFLAGS_WIN) -c $< -o $@

# Windows specific rule for resource compilation
$(WIN_RESOURCE_FILE): $(WIN_RESOURCE_SRC)
	@mkdir -p $(dir $@)
	$(WINDRES) $< -O coff -o $@

# Clean up
clean:
	rm -rf obj $(TARGET_LINUX) $(TARGET_WIN)

.PHONY: all clean linux windows