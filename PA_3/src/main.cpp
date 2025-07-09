#include "heap.hpp"
#include <iostream>

int main()
{
	int n;
	std::cin >> n;
	Heap heap(n);
	
	for(int i = 0; i < n; i++)
	{
		int elemento;
		std::cin >> elemento;
		heap.Inserir(elemento);
	}
	
	while(!heap.Vazio())
	{
		std::cout << heap.Remover() << " ";
	}
	std::cout << std::endl;
	
	return 0;
}