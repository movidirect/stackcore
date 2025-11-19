CXX = g++

# Base flags, without platform specifics
BASE_CXXFLAGS = -Wall -Wextra -std=c++17 -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends

IMGUI_DIR = imgui
DEPS = $(wildcard src/*.h)
SRC = $(wildcard src/*.cpp)

ifeq ($(OS),Windows_NT)

    LDLIBS = -lmingw32 -lSDL2main -lSDL2 -lopengl32 -lglu32 -lSDL2_image -lSDL2_mixer -mwindows
    LIB_DIR = lib/SDL2
    INCLUDE_DIR = include
    TARGET = Output/stackcore.exe
    CLEAN_CMD = powershell -Command "Remove-Item -Recurse -Force obj" && rm -f $(TARGET)
    PLATFORM_CXXFLAGS = -I$(INCLUDE_DIR) -I$(INCLUDE_DIR)/SDL2 -L$(LIB_DIR)

    # Windows specific resource compilation
    WIN_RESOURCE_FILE = resource.res
    WIN_RESOURCE_SRC = resource.rc
    WIN_SPECIFIC_OBJS = $(WIN_RESOURCE_FILE)

    # Rule to compile resource.rc into resource.res
    $(WIN_RESOURCE_FILE): $(WIN_RESOURCE_SRC)
	    windres $< -O coff -o $@
else
    # Use pkg-config to get SDL2 flags for non-Windows systems for more robustness
    SDL2_CFLAGS = $(shell pkg-config --cflags sdl2 SDL2_image SDL2_mixer)
    SDL2_LIBS = $(shell pkg-config --libs sdl2 SDL2_image SDL2_mixer)
    LDLIBS = $(SDL2_LIBS) -lGL -lGLU
    TARGET = Output/stackcore
    CLEAN_CMD = rm -rf obj && rm -f $(TARGET)
    PLATFORM_CXXFLAGS = $(SDL2_CFLAGS)
endif

# Common flags including platform specifics
COMMON_CXXFLAGS = $(BASE_CXXFLAGS) $(PLATFORM_CXXFLAGS)

# Release flags
RELEASE_CXXFLAGS = $(COMMON_CXXFLAGS) -O2 -DNDEBUG -fstack-protector-strong -Wformat -Wformat-security -D_FORTIFY_SOURCE=2

# Debug flags
DEBUG_CXXFLAGS = $(COMMON_CXXFLAGS) -g

# Default to release build
CXXFLAGS = $(RELEASE_CXXFLAGS)

OBJDIR = obj

# IMGUI sources
SRC += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SRC += $(IMGUI_DIR)/imgui_impl_sdl2.cpp $(IMGUI_DIR)/imgui_impl_opengl2.cpp


# Object files
# Change the .cpp extension to .o and place the objects in OBJDIR
ifeq ($(OS),Windows_NT)
    OBJ = $(SRC:%.cpp=$(OBJDIR)/%.o) $(WIN_SPECIFIC_OBJS)
else
    OBJ = $(SRC:%.cpp=$(OBJDIR)/%.o)
endif

# Default target
all: release

# Release target
release: CXXFLAGS = $(RELEASE_CXXFLAGS)
release: $(TARGET)

# Debug target
debug: CXXFLAGS = $(DEBUG_CXXFLAGS)
debug: $(TARGET)

# Rule to link the executable
$(TARGET): $(OBJ)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ) $(LDLIBS)

# Rule to compile each object file and ensure subdirectories exist
$(OBJDIR)/%.o: %.cpp $(DEPS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	$(CLEAN_CMD)

.PHONY: all clean release debug
