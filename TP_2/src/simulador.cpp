#include "../include/simulador.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

// Construtor, inicializa as variáveis
Simulador::Simulador() 
{
    qntd_armazens = 0;
    qntd_pacotes = 0;
    relogio = 0;
    capacidade = 0;
    latencia = 0;
    intervalo = 0;
    custo = 0;
}

// Destrutor, libera memória alocada 
Simulador::~Simulador() 
{
    for(int i = 0; i < qntd_armazens; i++) delete armazens[i];
    for(int i = 0; i < qntd_pacotes; i++) delete pacotes[i];
}

// Ajustando os parametros
void Simulador::set_parametros(int cap, int lat, int interv, int c, int num_armzns) 
{
    this->capacidade = cap;
    this->latencia = lat;
    this->intervalo = interv;
    this->custo = c;
}

// Adicionando armazem no sistema
void Simulador::add_armazem(const std::string& nome) 
{
    if(qntd_armazens < 100) armazens[qntd_armazens++] = new Armazem(nome);
    if (qntd_armazens >= 100)
    {
        std::cerr << "ERRO: Limite máximo de armazens atingido" << std::endl;
    }
}

// Criando conexao entre armazens
void Simulador::conectar_armazens(const std::string& origem, const std::string& destino) 
{
    Armazem* a = busca_armazem(origem);
    Armazem* b = busca_armazem(destino);
    if(a != nullptr && b != nullptr) 
    {
        // Como é não direcionado conectamos os dois a eles mesmos
        a->add_vizinho(destino);
        b->add_vizinho(origem);
    }
}

// Adiciona pacote com seus dados (considernado uma aplicação no qual o id do pacote é usado basta substituir qntd_pacotes em new Pacote() pelo id)
void Simulador::add_pacote(int id, const std::string& origem, const std::string& destino, double tempo) 
{
    if(qntd_pacotes < 100) 
    { 
        pacotes[qntd_pacotes] = new Pacote(qntd_pacotes, origem, destino, tempo);
        qntd_pacotes++;
    }
    if (qntd_pacotes >= 1000)
    {
        std::cerr << "Limite máximo de pacotes atingido" << std::endl;
    }
}

// Função para preparar o loop principal do código, aqui os pacotes tem suas rotas definidas, 
// eventos inseridos no escalonador, eventos sincronizados com os armazens, tempos iniciais, etc
void Simulador::inicializar_eventos() 
{
    // Agenda a chegada inicial de cada pacote lido no seu armazém de origem
    for(int i = 0; i < qntd_pacotes; i++) 
    {
        NoRota* rota = calcula_rota(pacotes[i]->get_origem(), pacotes[i]->get_destino());
        if (rota != nullptr) 
        {
            pacotes[i]->definir_rota(rota);
            Evento* e = new Evento(pacotes[i]->get_tempo_postagem(), CHEGADA, "", pacotes[i]->get_origem(), pacotes[i]);
            escalonador.inserir(e);
        }
    }
    
    // Agenda os primeiros eventos de transporte
    double primeiro_tempo_transporte = 0;
    if(qntd_pacotes > 0)
    {
        primeiro_tempo_transporte = pacotes[0]->get_tempo_postagem();
        for (int i = 1; i < qntd_pacotes; ++i) 
        {
            if (pacotes[i]->get_tempo_postagem() < primeiro_tempo_transporte) 
            {
                primeiro_tempo_transporte = pacotes[i]->get_tempo_postagem();
            }
        }
    }
    primeiro_tempo_transporte += intervalo;

    for(int i = 0; i < qntd_armazens; i++) 
    {
        Armazem* a = armazens[i];
        NoAdj* viz = a->get_vizinho();
        while(viz != nullptr) 
        {
            Evento* e = new Evento(primeiro_tempo_transporte, TRANSPORTE, a->get_nome(), viz->destino, nullptr);
            escalonador.inserir(e);
            viz = viz->next;
        }
    }
}

