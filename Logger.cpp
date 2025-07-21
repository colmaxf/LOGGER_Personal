
#include "Logger.h"
#include <iostream>
#include <iomanip>

LogStream::LogStream(Logger& logger, LogLevel level, int line, const char* function)
	: m_logger_ref(logger), m_level(level), m_line(line), m_function(function) {}

LogStream::~LogStream() {
    m_logger_ref.log(m_level, m_line, m_function, m_stream.str());
}


Logger& Logger::get_instance() {
    static Logger* instance = NULL;
    if (instance == NULL) { instance = new Logger(); }
    return *instance;
}
Logger::Logger() {
    InitializeCriticalSection(&m_critSection);
    m_log_file.open("log.txt", std::ios::out | std::ios::app);
}
Logger::~Logger() {
    if (m_log_file.is_open()) { m_log_file.close(); }
    DeleteCriticalSection(&m_critSection);
}
void Logger::set_log_file(const std::string& filename) {
    EnterCriticalSection(&m_critSection);
    if (m_log_file.is_open()) { m_log_file.close(); }
    m_log_file.open(filename.c_str(), std::ios::out | std::ios::app);
    LeaveCriticalSection(&m_critSection);
}


void Logger::log(LogLevel level, int line, const char* function, const std::string& message) {
    EnterCriticalSection(&m_critSection);
    if (m_log_file.is_open()) {
        m_log_file << get_current_timestamp()
                   << " " << get_level_string(level)
				   << " [" << function << ":" << line << "]"
				   << " " << message << std::endl;
    }
	LeaveCriticalSection(&m_critSection);
}

std::string Logger::get_level_string(LogLevel level) {
    switch (level) {
        case LOG_LEVEL_INFO:    return "[INFO]   ";
        case LOG_LEVEL_WARNING: return "[WARNING]";
        case LOG_LEVEL_ERROR:   return "[ERROR]  ";
        default:                return "[UNKNOWN]";
    }
}
std::string Logger::get_current_timestamp() {
    SYSTEMTIME st;
    GetLocalTime(&st);
    std::stringstream ss;
    ss << st.wYear << "-" << std::setw(2) << std::setfill('0') << st.wMonth << "-" << std::setw(2) << std::setfill('0') << st.wDay << " " << std::setw(2) << std::setfill('0') << st.wHour << ":" << std::setw(2) << std::setfill('0') << st.wMinute << ":" << std::setw(2) << std::setfill('0') << st.wSecond;
    return ss.str();
}
