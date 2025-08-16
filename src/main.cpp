#define SDL_MAIN_HANDLED

#include "controllers/kodeshController.h"

using namespace core::controller;

int main(int argc, char *argv[])
{
    KodeshController* kodesh = new KodeshController("Kodesh Engine v1.0: SDL + OpenGL 1.4", 950, 500);
    
    kodesh->InitManagers();
    kodesh->InitControllers();
    
    kodesh->MainLoop();
    kodesh->Destroy();
    
    delete kodesh;
    
    return 0;
}