// De acordo com o que foi pedido executa a movimentação principal dos pacotes
void Simulador::executar() 
{
    // Loop principal, continua enquanto existir eventos ou pacotes
    while (tem_pacotes() || !escalonador.is_vazio()) 
    {
        Evento* evento = escalonador.retirar_proximo();
        if (evento == nullptr) break; 

        relogio = evento->tempo;

        switch (evento->tipo) 
        {
            case CHEGADA:
                processar_evento_chegada(evento);
                break;
            case TRANSPORTE:
                processar_evento_transporte(evento);
                break;
        }
        delete evento;
    }
}

// Determina o que será feito para a chegada do pacote
void Simulador::processar_evento_chegada(Evento* evento) 
{
    if(evento->dados == nullptr)
    {
        std::cerr << "ERRO: Evento de chegada com dados nulos" << std::endl;
        return;
    }
    
    Pacote* p = static_cast<Pacote*>(evento->dados);
    std::string local_chegada = evento->destino;
    
    // Se o pacote já está no destino é marcado como entregue
    if (p->get_local_atual() == p->get_destino()) 
    {
        p->atualiza_estado(ENTREGUE);
        std::string msg = formatar_mensagem(MSG_ENTREGUE, p, local_chegada, local_chegada);
        imprimir_evento(relogio, msg);
    } 
    else 
    {
        // Caso não, ele armazena na seção do próximo ponto da rota
        Armazem* a = busca_armazem(local_chegada);
        
        if(a == nullptr)
        {
            std::cerr << "ERRO: Armazem para o armazenamento não existe" << std::endl;
            return;
        }
        
        std::string proximo_destino = p->get_next_destino();

        if (a && !proximo_destino.empty() && a->eh_vizinho(proximo_destino)) 
        {
            a->armazenar_pacote(p, proximo_destino, relogio);
            std::string msg = formatar_mensagem(MSG_ARMAZENADO, p, local_chegada, proximo_destino);
            imprimir_evento(relogio, msg);
        }
    }
}

// Faz as operações necessárias no momento de transporte (empilha, desempilha, recebe e envia pacotes, etc)
void Simulador::processar_evento_transporte(Evento* evento) 
{
    Armazem* origem = busca_armazem(evento->origem);
    const std::string& destino_secao = evento->destino;

    if (!origem || origem->secao_vazia(destino_secao)) 
    {
        if(tem_pacotes()) 
        {
            escalonador.inserir(new Evento(evento->tempo + intervalo, TRANSPORTE, evento->origem, evento->destino, nullptr));
        }
        return;
    }

    Pacote* pilha_removidos[1000];
    int total_removidos = 0;

    double tempo_final_remocao = remover_pacotes_da_secao(origem, destino_secao, evento->tempo, pilha_removidos, total_removidos);

    // Ordena os pacotes removidos para dar prioridade ao mais antigo com selection sort
    ordenar_pacotes_prioridade(pilha_removidos, total_removidos);

    // Processa os pacotes: os primeiros viajam, o resto é rearmazenado.
    int pacs_para_despachar = (total_removidos < capacidade) ? total_removidos : capacidade;
    if (pacs_para_despachar > 0)
    {
        despachar_pacotes(pilha_removidos, pacs_para_despachar, tempo_final_remocao, evento->origem, evento->destino);
    }
    
    if(total_removidos > pacs_para_despachar)
    {
        rearmazenar_resto_dos_pacotes(&pilha_removidos[pacs_para_despachar], total_removidos - pacs_para_despachar, tempo_final_remocao, origem, destino_secao);
    }

    // Agenda o próximo evento de transporte para a rota
    if(tem_pacotes()) 
    {
        escalonador.inserir(new Evento(evento->tempo + intervalo, TRANSPORTE, evento->origem, evento->destino, nullptr));
    }
}

// Verifica nos armazens se existem pacotes e se todos os pacotes foram entregues
// Se ainda existem pacotes a serem movidos retorna true
bool Simulador::tem_pacotes() const 
{
    for(int i = 0; i < qntd_pacotes; ++i) 
    {
        if(pacotes[i] != nullptr && pacotes[i]->get_estado() != ENTREGUE) 
        {
            return true;
        }
    }
    for(int i = 0; i < qntd_armazens; ++i) 
    {
        if(armazens[i] != nullptr && !armazens[i]->esta_vazio()) 
        {
            return true;
        }
    }
    return false;
}

