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

void SceneManager::CameraControls()
{
    // Implementar o controle de camera.
}

void SceneManager::Update()
{
    if (activeScene->activeObject != nullptr)
    {
        gizmos->SetTransform(&activeScene->activeObject->transform);
    }

    GetActiveCamera()->ApplyScreenToWorld(mouseX, mouseY);
    GetActiveCamera()->Update();

    CameraControls();

    gizmos->Update();
}

void SceneManager::Render()
{
    auto viewMatrix = GetActiveCamera()->GetViewProjection();

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(&viewMatrix.m[0][0]);
    
    activeScene->Render();
    gizmos->Render();
}
