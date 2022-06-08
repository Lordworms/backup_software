#include<iostream>
#include<string.h>
using namespace std;

class psw//加密单元
{
    public:
        //加密文件前八个子节为对应密码，由用户输入的密码来判定密码是否正确
        char password[8]={0};
        psw();//构造函数 
        psw(char* pwd);
        bool encode(char* filepath);
        bool decode(char* filepath,char* pwd);
};