#include "sceneManager.h"

SceneManager::SceneManager(int &width, int &height, int &mouseX, int &mouseY): 
    width(width), height(height), mouseX(mouseX), mouseY(mouseY)
{
    if (sceneList.size() == 0)
    {
        Scene *scene = new Scene(width, height);
        sceneList["Cena"] = scene;
        activeScene = scene;
    }

    camera = new Camera2D("_scene_manager_cam", width, height);
    gizmos = new Gizmo2D(this);
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

    if (gizmos != nullptr)
    {
        delete gizmos;
        gizmos = nullptr;
    }
}

void SceneManager::Update()
{
    gizmos->SetTransform(&activeScene->activeObject->transform);

    GetActiveCamera()->ApplyScreenToWorld(mouseX, mouseY);
    GetActiveCamera()->Update();
    gizmos->Update();
}

void SceneManager::Render()
{
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(&GetActiveCamera()->GetViewProjection().m[0][0]);
    
    activeScene->Render();
    gizmos->Render();
}
