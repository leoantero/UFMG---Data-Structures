#include "unionFind.hpp"
#include <iostream>

// Ao ordenar as arestas evitamos pegar arestas com alto custo logo no começo
// e podemos ir pegando as de menor custo
void ordenar_arestas(Aresta* arestas, int tamanho)
{
    for(int i = 0; i < tamanho; i++)
    {
        int min_idx = i;
        for(int j = i + 1; j < tamanho; j++)
        {
            if(arestas[j].custo < arestas[min_idx].custo)
            {
                min_idx = j;
            }
        }
        if(min_idx != 1)
        {
            Aresta temp = arestas[i];
            arestas[i] = arestas[min_idx];
            arestas[min_idx] = temp;
        }
    }
}

int main()
{
    int vertices, num_arestas;

    std::cin >> vertices >> num_arestas;

    // Vetor de arestas 
    Aresta* graph =  new Aresta[num_arestas];

    // Cada aresta conecta dois vértices u e v, e tem um determinado peso
    for(int i = 0; i < num_arestas; i++)
    {
        std::cin >> graph[i].u >> graph[i].v >> graph[i].custo;
    }

    // Ordenamos as arestas para facilitar o calculo 
    ordenar_arestas(graph, num_arestas);

    // Inicializa DSU com cada vértice sendo seu próprio conjunto 
    UnionFind uf(vertices);
    
    int custo_total = 0;

    // Percorre todas as arestas 
    for(int i = 0; i < num_arestas; i++)
    {
        int u = graph[i].u;
        int v = graph[i].v;
        if(uf.Find(u) != uf.Find(v)) // Verifica se os vértices já não estão no mesmo componente 
        {
            uf.Union(u, v);
            custo_total += graph[i].custo;
        }
    }

    std::cout << custo_total << std::endl;

    delete[] graph;
    return 0;
}