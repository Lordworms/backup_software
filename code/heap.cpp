#include"heap.h"


Minheap::Minheap(count_union* arr[],size_t len)
{
    now_num=0;
    for(size_t i=0;i<len;i++)
    {
        if(arr[i]->count>0)
        {
            node* tmp=new node;
            tmp->name=arr[i]->val;
            tmp->count=arr[i]->count;
            tmp->flag=true;
            insert(tmp);
        }
    }
    
    
}
Minheap::Minheap()
{
    now_num=0;
}
bool Minheap::adjust(int now_num){
            int current=now_num;
            int parent=(current-1)/2;
            node* tmp=new node;
            while(current>0)
            {
                if(heap[current]->count<heap[parent]->count)
                {
                    tmp=heap[current];
                    heap[current]=heap[parent];
                    heap[parent]=tmp;
                    current=parent;
                    parent=(parent-1)/2;
                }
                else
                {
                    break;
                }
            }
            return true;
}

bool Minheap::insert(node* x){
        heap[now_num]=x;
        adjust(now_num);
        now_num++;
        return true;

}

node* Minheap::pop()
{
            node* ret=heap[0];
            now_num--;
            heap[0]=heap[now_num];
            int now_par=0;
            int now_son=1;
            node* tmp;
            while(now_son<now_num)
            {   
                if(heap[now_son]->count>=heap[now_par]->count&&heap[now_son+1]->count>=heap[now_par]->count)
                {
                    break;
                }
                 else
                {   
                    if(heap[now_son]->count>heap[now_son+1]->count)
                    {
                        now_son++;
                    }
                    tmp=heap[now_par];
                    heap[now_par]=heap[now_son];
                    heap[now_son]=tmp;
                    now_par=now_son;
                    now_son=now_son*2+1;
                }
                //找儿子中较大的一个
                
                //无需调整
                
               
            }
            return ret;
}

void Minheap::print()
{
    for(int i=0;i<now_num;i++)
            {
                cout<<heap[i]->count<<' ';
            }
            cout<<endl;
    cout<<now_num<<endl;
    /*
    while(now_num>0)
    {
        cout<<pop()->count<<" ";
    }
    cout<<endl;
    cout<<now_num<<endl;
    */
}

void Minheap::clear()
{
    for(size_t i=0;i<256;++i)
    {
        heap[i]=NULL;
    }
}
bool Minheap::empty()
{
    return now_num>0;
}
int Minheap::size()
{
    return now_num;
}