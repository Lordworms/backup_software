#ifndef CLIENT_H
#define CLIENT_H
#pragma comment(lib , "ws2_32.lib")
#include<winsock2.h>
#include"tar.h"
#include<iostream>
#define HEADER 512
#define INFO_SIZE 256
using namespace std;
struct info
{
    char name[INFO_SIZE]={0};
    char dir[INFO_SIZE]={0};
};

int main_client(int number,char* addr,char* port);
int main_send(int number,char* addr,char* port);
#endif