#ifndef SCENE_H
#define SCENE_H

#include "gameObject.h"
#include <vector>

class Scene final
{
public:
    Scene();
    ~Scene();

    GameObject *GetObject(const char *name);
    
    void AddObject(GameObject *object);
    GameObject *AddObject(const char *name, Shape2D *shape = NULL);

    void RemoveObject(const char *name);

    void render();

private:
    std::vector<GameObject*> objectList;
};

#endif