#pragma once
#include <map>
#include <mutex>
#include <string>
#include <memory>
#include <fstream>
#include <optional>

namespace logging {

   enum class log_level : uint8_t
   {
      none,    //do not log any messages
      debug,   //log all messages
      info,    //log everything except debug messages
      warning, //log all warnings, errors, and fatal exceptions
      error,   //log error and fatal messages only
      fatal    //log fatal messages only
   };

   //converts a log_level value to a human readable string
   std::string to_string(const log_level level);
   
   constexpr bool is_loggable(const log_level verbosity,
                              const log_level msg_severity) {
      return verbosity > log_level::none  //the verbosity of the handler has not been switched off
          && !(verbosity > msg_severity); //the severity of the message is not below the verbosity level of the handler
   }
   
#pragma region - Abstractions that define the interface of a Logging Strategy
   struct HandlerMeta
   {
      std::string name;
      log_level   level;
   };

   class LogHandler
   {
   public:

      //disable copy semantics to prevent slicing
      LogHandler(const LogHandler&)             = delete;
      LogHandler& operator=(const LogHandler&)  = delete;

      //allow move semantics (for transfer of ownership)
      LogHandler(LogHandler&&)                  = default;
      LogHandler& operator=(LogHandler&&)       = default;

      virtual ~LogHandler() noexcept            = default;
      LogHandler()                              = default; //required for usage with STL containers

      LogHandler(const HandlerMeta& metadata) : m_name(metadata.name),
                                                m_level(metadata.level) {
         if (m_name.empty())
            throw std::invalid_argument("'name' cannot be an empty string");
      }
      
      virtual void log(const std::string& message, const log_level severity) = 0;
      
      std::string get_name() const {

         return m_name;
      }
      
      log_level get_level() const {

         return m_level;
      }

      void set_level(const log_level level) {

         m_level = level;
      }

   private:
      std::string m_name{};
      log_level m_level{ log_level::info };
   };
#pragma region - Abstract class that defines the interface of a LogHanlder

#pragma region - Concrete Logging Strategies
   class ConsoleHandler: public LogHandler
   {
   public:

      //cannot be default constructed
      ConsoleHandler()                                   = delete;

      //does not support copy-semantics
      ConsoleHandler(const ConsoleHandler&)              = delete;
      ConsoleHandler& operator=(const ConsoleHandler&)   = delete;

      //has a non-virtual public destructor; not be used as a base class
      ~ConsoleHandler() noexcept                         = default;

      //offers move semantics
      ConsoleHandler(ConsoleHandler&&)                   = default;
      ConsoleHandler& operator=(ConsoleHandler&&)        = default;

      ConsoleHandler(const HandlerMeta& metadata) : LogHandler(metadata) {}

      //a thread-safe implementation that routes all log messages to a console
      void log(const std::string& message,
               const log_level severity) final;

   private:
      std::mutex m_mutex{};
   };

   class FileHandler: public LogHandler
   {
   public:

      //cannot be default constrcuted
      FileHandler()                                = delete;

      //does not support copy-semantics
      FileHandler(const FileHandler&)              = delete;
      FileHandler& operator=(const FileHandler&)   = delete;

      //has a non-virtual public destructor; not be used as a base class
      ~FileHandler() noexcept                      = default;

      //offers move-semantics
      FileHandler(FileHandler&&)                   = default;
      FileHandler& operator=(FileHandler&&)        = default;

      FileHandler(const std::string& directory,
                  const std::string& filename,
                  const HandlerMeta& metadata);

      //a thread-safe implementation that routes all log messages to a file
      void log(const std::string& message,
               const log_level severity) final;

   private:
      std::mutex     m_mutex{};
      std::ofstream  m_fstream{};
   };
#pragma endregion - Concrete Logging Strategies

#pragma region - Logger
   class Logger final
   {
   public:
      
      ~Logger() noexcept               = default;
      
      //does not support copy and move semantics
      Logger(const Logger&)            = delete;
      Logger& operator=(const Logger&) = delete;

      Logger(Logger&&)                 = delete;
      Logger& operator=(Logger&&)      = delete;

      //Singleton
      static Logger& instance() {

         static auto singleton = Logger{};
         return singleton;
      }

      //methods for adding, removing and updating log handlers
      void add(const std::string& name,
               std::unique_ptr<LogHandler> handler);
      void remove(const std::string& name);
      std::optional<LogHandler*> get_handler(const std::string& name) const;
      
      //logging apis
      void debug(const std::string& message) const;
      void info(const std::string& message) const;
      void warn(const std::string& message) const;
      void error(const std::string& message) const;
      void fatal(const std::string& message) const;

   private:
      Logger() = default;

      void log(const std::string& message,
               const log_level severity) const;

      std::map<std::string, std::unique_ptr<LogHandler>> m_handlers{};
   };
#pragma region - Logger
}
