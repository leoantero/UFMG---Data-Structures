#ifndef CLIENTES_HPP
#define CLIENTES_HPP

#include <string>

// Lista que contém cada pacote e suas movimentações de cada cliente
struct ListaPacotesClientes
{
    ListaPacotesClientes* next;
    int id_pacote;
    int index_primeiro_evento;
    int index_ultimo_evento;
};

// Arvore dos clientes que contém o nome do cliente, a lista dos seus pacotes, sua altura e seus filhos
struct NoClientes
{
    NoClientes* esq;
    NoClientes* dir;
    std::string nome_cliente;
    ListaPacotesClientes* pacotes;
    int altura;
};

// Arvore AVL dos clientes que tem funções para manter seu balanceamento, 
// sendo assim possível fazer consultas em logn
class ArvoreClientes
{
    private: 
        NoClientes* raiz;

        int altura_no(NoClientes* no);
        int fator_balanceamento(NoClientes* no);
        NoClientes* rotacao_direita(NoClientes* d);
        NoClientes* rotacao_esquerda(NoClientes* e);
        NoClientes* inserir_helper(NoClientes* no, const std::string& nome, int id_pacote, int index_evento);
        NoClientes* buscar_helper(NoClientes* no, const std::string& nome);
        void limpar_helper(NoClientes* no);
        int max(int a, int b);

    public:
        ArvoreClientes();
        ~ArvoreClientes();

        void inserir_pacote(const std::string& nome, int id_pacote, int index_evento);
        ListaPacotesClientes* buscar_cliente(const std::string& nome);
        void limpar();
    };

#endif