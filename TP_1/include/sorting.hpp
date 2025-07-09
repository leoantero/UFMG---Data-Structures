#ifndef SORTING_HPP
#define SORTING_HPP

class Sorting
{
    public:
        void insertion_sort(int* V, int esq, int dir, int& cmp, int& mov, int& calls);
        
        void quicksort_insertion(int* V, int i, int j, int& cmp, int& mov, int& calls, int limiar_insertion);
        
        int median(int a, int b, int c);
        int partition(int* V, int l, int r, int& i, int& j, int& cmp, int& mov, int& calls);
        void swap(int& a, int& b, int& move);
};

#endif
