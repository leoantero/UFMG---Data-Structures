#include "ordenador_lp.hpp"
#include "sorting.hpp"
#include "estatisticas.hpp"
#include <cmath>
#include <iostream>

// Construtor 
Ordenador::Ordenador() : custos(nullptr), num_custos(0), capacidade_custos(0) {}

// Destrutor
Ordenador::~Ordenador()
{
    delete[] custos;
}

// Função para copiar um array para outro
void Ordenador::copy(int* V, int tam, int* copia) {
    for (int i = 0; i < tam; ++i) 
    {
        copia[i] = V[i];
    }
}

// Função que determina o algoritmo de ordenação de acordo com as especificações do vetor
void Ordenador::ordenador_universal(int* V, int tam, int min_tam_particao, int limiar_quebras, int& cmp, int& mov, int& calls) 
{
    int num_quebras = numero_quebras(V, tam);
    cmp = 0; mov = 0; calls = 0;

    Sorting ordenador;
    
    // Se o número de quebras for menor que o limiar de quebras isso indica
    // que o vetor já tem muitas partes ordenadas e que o insertion será melhor
    if (num_quebras < limiar_quebras) 
    {
        ordenador.insertion_sort(V, 0, tam - 1, cmp, mov, calls);
    } 
    else 
    {
        if (tam >= min_tam_particao) 
        {
            ordenador.quicksort_insertion(V, 0, tam-1, cmp, mov, calls, min_tam_particao);
        } 
        else 
        {
            ordenador.insertion_sort(V, 0, tam - 1, cmp, mov, calls);
        }
    }
}

// Função que faz os cálculos empiricamente para determinar se já é mais eficiente usar insertion 
int Ordenador::determina_limiar_particao(int* V, int tam, double limiar_custo)
{
    int limiar_quebras = numero_quebras(V, tam); 

    int min_mps = 2, max_mps = tam;
    int passo_mps = (max_mps - min_mps) / 5;
    if (passo_mps == 0) passo_mps++;

    float diff_custo = INFINITY;
    int lim_particao = min_mps;
    int it = 0;
    int vetor_mps[10];

    while ((diff_custo > limiar_custo) && (max_mps - min_mps >= 5))
    {
        num_custos = 0;
        int num_mps = 0;

        Estatisticas::print_iteracao(it);

        for (int t = min_mps; t <= max_mps; t += passo_mps)
        {
            // A cada iteração (medida no passo) cria um novo vetor, ordena, calcula, registra e imprime os custos
            int* copia = new int[tam];
            Ordenador::copy(V, tam, copia);
            
            if(!copia)
            {
                std::cerr << "Erro: o vetor da função determina limiar partição não foi alocado corretamente" << std::endl;
                return -1;
            }

            int cmp = 0, mov = 0, calls = 0;
            ordenador_universal(copia, tam, t, limiar_quebras, cmp, mov, calls);

            double custo = calcular_custo(cmp, mov, calls);
            registrar_est(custo);
            Estatisticas::print_result(t, custo, cmp, mov, calls);

            vetor_mps[num_mps] = t;
            delete[] copia;
            num_mps++;
        }

        // Procura pelo melhor indice no vetor com os custos
        int melhor_indice = menor_custo();

        // obtem o valor do lim particao e calcula nova faixa para obter os valores de diff custo
        int lim_particao_real = vetor_mps[melhor_indice];

        calcula_nova_faixa(melhor_indice, min_mps, max_mps, passo_mps, vetor_mps);

        lim_particao = lim_particao_real;

        // Após a função calcula nova faixa, procuramos os valores associados aos novos mps no vetor de mps
        int valor_novo_mps_max = getMPS(vetor_mps, max_mps);
        int valor_novo_mps_min = getMPS(vetor_mps, min_mps);

        diff_custo = fabs(custos[valor_novo_mps_max] - custos[valor_novo_mps_min]);
    
        Estatisticas::print_resumo(num_custos, lim_particao, diff_custo);

        it++;
    }

    return lim_particao;
}

// Função que procura e retorna um determinado mps em um vetor
int Ordenador::getMPS(int* mps, int mps_idx)
{
    int elemento = 0;
    for(int j = 0; j < 10; j++)
    {
        if(mps[j] == mps_idx) elemento = j;
    }
    return elemento;
}

// Calcula o custo com base nos coeficientes e nos parâmetros 
double Ordenador::calcular_custo(int cmp, int mov, int calls) const
{
    return a*cmp + b*mov + c*calls;
}

// Compara 2 elementos do vetor e checa se estão ordenados, se estiver totalmente ordenado o numero de quebras será 0
// Da mesma maneira, se estiver inversamente ordenado o numero de quebras será n - 1
int Ordenador::numero_quebras(int* V, int tam)
{
    int quebras = 0;
    for (int i = 1; i < tam; i++)
    {
        if (V[i] < V[i-1])
        {
            quebras++;
        }
    }
    return quebras;
}

// Caso o vetor de custos esteja cheio expande sua capacidade
void Ordenador::expandir_capacidade_custos()
{
    int nova_capacidade = capacidade_custos == 0 ? 1 : capacidade_custos * 2;
    double* new_array = new double[nova_capacidade];

    for (int i = 0; i < num_custos; i++)
    {
        new_array[i] = custos[i];
    }
    
    delete[] custos;
    custos = new_array;
    capacidade_custos = nova_capacidade;
}

// Registra no vetor custos
void Ordenador::registrar_est(double custo)
{
    if (num_custos >= capacidade_custos)
    {
        expandir_capacidade_custos();
    }
    custos[num_custos++] = custo;
}

// Procura pelo indice do menor elemento
int Ordenador::menor_custo()
{
    if (num_custos == 0) return 0;

    int menor_indice = 0;
    double menor = custos[0];

    for(int i = 1; i < num_custos; i++)
    {
        if (custos[i] < menor)
        {
            menor = custos[i];
            menor_indice = i;
        }
    }
    return menor_indice;
}

// Recalcula os novos valores de mps com base no lim de partição anterior 
void Ordenador::calcula_nova_faixa(int lim_particao, int& min_mps, int& max_mps, int& passo_mps, int* vetor_mps)
{
    int new_min, new_max;

    if(lim_particao == 0)
    {
        new_min = 0;
        new_max = 2;
    }
    else if (lim_particao >= 7)
    {
        new_min = 3;
        new_max = 5;
    }
    else
    {
        new_min = lim_particao - 1;
        new_max = lim_particao + 1;
    }

    min_mps = vetor_mps[new_min];
    max_mps = vetor_mps[new_max];

    passo_mps = (int)(max_mps - min_mps) / 5;
    if (passo_mps == 0) passo_mps++;
}