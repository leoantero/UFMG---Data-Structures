#ifndef EVENTO_HPP
#define EVENTO_HPP

#include <string>

// Arquivo com estruturas de dados auxiliares para o programa 
enum EstadoPacote
{
    RG,
    AR,
    RM,
    UR,
    TR,
    EN
};

// Cada evento tem um pacote, um estado e diversas outras informações que 
// serão usadas na impressão depois
struct Evento
{
    int tempo;
    EstadoPacote tipo;
    int id_pacote;

    std::string remetente;
    std::string destinatario;
    int armazem_origem;
    int armazem_destino;
    int secao_destino;
};

#endif