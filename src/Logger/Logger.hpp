#pragma once

#include <windows.h>
#include <format>
#include <filesystem>
#include <fstream>
#include <iostream>
#include "../Utils/Utils.hpp"

class Logger {
    static inline std::string file = Utils::getClientFolder() + "\\log.txt";
	static inline std::ofstream logFile = std::ofstream();

    static void log(const std::string& level, const std::string& message) {
        printf((level + ": " + message).c_str());
        if (logFile.is_open()) {
            logFile << level << ": " << message << std::endl;
            logFile.flush();
        }
    }
public:
    static void SetupLogger() {
        std::filesystem::remove(file);
        if (!std::filesystem::exists(file)) {
            std::filesystem::path p(file);
            std::filesystem::create_directories(p.parent_path().string());

            CloseHandle(CreateFileA(file.c_str(), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr)); 

        }
        
        logFile.open(file, std::ios::out | std::ios::app);
        if (!logFile.is_open()) {
            std::cout << "Failed to open log file" << std::endl;
        }
    }

    static void info(std::string text) {
        log("INFO", text);
    }

    static void error(std::string text) {
        log("ERROR", text);
    }

    static void debug(std::string text) {
        log("DEBUG", text);
    }
};