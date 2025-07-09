#include "../include/execucao.hpp"
#include "../include/evento.hpp"
#include "../include/pacotes.hpp"
#include "../include/clientes.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

// Ordena os índices pelo tempo e em caso de empate 
// considera o menor ID do pacote (insertion sort com essa adequação)
static void ordena_pacotes(int* indices, int n, Evento* base_de_eventos) 
{
    for (int i = 1; i < n; i++) 
    {
        int chave_idx = indices[i];
        int tempo_chave = base_de_eventos[chave_idx].tempo;
        int id_chave = base_de_eventos[chave_idx].id_pacote;
        int j = i - 1;

        while (j >= 0) 
        {
            int tempo_atual = base_de_eventos[indices[j]].tempo;
            int id_atual = base_de_eventos[indices[j]].id_pacote;

            if (tempo_atual > tempo_chave || (tempo_atual == tempo_chave && id_atual > id_chave)) 
            {
                indices[j + 1] = indices[j];
                j--;
            } 
            else break;
        }
        indices[j + 1] = chave_idx;
    }
}

// Converte string do tipo do evento para o enum correspondente
static EstadoPacote string_para_enum(const std::string& s) 
{
    if (s == "RG") return RG;
    if (s == "AR") return AR;
    if (s == "RM") return RM;
    if (s == "UR") return UR;
    if (s == "TR") return TR;
    if (s == "EN") return EN;
    return RG;
}

// Converte enum do tipo do evento para sua representação string
static std::string enum_para_string(EstadoPacote e) 
{
    switch (e) 
    {
        case RG: return "RG";
        case AR: return "AR";
        case RM: return "RM";
        case UR: return "UR";
        case TR: return "TR";
        case EN: return "EN";
        default: return "";
    }
}

// Formata a linha de saída de acordo com o tipo do evento
static std::string formatar_linha_evento(const Evento& e) 
{
    std::stringstream ss;
    ss << std::setw(7) << std::setfill('0') << e.tempo << " ";
    ss << "EV " << enum_para_string(e.tipo) << " ";
    ss << std::setw(3) << std::setfill('0') << e.id_pacote;
    switch (e.tipo) 
    {
        case RG:
            ss << " " << e.remetente << " " << e.destinatario << " " 
               << std::setw(3) << std::setfill('0') << e.armazem_origem << " " 
               << std::setw(3) << std::setfill('0') << e.armazem_destino;
            break;
        case AR: case RM: case UR:
            ss << " " << std::setw(3) << std::setfill('0') << e.armazem_destino << " " 
               << std::setw(3) << std::setfill('0') << e.secao_destino;
            break;
        case TR:
            ss << " " << std::setw(3) << std::setfill('0') << e.armazem_origem << " " 
               << std::setw(3) << std::setfill('0') << e.armazem_destino;
            break;
        case EN:
            ss << " " << std::setw(3) << std::setfill('0') << e.armazem_destino;
            break;
    }
    return ss.str();
}

