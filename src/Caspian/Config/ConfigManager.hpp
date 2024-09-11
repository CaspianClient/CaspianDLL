#pragma once

#include "json/json.hpp"
#include "../Utils/Utils.hpp"
#include <fstream>
#include <iostream>
#include <string>

using json = nlohmann::json;

class ConfigManager {
public:
    bool loadConfig() {
        std::ifstream configFile(filename);
        if (configFile.is_open()) {
            try {
                config << configFile; // Attempt to load JSON data from file
            } catch (const std::exception& e) {
                std::cout << "Error loading configuration: " << e.what() << std::endl;
                configFile.close();
                return false;
            }
            configFile.close();
            return true;
        }
        std::cout << "Failed to open config file: " << filename << std::endl;
        return false;
    }
    bool saveConfig() const {
    std::ofstream configFile(filename);
    if (configFile.is_open()) {
        configFile << config.dump(4); // pretty print with 4 spaces
        configFile.close();
        return true;
    }
    return false;
}

    template<typename T>
    T get(const std::string& module, const std::string& key) const {
    if (config.contains(module) && config[module].contains(key)) {
        return config[module][key].get<T>();
    }
    throw std::runtime_error("Key not found");
}
    
    template<typename T>
    void set(const std::string& module, const std::string& key, const T& value, bool override = true) {
    if (override || !config[module].contains(key)) {
        config[module][key] = value;
    }
}

private:
    std::string filename = Utils::getClientFolder() + "\\Configs\\Default.conf";
    json config;
};

extern ConfigManager ConfigMgr;
