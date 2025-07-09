#include "ordenador_lq.hpp"
#include "sorting.hpp"
#include "estatisticas.hpp"
#include <cmath>
#include <iostream>

// Construtor 
OrdenadorLQ::OrdenadorLQ() : custos(nullptr), num_custos(0), capacidade_custos(0) {}

// Destrutor
OrdenadorLQ::~OrdenadorLQ()
{
    delete[] custos;
}

// Função para copiar um array para outro
void OrdenadorLQ::copy(int* V, int tam, int* copia) {
    for (int i = 0; i < tam; ++i) 
    {
        copia[i] = V[i];
    }
}

// Embaralha o vetor com lq quebras
void OrdenadorLQ::shuffleVector(int* vet, int tam, int num_shuffle)
{
    int p1 = 0, p2 = 0, temp;
    for (int t = 0; t < num_shuffle; t++)
    {
        while (p1 == p2)
        {
            p1 = (int)(drand48() * tam);
            p2 = (int)(drand48() * tam);
        }
        temp = vet[p1];
        vet[p1] = vet[p2];
        vet[p2] = temp;
        p1 = p2 = 0;
    }
}

// Determina o lqdiff e limQuebras
int OrdenadorLQ::determina_limiar_quebras(int* vetor, int tam, double limiar_custo, int min_tam_particao, int seed, int numero_quebras)
{

    int min_lq = 1;
    int max_lq = tam / 2;
    int passo_lq = (max_lq - min_lq) / 5;
    if (passo_lq == 0) passo_lq++;

    double custos_insertion[10];
    int vetor_lqs[10];
    float lqdiff = INFINITY;
    int lim_quebras = min_lq;
    int it = 0;
    int num_lq = 10;

    while ((lqdiff > limiar_custo) && (num_lq >= 5))
    {
        num_custos = 0;
        num_lq = 0;

        Estatisticas::print_iteracao(it);

        for (int lq = min_lq; lq <= max_lq; lq += passo_lq)
        {
            // prepara dois vetores para testar, um para cada algoritmo de forma separada
            int* teste_qs = new int[tam];
            int* teste_in = new int[tam];
            OrdenadorLQ::copy(vetor, tam, teste_qs);
            OrdenadorLQ::copy(vetor, tam, teste_in);
            
            if(!teste_in || !teste_qs)
            {
                std::cerr << "Erro: o vetor da função determina limiar quebras não foi alocado corretamente" << std::endl;
                return -1;
            }

            // quicksort
            // gera a semente, embaralha o vetor, inicializa os custos de ordenação, ordena, salva os custos e imprime
            srand48(seed);
            shuffleVector(teste_qs, tam, lq);
            int cmp_qs=0, mov_qs=0, calls_qs=0;
            
            Sorting sorter;
            sorter.quicksort_insertion(teste_qs, 0, tam-1, cmp_qs, mov_qs, calls_qs, min_tam_particao);
            double custo_qs = calcular_custo(cmp_qs, mov_qs, calls_qs);
            Estatisticas::print_result_quebras("qs", lq, custo_qs, cmp_qs, mov_qs, calls_qs);

            // insertion
            // gera a semente, embaralha o vetor, inicializa os custos de ordenação, ordena, salva os custos e imprime
            srand48(seed);
            shuffleVector(teste_in, tam, lq);
            int cmp_in=0, mov_in=0, calls_in=0;

            sorter.insertion_sort(teste_in, 0, tam-1, cmp_in, mov_in, calls_in);
            double custo_in = calcular_custo(cmp_in, mov_in, calls_in);
            Estatisticas::print_result_quebras("in", lq, custo_in, cmp_in, mov_in, calls_in);

            // calcula diferença entre os custos dos dois algoritmos
            double diff_atual = fabs(custo_in - custo_qs);
            
            custos_insertion[num_lq] = custo_in;
            vetor_lqs[num_lq] = lq;

            // registra as diferenças entre o quick e o insertion sort
            registrar_est(diff_atual);

            // deletando os vetores alocados dinamicamente 
            delete[] teste_qs;
            delete[] teste_in;

            num_lq++;
        }

        // Escolhendo o melhor indice (melhor lq) e recalculando os valores certos para cada variável
        int melhor_indice = menor_custo();
        int melhor_lq = vetor_lqs[melhor_indice];
        
        int old_min = min_lq;
        int old_max = melhor_lq;
    
        calcula_nova_faixa(melhor_indice, min_lq, max_lq, old_min, old_max, passo_lq, num_lq, vetor_lqs);

        lqdiff = fabs(custos_insertion[old_max] - custos_insertion[old_min]);        
        
        lim_quebras = melhor_lq;

        Estatisticas::print_resumo_quebras(num_custos, lim_quebras, lqdiff);
        it++;
    }

    return lim_quebras;
}


// Calcula o custo com base nos coeficientes e nos parâmetros 
double OrdenadorLQ::calcular_custo(int cmp, int mov, int calls) const
{
    return a*cmp + b*mov + c*calls;
}

// Caso o vetor de custos esteja cheio expande sua capacidade
void OrdenadorLQ::expandir_capacidade_custos()
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
void OrdenadorLQ::registrar_est(double custo)
{
    if (num_custos >= capacidade_custos)
    {
        expandir_capacidade_custos();
    }
    custos[num_custos++] = custo;
}

// Procura pelo indice do menor elemento
int OrdenadorLQ::menor_custo()
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
void OrdenadorLQ::calcula_nova_faixa(int melhor_indice, int& min_lq, int& max_lq, int& new_min, int& new_max, int& passo_lq, int num_lq, int* vetor_lqs)
{
    if(melhor_indice == 0)
    {
        new_min = 0;
        new_max = 2;

    }
    else if (melhor_indice >= num_lq - 1)
    {
        new_min = num_lq - 3;
        new_max = num_lq - 1;
    }
    else
    {
        new_min = melhor_indice - 1;
        new_max = melhor_indice + 1;
    }

    min_lq = vetor_lqs[new_min];
    max_lq = vetor_lqs[new_max];


    passo_lq = (max_lq - min_lq) / 5;
    if (passo_lq == 0) passo_lq++;
}