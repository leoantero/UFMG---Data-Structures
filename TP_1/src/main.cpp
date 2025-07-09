#include <iostream>
#include <fstream>
#include <cstdlib>
#include "ordenador_lp.hpp"
#include "ordenador_lq.hpp"
#include "sorting.hpp"

int main(int argc, char* argv[])
{
    // Leitura e verificação se o código conseguiu ler o arquivo
    if (argc != 2)
    {
        std::cerr << "Uso: " << argv[0] << " <nome_do_arquivo>" << std::endl;
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    
    if (!inputFile)
    {
        std::cerr << "Erro ao abrir o arquivo: " << argv[1] << std::endl;
        return 1;
    }

    // Leitura dos parâmetros
    double limiar_custo, a, b, c;
    int seed, tam;

    inputFile >> seed >> limiar_custo >> a >> b >> c >> tam;
    if (tam <= 0)
    {
        std::cerr << "Tamanho inválido." << std::endl;
        return 1;
    }

    // Aloca e lê vetor original
    int* original = new int[tam];
    for (int i = 0; i < tam; i++)
    {
        inputFile >> original[i];
    }
    inputFile.close();

    if(!original)
    {
        std::cerr << "Erro: o vetor de entrada não foi alocado corretamente" << std::endl;
        return -1;
    }

    Ordenador ordenador_lp;
    OrdenadorLQ ordenador_lq;

    ordenador_lp.set_coeficientes(a, b, c);
    ordenador_lq.set_coeficientes(a, b, c);

    srand48(seed);

    // Cálculo do limiar de quebras
    int quebras_comeco = ordenador_lp.numero_quebras(original, tam);
    
    std::cout << "size " << tam
              << " seed " << seed
              << " breaks " << quebras_comeco
              << std::endl << std::endl;

    // Determinação do melhor limiar de partição, primeira parte da saída
    int min_tam_particao = ordenador_lp.determina_limiar_particao(original, tam, limiar_custo);

    // Cópia do vetor de entrada sendo ordenada para a função limQuebras
    int* vetor_ordenado = new int[tam];
    std::copy(original, original + tam, vetor_ordenado);
    Sorting sorter;
    int cmp_temp = 0, mov_temp = 0, calls_temp = 0;
    sorter.insertion_sort(vetor_ordenado, 0, tam - 1, cmp_temp, mov_temp, calls_temp);
    
    // Segunda parte da saída, limQuebras
    int limiar_quebras = ordenador_lq.determina_limiar_quebras(vetor_ordenado, tam, limiar_custo, min_tam_particao, seed, quebras_comeco);

    // Cópia final para ordenar com o melhor limiar
    int* V = new int[tam];
    std::copy(original, original + tam, V);

    int cmp = 0, mov = 0, calls = 0;
    ordenador_lp.ordenador_universal(V, tam, min_tam_particao, limiar_quebras, cmp, mov, calls);

    // Liberação de memória
    delete[] V;
    delete[] original;
    delete[] vetor_ordenado;

    return 0;
}