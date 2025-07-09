#ifndef PACOTES_HPP
#define PACOTES_HPP
#include <iostream>

// Estrutura de dados que armazena uma lista com os pacotes 
struct ListaPacote
{
    ListaPacote* next;
    int index_evento;
};

// Struct do árvore de pacotes, cada pacote contém dois nós, seu id, 
// sua altura e uma lista de eventos relacionados àquele pacote
struct NoPacote
{
    NoPacote* esq;
    NoPacote* dir;
    int id_pacote;
    ListaPacote* lista_eventos;
    int altura; 
};

// Classe da árvore dos pacotes que tem funções para obter elementos, balancear a arvore, fazer rotações, etc
// Para um bom funcionamento do algoritmo e para ter um bom resultado nas leituras as arvores são balanceadas,
// possibilitando procuras em logn
class ArvorePacotes
{
    private: 
        NoPacote* raiz;

        int altura_no(NoPacote* no);
        int fator_balanceamento(NoPacote* no);
        NoPacote* rotacao_direita(NoPacote* d);
        NoPacote* rotacao_esquerda(NoPacote* e);
        NoPacote* inserir_helper(NoPacote* no, int id_pacote, int index_evento);
        NoPacote* buscar_helper(NoPacote* no, int id_pacote);
        void limpar_helper(NoPacote* no);
        int max(int a, int b);

    public:
        ArvorePacotes();
        ~ArvorePacotes();

        void inserir_evento(int id_pacote, int index_evento);
        ListaPacote* buscar_pacote(int id_pacote);
        void limpar();
};

#endif