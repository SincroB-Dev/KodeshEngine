#include "sceneManager.h"

SceneManager::SceneManager(int& width, int& height, int& mouseX, int& mouseY): editor(this),
    width(width), height(height), mouseX(mouseX), mouseY(mouseY)
{
    if (sceneList.size() == 0)
    {
        Scene *scene = new Scene(width, height);
        sceneList["Cena"] = scene;
        activeScene = scene;
    }

    camera = new Camera2D("_scene_manager_cam", width, height);
}

SceneManager::~SceneManager()
{
    activeScene = nullptr;

    for (auto it = sceneList.begin(); it != sceneList.end();)
    {
        delete it->second;
        it->second = nullptr;
        it++;
    }

    delete camera;
    camera = nullptr;
}

void SceneManager::UpdateWhenEvent()
{
    if (activeScene->activeObject != nullptr)
    {
        editor.gizmos->SetTransform(&activeScene->activeObject->transform);
    }

    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {
        editor.HandleMouseClick();
    }

    GetActiveCamera()->ApplyScreenToWorld(mouseX, mouseY);
    GetActiveCamera()->Update();

    editor.HandleEvents(mouseX, mouseY);
}

void SceneManager::Update()
{
    activeScene->Update();
    
    // Atualizações do editor, devem ser removidas in game
    editor.Update();
    editor.camHandler->Apply(*GetActiveCamera(), mouseX, mouseY);
}

void SceneManager::Render()
{
    auto viewMatrix = GetActiveCamera()->GetViewProjection();

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(&viewMatrix.m[0][0]);
    
    editor.grid->Draw(); // Desenho do grid
    activeScene->Render(); // Renderização da cena ativa
    editor.gizmos->Render(); // Renderização dos gizmos da cena
}
