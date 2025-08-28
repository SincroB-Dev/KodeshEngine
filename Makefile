# Compilador
CXX = g++
CXXFLAGS = -g -O0 -Wall -std=c++17 -Iprogram/includes -DIMGUI_IMPL_OPENGL_LOADER_GLAD -MMD -MP

# Diretórios de código-fonte
SRC_DIRS = program/source \
             program/source/Core/Application \
             program/source/Core/Events \
             program/source/Core/Input \
             program/source/Core/Platform \
             program/source/Core/Helpers \
             program/source/Core/Maths \
             program/source/Core/ECS \
             program/source/Core/Renderer \
             program/source/Core/Resource \
             program/source/Core/Scene \
             program/source/Core/Systems \
             program/source/Core/Utils \
           program/source/Platform/ \
             program/source/Platform/OpenGL \
             program/source/Platform/SDL \
             program/source/Platform/Threads \
           program/source/Game \
             program/source/Game/Assets \
             program/source/Game/Scenes \
             program/source/Game/Scripts \
            program/source/Editor \
             program/source/Editor/Windows \
               program/source/Editor/Windows/UINodeEditor \
                 program/source/Editor/Windows/UINodeEditor/Nodes \
             program/source/Editor/Panels \
             program/source/Editor/UI \
               program/source/Editor/UI/Fontes \
             program/source/Editor/Tools

SRC = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.cpp))

# Bibliotecas externas (IMGUI e similares)
INCLUDE_SRC = \
    libs/imgui/*.cpp \
    libs/imgui/backends/imgui_impl_sdl2.cpp \
    libs/imgui/backends/imgui_impl_opengl2.cpp \
    libs/ImGuiFileDialog/ImGuiFileDialog.cpp \
    libs/ImGuiNodeEditor/*.cpp \
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
