#include "../include/escalonador.hpp"
#include <iostream>

// Começa com uma determinada capacidade e caso seja necessário, existe 
// a função redimensionar para aumentar o tamanho
Escalonador::Escalonador()
{
    capacidade = 32;
    tamanho = 0;
    heap = new Evento*[capacidade];
}

// Destrutor
Escalonador::~Escalonador()
{
    for(int i = 0; i < tamanho; i++) delete heap[i];
    delete[] heap;
}

// Função swap para facilitar troca no vetor
void Escalonador::swap(int i, int j)
{   
    Evento* temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
}

// As funções subir e descer fazem a manutenção do min heap de acordo 
// com a prioridade, garantindo que o escalonador funcione corretamente
void Escalonador::subir(int i) 
{
    while (i > 0) 
    {
        int pai = (i - 1) / 2;
        if (tem_maior_prioridade(heap[i], heap[pai])) 
        {
            swap(i, pai);
            i = pai;
        } 
        else 
        {
            break;
        }
    }
}

void Escalonador::descer(int i) 
{
    while (true) {
        int esq = 2 * i + 1;
        int dir = 2 * i + 2;
        int menor = i;

        if (esq < tamanho && tem_maior_prioridade(heap[esq], heap[menor])) 
        {
            menor = esq;
        }
        if (dir < tamanho && tem_maior_prioridade(heap[dir], heap[menor])) 
        {
            menor = dir;
        }
        if (menor != i) 
        {
            swap(i, menor);
            i = menor;
        } 
        else 
        {
            break;
        }
    }
}

// Caso a capacidade total seja atingida a função redimensionar dobra o tamanho 
void Escalonador::redimensionar()
{
    capacidade *= 2;
    Evento** novo = new Evento*[capacidade];
    for(int i = 0; i < tamanho; i++) novo[i] = heap[i];
    delete[] heap;
    heap = novo;
}

// Insere um novo evento no escalonador (heap)
void Escalonador::inserir(Evento* e)
{
    if (tamanho == capacidade) redimensionar();
    heap[tamanho] = e;
    subir(tamanho);
    tamanho++;
}

// Pega o próximo evento a ser retirado
Evento* Escalonador::retirar_proximo()
{
    if(tamanho == 0) 
    {
        std::cerr << "ERRO na retirada de eventos do escalonador" << std::endl;
        return nullptr;
    }
    Evento* min = heap[0];
    heap[0] = heap[--tamanho];
    descer(0);
    return min;
}

// Verifica se ainda existem eventos no escalonador
bool Escalonador::is_vazio() const
{
    return tamanho == 0;
}

// Função para determinar qual evento tem prioridade sobre o outro
bool Escalonador::tem_maior_prioridade(Evento* a, Evento* b) 
{
    // Critério 1: Tempo, o mais importante
    if (a->tempo < b->tempo) return true;
    if (a->tempo > b->tempo) return false;

    // Se o tempo é igual, precisamos comparar os dados do evento,
    // para comparar um evento de CHEGADA com um de TRANSPORTE
    // precisamos ter os "dados" de cada um no mesmo formato
    
    long long dados_a = 0;
    long long dados_b = 0;

    if (a->tipo == CHEGADA) 
    {
        dados_a = static_cast<Pacote*>(a->dados)->get_id();
    } 
    else 
    {
        dados_a = std::stoll(a->origem) * 1000 + std::stoll(a->destino);
    }

    if (b->tipo == CHEGADA) 
    {
        dados_b = static_cast<Pacote*>(b->dados)->get_id();
    } 
    else 
    {
        dados_b = std::stoll(b->origem) * 1000 + std::stoll(b->destino);
    }
    
    // Critério 2: Dados do evento
    if (dados_a < dados_b) return true;
    if (dados_a > dados_b) return false;

    // Critério 3: Tipo do evento, CHEGADA tem prioridade sobre TRANSPORTE
    if (a->tipo < b->tipo) return true;
    if (a->tipo > b->tipo) return false;

    // Se tudo for igual, usamos a sequência de criação
    return a->sequencia < b->sequencia;
}