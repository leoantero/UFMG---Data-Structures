#include "../include/pacote.hpp"
#include <iostream>

// Construtores e Destrutores
Tempos::Tempos()
{
    tempo_armazenado = 0;
    tempo_transito = 0;
    tempo_total = 0;
}

NoRota::NoRota(const string& nome)
{
    armazem_atual = nome;
    next = nullptr;
}

Pacote::Pacote(int id_, const string& ori, const string& dest, double postagem)
{
    id = id_;
    origem = ori;
    destino = dest;
    horario_postagem = postagem;

    rota = nullptr;
    proximo_destino = nullptr;

    estado = NAO_POSTADO;
    tempos = Tempos();
}

Pacote::~Pacote()
{
    liberar_rota();
}

// Define a rota do pacote no momento da postagem, inicializa os ponteiros
void Pacote::definir_rota(NoRota* inicio_rota)
{
    rota = inicio_rota;
    proximo_destino = inicio_rota;
}

// Avança o pacote nos armazens
void Pacote::avancar_rota()
{
    if(proximo_destino != nullptr)
    {
        proximo_destino = proximo_destino->next;
    }
}

// Atualiza o estado do pacote
void Pacote::atualiza_estado(Estado_pacote novo_estado)
{
    estado = novo_estado;
}

// Atualiza quando tempo o pacote ficou armazenado
void Pacote::add_tempo_armazenado(double tempo)
{
    tempos.tempo_armazenado += tempo;
    tempos.tempo_total += tempo;
}

// Getters
Estado_pacote Pacote::get_estado() const
{
    return estado;
}

int Pacote::get_id() const 
{
    return id;
}

string Pacote::get_origem() const
{   
    return origem;
}

string Pacote::get_destino() const
{
    return destino;
}

string Pacote::get_next_destino() const 
{
    if (proximo_destino != nullptr && proximo_destino->next != nullptr)
    {
        return proximo_destino->next->armazem_atual;
    }
    return "";
}

double Pacote::get_tempo_postagem() const
{
    return horario_postagem;
}

string Pacote::get_local_atual() const
{
    if(proximo_destino != nullptr)
    {
        return proximo_destino->armazem_atual;
    }
    return "";
}

double Pacote::get_tempo_entrada() const 
{
    return this->tempo_entrada_secao;
}

// Desfaz a rota do pacote
void Pacote::liberar_rota() 
{
    NoRota* atual = rota;
    while(atual != nullptr)
    {
        NoRota* next = atual->next;
        delete atual;
        atual = next;
    }
    rota = nullptr;
    proximo_destino = nullptr;
}

// Set para o momento que o pacote entra na seção
void Pacote::set_tempo_entrada(double tempo) 
{
    this->tempo_entrada_secao = tempo;
}