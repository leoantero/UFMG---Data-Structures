#ifndef SIMULADOR_HPP
#define SIMULADOR_HPP

#include <string>
#include "armazem.hpp"
#include "pacote.hpp"
#include "escalonador.hpp"
#include "evento.hpp"

using std::string;

// Enum para a formatação da mensagem, a mensagem é modificada de acordo com o estado do pacote
enum Mensagem_Estado_Pacote
{
    MSG_ARMAZENADO,
    MSG_REMOVIDO,
    MSG_TRANSITO,
    MSG_REARMAZENADO,
    MSG_ENTREGUE
};

class Simulador
{
    private:
        Armazem* armazens[100];
        int qntd_armazens;

        Pacote* pacotes[1000];
        int qntd_pacotes;

        Escalonador escalonador;

        double relogio;

        int capacidade, latencia, intervalo, custo;

        Armazem* busca_armazem(const string& nome) const;
        void processar_evento_chegada(Evento* evento);
        void processar_evento_transporte(Evento* evento);
        void imprimir_evento(double tempo, const string& mensagem);
        string formatar_mensagem(Mensagem_Estado_Pacote estado, Pacote* pacote, const string& origem, const string& destino);
        double remover_pacotes_da_secao(Armazem* origem, const string& destino_secao, double tempo_evento, Pacote* pacotes_removidos[], int& total_removidos);
        void despachar_pacotes(Pacote* pacotes_para_envio[], int num_pacotes, double tempo_despacho, const string& origem_evento, const string& destino_evento);
        void rearmazenar_resto_dos_pacotes(Pacote* pacotes_para_rearmazenar[], int num_pacotes, double tempo_processamento, Armazem* armazem_origem, const string& secao_destino);
        void ordenar_pacotes_prioridade(Pacote** pacotes_espera, int quantidade);
    
    public:
        Simulador();
        ~Simulador();

        void set_parametros(int cap, int lat, int interv, int custo, int num_armzns);

        void add_armazem(const string& nome);
        void conectar_armazens(const string& origem, const string& destino);
        void add_pacote(int id, const string& origem, const string& destino, double tempo);

        void inicializar_eventos();
        void executar();

        NoRota* calcula_rota(const string& origem, const std::string& destino);
        bool tem_pacotes() const;
};

#endif