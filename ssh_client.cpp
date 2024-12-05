#include "ssh_client.h"
#include <iostream>
#include <libssh/libssh.h>

SSHClient::SSHClient(const std::string& ip, const std::string& user, const std::string& password,
                     int port, int timeout, const std::string& keyPath)
    : ip(ip), username(user), password(password), port(port), timeout(timeout), keyPath(keyPath), session(nullptr) {}

SSHClient::~SSHClient() {
    if (session) {
        disconnect();
    }
}

bool SSHClient::connect() {
    session = ssh_new();
    if (!session) {
        return false;
    }

    ssh_options_set(session, SSH_OPTIONS_HOST, ip.c_str());
    ssh_options_set(session, SSH_OPTIONS_PORT, &port);
    ssh_options_set(session, SSH_OPTIONS_TIMEOUT, &timeout);

    if (!keyPath.empty()) {
        ssh_options_set(session, SSH_OPTIONS_IDENTITY, keyPath.c_str());
    }

    if (ssh_connect(session) != SSH_OK) {
        std::cerr << "Erro de conexão: " << ssh_get_error(session) << std::endl;
        ssh_free(session);
        return false;
    }

    if (!keyPath.empty()) {
        if (ssh_userauth_publickey_auto(session, nullptr, nullptr) != SSH_AUTH_SUCCESS) {
            std::cerr << "Erro de autenticação por chave: " << ssh_get_error(session) << std::endl;
            disconnect();
            return false;
        }
    } else {
        if (ssh_userauth_password(session, username.c_str(), password.c_str()) != SSH_AUTH_SUCCESS) {
            std::cerr << "Erro de autenticação: " << ssh_get_error(session) << std::endl;
            disconnect();
            return false;
        }
    }

    return true;
}

void SSHClient::disconnect() {
    if (session) {
        ssh_disconnect(session);
        ssh_free(session);
        session = nullptr;
    }
}

bool SSHClient::executeCommand(const std::string& command, std::string& output) {
    ssh_channel channel = ssh_channel_new(session);
    if (!channel) {
        return false;
    }

    if (ssh_channel_open_session(channel) != SSH_OK) {
        ssh_channel_free(channel);
        return false;
    }

    if (ssh_channel_request_exec(channel, command.c_str()) != SSH_OK) {
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        return false;
    }

    char buffer[256];
    int nbytes;
    while ((nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0)) > 0) {
        output.append(buffer, nbytes);
    }

    ssh_channel_close(channel);
    ssh_channel_free(channel);

    return true;
}
