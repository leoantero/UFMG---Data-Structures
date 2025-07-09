#ifndef HEAP_HPP
#define HEAP_HPP

/*  Você pode inserir os includes necessários para que sua classe funcione.
 * Alteracoes no arquivo so podem ser feitas quando explicitadas
 */

class Heap{

    private:

        int tamanho;
        int* data;

    public:
        Heap(int maxsize);
        ~Heap();

        void Inserir(int x);
        int Remover();
        // Função para fazer a troca 
        void Swap(int& a, int& b);

        //Retorna true caso o heap esteja vazio, false caso contrário.
        bool Vazio();

    
};

#endif