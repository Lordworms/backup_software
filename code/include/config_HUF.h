#ifndef CONFIG_HUF
#define CONFIG_HUF
#include<bitset>
#include<string>
using namespace std;
struct count_union
{
    unsigned char val;//对应的字符
    int count;//出现的次数
};//用于统计词频的结构体
//哈夫曼树的节点

struct node
{
    node* left=NULL;
    node* right=NULL;
    int count=0;//字符出现次数
    bool flag=false;//判断是否是字符节点
    unsigned char name=0x00;//对应的字符
};
struct huff_code
{
    bool flag=false;
    string code;
};
#endif