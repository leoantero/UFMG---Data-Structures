#ifndef ORDENADOR_LQ_HPP
#define ORDENADOR_LQ_HPP

class OrdenadorLQ
{
    public:
        OrdenadorLQ();
        ~OrdenadorLQ();
        
        void shuffleVector(int* vet, int tam, int num_shuffle);
        void copy(int* V, int tam, int* copy);
        int determina_limiar_quebras(int* V, int tam, double limiar_custo, int min_tam_particao, int seed, int numero_quebras);
        void set_coeficientes(double a, double b, double c)
        {
            this->a = a;
            this->b = b;
            this->c = c;
        }

    private:
        double calcular_custo(int cmp, int mov, int calls) const;
        void registrar_est(double custo);
        void print_est(double custo);
        int menor_custo();
        void calcula_nova_faixa(int melhor_indice, int& min_lq, int& max_lq, int& old_min, int& old_max, int& passo_lq, int num_lq, int* vetor_lqs);

        double a, b, c;

        double* custos;
        int num_custos;
        int capacidade_custos;

        void expandir_capacidade_custos();
};

#endif