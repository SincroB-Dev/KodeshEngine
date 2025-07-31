#include "sceneManager.h"

SceneManager::SceneManager(int &width, int &height)
{
    mainObject = new GameObject("player");
    mainObject->AttachShape(new Shape2DStar(mainObject->transform, mainObject->color));

    scene = new Scene(width, height);
    scene->AddObject(mainObject);
}

SceneManager::~SceneManager()
{
    delete scene;
    delete mainObject;
}

void SceneManager::Update()
{
    scene->mainCamera->Update();
}

void SceneManager::Render()
{
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(&scene->mainCamera->GetViewProjection().m[0][0]);
    
    scene->render();
}
