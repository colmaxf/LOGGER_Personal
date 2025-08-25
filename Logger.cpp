
#include "Logger.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdio>

LogStream::LogStream(Logger& logger, LogLevel level, int line, const char* function)
	: m_logger_ref(logger), m_level(level), m_line(line), m_function(function) {}

LogStream::~LogStream() {
    m_logger_ref.log(m_level, m_line, m_function, m_stream.str());
}

// Các hằng số để quản lý file log
const int MAX_LOG_FILES = 15;
const int FILES_TO_DELETE = 5;
const char* LOG_FILE_SUFFIX = "_logsys.txt";

std::string Logger::get_current_date_as_string() {
    SYSTEMTIME st;
    GetLocalTime(&st);
    std::stringstream ss;
    ss << st.wYear << "_" << std::setw(2) << std::setfill('0') << st.wMonth << "_" << std::setw(2) << std::setfill('0') << st.wDay;
    return ss.str();
}

void Logger::cleanup_old_logs() {
    std::vector<std::string> log_files;
    WIN32_FIND_DATAA find_data;
    std::string search_path = "*" + std::string(LOG_FILE_SUFFIX);
    HANDLE hFind = FindFirstFileA(search_path.c_str(), &find_data);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            log_files.push_back(find_data.cFileName);
        } while (FindNextFileA(hFind, &find_data) != 0);
        FindClose(hFind);
    }

    if (log_files.size() >= MAX_LOG_FILES) {
        std::sort(log_files.begin(), log_files.end());
        for (int i = 0; i < FILES_TO_DELETE; ++i) {
            std::remove(log_files[i].c_str());
        }
    }
}

void Logger::update_log_file_if_needed() {
    std::string today_date_str = get_current_date_as_string();
    if (m_current_date_string != today_date_str) {
        m_current_date_string = today_date_str;

        if (m_log_file.is_open()) {
            m_log_file.close();
        }

        std::string filename = m_current_date_string + LOG_FILE_SUFFIX;
        m_log_file.open(filename.c_str(), std::ios::out | std::ios::app);

        // Thực hiện dọn dẹp khi một file mới được tạo
        cleanup_old_logs();
    }
}

Logger& Logger::get_instance() {
    // Meyers' Singleton: an toàn cho luồng trong C++11+, không rò rỉ bộ nhớ.
    static Logger instance;
    return instance;
}
Logger::Logger() {
    InitializeCriticalSection(&m_critSection);
    // Việc khởi tạo file ban đầu sẽ được xử lý bởi update_log_file_if_needed
    // khi có lời gọi log đầu tiên.
    m_current_date_string = ""; // Buộc cập nhật trong lần log đầu tiên
}
Logger::~Logger() {
    if (m_log_file.is_open()) { m_log_file.close(); }
    DeleteCriticalSection(&m_critSection);
}

// Hàm này không còn phù hợp với logic tạo file log hàng ngày tự động.
// Một hàm mới để đặt thư mục log có thể được triển khai nếu cần.
void Logger::set_log_file(const std::string& filename) {
    // EnterCriticalSection(&m_critSection);
    // if (m_log_file.is_open()) { m_log_file.close(); }
    // m_log_file.open(filename.c_str(), std::ios::out | std::ios::app);
    // LeaveCriticalSection(&m_critSection);
}


void Logger::log(LogLevel level, int line, const char* function, const std::string& message) {
    EnterCriticalSection(&m_critSection);

    // Kiểm tra và cập nhật file log nếu ngày đã thay đổi
    update_log_file_if_needed();

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
