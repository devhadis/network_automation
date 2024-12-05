## **Documentação: Sistema de Automação de Configuração de Rede**

Este projeto é um sistema em **C++** para automação de tarefas administrativas em dispositivos de rede, como roteadores e switches, utilizando conexões **SSH**. Ele permite executar comandos em dispositivos remotamente e registrar os resultados em logs.

---

### **Pré-requisitos**

Antes de compilar e executar o projeto, verifique os seguintes requisitos:

#### 1. **Ambiente de Desenvolvimento**
- **Sistema Operacional:** Linux, macOS, ou Windows com suporte a SSH e compilador C++.
- **Compilador C++:** GCC (Linux/macOS) ou MinGW (Windows).
- **Versão mínima do C++:** Suporte a C++11 ou superior.

#### 2. **Dependências**
- **Biblioteca `libssh`:** Utilizada para conexões SSH.
  - **Linux (Ubuntu/Debian):**
    ```bash
    sudo apt-get install libssh-dev
    ```
  - **Fedora/RHEL:**
    ```bash
    sudo dnf install libssh-devel
    ```
  - **macOS (via Homebrew):**
    ```bash
    brew install libssh
    ```

- **Criar a Estrutura de Diretórios:**
  Navegue até o local onde deseja configurar o projeto e execute:
  ```bash
  mkdir -p network_automation/logs
  ```

- **Permissões:**
  Garanta que o diretório de logs tenha permissões de escrita:
  ```bash
  chmod -R 755 network_automation/logs
  ```

#### 3. **Editor de Código**
- Recomendado: **Visual Studio Code**, **CLion**, ou **Vim**.

---

### **Estrutura do Projeto**

```plaintext
network_automation/
├── main.cpp              # Arquivo principal
├── ssh_client.cpp        # Implementação da classe para conexões SSH
├── ssh_client.h          # Header file para conexões SSH
├── command_manager.cpp   # Gerenciamento de comandos predefinidos
├── command_manager.h     # Header file para comandos
├── logger.cpp            # Gerenciamento de logs
├── logger.h              # Header file para logs
├── logs/                 # Diretório para arquivos de log
│   └── execution_log.txt # Arquivo de log gerado pelo programa
```

---

### **Compilação e Execução**

1. **Navegue até a pasta do projeto:**
   ```bash
   cd network_automation
   ```

2. **Compile o projeto:**
   Use o seguinte comando para compilar o programa:
   ```bash
   g++ -o network_automation main.cpp ssh_client.cpp command_manager.cpp logger.cpp -lssh
   ```

3. **Execute o programa:**
   Após a compilação, execute o programa:
   ```bash
   ./network_automation
   ```

---

### **Como Usar**

1. **Inicie o programa:**
   Após executar o comando `./network_automation`, o sistema exibirá o cabeçalho e solicitará informações sobre o dispositivo.

2. **Preencha as Informações do Dispositivo:**
   - **IP do Dispositivo:** Endereço IP do dispositivo de rede.
   - **Porta:** A porta SSH do dispositivo (padrão: 22).
   - **Nome do Dispositivo:** Opcional, usado para identificação nos logs.
   - **Timeout:** Tempo de espera para a conexão (em segundos).
   - **Método de Autenticação:**
     - **Senha:** Forneça nome de usuário e senha.
     - **Chave:** Forneça o caminho para a chave SSH privada e o nome de usuário.

3. **Escolha a Tarefa:**
   O programa exibirá as opções de tarefa:
   - **1:** Configurar VLAN.
   - **2:** Configurar Rota Estática.
   - **3:** Fazer Backup de Configuração.

4. **Confirme e Aguarde:**
   O sistema tentará conectar ao dispositivo, executar a tarefa e exibir a saída.

5. **Decida Continuar ou Sair:**
   Após completar a operação, o programa perguntará:
   ```plaintext
   Deseja realizar outra operação? (Y/N):
   ```
   - **Digite `Y`:** Para continuar e realizar outra operação.
   - **Digite `N`:** Para sair do programa.

6. **Verifique os Logs:**
   Os resultados das operações e eventuais erros serão registrados no arquivo:
   ```plaintext
   network_automation/logs/execution_log.txt
   ```

---

### **Exemplo de Uso**

#### **Entrada no Programa**
```plaintext
IP do dispositivo: 192.168.1.1
Porta (padrão: 22): 22
Nome do dispositivo (opcional): Switch_A
Timeout de conexão (segundos, padrão: 30): 30
Método de autenticação (senha/chave): senha
Usuário: admin
Senha: admin123
Escolha uma tarefa:
1. Configurar VLAN
2. Configurar Rota Estática
3. Fazer Backup de Configuração
Opção: 1
```

#### **Saída no Terminal**
```plaintext
Tentando conectar ao dispositivo...
Comando executado com sucesso!
Saída:
VLAN configurada com sucesso.
Deseja realizar outra operação? (Y/N): N
Encerrando o sistema. Obrigado por usar o programa!
```

#### **Log Gerado**
Arquivo `network_automation/logs/execution_log.txt`:
```plaintext
2024-12-04 15:30:12 - Tarefa executada com sucesso no dispositivo 192.168.1.1 (Switch_A)
```

---

### **Problemas Comuns e Soluções**

1. **Erro: `libssh.h: No such file or directory`**
   - A biblioteca `libssh` não está instalada ou configurada.
   - Solução: Instale `libssh-dev` no sistema:
     ```bash
     sudo apt-get install libssh-dev
     ```

2. **Erro: `Permission Denied` ao criar logs**
   - O diretório `logs/` não tem permissões de escrita.
   - Solução: Ajuste as permissões:
     ```bash
     chmod -R 755 network_automation/logs
     ```

3. **Erro: `SSH authentication failed`**
   - Nome de usuário, senha ou chave SSH incorretos.
   - Solução: Verifique as credenciais e tente novamente.

---

### **Contribuições**

Contribuições são bem-vindas! Para sugestões ou melhorias:
- Faça um fork do projeto no GitHub (ou outra plataforma de controle de versão).
- Submeta um pull request com suas mudanças.
