
#include <iostream>
#include <exception>
#include <filesystem>
#include "utils.h"
#include "logger.h"

namespace logging {

   std::string to_string(const log_level level) {

      switch (level) {

      case log_level::debug:     return "DEBUG";
      case log_level::info:      return "INFO";
      case log_level::warning:   return "WARNING";
      case log_level::error:     return "ERROR";
      case log_level::fatal:     return "FATAL";
      default:                   return "";
      }
   }
   
#pragma region - Concrete Log Hanlders
#pragma region - ConsoleHandler implementation
   void ConsoleHandler::log(const std::string& message, const log_level severity) {

      const auto verbosity = get_level();
      if (!is_loggable(verbosity, severity))
         return;

      //acquire the lock before writing to the file stream for thread safety
      const auto lock = std::lock_guard<std::mutex>{ m_mutex };
      std::cout << utils::timestamp() 
                << " - (" << to_string(severity) << ")\t: "
                << message 
                << std::endl << std::flush;
   }
#pragma endregion - ConsoleHandler implementation

#pragma region - FileHandler implementation
   FileHandler::FileHandler(const std::string& directory,
                          const std::string& filename,
                          const HandlerMeta& metadata) : LogHandler(metadata) {

      if (directory.empty())
         throw std::invalid_argument("'directory' cannot be empty");

      if (filename.empty())
         throw std::invalid_argument("'filename' cannot be empty");

      auto log_dir = std::filesystem::path{ directory,
                                            std::filesystem::path::generic_format };

      if (!std::filesystem::exists(log_dir))
         std::filesystem::create_directory(log_dir);

      auto log_file = log_dir.string() + "/" + filename + ".log";
      m_fstream = std::ofstream{ log_file, std::ios::out | std::ios::ate };

      if (!m_fstream.is_open())
         throw std::domain_error("failed to open the log file: '" + log_file + "'");
   }

   void FileHandler::log(const std::string& message, const log_level severity) {

      const auto verbosity = get_level();
      if (!is_loggable(verbosity, severity))
         return;

      //acquire the lock before writing to the file stream for thread safety
      const auto lock = std::lock_guard<std::mutex>{ m_mutex };
      m_fstream << utils::timestamp()
                << " - (" << to_string(severity) << ")\t: "
                << message
                << std::endl << std::flush;
   }
#pragma endregion - FileHandler implementation
#pragma endregion - Concrete Log Handlers

#pragma region - Logger implementation
   void Logger::add(const std::string& name,
                    std::unique_ptr<LogHandler> handler) {

      if (name.empty())
         throw std::invalid_argument("'name' cannot be empty");

      if (!handler)
         throw std::invalid_argument("'handler' cannot be a nullptr");

      const auto search = m_handlers.find(name);
      if (search == m_handlers.cend()) //do not allow duplicate
         m_handlers[name] = std::move(handler);
   }

   void Logger::remove(const std::string& name) {

      const auto position = m_handlers.find(name);
      if (position != m_handlers.cend()) 
         m_handlers.erase(position);
   }

   std::optional<LogHandler*> Logger::get_handler(const std::string& name) const {

      const auto search = m_handlers.find(name);
      if (search != m_handlers.cend())
         return search->second.get();

      return std::nullopt;
   }

   void Logger::log(const std::string& message,
                    const log_level severity) const
   {
      for (const auto& [name, handler] : m_handlers) {
         handler->log(message, severity);
      }
   }

   void Logger::debug(const std::string& message) const {

      log(message, log_level::debug);
   }

   void Logger::info(const std::string& message) const {

      log(message, log_level::info);
   }

   void Logger::warn(const std::string& message) const {

      log(message, log_level::warning);
   }

   void Logger::error(const std::string& message) const {

      log(message, log_level::error);
   }

   void Logger::fatal(const std::string& message) const {

      log(message, log_level::fatal);
   }
#pragma endregion - Logger implementation
}