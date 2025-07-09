#include "graph.hpp"

Grafo::Grafo() {}

Grafo::~Grafo() {}

void Grafo::InsereVertice() 
{
    vertices.AdicionaVertice();
}

void Grafo::InsereAresta(int v, int w)
{
    vertices.AdicionaAresta(v, w);
}

int Grafo::QuantidadeVertices()
{
    return vertices.QuantidadeVertices();
}

int Grafo::QuantidadeArestas()
{
    return vertices.QuantidadeArestas();
}

int Grafo::GrauMinimo()
{
    return vertices.GrauMinimo();
}

int Grafo::GrauMaximo()
{
    return vertices.GrauMaximo();
}

void Grafo::ImprimeVizinhos(int v)
{
    vertices.ImprimeVizinhos(v);
}