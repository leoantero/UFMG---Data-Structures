#ifndef ARMAZEM_HPP
#define ARMAZEM_HPP

#include "pacote.hpp"
#include <string>

using std::string;

struct NoAdj 
{
    string destino;
    NoAdj* next;

    NoAdj(const string& x);
};

struct NoPilha
{
    Pacote* pacote;
    double tempo_entrada;
    NoPilha* next;

    NoPilha(Pacote* p, double tempo);
};

struct Secao
{
    string destino;
    NoPilha* topo;
    Secao* next;

    Secao(const string& dest);
};

class Armazem 
{
    private:
        string nome;
        NoAdj* lista_adjacentes;
        Secao* lista_secoes;
        void liberar_vizinhos();
        void liberar_secoes();

    public: 
        Armazem(const string& nome_armazem);
        ~Armazem();

        void add_vizinho(const string& nome_vizinho);
        bool eh_vizinho(const string& destino) const;

        void armazenar_pacote(Pacote* pacote, const string& destino, double tempo);
        Pacote* retirar_pacote(const string& destino_da_secao, double tempo);
        bool secao_vazia(const string& destino) const;
        bool esta_vazio() const;

        string get_nome() const;
        NoAdj* get_vizinho() const;
};

#endif