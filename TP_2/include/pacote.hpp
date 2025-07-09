#ifndef PACOTE_HPP
#define PACOTE_HPP

#include <string>

using std::string;

// Structs auxiliares que armazenam informações sobre o pacote
struct Tempos
{
    double tempo_armazenado;
    double tempo_transito;
    double tempo_total;

    Tempos();
};

enum Estado_pacote
{
    NAO_POSTADO,
    ARMAZENADO,
    REMOVIDO,
    EM_TRANSPORTE,
    ENTREGUE
};

// Lista encadeada da rota
struct NoRota
{   
    string armazem_atual;
    NoRota* next;

    NoRota(const string& nome);
};

// Pacote em si
class Pacote
{
    private:
        unsigned int id;
        string origem, destino;
        NoRota* rota;
        NoRota* proximo_destino;

        Estado_pacote estado;
        Tempos tempos;

        double horario_postagem;
        double tempo_entrada_secao;

    public: 
        Pacote(int id_, const string& orig, const string& dest, double postagem);
        ~Pacote();

        void definir_rota(NoRota* inicio_rota);
        void avancar_rota();

        void atualiza_estado(Estado_pacote novo_estado);
        Estado_pacote get_estado() const;

        void add_tempo_armazenado(double tempo);
        double get_tempo_postagem() const;

        int get_id() const;
        string get_origem() const;
        string get_destino() const;
        string get_next_destino() const;
        string get_local_atual() const;

        void liberar_rota();

        void set_tempo_entrada(double tempo);
        double get_tempo_entrada() const;
};

#endif