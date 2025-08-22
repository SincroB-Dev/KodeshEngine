# Compilador
CXX = g++
CXXFLAGS = -g -O0 -Wall -std=c++17 -Iincludes -DIMGUI_IMPL_OPENGL_LOADER_GLAD -MMD -MP

# Diretórios de código-fonte
SRC_DIRS = src \
             src/Core/Application \
             src/Core/Events \
             src/Core/Input \
             src/Core/Platform \
             src/Core/ECS \
             src/Core/Renderer \
             src/Core/Resource \
             src/Core/Scene \
             src/Core/Systems \
             src/Core/Utils \
           src/Platform/ \
             src/Platform/OpenGL \
             src/Platform/SDL \
             src/Platform/Threads \
           src/Game \
             src/Game/Assets \
             src/Game/Scenes \
             src/Game/Scripts \
            src/Editor \
             src/Editor/Panels \
             src/Editor/Tools

SRC = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.cpp))

# Bibliotecas externas (IMGUI e similares)
INCLUDE_SRC = \
    libs/imgui/*.cpp \
    libs/imgui/backends/imgui_impl_sdl2.cpp \
    libs/imgui/backends/imgui_impl_opengl2.cpp \
    libs/ImGuiFileDialog/ImGuiFileDialog.cpp \
    libs/ImGuiColorTextEdit/TextEditor.cpp
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
    LUA_LIBDIR  = -Llibs/lua
    LUA_LIB     = -llua -Ilibs -ldl
else ifneq (,$(findstring MINGW, $(UNAME_S)))
    SDL_INCLUDE = -Ic:/dev/libs/SDL2/include
    SDL_LIBDIR  = -Lc:/dev/libs/SDL2/lib
    SDL_LIB     = -lmingw32 -lSDL2main -lSDL2
    GL_LIB      = -lopengl32 -lglu32 -Wl,-subsystem,console
    LIBS_EXTRA  =
    LUA_LIBDIR  = -Llibs/lua
    LUA_LIB     = -llua -Ilibs -ldl
else
    SDL_INCLUDE = `sdl2-config --cflags` -Ilibs
    SDL_LIBDIR  =
    SDL_LIB     = `sdl2-config --libs` -Ilibs
    GL_LIB      = -lGL -lGLU -ldl
    LIBS_EXTRA  = -pthread
    LUA_LIBDIR  = -Llibs/lua
    LUA_LIB     = -llua -ldl
endif

# Transformar todos os .cpp em .o dentro de build/
OBJ_SRC = $(SRC:%.cpp=build/%.o)
OBJ_INCLUDES = $(INCLUDES:%.cpp=build/%.o)

DEPS =  $(OBJ_SRC:.o=.d) $(OBJ_INCLUDES:.o=.d)

# Regra principal
all: $(OUT)

# Link final
$(OUT): $(OBJ_SRC) $(OBJ_INCLUDES)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(SDL_INCLUDE) $^ -o $@ $(SDL_LIBDIR) $(SDL_LIB) $(GL_LIB) $(LIBS_EXTRA) $(LUA_LIB) $(LUA_LIBDIR)

# Compilação de cada .cpp para .o
build/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(SDL_INCLUDE) -c $< -o $@

# Limpeza
clean:
	rm -rf build
	
run:
	cd build;./app;cd ..
	
debug:
	cd build;gdb ./app;cd ..

# Incluí dependencias geradas pelo compilador
-include $(DEPS)
