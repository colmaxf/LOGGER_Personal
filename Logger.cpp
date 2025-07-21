
#include "Logger.h"
#include <iostream>
#include <iomanip>
#include <chrono> // C++ handle timstamp

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
    m_log_file.open("log.txt", std::ios::out | std::ios::app);
}
Logger::~Logger() {
    if (m_log_file.is_open()) { m_log_file.close(); }
}
void Logger::set_log_file(const std::string& filename) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_log_file.is_open()) { m_log_file.close(); }
    m_log_file.open(filename.c_str(), std::ios::out | std::ios::app);
}


void Logger::log(LogLevel level, int line, const char* function, const std::string& message) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_log_file.is_open()) {
        m_log_file << get_current_timestamp()
                   << " " << get_level_string(level)
				   << " [" << function << ":" << line << "]"
				   << " " << message << std::endl;
    }
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
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    return ss.str();
}
