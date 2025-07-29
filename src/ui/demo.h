ImGui::Begin("Demo ImGui - Componentes");

// Botão
if (ImGui::Button("Clique aqui")) {
    // ação
}

// Checkbox
static bool check = true;
ImGui::Checkbox("Ativar modo teste", &check);

// Slider
static float slider = 0.5f;
ImGui::SliderFloat("Brilho", &slider, 0.0f, 1.0f);

// Drag float
ImGui::DragFloat("Velocidade", &slider, 0.01f, 0.0f, 10.0f);

// Input text
static char texto[128] = "Texto aqui";
ImGui::InputText("Entrada de texto", texto, IM_ARRAYSIZE(texto));

// Input number
static int contador = 0;
ImGui::InputInt("Quantidade", &contador);

// Radio buttons
static int modo = 0;
ImGui::RadioButton("Modo A", &modo, 0); ImGui::SameLine();
ImGui::RadioButton("Modo B", &modo, 1);

// Combo box
static const char* itens[] = { "Opção 1", "Opção 2", "Opção 3" };
static int itemSelecionado = 0;
ImGui::Combo("Seleção", &itemSelecionado, itens, IM_ARRAYSIZE(itens));

// Color picker
static ImVec4 cor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
ImGui::ColorEdit4("Cor", (float*)&cor);

// Tree
if (ImGui::TreeNode("Avançado")) {
    ImGui::Text("Conteúdo interno...");
    ImGui::Checkbox("Ativar recurso", &check);
    ImGui::TreePop();
}

// Tab bar
if (ImGui::BeginTabBar("Abas")) {
    if (ImGui::BeginTabItem("Aba 1")) {
        ImGui::Text("Conteúdo da Aba 1");
        ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Aba 2")) {
        ImGui::Text("Conteúdo da Aba 2");
        ImGui::EndTabItem();
    }
    ImGui::EndTabBar();
}

// Progress bar
ImGui::ProgressBar(slider);

// Separador
ImGui::Separator();

// Tooltip
ImGui::Text("Passe o mouse aqui");
if (ImGui::IsItemHovered())
    ImGui::SetTooltip("Tooltip de exemplo");

// Janela modal
static bool mostrarPopup = false;
if (ImGui::Button("Mostrar Popup")) {
    mostrarPopup = true;
    ImGui::OpenPopup("Aviso");
}

if (ImGui::BeginPopupModal("Aviso", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::Text("Você realmente deseja continuar?");
    if (ImGui::Button("Sim")) { mostrarPopup = false; ImGui::CloseCurrentPopup(); }
    ImGui::SameLine();
    if (ImGui::Button("Não")) { mostrarPopup = false; ImGui::CloseCurrentPopup(); }
    ImGui::EndPopup();
}

ImGui::End();
