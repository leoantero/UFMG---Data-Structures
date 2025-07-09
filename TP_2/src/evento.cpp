#include "../include/evento.hpp"

int Evento::contador_sequencia = 0;

Evento::Evento(double t, tipo_evento tp, const string& ori, const string& dest, void* d)
{
    tempo = t;
    tipo = tp;
    origem = ori;
    destino = dest;
    dados = d;
    sequencia = contador_sequencia++;
}