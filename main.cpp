#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <limits>
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

int main() {
    printHeader();
    slowPrint(GREEN "Bem-vindo ao Sistema de Automação de Configuração de Rede!\n\n" RESET, 30);

    char continueRunning = 'Y'; // Controle do loop

    while (toupper(continueRunning) == 'Y') {
        // Entrada de dados do usuário
        std::string ip, username, password, deviceName, authMethod, keyPath;
        int port = 22;  // Porta padrão para SSH
        int timeout = 30; // Timeout padrão (segundos)

        std::cout << CYAN "IP do dispositivo: " RESET;
        std::cin >> ip;

        std::cout << CYAN "Porta (padrão: 22): " RESET;
        std::cin >> port;

        std::cout << CYAN "Nome do dispositivo (opcional): " RESET;
        std::cin.ignore(); // Limpa o buffer antes do getline
        std::getline(std::cin, deviceName);

        std::cout << CYAN "Timeout de conexão (segundos, padrão: 30): " RESET;
        std::cin >> timeout;

        std::cout << CYAN "Método de autenticação (senha/chave): " RESET;
        std::cin >> authMethod;

        if (authMethod == "senha") {
            std::cout << CYAN "Usuário: " RESET;
            std::cin >> username;

            std::cout << CYAN "Senha: " RESET;
            std::cin >> password;
        } else if (authMethod == "chave") {
            std::cout << CYAN "Usuário: " RESET;
            std::cin >> username;

            std::cout << CYAN "Caminho para a chave SSH: " RESET;
            std::cin >> keyPath;
        } else {
            std::cerr << RED "Método de autenticação inválido.\n" RESET;
            continue;
        }

        // Exibe opções de tarefa
        slowPrint("\nEscolha uma tarefa:\n", 30);
        std::cout << BOLD << BLUE
                  << "1. Configurar VLAN\n"
                  << "2. Configurar Rota Estática\n"
                  << "3. Fazer Backup de Configuração\n"
                  << RESET;
        std::cout << CYAN "Opção: " RESET;
        int option;
        std::cin >> option;

        // Obtém o comando
        std::string command = CommandManager::getTask(option);
        if (command.empty()) {
            std::cout << RED "Opção inválida. Tente novamente.\n" RESET;
            continue;
        }

        // Configuração do cliente SSH
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

        // Pergunta ao usuário se deseja continuar
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpa o buffer
        std::cout << CYAN "Deseja realizar outra operação? (Y/N): " RESET;
        std::cin >> continueRunning;

        // Valida entrada
        while (toupper(continueRunning) != 'Y' && toupper(continueRunning) != 'N') {
            std::cout << RED "Entrada inválida. Por favor, insira 'Y' para continuar ou 'N' para sair: " RESET;
            std::cin >> continueRunning;
        }
    }

    std::cout << GREEN "\nEncerrando o sistema. Obrigado por usar o programa!\n" RESET;
    return 0;
}