// Função principal que lê e processa o arquivo de entrada linha por linha
// monta os eventos, popula árvores e responde consultas do tipo PC e CL
void processar_entrada(std::ifstream& arquivo) 
{
    int capacidade_eventos = 100;
    int num_eventos = 0;
    Evento* todos_eventos = new Evento[capacidade_eventos];

    ArvorePacotes arvore_pacotes;
    ArvoreClientes arvore_clientes;

    std::string linha;
    while (std::getline(arquivo, linha)) 
    {
        if (linha.empty())
        {
            continue;
        }
        
        std::stringstream ss(linha);
        int tempo_evento;
        std::string tipo_linha;

        if (!(ss >> tempo_evento >> tipo_linha)) {
            std::cerr << "Erro: linha da entrada com formato inválido \"" << linha << "\"" << std::endl;
            continue;
        }

        // Evento, nesse caso as informações são armazenadas no sistema
        if (tipo_linha == "EV") 
        {
            // Realoca vetor de eventos se necessário
            if (num_eventos == capacidade_eventos) 
            {
                int nova_capacidade = capacidade_eventos * 2;
                Evento* novo_array = new Evento[nova_capacidade];
                for (int i = 0; i < num_eventos; ++i) 
                {
                    novo_array[i] = todos_eventos[i];
                }
                delete[] todos_eventos;
                todos_eventos = novo_array;
                capacidade_eventos = nova_capacidade;
            }

            // Monta evento a partir da linha
            Evento ev;
            ev.tempo = tempo_evento;
            std::string tipo_ev_str;
            
            if (!(ss >> tipo_ev_str >> ev.id_pacote)) 
            {
                 std::cerr << "Erro: linha da entrada com formato inválido \"" << linha << "\"" << std::endl;
                 continue;
            }
            
            ev.tipo = string_para_enum(tipo_ev_str);

            // Lê os campos específicos de acordo com o tipo do evento
            if (ev.tipo == RG) 
            {
                ss >> ev.remetente >> ev.destinatario >> ev.armazem_origem >> ev.armazem_destino;
            } 
            else if (ev.tipo == AR || ev.tipo == RM || ev.tipo == UR) 
            {
                ss >> ev.armazem_destino >> ev.secao_destino;
            } 
            else if (ev.tipo == TR) 
            {
                ss >> ev.armazem_origem >> ev.armazem_destino;
            } 
            else if (ev.tipo == EN) 
            {
                ss >> ev.armazem_destino;
            }

            // Armazena o evento
            int index_novo_evento = num_eventos;
            todos_eventos[index_novo_evento] = ev;
            num_eventos++;

            // Atualiza árvore de pacotes
            arvore_pacotes.inserir_evento(ev.id_pacote, index_novo_evento);

            // Atualiza árvore de clientes
            if (ev.tipo == RG) 
            {
                arvore_clientes.inserir_pacote(ev.remetente, ev.id_pacote, index_novo_evento);
                arvore_clientes.inserir_pacote(ev.destinatario, ev.id_pacote, index_novo_evento);
            } 
            else 
            {
                ListaPacote* lista = arvore_pacotes.buscar_pacote(ev.id_pacote);
                if (lista) 
                {
                    const Evento& evento_rg = todos_eventos[lista->index_evento];
                    arvore_clientes.inserir_pacote(evento_rg.remetente, ev.id_pacote, index_novo_evento);
                    arvore_clientes.inserir_pacote(evento_rg.destinatario, ev.id_pacote, index_novo_evento);
                }
            }
        } 
        // Consulta de pacote
        else if (tipo_linha == "PC") 
        {
            int id_pacote_consulta;
            
            if (!(ss >> id_pacote_consulta)) 
            {
                std::cerr << "Erro: linha da entrada com formato inválido \"" << linha << "\"" << std::endl;
                continue;
            }
            
            std::cout << std::setw(6) << std::setfill('0') << tempo_evento << " PC " 
                      << std::setw(3) << std::setfill('0') << id_pacote_consulta << std::endl;

            // Pega os eventos associados ao pacote
            ListaPacote* eventos_pacote = arvore_pacotes.buscar_pacote(id_pacote_consulta);
            
            // Conta quantos eventos ocorreram até aquele tempo
            int count = 0;
            ListaPacote* temp = eventos_pacote;
            
            while(temp) 
            {
                if(todos_eventos[temp->index_evento].tempo <= tempo_evento) count++;
                temp = temp->next;
            }
            std::cout << count << std::endl;

            // Exibe os eventos válidos
            temp = eventos_pacote;
            while(temp) 
            {
                const Evento& ev_atual = todos_eventos[temp->index_evento];
                if(ev_atual.tempo <= tempo_evento) std::cout << formatar_linha_evento(ev_atual) << std::endl;
                temp = temp->next;
            }
        } 
        // Consulta de cliente
        else if (tipo_linha == "CL") 
        {
            std::string nome_cliente;
            
            if (!(ss >> nome_cliente)) 
            {
                std::cerr << "Erro: linha da entrada com formato inválido \"" << linha << "\"" << std::endl;
                continue;
            }
            
            std::cout << std::setw(6) << std::setfill('0') << tempo_evento << " CL " << nome_cliente << std::endl;

            // Aloca vetor de índices dos eventos relacionados ao cliente
            int capacidade_idx = 50;
            int contador_idx = 0;
            int* indices_relevantes = new int[capacidade_idx];

            ListaPacotesClientes* pacotes_do_cliente = arvore_clientes.buscar_cliente(nome_cliente);
            ListaPacotesClientes* temp_cliente = pacotes_do_cliente;

            while(temp_cliente != nullptr) 
            {
                // Aumenta vetor se necessário
                if (contador_idx == capacidade_idx) 
                {
                    int nova_capacidade = capacidade_idx * 2;
                    int* novo_array_indices = new int[nova_capacidade];
                    for (int i = 0; i < contador_idx; ++i) 
                        novo_array_indices[i] = indices_relevantes[i];

                    delete[] indices_relevantes;
                    indices_relevantes = novo_array_indices;
                    capacidade_idx = nova_capacidade;
                }

                // Adiciona o primeiro evento do pacote
                indices_relevantes[contador_idx++] = temp_cliente->index_primeiro_evento;

                // Busca o último evento válido até aquele tempo
                int ultimo_evento_valido_idx = -1;
                ListaPacote* eventos_do_pacote = arvore_pacotes.buscar_pacote(temp_cliente->id_pacote);
                ListaPacote* temp_pacote = eventos_do_pacote;
                while(temp_pacote != nullptr) 
                {
                    if (todos_eventos[temp_pacote->index_evento].tempo <= tempo_evento) 
                        ultimo_evento_valido_idx = temp_pacote->index_evento;

                    temp_pacote = temp_pacote->next;
                }

                // Se for diferente do primeiro, adiciona também o último
                if (ultimo_evento_valido_idx != -1 && ultimo_evento_valido_idx != temp_cliente->index_primeiro_evento) 
                {
                    if (contador_idx == capacidade_idx) 
                    {
                        int nova_capacidade = capacidade_idx * 2;
                        int* novo_array_indices = new int[nova_capacidade];
                        for (int i = 0; i < contador_idx; ++i) 
                            novo_array_indices[i] = indices_relevantes[i];

                        delete[] indices_relevantes;
                        indices_relevantes = novo_array_indices;
                        capacidade_idx = nova_capacidade;
                    }
                    indices_relevantes[contador_idx++] = ultimo_evento_valido_idx;
                }

                temp_cliente = temp_cliente->next;
            }

            // Ordena os eventos por tempo e ID de pacote
            ordena_pacotes(indices_relevantes, contador_idx, todos_eventos);

            // Exibe os eventos ordenados
            std::cout << contador_idx << std::endl;
            for (int i = 0; i < contador_idx; i++) 
            {
                int evento_idx = indices_relevantes[i];
                std::cout << formatar_linha_evento(todos_eventos[evento_idx]) << std::endl;
            }

            delete[] indices_relevantes;
        }      
    }

    delete[] todos_eventos;
}