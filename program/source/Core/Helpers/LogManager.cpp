#include "Core/Helpers/LogManager.hpp"

namespace core
{
	namespace systems
	{
		LogManager* LogManager::Main = nullptr;

		// Inclui um log na listagem, limitando a 1000 logs.
        void LogManager::PushLog(LogType type, const std::string& msg)
        {
            m_Logs.push_back({ msg, type });
            if (m_Logs.size() > 1000) m_Logs.erase(m_Logs.begin());
        }

        void LogManager::Clear()
        {
        	m_Logs.clear();
        }

        std::vector<LogEntry>& LogManager::GetLogs()
        {
        	return Main->m_Logs;
        }

        void LogManager::Instantiate()
        {
        	if (Main == nullptr)
        	{
        		Main = this;
        		return;
        	}

        	std::runtime_error("Não é possível instanciar mais de um LogManager");
        }
	}
}