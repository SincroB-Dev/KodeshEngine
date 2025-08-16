#include "sceneManager.h"

SceneManager::SceneManager(int& width, int& height, int& mouseX, int& mouseY, SDL_Event& event)
    : editor(*this), width(width), height(height), mouseX(mouseX), mouseY(mouseY), event(event),
     EngineEditMode(true)
{
    if (sceneList.size() == 0)
    {
        Scene *scene = new Scene(width, height);
        sceneList["Cena"] = scene;
        activeScene = scene;
    }

    camera = new Camera2D("_scene_manager_cam", width, height);
}

SceneManager::SceneManager(SceneManager& clone)
    : editor(*this), width(clone.width), height(clone.height), mouseX(clone.mouseX), mouseY(clone.mouseY), event(clone.event),
     EngineEditMode(true)
{}

SceneManager::~SceneManager()
{
    activeScene = nullptr;

    for (auto it = sceneList.begin(); it != sceneList.end();)
    {
        delete it->second;
        it->second = nullptr;
        it++;
    }

    if (camera != nullptr)
    {
        delete camera;
        camera = nullptr;
    }
}

void SceneManager::UpdateWhenEvent()
{
    if (EngineEditMode)
    {
        if (activeScene->activeObject != nullptr)
        {
            editor.gizmos->SetTransform(&activeScene->activeObject->transform);
        }

        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
        {
            editor.HandleMouseClick();
        }
        
        editor.HandleEvents(mouseX, mouseY);
    }
    
    GetActiveCamera()->ApplyScreenToWorld(mouseX, mouseY);
    GetActiveCamera()->Update();
}

void SceneManager::Update()
{
    activeScene->Update();

    // Atualizações do editor, devem ser removidas in game
    if (EngineEditMode)
    {
        editor.Update();
        editor.camHandler->Apply(*GetActiveCamera(), mouseX, mouseY);
    }
}

SceneManager* SceneManager::Clone()
{
    SceneManager* copy = new SceneManager(*this);
    
    for (auto it=sceneList.begin(); it!=sceneList.end(); it++)
    {
        copy->sceneList[std::string(it->first).c_str()] = it->second->Clone();
        
        if (activeScene->id == it->second->id)
        {
            copy->activeScene = copy->sceneList[std::string(it->first)];
        }
    }
    
    copy->camera = nullptr;
    
    return copy;
}

const char* SceneManager::GetActiveSceneName() const
{
    for (auto it=sceneList.begin(); it!=sceneList.end(); it++)
    {
        if (it->second->id == activeScene->id)
        {
            return std::string(it->first).c_str();
        }
    }
    
    return "<no_scene>";
}

void SceneManager::Render()
{
    auto viewMatrix = GetActiveCamera()->GetViewProjection();

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(&viewMatrix.m[0][0]);
    
    if (EngineEditMode)
    {
        editor.grid->Draw(); // Desenho do grid
        activeScene->Render(); // Renderização da cena ativa
        editor.gizmos->Render(); // Renderização dos gizmos da cena
    }
    else
    {
        activeScene->Render();
    }
}
