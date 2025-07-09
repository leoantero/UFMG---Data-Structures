#include "heap.hpp"
#include <iostream>
#include <stdexcept>

Heap::Heap(int maxsize) : tamanho (0)
{
	data = new int[maxsize];
}

Heap::~Heap()
{
	delete[] data;
}

void Heap::Swap(int& a, int& b)
{
	int temp = a;
	a = b;
	b = temp;
}

void Heap::Inserir(int x)
{
	int i =  tamanho;
	data[i] = x;

	while(i > 0)
	{
		int pai = (i - 1)/2;
		if (data[i] >= data[pai]) break;
		Swap(data[i], data[pai]);
		i = pai;
	}
	tamanho++;
}
	
int Heap::Remover()
{
	if (tamanho == 0) throw std::underflow_error("Erro: Não é possível remover elementos pois o Heap está vazio");
		
	int raiz = data[0];
	data[0] = data[tamanho-1];
	tamanho--;
		
	int i = 0;
	while(true)
	{
		int esq = 2 * i + 1;
		int dir = 2 * i + 2;
		int menor = i;
			
		if (esq < tamanho && data[esq] < data[menor]) menor = esq;
		if (dir < tamanho && data[dir] < data[menor]) menor = dir;
			
		if (menor == i) break;
			
		Swap(data[i], data[menor]);
		i = menor;
	}
	return raiz;
}
	
bool Heap::Vazio()
{
	return tamanho == 0;
}