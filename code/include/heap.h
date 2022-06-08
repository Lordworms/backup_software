#ifndef HEAP_H
#define HEAP_H
#include<iostream>
#include"config_HUF.h"
using namespace std;
#define MAX_SIZE 512
class Minheap
{
    public:
        Minheap();
        Minheap(count_union* arr[],size_t len);
        bool insert(node* x);
        bool adjust(int now_num);
        node* pop();
        void print();
        void clear();//清空堆
        bool empty();
        int size();
    private:
        int now_num;
        node* heap[MAX_SIZE];
};
#endif