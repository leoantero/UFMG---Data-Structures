#include "../include/armazem.hpp"
#include <iostream>

NoAdj::NoAdj(const string& x)
{
    destino = x;
    next = nullptr;
}

NoPilha::NoPilha(Pacote* p, double tempo)
{
    pacote = p;
    next = nullptr;
    tempo_entrada = tempo;
}

Secao::Secao(const string& dest)
{
    destino = dest;
    topo = nullptr;
    next = nullptr;
}

Armazem::Armazem(const string& nome_)
{
    nome = nome_;
    lista_adjacentes = nullptr;
    lista_secoes = nullptr;
}

Armazem::~Armazem()
{
    liberar_vizinhos();
    liberar_secoes();
}

// Faz a adição de um novo armazém e a seção 
void Armazem::add_vizinho(const string& nome_vizinho)
{
    // Verifica se já existe
    NoAdj* atual = lista_adjacentes;
    while(atual != nullptr)
    {
        if(atual->destino == nome_vizinho) return; //Já está lá
        atual = atual->next;
    }
    
    // Adiciona o novo armazem na lista_secoes e na lista de adjacencia de armazens
    NoAdj* novo = new NoAdj(nome_vizinho);
    novo->next = lista_adjacentes;
    lista_adjacentes = novo;

    Secao* nova_secao = new Secao(nome_vizinho);
    nova_secao->next = lista_secoes;
    lista_secoes = nova_secao;

}

// Verifica se um determinado armazém tem conexão com o outro
bool Armazem::eh_vizinho(const string& destino) const
{
    NoAdj* atual = lista_adjacentes;
    while(atual != nullptr)
    {
        if(atual->destino == destino) return true;
        atual = atual->next;
    }

    return false;
}

// Insere o pacote na pilha 
void Armazem::armazenar_pacote(Pacote* pacote, const string& destino, double tempo)
{
    Secao* s = lista_secoes;
    while (s != nullptr)
    {
        if(s->destino == destino)
        {
            NoPilha* novo_no = new NoPilha(pacote, tempo);
            novo_no->next = s->topo;
            s->topo = novo_no;
            pacote->set_tempo_entrada(tempo);
            pacote->atualiza_estado(ARMAZENADO);
            break;
        }
        s = s->next;
    }
    
}

// Retira o pacote do topo 
Pacote* Armazem::retirar_pacote(const string& destino, double tempo)
{
    Secao* s = lista_secoes;
    while(s != nullptr)
    {
        if(s->destino == destino)
        {
            if(s->topo != nullptr && s->topo->pacote != nullptr)
            { 
                NoPilha* no_removido = s->topo;
                Pacote* pacote_retirado = no_removido->pacote;
                s->topo = no_removido->next;
                
                delete no_removido;
                return pacote_retirado;
            }
            return nullptr;
        }
        s = s->next;
        
    }
    std::cerr << "ERRO: Não foi possível fazer a retirada do pacote" << std::endl;
    return nullptr;
}

// Retorna o estado da seção, se está vazia ou não 
bool Armazem::secao_vazia(const string& destino) const
{
    Secao* s = lista_secoes;
    while(s != nullptr)
    {
        if(s->destino == destino)
        {
            return s->topo == nullptr;
        }
        s = s->next;
    }
    return true;
}

string Armazem::get_nome() const 
{
    return nome;
}

NoAdj* Armazem::get_vizinho() const
{
    return lista_adjacentes;
}

// Função para o destrutor, deleta o que foi criado e evita problemas
void Armazem::liberar_vizinhos() 
{
    while(lista_adjacentes != nullptr)
    {
        NoAdj* temp = lista_adjacentes;
        lista_adjacentes = lista_adjacentes->next;
        delete temp;
    }
}

// Função para o destrutor, deleta o que foi criado e evita problemas
void Armazem::liberar_secoes()
{
    while(lista_secoes != nullptr)
    {
        Secao* temp = lista_secoes;
        lista_secoes = lista_secoes->next;

        while(temp->topo != nullptr)
        {
            NoPilha* pilha_temp = temp->topo;
            temp->topo = temp->topo->next;
            delete pilha_temp;
        }
        delete temp;
    }
}

bool Armazem::esta_vazio() const
{
    Secao* s = lista_secoes;
    while(s != nullptr)
    {
        if(s->topo != nullptr) return false;
        s = s->next;
    }
    return true;
}