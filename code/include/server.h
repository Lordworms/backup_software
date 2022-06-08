#ifndef SERVER_H
#define SERVER_H
#include<winsock2.h>
#include<stdio.h>
#include<stdlib.h>
#include<io.h>
#include<iostream>
#include"tar.h"
using namespace std;
#pragma comment(lib , "ws2_32.lib")
#define DISK_BASE "D:\\2021_software_class\\netdisk"//数据库的根目录
struct person_name//用于保存用户名
{
    char name[64]={0};
};
bool check_name(char* name);
DWORD WINAPI ClientThread(LPVOID lpParam);
int main_server(int argc,char *port);
#endif