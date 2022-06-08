#include"huffman.h"

huffman_tree::huffman_tree()
{
    node_num=0;
    heap=NULL;
}
huffman_tree::huffman_tree(count_union* arr[],size_t len)
{   
    node_num=0;
    Minheap* tmpheap=new Minheap(arr,len);
    heap=tmpheap;
    root=create_tree();
    node_num++;
}
node* huffman_tree::create_tree()
{
    while(heap->size()>1)
    {
        node* left=heap->pop();
        
        node_num++;
        node* right=heap->pop();
       
        node_num++;
        node* tmp=new node;
        tmp->count=left->count+right->count;
        tmp->left=left;
        tmp->right=right;
        heap->insert(tmp);
    }    
    return heap->pop();
}
void huffman_tree::print()
{
   


}