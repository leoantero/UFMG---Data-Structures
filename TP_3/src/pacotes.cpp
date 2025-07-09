#include "../include/pacotes.hpp"

// Construtores e Destrutores 
ArvorePacotes::ArvorePacotes()
{
    this->raiz = nullptr;
}

ArvorePacotes::~ArvorePacotes()
{
    limpar();
}

// Função de comparação que retorna o maior, usada no balanceamento
int ArvorePacotes::max(int a, int b)
{
    if (a > b) return a;
    return b;
}

// Funções que fazem a limpeza de toda árvore deletando os nós
void ArvorePacotes::limpar()
{
    limpar_helper(this->raiz);
    this->raiz = nullptr;
}

void ArvorePacotes::limpar_helper(NoPacote* no)
{
    if(no == nullptr)
    {
        return;
    }

    limpar_helper(no->esq);
    limpar_helper(no->dir);

    ListaPacote* atual = no->lista_eventos;
    while(atual != nullptr)
    {
        ListaPacote* next = atual->next;
        delete atual;
        atual = next;
    }
    delete no;
}

// Calcula a altura do nó desejado recursivamente (altura da esquerda - altura da direita)
int ArvorePacotes::altura_no(NoPacote* no)
{
    if(no==nullptr) return 0;
    return no->altura;
}

// Retorna o fator de balanceamento do nó desejado, informação que será usada para 
// verificar se a árvore está balanceada ou não
int ArvorePacotes::fator_balanceamento(NoPacote* no)
{
    if(no == nullptr) return 0;
    return altura_no(no->esq) - altura_no(no->dir);
}

// Faz a rotação a direita do nó movimentando os ponteiros
// Move o filho esquerdo para cima e o nó atual para a direita dele
NoPacote* ArvorePacotes::rotacao_direita(NoPacote* d)
{
    NoPacote* e = d->esq;
    NoPacote* aux = e->dir;

    e->dir = d;
    d->esq = aux;

    // Atualiza alturas
    d->altura = max(altura_no(d->esq), altura_no(d->dir)) + 1;
    e->altura = max(altura_no(e->esq), altura_no(e->dir)) + 1;

    return e;
}

// Faz a rotação a esquerda do nó movimentando os ponteiros
// Move o filho direito para cima e o nó atual para a esquerda dele
NoPacote* ArvorePacotes::rotacao_esquerda(NoPacote* e)
{
    NoPacote* d = e->dir;
    NoPacote* aux = d->esq;

    d->esq = e;
    e->dir = aux;

    e->altura = max(altura_no(e->esq), altura_no(e->dir)) + 1;
    d->altura = max(altura_no(d->esq), altura_no(d->dir)) + 1;
    
    return d;
}

// Insere um evento na árvore usando o id do pacote como chave
void ArvorePacotes::inserir_evento(int id_pacote, int index_evento)
{
    this->raiz = inserir_helper(this->raiz, id_pacote, index_evento);
}

// Função que faz a inserção de um novo nó ou adiciona um evento a um pacote existente com recursão
NoPacote* ArvorePacotes::inserir_helper(NoPacote* no, int id_pacote, int index_evento)
{
    // Caso o nó ainda não exista, cria um novo nó e adiciona o primeiro evento relacionado a ele
    if (no == nullptr)
    {
        ListaPacote* novo_evento = new ListaPacote();
        novo_evento->index_evento = index_evento;
        novo_evento->next = nullptr;

        NoPacote* novo_no = new NoPacote();
        novo_no->id_pacote = id_pacote;
        novo_no->esq = nullptr;
        novo_no->dir = nullptr;
        novo_no->altura = 1;
        novo_no->lista_eventos = novo_evento;
        return novo_no;
    }

    // De acordo com o id do pacote é armazenado no local adequado da árvore
    if (id_pacote < no->id_pacote)
    {
        no->esq = inserir_helper(no->esq, id_pacote, index_evento);
    }
    else if (id_pacote > no->id_pacote)
    {
        no->dir = inserir_helper(no->dir, id_pacote, index_evento);
    }
    else
    {
        // Caso o id já exista adiciona evento ao final da lista de eventos do pacote naquele nó
        ListaPacote* novo_evento = new ListaPacote();
        novo_evento->index_evento = index_evento;
        novo_evento->next = nullptr;

        ListaPacote* atual = no->lista_eventos;
        while (atual->next != nullptr)
        {
            atual = atual->next;
        }
        atual->next = novo_evento;
        return no;
    }    

    // Atualiza altura e verifica se necessita de fazer balanceamento
    no->altura = 1 + max(altura_no(no->esq), altura_no(no->dir));
    int fb = fator_balanceamento(no);

    if(fb > 1 && id_pacote < no->esq->id_pacote) return rotacao_direita(no);
    if(fb < -1 && id_pacote > no->dir->id_pacote) return rotacao_esquerda(no);
    if(fb > 1 && id_pacote > no->esq->id_pacote)
    {
        no->esq = rotacao_esquerda(no->esq);
        return rotacao_direita(no);
    }
    if(fb < -1 && id_pacote < no->dir->id_pacote)
    {
        no->dir = rotacao_direita(no->dir);
        return rotacao_esquerda(no);
    }
    return no;
}

// Busca a lista com os eventos de um pacote específico 
ListaPacote* ArvorePacotes::buscar_pacote(int id_pacote)
{
    NoPacote* result = buscar_helper(this->raiz, id_pacote);
    if (result != nullptr)
    {
        return result->lista_eventos;
    }
    return nullptr;
}

// Busca um determinado nó (pacote) navegando pela árvore recursivamente
NoPacote* ArvorePacotes::buscar_helper(NoPacote* no, int id_pacote)
{
    if (no == nullptr || no->id_pacote == id_pacote) return no;

    if (id_pacote < no->id_pacote) return buscar_helper(no->esq, id_pacote);

    return buscar_helper(no->dir, id_pacote);
}