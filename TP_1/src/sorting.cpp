#include "sorting.hpp"

// Insertion sort usado no quicksort com esq e dir
void Sorting::insertion_sort(int* V, int esq, int dir, int& cmp, int& mov, int& calls) 
{
    ++calls;
    
    // Começa na primeira posição, vai avançando e comparando com os indices anteriores 
    for (int i = esq + 1; i <= dir; i++) 
    {
        mov++;
        int key = V[i];
        int j = i - 1;

        while (j >= esq && V[j] > key) 
        {
            cmp++;
            mov++;
            V[j + 1] = V[j];
            j--;
        }
        cmp++;
        V[j + 1] = key;
        mov++;
    }
}

// Função com o quicksort otimizado, mediana de 3 e insertion sort para partições pré calculadas
void Sorting::quicksort_insertion(int* V, int esq, int dir, int& cmp, int& mov, int& calls, int limiar_insertion)
{
    
    if(esq >= dir) return; // Para se o array já está ordenado 

    int i, j;
    partition(V, esq, dir, i, j, cmp, mov, calls);
    calls++;

    if(esq < j) // Calculos para saber se já é mais eficiente usar o insertion sort
    {
        if (j - esq + 1 <= limiar_insertion)
        {
            insertion_sort(V, esq, j, cmp, mov, calls);
        }
        else
        {
            quicksort_insertion(V, esq, j, cmp, mov, calls, limiar_insertion);
        }
    }
    if(i < dir) // Calculos para saber se já é mais eficiente usar o insertion sort
    {
        if (dir - i + 1 <= limiar_insertion)
        {
            insertion_sort(V, i, dir, cmp, mov, calls);
        }
        else 
        {
            quicksort_insertion(V, i, dir, cmp, mov, calls, limiar_insertion);
        }
    }
}

// Cálculo da Mediana usado na função da partição
int Sorting::median (int a, int b, int c) {
    if ((a <= b) && (b <= c)) return b;  // a b c
    if ((a <= c) && (c <= b)) return c;  // a c b
    if ((b <= a) && (a <= c)) return a;  // b a c
    if ((b <= c) && (c <= a)) return c;  // b c a
    if ((c <= a) && (a <= b)) return a;  // c a b
    return b;                            // c b a
}

// Função swap para facilitar a troca
void Sorting::swap(int& a, int& b, int& mov) 
{
    int temp = a;
    a = b;
    b = temp;
    mov += 3;
}

// Partição com mediana de 3
int Sorting::partition(int* V, int l, int r, int& i, int& j, int& cmp, int& mov, int& calls)
{
    calls++;

    int m = (l + r) / 2;
    int pivot = median(V[l], V[m], V[r]);

    i = l;
    j = r;

    while (i <= j) 
    {
        while (V[i] < pivot) // i encontra alguém menor
        {
            cmp++;
            i++;
        }
        cmp++;

        while (V[j] > pivot) // j encontra alguém maior
        {
            cmp++;
            j--;
        }
        cmp++;

        if (i <= j) 
        {
            swap(V[i], V[j], mov); // troca e muda a posição de i
            i++;
            j--;
        }
    }
    return i;
}