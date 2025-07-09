#include "unionFind.hpp"

// Construtor
UnionFind::UnionFind(int n) 
{
    subconjuntos = new Subconjunto[n];
    tamanho = n;
    for(int i = 0; i < tamanho; i++)
    {
        Make(i);
    }
}

// Destrutor
UnionFind::~UnionFind() 
{
    delete[] subconjuntos;
}

// Unir dois conjuntos
// caso eles sejam de conjuntos diferentes o de maior rank prevalece 
void UnionFind::Union(int x, int y)
{
    int raiz_x = Find(x);
    int raiz_y = Find(y);

    if(raiz_y == raiz_x) return; // já estão no mesmo conjunto
    
    if(subconjuntos[raiz_x].rank < subconjuntos[raiz_y].rank)
    {
        subconjuntos[raiz_x].representante = raiz_y;
    } 
    else if(subconjuntos[raiz_x].rank > subconjuntos[raiz_y].rank)
    {
        subconjuntos[raiz_y].representante = raiz_x;
    }
    else
    {
        subconjuntos[raiz_y].representante = raiz_x;
        subconjuntos[raiz_x].rank++;
    }
    
}

// Retona o representante do grupo x
int UnionFind::Find(int x)
{
    if(subconjuntos[x].representante != x)
    {
        subconjuntos[x].representante = Find(subconjuntos[x].representante);
    }
    return subconjuntos[x].representante;
}

// Cria subconjuntos com o único elemento sendo o representante 
void UnionFind::Make(int x)
{
    subconjuntos[x].representante = x;
    subconjuntos[x].rank = 0;
}