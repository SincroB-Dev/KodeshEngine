#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <initializer_list>

namespace core
{
    namespace ui
    {
        // Tipos de log
        enum class LogType { INFO, WARNING, ERROR };

        // Estrutura do log
        struct LogEntry 
        {
            std::string message;
            LogType type;
        };
        
        struct LogWindow
        {
            std::vector<LogEntry> logs;
            
            bool showLogWindow, isOpen, autoScroll, 
                showInfo, showWarning, showError;
                
            LogWindow();
            ~LogWindow();
            
            static LogWindow* Instance;
            
            void AddLog(const std::string& msg, LogType type = LogType::INFO);
            void Show();
            
            inline static void Log(const std::string& msg, LogType type)
            {
                if (Instance != nullptr)
                {
                    Instance->AddLog(msg, type);
                }
            }
            
            inline static void Log(std::initializer_list<std::string> msgs, LogType type)
            {
                std::ostringstream oss;
                
                for (auto it=msgs.begin(); it!=msgs.end(); it++)
                {
                    oss << *it;
                }
                
                Log(oss.str(), type);
            }
        };
    }
}
