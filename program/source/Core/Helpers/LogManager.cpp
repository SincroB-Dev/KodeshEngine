#include "Core/Helpers/LogManager.hpp"
#include <stdexcept>

namespace core::systems
{
	LogManager* LogManager::Main = nullptr;

    void LogManager::PushLog(LogType type, const std::string& msg)
    {
        m_Logs.push_back({ msg, type });
        if (m_Logs.size() > m_LogsLimit) m_Logs.erase(m_Logs.begin());
    }

    void LogManager::Clear()
    {
        if (Main != nullptr)
        {
    	   Main->m_Logs.clear();
        }
    }

    std::vector<LogEntry>& LogManager::GetLogs()
    {
    	return Main->m_Logs;
    }

    void LogManager::Create(unsigned int limit)
    {
    	if (Main == nullptr)
    	{
    		Main = new LogManager(limit);
    		return;
    	}

    	throw std::runtime_error("Não é possível instânciar mais de um Logger!");
    }

    void LogManager::Destroy()
    {
        if (Main != nullptr)
        {
            delete Main;
            Main = nullptr;
            std::cout << "[Kernel] " << "Instância principal de Logger destruída." << std::endl;
            return;
        }

        throw std::runtime_error("Nenhuma instância de Logger foi construída!");
    }

    bool LogManager::HasLogger()
    {
        return Main != nullptr;
    }
}