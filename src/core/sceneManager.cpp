#include "sceneManager.h"

SceneManager::SceneManager()
{
    mainObject = new GameObject("player");
    mainObject->AttachShape(new Shape2DStar(mainObject->transform, mainObject->color));

    scene = new Scene();
    scene->AddObject(mainObject);
}

SceneManager::~SceneManager()
{
    delete scene;
    delete mainObject;
}

void SceneManager::Render()
{
    scene->render();
}
