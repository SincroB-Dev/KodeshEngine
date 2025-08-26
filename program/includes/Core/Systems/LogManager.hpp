#pragma once

#include <string>
#include <iostream>
#include <vector>

namespace core
{
	namespace systems
	{
		// Tipos de log, para filtragem
        enum class LogType { 
        	Info, 
        	Warning,
        	Error,
        	EDebug // Debug da engine, mensagens de inicialização e etc...
        };

        // Abstração de entrada do log
        struct LogEntry 
        {
            std::string Message;
            LogType Type;
        };

        class LogManager
        {
        public:
            LogManager() {}

        	// Permite o envio de logs de qualquer parte do sistema
        	static LogManager* Main;
            
            // Insere um log na listagem
            void PushLog(LogType type, const std::string& msg);

            // Limpa a listagem de logs
            void Clear();

            // Chamada para setar o main para acesso do log em qualquer parte do sistema
            void Instantiate();
            
            template<typename... Args>
            static void Log(LogType type, Args&... msgs);

            // Captura de listagem de logs do Main
            static std::vector<LogEntry>& GetLogs();

        private:
        	std::vector<LogEntry> m_Logs;    	
        };

        template<typename... Args>
        void LogManager::Log(LogType type, Args&... msgs)
        {
            if (Main != nullptr)
            {
                Main->PushLog((std::string(msgs) + ...));
            }
        }
	}
}