#ifndef LISTAADJACENCIA_HPP
#define LISTAADJACENCIA_HPP

class ListaAdjacencia
{
    public:
    ListaAdjacencia();
    ~ListaAdjacencia();

    void AdicionaVertice();
    void AdicionaAresta(int v, int w);
    int QuantidadeVertices() const;
    int QuantidadeArestas() const;
    int Grau(int v) const;
    int GrauMinimo() const;
    int GrauMaximo() const;
    void ImprimeVizinhos(int v) const;

    private:
    struct No
    {
        int valor;
        No* next;
    };
    
    struct Lista
    {
        No* start;
    };

    Lista* adjacencia;
    int capacidade;
    int tamanho;
    int arestas;

    void Redimensionar();
    void Inserir(Lista& lista, int valor);
    int Tamanho(const Lista& lista) const;
    void Imprimir(const Lista& lista) const;
    bool Contem(const Lista& lista, int valor) const;
};
#endif
