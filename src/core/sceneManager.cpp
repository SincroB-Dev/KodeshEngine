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
    //if (event.key.keysym.sym == SDLK_LCTRL)
    {
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
        {
            if (!dragCam)
            {
                dragCam = true;
                dragStartMouse = vec2(mouseX, mouseY);
            }
        }
        else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
        {
            dragCam = false;
        }
    }

    if (dragCam)
    {
        vec2 delta = vec2(mouseX, mouseY) - dragStartMouse;

        camera->position.x -= delta.x / 100.0f;
        camera->position.y += delta.y / 100.0f;

        dragStartMouse = vec2(mouseX, mouseY);
    }
    else
    {

    }
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
