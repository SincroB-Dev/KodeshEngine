# Simple2DEngine

Uma engine 2D modular e em evolução, construída do zero com SDL2, OpenGL 1.4 e ImGui. O objetivo do projeto é servir como base de estudo e experimentação, com foco em desenvolvimento progressivo de uma engine gráfica moderna, começando pelas bases mais simples e avançando até tecnologias mais recentes como OpenGL moderno e possivelmente Vulkan.

---

## ✨ Objetivo

O projeto foi iniciado com a intenção de **explorar, entender e evoluir gradualmente** a criação de uma engine gráfica, com foco em aprendizado prático. A ideia é manter o código modular, acessível e orientado à construção de conhecimento, partindo de uma base simples até alcançar estruturas mais robustas e modernas.

---

## 🚀 Tecnologias Utilizadas

- **Linguagem:** C++
- **Renderização:** OpenGL 1.4
- **Janela e eventos:** SDL2
- **Interface de usuário:** Dear ImGui

---

## 🧱 Estrutura Atual

### 🔹 Core
- **GameObject**: Classe base responsável pela lógica de objetos na cena. Planeja-se substituí-la futuramente por um sistema de entidades (`Entity`) mais flexível.
- **Scene**: Classe responsável por controlar a renderização e update de componentes.
- **SceneManager**: Classe responsável pelo gerenciamento das cenas em atividade no loop principal.
- **Transformer2D**: Gerencia posição, rotação e escala (localPosition, localAngle, localScale) com base em um `Point`.

### 🔹 Shapes
Sistema de modelagem 2D simples baseado na classe abstrata `Shape2D`, com as seguintes formas já implementadas:

- `Circle`
- `Quad`
- `Star`

### 🔹 Matemática
Biblioteca interna de vetores e pontos para suporte a transformações e geometria:

- `Vec2`, `Vec3`, `Vec4`
- `Point`
- `Transformer2D`

---

## 🛣️ Próximos Passos

- Refatorar `GameObject` para `Entity`, adotando um modelo baseado em componentes (ECS).
- Adicionar novos shapes 2D e suporte a sprites.
- Implementar sistema de cena com camadas.
- Evoluir para versões mais modernas do OpenGL (ex: 3.3+).
- Implementar suporte a shaders customizados.
- Adicionar integração com física 2D (como Box2D).
- Melhorar ferramentas de depuração via ImGui.

---

## 📦 Compilação e Execução

**Requisitos:**
- C++17 ou superior
- SDL2
- OpenGL 1.4
- Dear ImGui
- CMake (opcional, dependendo do setup)

## 👤 Autor
Desenvolvido por Vinicius Okami
Projeto em constante evolução 🚧

**Instruções básicas:**
Deve-se incluir o projeto imgui e o projeto ImGuiFileDialog na pasta libs.

```bash
# Compilação (exemplo com g++)
CXX = g++ -pipe
SRC = src/*.cpp src/ui/*.cpp src/core/*.cpp src/utils/serializer/*.cpp src/models/shapes/*.cpp src/maths/*.cpp
INCLUDES =  libs/imgui/*.cpp libs/imgui/backends/imgui_impl_sdl2.cpp libs/imgui/backends/imgui_impl_opengl2.cpp libs/ImGuiFileDialog/ImGuiFileDialog.cpp
OUT = build/app
CFLAGS = -Wall -std=c++17
LIBS = -lSDL2 -ldl -lGL -lGLU -lstdc++ -DIMGUI_IMPL_OPENGL_LOADER_GLAD

all:
	$(CXX) $(SRC) $(INCLUDES) -o $(OUT) $(FLAGS) $(LIBS)