#include <iostream>
#include <fstream>
#include <string>
#include "../include/simulador.hpp"

int main(int argc, char* argv[]) 
{
    // Leitura e verificação se o código conseguiu ler o arquivo
    if (argc != 2) 
    {
        std::cerr << "Uso correto: " << argv[0] << " <arquivo_entrada>" << std::endl;
        return 1;
    }

    std::ifstream arquivo(argv[1]);
    if (!arquivo.is_open()) 
    {
        std::cerr << "Erro ao abrir o arquivo: " << argv[1] << std::endl;
        return 1;
    }

    // Leitura dos parâmetros de entrada 
    int capacidade, latencia, intervalo, custo_remocao, numero_armazens;
    arquivo >> capacidade >> latencia >> intervalo >> custo_remocao;
    arquivo >> numero_armazens;

    Simulador simulador;
    simulador.set_parametros(capacidade, latencia, intervalo, custo_remocao, numero_armazens);
   

    // Adicionar armazéns
    for(int i = 0; i < numero_armazens; i++) 
    {
        simulador.add_armazem(std::to_string(i));
    }

    // Ler matriz de adjacência e conectar armazéns
    for(int i = 0; i < numero_armazens; i++) 
    {
        for(int j = 0; j < numero_armazens; j++) 
        {
            int conexao;
            arquivo >> conexao;
            if (conexao == 1 && i != j) 
            {
                simulador.conectar_armazens(std::to_string(i), std::to_string(j));
            }
        }
    }

    // Ler pacotes
    int num_pacotes;
    arquivo >> num_pacotes;

    for(int i = 0; i < num_pacotes; i++) 
    {
        double tempo;
        std::string token_pac, token_org, token_dst, origem, destino;
        int id;
        
        if (arquivo >> tempo >> token_pac >> id >> token_org >> origem >> token_dst >> destino) 
        {
            if (token_pac == "pac" && token_org == "org" && token_dst == "dst") 
            {
                simulador.add_pacote(id, origem, destino, tempo);
            }
        }
    }
    
    if(numero_armazens <= 0 || num_pacotes <= 0)
    {
        std::cerr << "ERRO: Número de armazens e de pacotes deve ser inteiro positivo" << std::endl;
        return 1;
    }
    
    arquivo.close();

    // Executar simulação
    simulador.inicializar_eventos();
    simulador.executar();
    return 0;
}