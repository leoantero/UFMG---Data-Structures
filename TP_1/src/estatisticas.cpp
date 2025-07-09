#include "estatisticas.hpp"
#include <iostream>
#include <iomanip>

// Função para imprimir as iterações 
void Estatisticas::print_iteracao(int it) 
{
    std::cout << "iter " << it << std::endl;
}

// Função para imprimir o resultado a cada linha 
void Estatisticas::print_result(int mps, double cost, int cmp, int move, int calls) 
{
    std::cout << "mps " << mps 
              << " cost " << std::fixed << std::setprecision(9) << cost 
              << " cmp " << cmp 
              << " move " << move 
              << " calls " << calls 
              << std::endl;
}

// Função para imprimir o resumo ao final da iteração
void Estatisticas::print_resumo(int nummps, int lim_particao, float mpsdiff) 
{
    std::cout << "nummps " << nummps 
              << " limParticao " << lim_particao 
              << " mpsdiff " << std::fixed << std::setprecision(6) << mpsdiff 
              << std::endl << std::endl;
}

// Impressão de cada linha da saída de limQuebras
void Estatisticas::print_result_quebras(const char* alg, int lq, double custo, int cmp, int move, int calls)
{
    std::cout << alg << " lq " << lq 
              << " cost " << std::fixed << std::setprecision(9) << custo
              << " cmp " << cmp 
              << " move " << move 
              << " calls " << calls << std::endl;
}

// Resumo das saídas de limQuebras
void Estatisticas::print_resumo_quebras(int numlq, int lim_quebras, double diff)
{
    std::cout << "numlq " << numlq 
              << " limQuebras " << lim_quebras
              << " lqdiff " << std::fixed << std::setprecision(6)
              << diff << std::endl << std::endl;
}