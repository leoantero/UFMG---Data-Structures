#include "../include/clientes.hpp"

// Construtores e Destrutores
ArvoreClientes::ArvoreClientes()
{
    this->raiz = nullptr;
}

ArvoreClientes::~ArvoreClientes()
{
    limpar();
}

// Função auxiliar que retorna o maior entre dois valores
int ArvoreClientes::max(int a, int b)
{
    if(a > b) return a;
    return b;
}

// Limpa toda a árvore recursivamente a partir da raiz
void ArvoreClientes::limpar()
{
    limpar_helper(this->raiz);
    this->raiz = nullptr;
}

// Função recursiva que libera memória de cada nó da árvore e sua lista de pacotes
void ArvoreClientes::limpar_helper(NoClientes* no)
{
    if(no == nullptr)
    {
        return;
    }

    limpar_helper(no->esq);
    limpar_helper(no->dir);

    ListaPacotesClientes* atual = no->pacotes;
    while(atual != nullptr)
    {
        ListaPacotesClientes* next = atual->next;
        delete atual;
        atual = next;
    }
    delete no;
}

// Retorna a altura do nó, usada para calcular balanceamento
int ArvoreClientes::altura_no(NoClientes* no)
{
    if (no == nullptr) return 0;
    return no->altura;
}

// Calcula o fator de balanceamento de um nó (altura da esquerda - altura da direita)
int ArvoreClientes::fator_balanceamento(NoClientes* no)
{
    if (no == nullptr) return 0;
    return altura_no(no->esq) - altura_no(no->dir);
}

// Rotação a direita, sobe o filho esquerdo e desce o nó atual
NoClientes* ArvoreClientes::rotacao_direita(NoClientes* d)
{
    NoClientes* e = d->esq;
    NoClientes* aux = e->dir;

    e->dir = d;
    d->esq = aux;

    d->altura = max(altura_no(d->esq), altura_no(d->dir)) + 1;
    e->altura = max(altura_no(e->esq), altura_no(e->dir)) + 1;

    return e;
}

// Rotação a esquerda, sobe o filho direito e desce o nó atual
NoClientes* ArvoreClientes::rotacao_esquerda(NoClientes* e)
{
    NoClientes* d = e->dir;
    NoClientes* aux = d->esq;

    d->esq = e;
    e->dir = aux;

    e->altura = max(altura_no(e->esq), altura_no(e->dir)) + 1;
    d->altura = max(altura_no(d->esq), altura_no(d->dir)) + 1;
    
    return d;
}

// Insere um pacote de um cliente na árvore
void ArvoreClientes::inserir_pacote(const std::string& nome, int id_pacote, int index_evento)
{
    this->raiz = inserir_helper(this->raiz, nome, id_pacote, index_evento);
}

// Função recursiva que insere novo cliente ou atualiza lista de pacotes existente
NoClientes* ArvoreClientes::inserir_helper(NoClientes* no, const std::string& nome, int id_pacote, int index_evento)
{
    // Caso ainda não exista cria um novo nó de cliente com pacote
    if (no == nullptr)
    {
        ListaPacotesClientes* novo_pacote = new ListaPacotesClientes();
        novo_pacote->id_pacote = id_pacote;
        novo_pacote->index_primeiro_evento = index_evento;
        novo_pacote->index_ultimo_evento = index_evento;
        novo_pacote->next = nullptr;

        NoClientes* novo_no = new NoClientes();
        novo_no->nome_cliente = nome;
        novo_no->esq = nullptr;
        novo_no->dir = nullptr;
        novo_no->altura = 1;
        novo_no->pacotes = novo_pacote;
        return novo_no;
    }

    // Faz o caminho pela árvore de acordo com a ordem alfabética 
    if (nome < no->nome_cliente)
    {
        no->esq = inserir_helper(no->esq, nome, id_pacote, index_evento);
    }
    else if (nome > no->nome_cliente)
    {
        no->dir = inserir_helper(no->dir, nome, id_pacote, index_evento);
    }
    else
    {
        // Se o nome já existe atualiza ou adiciona pacote na lista
        ListaPacotesClientes* atual = no->pacotes;
        ListaPacotesClientes* ultimo = nullptr;
        bool find = false;

        while (atual != nullptr)
        {
            if (atual->id_pacote == id_pacote)
            {
                atual->index_ultimo_evento = index_evento;
                find = true;
                break;
            }
            ultimo = atual;
            atual = atual->next;
        }

        if (!find)
        {
            ListaPacotesClientes* novo_pacote = new ListaPacotesClientes();
            novo_pacote->id_pacote = id_pacote;
            novo_pacote->index_primeiro_evento = index_evento;
            novo_pacote->index_ultimo_evento = index_evento;
            novo_pacote->next = nullptr;
            if (ultimo != nullptr)
            {
                ultimo->next = novo_pacote;
            }
        }
        return no;
    }

    // Atualiza altura e faz as rotações necessárias para manter árvore balanceada
    no->altura = 1 + max(altura_no(no->esq), altura_no(no->dir));

    int fb = fator_balanceamento(no);

    if(fb > 1 && nome < no->esq->nome_cliente) return rotacao_direita(no);
    if(fb < -1 && nome > no->dir->nome_cliente) return rotacao_esquerda(no);
    if(fb > 1 && nome > no->esq->nome_cliente)
    {
        no->esq = rotacao_esquerda(no->esq);
        return rotacao_direita(no);
    }
    if(fb < -1 && nome < no->dir->nome_cliente)
    {
        no->dir = rotacao_direita(no->dir);
        return rotacao_esquerda(no);
    }
    return no;
}

// Busca lista de pacotes de um cliente pelo nome
ListaPacotesClientes* ArvoreClientes::buscar_cliente(const std::string& nome)
{
    NoClientes* result = buscar_helper(this->raiz, nome);
    if (result != nullptr)
    {
        return result->pacotes;
    }
    return nullptr;
}

// Função recursiva que rocurando o cliente na árvore
NoClientes* ArvoreClientes::buscar_helper(NoClientes* no, const std::string& nome)
{
    if (no == nullptr || no->nome_cliente == nome) return no;

    if (nome < no->nome_cliente) return buscar_helper(no->esq, nome);

    return buscar_helper(no->dir, nome);
}
