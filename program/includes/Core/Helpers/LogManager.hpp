#pragma once

#include <string>
#include <iostream>
#include <vector>

namespace core::systems
{
	/**
     * @brief Tipos de log, para filtragem
     **/
    enum class LogType { 
    	Info, 
    	Warning,
    	Error,
    	EDebug // Debug da engine, mensagens de inicialização e etc...
    };

    /**
     * @brief Abstração de entrada do log
     **/
    struct LogEntry 
    {
        std::string Message;
        LogType Type;
    };

    class LogManager
    {
        /**
         * @brief Instancia privada, para que apenas a função create possa inicializar o logger.
         **/
        LogManager(unsigned int limit): m_LogsLimit(limit) {}

        /**
    	 * @brief Singletron responesável por administrar os logs, permitido apenas 1 instâcia.
         **/
    	static LogManager* Main;

        /**
         * @brief Insere um log na listagem.
         **/
        void PushLog(LogType type, const std::string& msg);

    public:
        /**
         * @brief Limpa a listagem de logs
         **/
        static void Clear();

        /**
         * @brief Chamada para setar o main para acesso do log em qualquer parte do sistema.
         **/
        static void Create(unsigned int limit = 1000);

        /**
         * @brief Destrói o logger aberto, lembrando que o logger é um singletron.
         **/
        static void Destroy();

        /**
         * @brief Verifica se há logger instanciado.
         **/
        static bool HasLogger();
        
        /**
         * @brief Efetua um log na instancia singletron.
         **/
        template<typename... Args>
        static void Log(LogType type, const Args&... msgs);

        /**
         * @brief Captura de listagem de logs do Main.
         **/
        static std::vector<LogEntry>& GetLogs();

    private:
    	std::vector<LogEntry> m_Logs;

        unsigned int m_LogsLimit;	
    };

    template<typename... Args>
    void LogManager::Log(LogType type, const Args&... msg)
    {
        if (Main != nullptr)
        {
            Main->PushLog(type, (std::string(msg) + ...));
        }
        else
        {
            static bool notifyAboutInstance = true;

            // Aviso único de que o logger não apresentará notificações na interface.
            if (notifyAboutInstance)
            {
                std::cout << "[Kernel] " << "Logger não encontrado, logs serão despachados no terminal." << std::endl;

                notifyAboutInstance = false;
            }

            // Essa fun
            static auto si_GetType = [&type]() {
                if (type == LogType::Info) return "Info";
                else if (type == LogType::Warning) return "Warning";
                else if (type == LogType::Error) return "Error";
                else if (type == LogType::EDebug) return "Kernel";
                else return "Unknown";
            };

            std::cout << "[" << si_GetType() << "] " << (std::string(msg) + ...) << std::endl; 
        }
    }
}