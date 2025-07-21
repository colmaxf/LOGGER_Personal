
#ifndef LOGGER_H
#define LOGGER_H

#include <System.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <windows.h>

enum LogLevel { LOG_LEVEL_INFO, LOG_LEVEL_WARNING, LOG_LEVEL_ERROR };

class Logger;

class LogStream {
public:

	LogStream(Logger& logger, LogLevel level, int line, const char* function);
    ~LogStream();

    template <typename T>
    LogStream& operator<<(const T& value) {
        m_stream << value;
        return *this;
    }

    LogStream& operator<<(const System::UnicodeString& value) {
        m_stream << AnsiString(value);
        return *this;
    }

private:
    Logger& m_logger_ref;
    LogLevel m_level;
    std::stringstream m_stream;


	int m_line;
    const char* m_function;
};

class Logger {
public:
    static Logger& get_instance();
    void set_log_file(const std::string& filename);

    void log(LogLevel level, int line, const char* function, const std::string& message);

private:
    Logger();
    ~Logger();
    Logger(const Logger&);
    Logger& operator=(const Logger&);

    std::string get_level_string(LogLevel level);
    std::string get_current_timestamp();

    std::ofstream m_log_file;
    CRITICAL_SECTION m_critSection;
};


#define LOG_INFO LogStream(Logger::get_instance(), LOG_LEVEL_INFO, __LINE__, __FUNCTION__)
#define LOG_WARNING LogStream(Logger::get_instance(), LOG_LEVEL_WARNING, __LINE__, __FUNCTION__)
#define LOG_ERROR LogStream(Logger::get_instance(), LOG_LEVEL_ERROR, __LINE__, __FUNCTION__)

#endif // LOGGER_H
