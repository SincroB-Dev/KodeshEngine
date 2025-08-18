#pragma once

namespace core
{
    namespace controller
    {
        class KodeshController;
        
        enum class EngineState {
            Edit,   // Modo de edição, UI visível
            Play,   // Jogo rodando, UI de edição oculta
            Pause   // (Opcional) pausa do jogo
        };
        
        class EngineController
        {
            EngineState state;
            
            friend class KodeshController;
            
        public:
            KodeshController& kodesh;
            
            EngineController(KodeshController& kodesh);
            ~EngineController();
            
            void SwitchState(EngineState state);
        };
    }
}
