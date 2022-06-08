#ifndef FILE_H
#define FILE_H
#include<iostream>
#include<stdio.h>
#include"config.h"
#include"huffman.h"
#include<string.h>
using namespace std;
#define MAX 512
class file
{
    public:
        file();
        file(const char* name);
        void frequency();//用于统计词频
        void print();//打印词频
        bool huffcode_gen(node* now,string par);
        bool compress();//压缩文件
        bool uncompress(const char* name);//
    private:
        count_union* char_256[MAX];//保存一个字节所有字符可能情况的词频
        const char* name=NULL;//文件名
        huff_code* huf_code[MAX_SIZE];
        huffman_tree* huf;
        int file_count=0;
};
#endif