// Procura no array de armazens o armazem desejado
Armazem* Simulador::busca_armazem(const std::string& nome) const 
{
    for(int i = 0; i < qntd_armazens; i++) 
    {
        if(armazens[i]->get_nome() == nome) return armazens[i];
    }
    std::cerr << "ERRO: Armazem não encontrado na busca" << std::endl;
    return nullptr;
}

// Função para imprimir a saída no padrão esperado
void Simulador::imprimir_evento(double tempo, const std::string& mensagem) 
{
    std::cout << std::setw(7) << std::setfill('0') << static_cast<long>(tempo) 
              << " " << mensagem << std::endl;
}

// Calcula rota com algoritmo de busca em largura
NoRota* Simulador::calcula_rota(const std::string& origem, const std::string& destino) 
{
    if(qntd_armazens <= 0) 
    {
        std::cerr << "ERRO: Número de armazens insuficiente para o cálculo da rota" << std::endl;
        return nullptr;
    }
    
    // Arrays auxiliares
    std::string nomes[100];
    bool visitado[100] = { false };
    int anterior[100];
    int origem_idx = -1, destino_idx = -1;

    for(int i = 0; i < qntd_armazens; i++) 
    {
        if(!armazens[i]) continue;
        nomes[i] = armazens[i]->get_nome();
        if(nomes[i] == origem) origem_idx = i;
        if(nomes[i] == destino) destino_idx = i;
        anterior[i] = -1;
    }

    if (origem_idx == -1 || destino_idx == -1) 
    {
        std::cerr << "ERRO: Não foi possível calcular a rota do armazem " << origem << " para o armazem " << destino << std::endl;
        return nullptr;
    }

    int fila[100], inicio = 0, fim = 0;
    fila[fim++] = origem_idx;
    visitado[origem_idx] = true;

    // Busca em Largura para encontrar o menor caminho
    while(inicio < fim) 
    {
        int atual = fila[inicio++];
        if (atual == destino_idx) break;
        Armazem* arm = armazens[atual];
        NoAdj* vizinho = arm->get_vizinho();
        while(vizinho != nullptr) 
        {
            for(int j = 0; j < qntd_armazens; j++) 
            {
                if (nomes[j] == vizinho->destino && !visitado[j]) 
                {
                    visitado[j] = true;
                    anterior[j] = atual;
                    fila[fim++] = j;
                }
            }
            vizinho = vizinho->next;
        }
    }

    if (!visitado[destino_idx]) 
    {
        std::cerr << "ERRO: Não foi possível calcular uma rota entre os armazens" << std::endl;
        return nullptr;
    }

    // Reconstroi a rota e retorna como uma lista encadeada
    int caminho[100], tam = 0;
    int atual = destino_idx;
    while(atual != -1) 
    {
        caminho[tam++] = atual;
        atual = anterior[atual];
    }
    NoRota* head = nullptr;
    NoRota* tail = nullptr;
    for (int i = tam - 1; i >= 0; i--) 
    {
        NoRota* novo = new NoRota(nomes[caminho[i]]);
        if(!head) 
        {
            head = tail = novo;
        } 
        else 
        {
            tail->next = novo;
            tail = novo;
        }
    }
    return head;
}

