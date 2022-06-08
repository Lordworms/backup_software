#ifndef HUFFMAN_H
#define HUFFMAN_h
#include"heap.h"
#include"config_HUF.h"
#include<string>


//如果该节点是代表一个编码节点的话，那么对应的name!=0x00
class huffman_tree
{
  private:
    int node_num;//节点的个数
    
  public:
    Minheap* heap;
    node* root;//根节点 
    node* create_tree();//通过传入的键值对来建树
    huffman_tree();//构造函数
    huffman_tree(count_union* arr[],size_t len);//用数组初始化heap
    void print();
};
#endif