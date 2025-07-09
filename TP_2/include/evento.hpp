#ifndef EVENTO_HPP
#define EVENTO_HPP

#include <string>

using std::string;

enum tipo_evento
{
    CHEGADA, // Prioridade maior
    TRANSPORTE // Prioridade menor
};

struct Evento
{
    double tempo;
    tipo_evento tipo;
    string origem;
    string destino;
    void* dados;

    // Variáveis para desempatar
    static int contador_sequencia;
    int sequencia;

    long long chave_prioridade;
    
    Evento(double t, tipo_evento tp, const string& ori, const string& dest, void* d);
};

#endif