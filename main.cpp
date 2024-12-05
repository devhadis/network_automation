#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <limits>
#include <functional>
#include "ssh_client.h"
#include "command_manager.h"
#include "logger.h"

// Cores usando ANSI Escape Codes
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

// Função para impressão lenta
void slowPrint(const std::string& text, int delay = 50) {
    for (char c : text) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
}

// Função para exibir cabeçalho
void printHeader() {
    std::cout << BOLD << CYAN
              << "╔════════════════════════════════════════════╗\n"
              << "║       SISTEMA DE CONFIGURAÇÃO DE REDE      ║\n"
              << "╚════════════════════════════════════════════╝\n"
              << RESET;
}

// Função genérica para limpar o buffer de entrada
void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Função genérica para validação de entrada
template <typename T>
T getValidatedInput(const std::string& prompt, const std::function<bool(T)>& validate, const std::string& errorMessage) {
    T value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;

        if (std::cin.fail()) {
            clearInput();
            std::cout << RED "Entrada inválida. Tente novamente.\n" RESET;
        } else if (!validate(value)) {
            clearInput();
            std::cout << RED << errorMessage << RESET << "\n";
        } else {
            clearInput();
            return value;
        }
    }
}

// Sobrecarga para capturar strings
std::string getValidatedInput(const std::string& prompt, const std::function<bool(const std::string&)>& validate, const std::string& errorMessage) {
    std::string value;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, value);

        if (validate(value)) {
            return value;
        } else {
            std::cout << RED << errorMessage << RESET << "\n";
        }
    }
}

int main() {
    printHeader();
    slowPrint(GREEN "Bem-vindo ao Sistema de Automação de Configuração de Rede!\n\n" RESET, 30);

    char continueRunning = 'Y';

    while (toupper(continueRunning) == 'Y') {
        // Captura IP
        std::string ip = getValidatedInput<std::string>(
            CYAN "IP do dispositivo: " RESET,
            [](const std::string& value) { return !value.empty(); },
            "O IP não pode estar vazio."
        );

        // Captura Porta
        int port = getValidatedInput<int>(
            CYAN "Porta (padrão: 22): " RESET,
            [](int value) { return value > 0 && value <= 65535; },
            "A porta deve ser um número entre 1 e 65535."
        );

        // Nome do dispositivo (opcional)
        std::string deviceName = getValidatedInput<std::string>(
            CYAN "Nome do dispositivo (opcional): " RESET,
            [](const std::string&) { return true; },  // Sempre válido
            ""
        );

        // Timeout
        int timeout = getValidatedInput<int>(
            CYAN "Timeout de conexão (segundos, padrão: 30): " RESET,
            [](int value) { return value > 0; },
            "O timeout deve ser um número positivo."
        );

        // Método de autenticação
        std::string authMethod = getValidatedInput<std::string>(
            CYAN "Método de autenticação (senha/chave): " RESET,
            [](const std::string& value) { return value == "senha" || value == "chave"; },
            "O método de autenticação deve ser 'senha' ou 'chave'."
        );

        // Captura Usuário e Credenciais
        std::string username, password, keyPath;
        if (authMethod == "senha") {
            username = getValidatedInput<std::string>(
                CYAN "Usuário: " RESET,
                [](const std::string& value) { return !value.empty(); },
                "O usuário não pode estar vazio."
            );
            password = getValidatedInput<std::string>(
                CYAN "Senha: " RESET,
                [](const std::string& value) { return !value.empty(); },
                "A senha não pode estar vazia."
            );
        } else if (authMethod == "chave") {
            username = getValidatedInput<std::string>(
                CYAN "Usuário: " RESET,
                [](const std::string& value) { return !value.empty(); },
                "O usuário não pode estar vazio."
            );
            keyPath = getValidatedInput<std::string>(
                CYAN "Caminho para a chave SSH: " RESET,
                [](const std::string& value) { return !value.empty(); },
                "O caminho para a chave SSH não pode estar vazio."
            );
        }

        // Escolha de tarefa
        int option = getValidatedInput<int>(
            BOLD + std::string(BLUE "1. Configurar VLAN\n"
                                   "2. Configurar Rota Estática\n"
                                   "3. Fazer Backup de Configuração\n"
                                   CYAN "Opção: " RESET),
            [](int value) { return value >= 1 && value <= 3; },
            "Escolha inválida. Por favor, escolha 1, 2 ou 3."
        );

        // Obtém comando e executa
        std::string command = CommandManager::getTask(option);
        SSHClient client(ip, username, password, port, timeout, keyPath);

        slowPrint("\nTentando conectar ao dispositivo...\n", 50);
        if (!client.connect()) {
            Logger::log("Erro ao conectar ao dispositivo: " + ip);
            std::cerr << RED "Erro ao conectar ao dispositivo.\n" RESET;
            continue;
        }

        // Executa o comando
        std::string output;
        if (client.executeCommand(command, output)) {
            std::cout << GREEN "\nComando executado com sucesso!\n" RESET;
            std::cout << YELLOW "Saída:\n" RESET << output << "\n";
            Logger::log("Tarefa executada com sucesso no dispositivo " + ip +
                        (deviceName.empty() ? "" : " (" + deviceName + ")"));
        } else {
            std::cerr << RED "Erro ao executar o comando.\n" RESET;
            Logger::log("Erro ao executar o comando no dispositivo " + ip);
        }

        client.disconnect();
        slowPrint("\nSessão encerrada.\n", 30);

        // Pergunta se o usuário deseja continuar
        continueRunning = getValidatedInput<char>(
            CYAN "Deseja realizar outra operação? (Y/N): " RESET,
            [](char value) { return toupper(value) == 'Y' || toupper(value) == 'N'; },
            "Entrada inválida. Insira 'Y' para sim ou 'N' para não."
        );
    }

    std::cout << GREEN "\nEncerrando o sistema. Obrigado por usar o programa!\n" RESET;
    return 0;
}
