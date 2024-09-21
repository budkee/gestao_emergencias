/**************************************************
*
* Isabele Bicudo e Kaê de Oliveira Budke
* Trabalho 1
* Professor(a): Fábio Henrique Viduani Martinez
*
*/

// Bibliotecas
#include <iostream>
#include <cstring>
#include <stdlib.h>

#define MAX 50  

// ---------- Registros médicos ---------- //
/*
    "Uma tabela de dispersão é usada para armazenar os registros médicos dos pacientes, permitindo uma busca rápida pelo seu histórico, onde a chave é o número de identificação do paciente, mais outras informações como nome e enfermidade."
*/
class Paciente {
public:
    char nome[MAX];
    char enfermidade[MAX];

    Paciente() {
        nome[0] = '\0';
        enfermidade[0] = '\0';
    }
};

class TabelaDispersao {
private:

    Paciente pacientes[MAX];
    int ids[MAX];

    int calcular_id(const char* nome) {
        int soma = 0;
        for (int i = 0; i < strlen(nome); i++) {
            soma += nome[i];
        }
        return soma % MAX;
    }

    // Função de dispersão
    int insere_aberto(int x) {
        int i = 0;
        int j;
        do {
            // Calcula a posição j garantindo que esteja dentro dos limites da tabela
            j = (x + i) % MAX;
            // Se a posição j estiver vazia, insere x
            if (ids[j] == -1) {
                ids[j] = x;
                return j;
            }
            i++;
        } while (i < MAX); // Enquanto não percorrer toda a tabela
        return MAX; // Tabela cheia
    }

public:

    // Inicializando a tabela com posições vazias
    TabelaDispersao() {
        for (int i = 0; i < MAX; i++) {
            ids[i] = -1;
        }
    }

    void cadastrar_paciente(const char* nome, const char* enfermidade) {
        
        int id = calcular_id(nome);
        int posicao = insere_aberto(id);
        // Se a posição estiver vazia, insere o paciente
        if (posicao < MAX) {
            strcpy(pacientes[posicao].nome, nome);
            strcpy(pacientes[posicao].enfermidade, enfermidade);
        } else {
            std::cout << "Tabela cheia\n";
        }
        // Registra a chamada (?) 

    }

    void imprimir_identificacoes() {
        std::cout << "\nIdentificações dos pacientes:\n";
        for (int i = 0; i < MAX; i++) {
            // Se a posição não estiver vazia, imprime o ID e o nome do paciente
            if (ids[i] != -1) {
                std::cout << ids[i] << " " << pacientes[i].nome << "\n";
            }
        }
    }
    
    // Retorna o paciente com o ID de interesse
    Paciente* get_paciente(int id) {
        for (int i = 0; i < MAX; i++) {
            // Se o ID for encontrado, retorna o paciente
            if (ids[i] == id) {
                return &pacientes[i];
            }
        }
        // Se não encontrar o ID, retorna ponteiro nulo
        return nullptr;
    }

};

// ---------- Gerenciamento das chamadas de emergência ---------- //
/*
    "Uma lista de prioridades é usada para gerenciar as chamadas de emergência, onde cada chamada é classificada de acordo com o nível de gravidade. A gravidade é atribuída como um valor numérico: quanto maior o número, maior a gravidade. Assim, o sistema precisa sempre atender primeiro as chamadas mais críticas. Ou seja, as chamadas de emergência são armazenadas na lista de prioridades e a operação principal é a *inserção* de novas chamadas e a *extração* do paciente mais urgente."
*/

class Chamada {
public:
    
    int id_paciente;
    int gravidade;
    // Construtor
    Chamada(int id, int grav) : id_paciente(id), gravidade(grav) {}

    // 

};


class Sistema {
private:

    TabelaDispersao tabela;
    Chamada* chamadas[MAX];
    int num_chamadas;

public:
    // Construtor
    Sistema() : num_chamadas(0) {}
    
    // Cadastra pacientes
    void cadastrar_paciente(const char* nome, const char* enfermidade) {
        tabela.cadastrar_paciente(nome, enfermidade);
    }
    
    // Adiciona chamadas de emergência | Inserção
    void adicionar_chamada(int id, int gravidade) {
        if (num_chamadas < MAX) {
            chamadas[num_chamadas++] = new Chamada(id, gravidade);
        }
    }
    
    // ***Atendimento das chamadas*** | Extração - Max-Heap
    void atender_chamadas() {
        // Ordena as chamadas por gravidade (urgência)
        for (int i = 0; i < num_chamadas - 1; i++) {
            for (int j = 0; j < num_chamadas - i - 1; j++) {
                if (chamadas[j]->gravidade < chamadas[j + 1]->gravidade) {
                    std::swap(chamadas[j], chamadas[j + 1]);
                }
            }
        }

        // Imprime a lista de atendimentos prioritários | Extração do mais urgente
        std::cout << "\nLista de Atendimentos Prioritários:\n";
        for (int i = 0; i < num_chamadas; i++) {
            // Retorna o paciente com o ID da chamada
            Paciente* paciente = tabela.get_paciente(chamadas[i]->id_paciente);
            // Se o paciente for encontrado, imprime o nome
            if (paciente != nullptr) {
                std::cout << paciente->nome << "\n";
            }
        }
    }
};

int main() {
    
    TabelaDispersao tabela;
    Sistema sistema;

    // Cadastrando pacientes
    int n;
    std::cout << "Número de pacientes: ";
    std::cin >> n;
    std::cin.ignore(); // Limpa o buffer

    for (int i = 0; i < n; i++) {
        char linha[MAX];
        std::cout << "Digite o nome e a enfermidade: ";
        std::cin.getline(linha, sizeof(linha));

        char* nome = strtok(linha, ",");
        char* enfermidade = strtok(nullptr, ",");

        if (nome && enfermidade) {
            sistema.cadastrar_paciente(nome, enfermidade);
        } else {
            std::cout << "Entrada inválida!\n";
            i--; // Volta para ler a entrada corretamente
        }
    }

    tabela.imprimir_identificacoes();

    // Cadastrando chamadas de emergência
    std::cout << "Digite as chamadas (id_paciente gravidade):\n";
    for (int i = 0; i < n; i++) {
        int id, gravidade;
        std::cin >> id >> gravidade;
        sistema.adicionar_chamada(id, gravidade);
    }

    sistema.atender_chamadas();

    return 0;

};
