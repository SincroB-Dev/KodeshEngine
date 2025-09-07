# KodeshEngine

## Arquitetura de pastas da engine
```
KodeshEngine/
|
+-- Core/                  # Núcleo da engine (base independente do jogo)
|     |--- Application/    # Inicialização, loop principal, pipeline
|     |--- ECS/            # Entity + Component System (entidades e seus dados)
|     |----+ Systems/      # Systemas que manipulam dados de entidades
|     |--- Events/         # Eventos (window, custom events)
|     |--- Helpers/        # Métodos, operadores e classes que servem de apoio
|     |--- Input/          # Gerenciamento de entradas (input de mouse, teclado, joy)
|     |--- Renderer/       # Abstração do render (SDL/OpenGL)
|     |--- Resources/      # Gerenciamento de assets (texturas, shaders, fontes)
|     |--- Scene/          # Relacionamentos expressos com a scene
|     |--- Serialization/  # Controle de serialização de dados para save/load
|     |--- Systems/        # Gerenciadores específicos (UI, Physics, Audio)
|     +--- Utils/          # Helpers, logging, math, time
|
+-- Platform/              # Código específico de plataforma
|     |--- SDL/            # Implementação SDL (janelas, input)
|     |--- OpenGL/         # Implementações de Renderer
|     +--- Threads/        # Worker threads, jobs, parallelism
|
|-- Editor/                # Futuro: Editor de engine (UI separada do runtime)
|     |--- Panels/         # Paineis de controle da cena e ambiente
|     |--- UI/             # Componentes de interface
|     |--- Windows/        # Janelas e ferramentas isoladas
|     |----+ UINodeEditor/ # Ferramentas exclusivas do editor de nodes
|     +--- Tools/          # Gizmos e ferramentas de edição
|
|-- Game/                  # Onde o usuário final vai plugar o jogo
|     |--- Scripts/        # Scripts em Lua (caso decida colocar binds)
|     |--- Assets/         # Imagens, sons, shaders
|     +--- Scenes/         # Mapas, prefabs e entidades salvas
|
+-- libs/                  # Dependencias externas, devem ser compiladas com o projeto
|
+-- assets/                # Assets da engine, antes da compilação (fontes, imagens, sons)
```

## 🔗 Node Graph

Como seguiremos a partir deste momento para a criação de nodes funcionais na engine.

**1. Nodes diretos (binding de componente)**
	Esses servem como “entrada/saída” do grafo, permitindo editar ou consultar valores de componentes.

	- Transform Node → expõe position, rotation, scale.
	- Mesh Node → referencia uma malha já carregada e permite associar materiais.
	- Shape Node → gera primitivas (quad, circle, cube) sem precisar de mesh externa.
	- Lifetime Node → controla tempo de vida da entidade (spawn/kill).
	- Tag Node → útil para filtragem ou lógica condicional baseada em categorias.

**2. Nodes de utilidade (dados auxiliares)**
	Servem como operadores/conversores de valores.
	
	- Math Node (add, sub, mul, div, lerp).
	- Vector Node (constrói/fragmenta vec2/vec3).
	- Bool/Condition Node (switch, if, compare).
	- Time Node (deltaTime, totalTime).
	- Random Node (float, int, vec3).

**3. Nodes de fluxo/execução**
	Controlam lógica do grafo.

	- Event Node → OnUpdate, OnStart, OnDestroy.
	- Branch Node → fluxo condicional baseado em bool.
	- Sequence Node → executa várias saídas em ordem.

**4. Nodes de ambiente/engine**
	Começam a expandir além de dados crus.

	- Camera Node → manipula câmera (posição, proj).
	- Input Node → teclado, mouse, gamepad.
	- Physics Node (se for evoluir depois) → colisores, forças, velocidade.
	- Spawner Node → cria entidades com componentes específicos.

**5. Ordem de inicialização**

	- Event Node (OnUpdate / OnStart).
	- Input Node (para interações básicas).
	- Transform Node (base para tudo).
	- Math/Vector Nodes (apoio).

Com esses, já é possível ter um grafo que cria entidade, posiciona e movimenta — um ciclo mínimo funcional.

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
Projeto em desenvolvimento 🚧
