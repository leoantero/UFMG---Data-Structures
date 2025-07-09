#ifndef ESCALONADOR_HPP
#define ESCALONADOR_HPP

#include "evento.hpp"
#include "pacote.hpp"

class Escalonador 
{
    private:
        Evento** heap;
        int capacidade;
        int tamanho;

        void subir(int i);
        void descer(int i);
        void swap(int i, int j);
        void redimensionar();

    public:
        Escalonador();
        ~Escalonador();

        void inserir(Evento* e);
        Evento* retirar_proximo();
        bool is_vazio() const;
        bool tem_maior_prioridade(Evento* a, Evento* b);
};

#endif