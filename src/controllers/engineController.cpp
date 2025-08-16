#include "engineController.h"
#include "kodeshController.h"

namespace core
{
    namespace controller
    {
        EngineController::EngineController(KodeshController& kodesh)
            : state(EngineState::Edit), kodesh(kodesh)
        {}
        
        EngineController::~EngineController()
        {}
        
        void EngineController::SwitchState(EngineState state)
        {
            switch(state)
            {
                case EngineState::Edit:
                    kodesh.ShowUI();
                    if (this->state == EngineState::Play || this->state == EngineState::Pause)
                    {
                        delete kodesh.sc_manager_bkp;
                    }
                    break;
                    
                case EngineState::Play:
                    kodesh.HideUI();
                    if (this->state == EngineState::Edit)
                    {
                        kodesh.sc_manager_bkp = kodesh.sc_manager->Clone();
                        kodesh.sc_manager_bkp->EngineEditMode = false;
                    }
                    break;
                    
                case EngineState::Pause:
                    kodesh.ShowUI();
                    // Logica para pausa de todos os processos...
                    break;
            }
            
            this->state = state;
        }
    }
}
