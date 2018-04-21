#pragma once


#include <Manager.h>
#include "File.h"
#include <chrono>
#include <ctime>
#include <sstream>
#include "../os/OSManager.h"

#define LOGF(s) LoggingManager::get().log(s)

namespace config {
    constexpr auto log_filename = "last_log.log";
}

class LoggingManager : public Manager<LoggingManager> {
    friend class CRSP<LoggingManager>;
private:
    LoggingManager() {}
    ~LoggingManager() {}

public:

    void start_up() override {
        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        auto ss = std::stringstream();
        ss << "LOGGING RUN @ " << std::ctime(&now) << "\n\n";

		m_logging_file = OSManager::get().executable_path() + "/" + config::log_filename;

        write_to_file(m_logging_file.c_str(), ss.str().c_str());
    }

    void shut_down() override {

        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        auto ss = std::stringstream();
        ss << "\n\nEND OF LOG @ " << std::ctime(&now);

        append_to_file(m_logging_file.c_str(), ss.str().c_str());
    }

    template<typename T>
    void log(T msg) {

        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        auto ss = std::stringstream();
        ss << " - " << std::ctime(&now) << msg << "\n\n";

        append_to_file(m_logging_file.c_str(), ss.str().c_str());
    }

private:
	std::string m_logging_file{};
};