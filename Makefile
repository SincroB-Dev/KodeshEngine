# Compilador
CXX = g++
CXXFLAGS = -g -Wall -std=c++17 -DIMGUI_IMPL_OPENGL_LOADER_GLAD

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

# Detectar plataforma
UNAME_S := $(shell uname -s)

ifeq ($(OS),Windows_NT)
    SDL_INCLUDE = -Ic:/dev/libs/SDL2/include
    SDL_LIBDIR  = -Lc:/dev/libs/SDL2/lib
    SDL_LIB     = -lmingw32 -lSDL2main -lSDL2
    GL_LIB      = -lopengl32 -lglu32 -Wl,-subsystem,console
    LIBS_EXTRA  =
else ifneq (,$(findstring MINGW, $(UNAME_S)))
    SDL_INCLUDE = -Ic:/dev/libs/SDL2/include
    SDL_LIBDIR  = -Lc:/dev/libs/SDL2/lib
    SDL_LIB     = -lmingw32 -lSDL2main -lSDL2
    GL_LIB      = -lopengl32 -lglu32 -Wl,-subsystem,console
    LIBS_EXTRA  =
else
    SDL_INCLUDE = `sdl2-config --cflags`
    SDL_LIBDIR  =
    SDL_LIB     = `sdl2-config --libs`
    GL_LIB      = -lGL -lGLU -ldl
    LIBS_EXTRA  = -pthread
endif

# Transformar todos os .cpp em .o dentro de build/
OBJ_SRC = $(SRC:%.cpp=build/%.o)
OBJ_INCLUDES = $(INCLUDES:%.cpp=build/%.o)

# Regra principal
all: $(OUT)

# Link final
$(OUT): $(OBJ_SRC) $(OBJ_INCLUDES)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(SDL_INCLUDE) $^ -o $@ $(SDL_LIBDIR) $(SDL_LIB) $(GL_LIB) $(LIBS_EXTRA)

# Compilação de cada .cpp para .o
build/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(SDL_INCLUDE) -c $< -o $@

# Limpeza
clean:
	rm -rf build
