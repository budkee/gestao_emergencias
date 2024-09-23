/**************************************************
 *
 * Isabele Bicudo e Kaê de Oliveira Budke
 * Trabalho 1
 * Professor(a): Fábio Henrique Viduani Martinez
 *
 */

// Bibliotecas
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <climits>
// Permite declarar variáveis e funções relacionadas a strings e vetores sem o uso de std::
using std::string;
using std::vector;

#define MAX 50

// ---------- Registros médicos ---------- //
/*
    "Uma tabela de dispersão é usada para armazenar os registros médicos dos pacientes, permitindo uma busca rápida pelo seu histórico, onde a chave é o número de identificação do paciente, mais outras informações como nome e enfermidade."
*/
class Paciente
{
public:
    string nome;
    string enfermidade;

    Paciente() : nome(""), enfermidade("") {}
};

class TabelaDispersao
{
private:
    Paciente pacientes[MAX];
    int ids[MAX];

    int calcular_id(const string &nome)
    {
        int soma = 0;
        for (char c : nome)
        {
            soma += c;
        }
        return soma % MAX;
    }

    // Função de dispersão
    int insere_aberto(int x)
    {
        int i = 0;
        int j;
        do
        {
            // Calcula a posição j garantindo que esteja dentro dos limites da tabela
            j = (x + i) % MAX;
            // Se a posição j estiver vazia, insere x
            if (ids[j] == -1)
            {
                ids[j] = x;
                return j;
            }
            i++;
        } while (i < MAX); // Enquanto não percorrer toda a tabela
        return MAX; // Tabela cheia
    }

public:
    // Construtor da tabela com posições vazias
    TabelaDispersao()
    {
        for (int i = 0; i < MAX; i++)
        {
            ids[i] = -1;
        }
    }

    // Processar entrada do usuário
    void processar_entrada(const string &linha)
    {

        string nome, enfermidade;
        size_t virgula_pos = linha.find(',');
        // Se encontrar a vírgula, separa nome e enfermidade
        if (virgula_pos != string::npos)
        {
            nome = linha.substr(0, virgula_pos);
            enfermidade = linha.substr(virgula_pos + 1);

            // Remove espaços em branco no início da enfermidade
            enfermidade.erase(0, enfermidade.find_first_not_of(" "));

            // Verifica se as strings não estão vazias
            if (!nome.empty() && !enfermidade.empty())
            {
                cadastrar_paciente(nome, enfermidade);
            }
            else
            {
                std::cout << "Entrada inválida!\n";
            }
        }
        else
        {
            std::cout << "Formato inválido! Esperado: Nome, Enfermidade\n";
        }
    }

    void cadastrar_paciente(const string &nome, const string &enfermidade)
    {

        int id = calcular_id(nome);
        int posicao = insere_aberto(id);
        // Se a posição estiver vazia, insere o paciente
        if (posicao < MAX)
        {
            pacientes[posicao].nome = nome;
            pacientes[posicao].enfermidade = enfermidade;
        }
        else
        {
            std::cout << "Tabela cheia\n";
        }
    }

    void imprimir_identificacoes()
    {
        std::cout << "\nIdentificações dos pacientes:\n";
        for (int i = 0; i < MAX; i++)
        {
            // Se a posição não estiver vazia, imprime o ID e o nome do paciente
            if (ids[i] != -1)
            {
                std::cout << ids[i] << " " << pacientes[i].nome << "\n";
            }
        }
    }

