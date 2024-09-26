/**************************************************
 *
 * Isabelle Bicudo e Kaê de Oliveira Budke
 * Trabalho 1
 * Professor(a): Fábio Henrique Viduani Martinez
 *
 */

// Bibliotecas
#include <iostream>
#include <cstdlib>

#define MAX 200
#define MAX_NOME 70
#define MAX_ENFERMIDADE 50

// ---------- Registros médicos ---------- //

class Paciente
{
public:
    char nome[MAX_NOME];
    char enfermidade[MAX_ENFERMIDADE];

    Paciente()
    {
        nome[0] = '\0';
        enfermidade[0] = '\0';
    }
};

class TabelaDispersao
{
private:
    Paciente pacientes[MAX];
    int ids[MAX];

    int calcular_id(const char *nome)
    {
        int hash = 0;
        int primo = 31; // Fator primo para melhor dispersão

        for (int i = 0; nome[i] != '\0'; i++)
        {
            hash = primo * hash + nome[i]; // Multiplica o hash anterior por um número primo e adiciona o valor ASCII do caractere
        }

        return abs(hash) % MAX; // Garante que o hash seja positivo e dentro do intervalo permitido
    }

    int insere_aberto(int x)
    {
        int i = 0;
        int j;
        do
        {
            j = (x + i * i) % MAX;
            if (ids[j] == -1)
            {
                ids[j] = x;
                return j;
            }
            i++;
        } while (i < MAX);
        return MAX; // Tabela cheia
    }

    void copiar_string(const char *origem, char *destino)
    {
        int i = 0;
        while (origem[i] != '\0' && i < MAX_NOME - 1)
        {
            destino[i] = origem[i];
            i++;
        }
        destino[i] = '\0';
    }

public:
    TabelaDispersao()
    {
        for (int i = 0; i < MAX; i++)
        {
            ids[i] = -1;
        }
    }

    void processar_entrada(char *linha)
    {
        char nome[MAX_NOME];
        char enfermidade[MAX_ENFERMIDADE];
        int i = 0, j = 0;

        // Separar nome e enfermidade pela vírgula
        while (linha[i] != ',' && linha[i] != '\0' && i < MAX_NOME - 1)
        {
            nome[i] = linha[i];
            i++;
        }
        nome[i] = '\0';

        if (linha[i] == ',')
        {
            i++; // Ignorar a vírgula
        }

        while (linha[i] == ' ')
        {
            i++; // Ignorar espaços após a vírgula
        }

        while (linha[i] != '\0' && j < MAX_ENFERMIDADE - 1)
        {
            enfermidade[j] = linha[i];
            i++;
            j++;
        }
        enfermidade[j] = '\0';

        if (nome[0] != '\0' && enfermidade[0] != '\0')
        {
            cadastrar_paciente(nome, enfermidade);
        }
        else
        {
            std::cout << "Entrada inválida!\n";
        }
    }

    void cadastrar_paciente(const char *nome, const char *enfermidade)
    {
        int id = calcular_id(nome);
        int posicao = insere_aberto(id);
        if (posicao < MAX)
        {
            copiar_string(nome, pacientes[posicao].nome);
            copiar_string(enfermidade, pacientes[posicao].enfermidade);
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
            if (ids[i] != -1)
            {
                std::cout << ids[i] << " " << pacientes[i].nome << "\n";
            }
        }
    }

    Paciente *get_paciente(int id)
    {
        for (int i = 0; i < MAX; i++)
        {
            if (ids[i] == id)
            {
                return &pacientes[i];
            }
        }
        return nullptr;
    }
};

// ---------- Gerenciamento das chamadas de emergência ---------- //

class Chamada
{
public:
    int id_paciente;
    int gravidade;

    Chamada(int id, int grav) : id_paciente(id), gravidade(grav) {}
};

// --------- Manutenção do Max-Heap | Lista de Prioridades --------- //

class MaxHeap
{
private:
    Chamada *chamadas[MAX];
    int tamanho_atual;

    int pai(int i)
    {
        return (i - 1) / 2;
    }

    int esquerdo(int i)
    {
        return 2 * i + 1;
    }

    int direito(int i)
    {
        return 2 * i + 2;
    }

    void troca(int i, int j)
    {
        Chamada *aux = chamadas[i];
        chamadas[i] = chamadas[j];
        chamadas[j] = aux;
    }

    void desce(int i)
    {
        int e = esquerdo(i);
        int d = direito(i);
        int maior = i;

        if (e < tamanho_atual && chamadas[e]->gravidade > chamadas[i]->gravidade)
        {
            maior = e;
        }

        if (d < tamanho_atual && chamadas[d]->gravidade > chamadas[maior]->gravidade)
        {
            maior = d;
        }

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
    MaxHeap() : tamanho_atual(0) {}

    void insere(Chamada *chamada)
    {
        if (tamanho_atual < MAX)
        {
            chamadas[tamanho_atual] = chamada;
            sobe(tamanho_atual);
            tamanho_atual++;
        }
    }

    Chamada *extrai_maxima()
    {
        if (tamanho_atual > 0)
        {
            Chamada *maior = chamadas[0];
            chamadas[0] = chamadas[tamanho_atual - 1];
            tamanho_atual--;
            desce(0);
            return maior;
        }
        else
        {
            return nullptr;
        }
    }

    int tamanho()
    {
        return tamanho_atual;
    }
};

class ListaPrioridade
{
private:
    TabelaDispersao *tabela;
    MaxHeap chamadas_heap;

public:
    ListaPrioridade(TabelaDispersao *t) : tabela(t) {}

    void adicionar_chamada(int id, int gravidade)
    {
        Chamada *nova_chamada = new Chamada(id, gravidade);
        chamadas_heap.insere(nova_chamada);
    }

    void atender_chamadas()
    {
        std::cout << "\nLista de Atendimentos Prioritários: \n";

        while (chamadas_heap.tamanho() > 0)
        {
            Chamada *chamada_mais_urgente = chamadas_heap.extrai_maxima();
            Paciente *paciente = tabela->get_paciente(chamada_mais_urgente->id_paciente);

            if (paciente != nullptr)
            {
                std::cout << paciente->nome << "\n";
            }
            delete chamada_mais_urgente;
        }
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
        char linha[MAX_NOME + MAX_ENFERMIDADE];
        std::cout << "Digite o nome e a enfermidade (formato: Nome, Enfermidade): ";
        std::cin.getline(linha, sizeof(linha));

        tabela.processar_entrada(linha);
    }

    tabela.imprimir_identificacoes();

    // ----------------- Cadastrando chamadas de emergência ----------------- //
    std::cout << "\nDigite as chamadas (id_paciente gravidade):\n";

    for (int i = 0; i < n; i++)
    {
        int id, gravidade;
        std::cin >> id >> gravidade;
        lista_prioridade.adicionar_chamada(id, gravidade);
    }

    lista_prioridade.atender_chamadas();

    return 0;
}