#ifndef SSH_CLIENT_H
#define SSH_CLIENT_H

#include <string>
#include <libssh/libssh.h>

class SSHClient {
public:
    SSHClient(const std::string& ip, const std::string& user, const std::string& password,
              int port = 22, int timeout = 30, const std::string& keyPath = "");
    ~SSHClient();

    bool connect();
    void disconnect();
    bool executeCommand(const std::string& command, std::string& output);

private:
    std::string ip;
    std::string username;
    std::string password;
    std::string keyPath;
    int port;
    int timeout;
    ssh_session session;
};

#endif // SSH_CLIENT_H