// Função para melhorar legibilidade do código, encaminha uma determinada saída com 
// string stream de acordo com o que será impresso
string Simulador::formatar_mensagem(Mensagem_Estado_Pacote tipo, Pacote* p, const string& origem, const string& destino)
{
    std::ostringstream oss;
    oss << "pacote " << std::setw(3) << std::setfill('0') << p->get_id();

    switch (tipo)
    {
        case MSG_ARMAZENADO:
            oss << " armazenado em " << std::setw(3) << std::setfill('0') << origem
                << " na secao " << std::setw(3) << std::setfill('0') << destino;
            break;
        case MSG_REMOVIDO:
            oss << " removido de " << std::setw(3) << std::setfill('0') << origem
                << " na secao " << std::setw(3) << std::setfill('0') << destino;
            break;
        case MSG_TRANSITO:
            oss << " em transito de " << std::setw(3) << std::setfill('0') << origem
                << " para " << std::setw(3) << std::setfill('0') << destino;
            break;
        case MSG_REARMAZENADO:
            oss << " rearmazenado em " << std::setw(3) << std::setfill('0') << origem
                << " na secao " << std::setw(3) << std::setfill('0') << destino;
            break;
        case MSG_ENTREGUE:
            oss << " entregue em " << std::setw(3) << std::setfill('0') << origem;
            break;
    }
    return oss.str();
}

// Ordenação com insertion sort pois a amostragem é pequena
void Simulador::ordenar_pacotes_prioridade(Pacote** pacotes_espera, int quantidade)
{
       for(int i = 1; i < quantidade; i++)
       {
           Pacote* chave = pacotes_espera[i];
           int j = i - 1;
           
           while (j >= 0 && (pacotes_espera[j]->get_tempo_entrada() > chave->get_tempo_entrada() || 
                   (pacotes_espera[j]->get_tempo_entrada() == chave->get_tempo_entrada() && pacotes_espera[j]->get_id() > chave->get_id())))
           {
               pacotes_espera[j + 1] = pacotes_espera[j];
               j = j - 1;
           }
           pacotes_espera[j + 1] = chave;
       }
}

// Funções que auxiliam na modularização da função processa evento transporte

// Executa repetidas operações  de retirada pacote (pop da pilha) até que a pilha esteja vazia
double Simulador::remover_pacotes_da_secao(Armazem* origem, const string& destino_secao, double tempo_evento, Pacote* pacotes_removidos[], int& total_removidos)
{
    total_removidos = 0;

    while (!origem->secao_vazia(destino_secao)) 
    {
        pacotes_removidos[total_removidos++] = origem->retirar_pacote(destino_secao, tempo_evento);
    }
    
    double tempo_processamento = tempo_evento;
    for (int i = 0; i < total_removidos; ++i) 
    {
        tempo_processamento += custo;
        Pacote* p = pacotes_removidos[i];
        p->add_tempo_armazenado(tempo_processamento - p->get_tempo_entrada());
        imprimir_evento(tempo_processamento, formatar_mensagem(MSG_REMOVIDO, p, origem->get_nome(), destino_secao));
    }

    return tempo_processamento;
}

// Os pacotes que essa função recebe já são os pacotes que estavam a mais tempo
// na pilha e foram retirados para o transporte
void Simulador::despachar_pacotes(Pacote* pacotes_para_envio[], int num_pacotes, double tempo_despacho, const string& origem_evento, const string& destino_evento)
{
    for(int i = 0; i < num_pacotes; ++i)
    {
        Pacote* p = pacotes_para_envio[i];
        p->atualiza_estado(EM_TRANSPORTE);
        imprimir_evento(tempo_despacho, formatar_mensagem(MSG_TRANSITO, p, origem_evento, destino_evento));
        p->avancar_rota();
        escalonador.inserir(new Evento(tempo_despacho + this->latencia, CHEGADA, origem_evento, destino_evento, p));
    }
}

// Faz o inverso da função remover pacotes, ela executa múltiplos pushs na pilha 
// para os pacote que restaram e não foram transportados
void  Simulador::rearmazenar_resto_dos_pacotes(Pacote* pacotes_para_rearmazenar[], int num_pacotes, double tempo_rearmazenamento, Armazem* armazem_origem, const string& secao_destino)
{
    for(int i = 0; i < num_pacotes; ++i)
    {
        Pacote* p = pacotes_para_rearmazenar[i];
        armazem_origem->armazenar_pacote(p, secao_destino, tempo_rearmazenamento);
        imprimir_evento(tempo_rearmazenamento, formatar_mensagem(MSG_REARMAZENADO, p, armazem_origem->get_nome(), secao_destino));
    }
}