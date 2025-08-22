# KodeshEngine

## Nova arquitetura de pastas

KodeshEngine/
|
+-- Core/                  # Núcleo da engine (base independente do jogo)
|     |--- Application/    # Inicialização, loop principal, pipeline
|     |--- ECS/            # Entity + Component System (entidades e seus dados)
|     |--- Events/         # Eventos (window, custom events)
|     |--- Input/          # Gerenciamento de entradas (input de mouse, teclado, joy)
|     |--- Scene/          # Relacionamentos expressos com a scene
|     |--- Renderer/       # Abstração do render (SDL/OpenGL)
|     |--- Resources/      # Gerenciamento de assets (texturas, shaders, fontes)
|     |--- Systems/        # Gerenciadores específicos (UI, Physics, Audio)
|     +--- Utils/          # Helpers, logging, math, time
|
+-- Platform/              # Código específico de plataforma
|     |--- SDL/            # Implementação SDL (janelas, input)
|     |--- OpenGL/         # Implementações de Renderer
|     +--- Threads/        # Worker threads, jobs, parallelism
|
|-- Editor/                # Futuro: Editor de engine (UI separada do runtime)
|     |--- Panels/         # UI de cenas, hierarquias, inspector
|     +--- Tools/          # Gizmos e ferramentas de edição
|
|-- Game/                  # Onde o usuário final vai plugar o jogo
|     |--- Scripts/        # Scripts em Lua (caso decida colocar binds)
|     |--- Assets/         # Imagens, sons, shaders
|     +--- Scenes/         # Mapas, prefabs e entidades salvas
|
+-- libs/                  # Dependencias externas, devem ser compiladas com o projeto
|
+-- assets/                # Assets da engine (fontes, imagens, sons)


## Trabalhar em um pipeline de renderização limpo

1. Update -> Lógica do jogo, input, física.
2. Scene Collect -> Percorre entidades visiveis e gera render commands.
3. Sort -> Organiza por shader, textura (para reduzir state changes)
4. Submit -> Envia comandos para renderer
5. Renderer -> Executa draw calls.
6. UI Layer -> Desenha UI por ultimo.
7. Present -> Swap de buffers.


## 📦 Compilação e Execução

**Requisitos:**
- C++17 ou superior
- Lua, Sol2
- SDL2
- OpenGL 1.4
- Dear ImGui
- CMake (opcional, dependendo do setup)

## 👤 Autor
Desenvolvido por Vinicius Okami
Projeto em constante evolução 🚧
