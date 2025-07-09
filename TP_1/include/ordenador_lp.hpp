#ifndef ORDENADOR_LP_HPP
#define ORDENADOR_LP_HPP

class Ordenador 
{
    public:
        Ordenador();
        ~Ordenador();
        
        void copy(int* V, int tam, int* copia);
        void ordenador_universal(int* V, int tam, int min_tam_particao, int limiar_quebras, int& cmp, int& mov, int& calls);
        int determina_limiar_particao(int* V, int tam, double limiar_custo);
        int getMPS(int* mps, int mps_idx);
        int numero_quebras(int* V, int tam);
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
        void calcula_nova_faixa(int lim_particao, int& minMPS, int& maxMPS, int& passoMPS, int* vetor_mps);

        double a, b, c;

        double* custos;
        int num_custos;
        int capacidade_custos;

        void expandir_capacidade_custos();
};

#endif