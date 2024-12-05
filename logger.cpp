#include "logger.h"
#include <fstream>
#include <iostream>

void Logger::log(const std::string& message) {
    std::ofstream logfile("logs/execution_log.txt", std::ios::app);
    if (logfile.is_open()) {
        logfile << message << std::endl;
        logfile.close();
    } else {
        std::cerr << "Erro ao abrir o arquivo de log.\n";
    }
}
