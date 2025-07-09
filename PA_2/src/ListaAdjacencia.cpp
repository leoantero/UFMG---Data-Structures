#include "ListaAdjacencia.hpp"
#include <iostream>

// Construtor inicializando a lista
ListaAdjacencia::ListaAdjacencia() : capacidade(4), tamanho(0), arestas(0)
{
    adjacencia = new Lista[capacidade];
    for (int i = 0; i < capacidade; i++)
    {
        adjacencia[i].start = nullptr;
    }
}

// Destrutor deletando cada parte da lista
ListaAdjacencia::~ListaAdjacencia() 
{
    for (int i = 0; i < tamanho; i++)
    {
        No* atual = adjacencia[i].start;
        while(atual)
        {
            No* temp = atual;
            atual = atual->next;
            delete temp;
        }
    }
    delete[] adjacencia;
}

// Aloca mais capacidade
void ListaAdjacencia::Redimensionar()
{
    int NovaCapacidade = capacidade*2;
    Lista* novo = new Lista[NovaCapacidade];
    for (int i = 0; i < NovaCapacidade; i++)
    {
        novo[i].start = (i < capacidade) ? adjacencia[i].start : nullptr;
    }
    delete[] adjacencia;
    adjacencia = novo;
    capacidade = NovaCapacidade;
}

// Adiciona um vértice novo
void ListaAdjacencia::AdicionaVertice() 
{
    if (tamanho >= capacidade)
    {
        Redimensionar();
    }
    adjacencia[tamanho].start = nullptr;
    ++tamanho;
}

// Insere um valor
void ListaAdjacencia::Inserir(Lista& lista, int valor)
{
    if(Contem(lista, valor)) return;

    No* novo = new No{valor, nullptr};
    if(!lista.start)
    {
        lista.start = novo;
    }
    else
    {
        No* atual = lista.start;
        while(atual->next) atual = atual->next;
        atual->next = novo;
    }
}

// Verifica se um determinado valor já está na lista
bool ListaAdjacencia::Contem(const Lista& lista, int valor) const
{
    No* atual = lista.start;
    while(atual)
    {
        if(atual->valor == valor) return true;
        atual = atual->next;
    }
    return false;
}

// Adiciona uma aresta conectando vértices
void ListaAdjacencia::AdicionaAresta(int v, int w)
{
    if (v >= tamanho || w >= tamanho) return;
    
    if (!Contem(adjacencia[v], w))
    {
        Inserir(adjacencia[v], w);
        Inserir(adjacencia[w], v);
        ++arestas;
    }
}

// Retorna o numero de vertices
int ListaAdjacencia::QuantidadeVertices() const 
{
    return tamanho;
}

// Retorna o numero de arestas
int ListaAdjacencia::QuantidadeArestas() const
{
    return arestas;
}

int ListaAdjacencia::Tamanho(const Lista& lista) const
{
    int count = 0;
    No* atual = lista.start;
    while(atual)
    {
        ++count;
        atual = atual->next;
    }
    return count;
}

int ListaAdjacencia::Grau(int v) const
{
    if (v >= tamanho) return 0;
    return Tamanho(adjacencia[v]);
}

// Procura pelo nó de menor grau 
int ListaAdjacencia::GrauMinimo() const
{
    int min = Grau(0);
    for (int i = 1; i < tamanho; i++)
    {
        int g = Grau(i);
        if (g < min) min = g;
    }
    return min;
}

// Procura pelo nó de maior grau 
int ListaAdjacencia::GrauMaximo() const
{
    int max = Grau(0);
    for (int i = 1; i < tamanho; i++)
    {
        int g = Grau(i);
        if (g > max) max = g;
    }
    return max;
}

// Mostra a estrutura 
void ListaAdjacencia::Imprimir(const Lista& lista) const
{
    No* atual = lista.start;
    while(atual)
    {
        std::cout << atual->valor << " ";
        atual = atual->next;
    }
    std::cout << std::endl;
}

// Mostra os vizinhos
void ListaAdjacencia::ImprimeVizinhos(int v) const
{
    if (v < tamanho) Imprimir(adjacencia[v]);
}