    // Retorna o paciente com o ID de interesse
    Paciente *get_paciente(int id)
    {
        for (int i = 0; i < MAX; i++)
        {
            // Se o ID for encontrado, retorna o paciente
            if (ids[i] == id)
            {
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

class Chamada
{
public:
    int id_paciente;
    int gravidade;
    // Construtor
    Chamada(int id, int grav) : id_paciente(id), gravidade(grav) {}
};

// --------- Manutenção do Max-Heap | Lista de Prioridades --------- //
class MaxHeap
{
private:
    vector<Chamada *> chamadas;

    // Funções para manter a propriedade de Max-Heap
    int pai(int i)
    {
        return (i - 1) / 2;
    }

    int esquerdo(int i)
    {
        return 2 * (i + 1) - 1;
    }

    int direito(int i)
    {
        return 2 * (i + 1);
    }

    void troca(int i, int j)
    {
        Chamada *aux = chamadas[i];
        chamadas[i] = chamadas[j];
        chamadas[j] = aux;
    }

    void desce(int i)
    {
        int e, d, maior;

        e = esquerdo(i);
        d = direito(i);
        maior = i;

        if (e < (int)chamadas.size() && chamadas[e]->gravidade > chamadas[i]->gravidade)
            maior = e;
        else
            maior = i;

        if (d < (int)chamadas.size() && chamadas[d]->gravidade > chamadas[maior]->gravidade)
            maior = d;

        if (maior != i)
        {
            troca(i, maior);
            desce(maior);
        }
    }

    void sobe(int i)
    {
        while (i > 0 && chamadas[pai(i)]->gravidade < chamadas[i]->gravidade)
        {
            troca(i, pai(i));
            i = pai(i);
        }
    }

public:
    MaxHeap() {};

    void insere(Chamada *chamada)
    {
        // Insere a chamada no final do vetor
        chamadas.push_back(chamada);
        // Restaura a propriedade Max-Heap
        sobe(chamadas.size() - 1);
    }

    Chamada *extrai_maxima()
    {

        if (chamadas.size() > 0)
        {

            Chamada *maior = chamadas[0];
            chamadas[0] = chamadas.back();
            chamadas.pop_back();
            desce(0);
            return maior;
        }
        else
        {
            return nullptr; // Lista vazia
        }
    }

    int tamanho()
    {
        return chamadas.size();
    }
};

class ListaPrioridade
{
private:
    TabelaDispersao *tabela;
    MaxHeap chamadas_heap;

public:
    // Construtor
    ListaPrioridade(TabelaDispersao *t) : tabela(t) {}

    // Adiciona chamadas de emergência | Construção do MaxHeap e Inserção na Lista de Prioridades
    void adicionar_chamada(int id, int gravidade)
    {
        Chamada *nova_chamada = new Chamada(id, gravidade);
        chamadas_heap.insere(nova_chamada);
    }

    // chamadas_heap.constroi_max_heap(num_chamadas, chamadas_heap.chamadas);

    // Atendimento das chamadas | Extração Lista de Prioridades
    void atender_chamadas()
    {
        std::cout << "\nLista de Atendimentos Prioritários: \n";

        // Imprime a lista de atendimentos prioritários | Extração do mais urgente ao menos urgente
        while (chamadas_heap.tamanho() > 0)
        {
            Chamada *chamada_mais_urgente = chamadas_heap.extrai_maxima();
            Paciente *paciente = tabela->get_paciente(chamada_mais_urgente->id_paciente);

            if (paciente != nullptr)
            {
                std::cout << paciente->nome << " " << chamada_mais_urgente->gravidade << "\n";
            }
            delete chamada_mais_urgente;
        }

        // while (chamadas_heap.tamanho() > 0)
        // {
        //     Chamada *chamada_mais_urgente = chamadas_heap.extrai_maxima();
        //     std::cout << "Extraindo chamada - ID do paciente: " << chamada_mais_urgente->id_paciente
        //             << ", Gravidade: " << chamada_mais_urgente->gravidade << "\n";

        //     Paciente *paciente = tabela->get_paciente(chamada_mais_urgente->id_paciente);

        //     if (paciente != nullptr)
        //     {
        //         std::cout << "Paciente: " << paciente->nome << " com gravidade: "
        //                 << chamada_mais_urgente->gravidade << "\n";
        //     }
        //     else
        //     {
        //         std::cout << "Erro: Paciente com ID " << chamada_mais_urgente->id_paciente << " não encontrado!\n";
        //     }

        //     delete chamada_mais_urgente;
        // }
    }
};

int main()
{

    TabelaDispersao tabela;
    ListaPrioridade lista_prioridade(&tabela);

    // ----------------- Cadastrando pacientes ----------------- //
    int n;
    std::cout << "Número de pacientes: ";
    std::cin >> n;
    std::cin.ignore(); // Limpa o buffer

    for (int i = 0; i < n; i++)
    {
        char linha[MAX];
        std::cout << "Digite o nome e a enfermidade (formato: Nome, Enfermidade): ";
        std::cin.getline(linha, sizeof(linha));

        tabela.processar_entrada(linha); // Passa a linha completa para processar_entrada
    }

    tabela.imprimir_identificacoes(); // Imprime as identificações e os respectivos nomes dos pacientes

    // ----------------- Cadastrando chamadas de emergência ----------------- //
    std::cout << "Digite as chamadas (id_paciente gravidade):\n";

    for (int i = 0; i < n; i++)
    {
        int id, gravidade;
        std::cin >> id >> gravidade;
        lista_prioridade.adicionar_chamada(id, gravidade); // Adiciona a gravidade da chamada
    }

    lista_prioridade.atender_chamadas(); // Imprime a lista de prioridades

    return 0;
};
