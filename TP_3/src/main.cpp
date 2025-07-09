#include <iostream>
#include <fstream>
#include <string>
#include "../include/execucao.hpp"

int main(int argc, char* argv[]) 
{

    // Le o arquivo e verifica se foi aberto corretamente 
    if (argc != 2) 
    {
        std::cerr << "Uso correto: " << argv[0] << " <arquivo_de_entrada>" << std::endl;
        return 1;
    }

    std::ifstream arquivo(argv[1]);
    
    if (!arquivo.is_open()) 
    {
        std::cerr << "Erro ao abrir o arquivo: " << argv[1] << std::endl;
        return 1;
    }

    // Direciona para a parte do código que processa a entrada e roda a lógica 
    processar_entrada(arquivo);

    // Fecha o arquivo
    arquivo.close();
    return 0;
}