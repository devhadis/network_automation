#include "command_manager.h"

std::string CommandManager::getTask(int option) {
    switch (option) {
        case 1: // Configurar VLAN
            return "configure terminal\nvlan 10\nname TestVLAN\nexit";
        case 2: // Configurar Rota Estática
            return "ip route 192.168.2.0 255.255.255.0 192.168.1.1";
        case 3: // Fazer Backup de Configuração
            return "copy running-config startup-config";
        default:
            return "";
    }
}
