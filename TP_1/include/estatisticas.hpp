#ifndef ESTATISTICAS_HPP
#define ESTATISTICAS_HPP

// Classe para impressões das estatísticas
class Estatisticas 
{
    public: 
        static void print_iteracao(int it);
        static void print_result(int mps, double cost, int cmp, int move, int calls);
        static void print_resumo(int nummps, int lim_particao, float mpsdiff);
        static void print_result_quebras(const char* alg, int lq, double custo, int cmp, int move, int calls);
        static void print_resumo_quebras(int numlq, int lim_quebras, double diff);
};

#endif