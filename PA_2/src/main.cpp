#include <iostream>
#include <string>
#include "graph.hpp"

int main(int argc, char* argv[]) 
{
    if (argc != 2)
    {
        std::cerr << "Uso: ./pa2.out [-d|-n|-k]\n";
        return 1;
    }

    std::string opcao = argv[1];

    int vertices;
    std::cin >> vertices;

    Grafo g;
    for (int i = 0; i < vertices; i++)
    {
        g.InsereVertice();
    }

    for (int i = 0; i < vertices; i++)
    {
        int arestas;
        std::cin >> arestas;
        for (int j = 0; j < arestas; j++)
        {
            int vizinho;
            std::cin >> vizinho;
            g.InsereAresta(i, vizinho);
        }
    }

    if (opcao == "-d")
    {
        std::cout << g.QuantidadeVertices() << std::endl;
        std::cout << g.QuantidadeArestas() << std::endl;
        std::cout << g.GrauMinimo() << std::endl;
        std::cout << g.GrauMaximo() << std::endl;
    } 
    else if (opcao == "-n")
    {
        for (int i = 0; i < g.QuantidadeVertices(); i++)
        {
            g.ImprimeVizinhos(i);
        }
    }
    else if (opcao == "-k")
    {
        std::cout << (g.QuantidadeVertices() > 0 && g.GrauMinimo() == g.QuantidadeVertices() - 1 ? 1 : 0) << std::endl;
    }
    return 0;
}