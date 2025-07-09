#include "Set.hpp"
#include <iostream>

using namespace std;

StringSet::StringSet(int n)
{
    tamanhoOriginal = n;
    tamanhoTabela = (n < 8) ? 8 : n; // tamanho minimo para a tabela
    tamanhoConjunto = 0;

    tabela = new ElementoTabela[tamanhoTabela];

    for(int i = 0; i < tamanhoTabela; i++)
    {
        tabela[i].vazio = true;
        tabela[i].retirada = false;
    }
}

StringSet::~StringSet()
{
    delete[] tabela;
}

// insere uma string s e redimensiona quando 
void StringSet::Inserir(string s)
{
    if(Pertence(s)) return; //ignora caso o elemento já esteja lá

    if((double)tamanhoConjunto / tamanhoTabela >= 0.7)
    {
        Rehash(0);
    }

    int pos = Hash(s);
    int i = 0;

    while(i < tamanhoTabela)
    {
        int idx = (pos + i) % tamanhoTabela;
        if(tabela[idx].vazio || tabela[idx].retirada)
        {
            tabela[idx].dado = s;
            tabela[idx].vazio = false;
            tabela[idx].retirada = false;
            tamanhoConjunto++;
            return;
        }
        i++;
    }
}

// remove a string s 
void StringSet::Remover(string s)
{
    int pos = Hash(s);
    int i = 0;

    while(i < tamanhoTabela && !tabela[(pos + i) % tamanhoTabela].vazio)
    {
        int idx = (pos + i) % tamanhoTabela;

        if(!tabela[idx].retirada && tabela[idx].dado == s)
        {
            tabela[idx].retirada = true;
            tamanhoConjunto--;
            return;
        }
        i++;
    }
}

// verifica se a string s pertence ao conjunto
bool StringSet::Pertence(string s)
{
    int pos = Hash(s);
    int i = 0;

    while(i < tamanhoTabela && (!tabela[(pos + i) % tamanhoTabela].vazio))
    {
        int idx = (pos + i) % tamanhoTabela;
        if(!tabela[idx].retirada && (tabela[idx].dado == s))
        {
            return true;
        }
        i++;
    }
    return false;
}

// gera um novo conjunto com a interseção com s
StringSet* StringSet::Intersecao(StringSet* S)
{
    StringSet* resultado = new StringSet(tamanhoTabela < S->tamanhoTabela ? tamanhoTabela : S->tamanhoTabela);

    for(int i = 0; i < tamanhoTabela; i++)
    {
        if(!tabela[i].vazio && !tabela[i].retirada && S->Pertence(tabela[i].dado))
        {
            resultado->Inserir(tabela[i].dado);
        }
    }
    return resultado;
}  

// gera um novo conjunto com a uniao com s
StringSet* StringSet::Uniao(StringSet* S)
{
    StringSet* resultado = new StringSet(tamanhoTabela + S->tamanhoTabela);
    for (int i = 0; i < tamanhoTabela; i++)
    {
        if(!tabela[i].vazio && !tabela[i].retirada)
        {
            resultado->Inserir(tabela[i].dado);
        }
    }

    for (int i = 0; i < S->tamanhoTabela; i++)
    {
        if(!S->tabela[i].vazio && !S->tabela[i].retirada)
        {
            resultado->Inserir(S->tabela[i].dado);
        }
    }
    return resultado;
}

// gera um novo conjunto com a diferença simestrica com s
StringSet* StringSet::DiferencaSimetrica(StringSet* S)
{
    StringSet* resultado = new StringSet(tamanhoTabela + S->tamanhoTabela);
    for (int i = 0; i < tamanhoTabela; i++)
    {
        if(!tabela[i].vazio && !tabela[i].retirada && !S->Pertence(tabela[i].dado))
        {
            resultado->Inserir(tabela[i].dado);
        }
    }

    for (int i = 0; i < S->tamanhoTabela; i++)
    {
        if(!S->tabela[i].vazio && !S->tabela[i].retirada && !Pertence(S->tabela[i].dado))
        {
            resultado->Inserir(S->tabela[i].dado);
        }
    }
    return resultado;
}

// mostra os elementos 
void StringSet::Imprimir()
{   
    cout << "{ ";
    
    bool first = true;
    for(int i = 0; i < tamanhoTabela; i++)
    {
        if(!tabela[i].vazio && !tabela[i].retirada)
        {
            if(!first)
            {
                cout << ", ";
            }
            cout << tabela[i].dado;
            first = false;
        }
    }
    cout << " }" << endl;
}

// funcao hash baseada em multiplicacao de primos
int StringSet::Hash(string s)
{
    unsigned long h = 0;
    for(char c : s)
    {
        h = (h * 33 + c);
    }
    return h % tamanhoTabela;
}

// redimensiona
void StringSet::Rehash(int pos)
{
    int nova_Tabela = tamanhoTabela * 2;
    ElementoTabela* nova = new ElementoTabela[nova_Tabela];

    for(int i = 0; i < nova_Tabela; i++)
    {
        nova[i].vazio = true;
        nova[i].retirada = false;
    }

    ElementoTabela* antigo = tabela;
    int tamanho_antes = tamanhoTabela;

    tabela = nova;
    tamanhoTabela = nova_Tabela;
    tamanhoConjunto = 0;

    for(int i = 0; i < tamanho_antes; i++)
    {
        if(!antigo[i].vazio && !antigo[i].retirada)
        {
            Inserir(antigo[i].dado);
        }
    }

    delete[] antigo;
}

void::StringSet::Resize(size_t tamanho)
{

}