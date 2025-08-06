# Compilador
CXX = g++ -g

# Diretórios de código-fonte
SRC_DIRS = src src/ui src/ui/sidedock src/core src/utils/serializer src/models/shapes src/maths src/core/editor
SRC = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.cpp))

# Bibliotecas externas (IMGUI e similares)
INCLUDE_SRC = \
    libs/imgui/*.cpp \
    libs/imgui/backends/imgui_impl_sdl2.cpp \
    libs/imgui/backends/imgui_impl_opengl2.cpp \
    libs/ImGuiFileDialog/ImGuiFileDialog.cpp
INCLUDES = $(wildcard $(INCLUDE_SRC))

# Arquivo final
OUT = build/app

# Flags comuns
CFLAGS = -Wall -std=c++17 -DIMGUI_IMPL_OPENGL_LOADER_GLAD

# Detectar plataforma
UNAME_S := $(shell uname -s)

ifeq ($(OS),Windows_NT)
    # Windows via cmd/powershell
    SDL_INCLUDE = -Ic:/dev/libs/SDL2/include
    SDL_LIBDIR  = -Lc:/dev/libs/SDL2/lib
    SDL_LIB     = -lmingw32 -lSDL2main -lSDL2
    GL_LIB      = -lopengl32 -lglu32 -Wl,-subsystem,console
    LIBS_EXTRA  =
else ifneq (,$(findstring MINGW, $(UNAME_S)))
    # Windows via MSYS2/MinGW
    SDL_INCLUDE = -Ic:/dev/libs/SDL2/include
    SDL_LIBDIR  = -Lc:/dev/libs/SDL2/lib
    SDL_LIB     = -lmingw32 -lSDL2main -lSDL2
    GL_LIB      = -lopengl32 -lglu32 -Wl,-subsystem,console
    LIBS_EXTRA  =
else
    # Linux (Ubuntu etc.)
    SDL_INCLUDE = `sdl2-config --cflags`
    SDL_LIBDIR  =
    SDL_LIB     = `sdl2-config --libs`
    GL_LIB      = -lGL -lGLU -ldl
    LIBS_EXTRA  = -pthread
endif

# Regra principal
all:
	$(CXX) $(CFLAGS) $(SDL_INCLUDE) $(SRC) $(INCLUDES) -o $(OUT) $(SDL_LIBDIR) $(SDL_LIB) $(GL_LIB) $(LIBS_EXTRA)
