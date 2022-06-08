#ifndef CONFIG_H
#define CONFIG_H
#include<iostream>
#include<string>
#include<vector>
#include<string.h>
#include <windows.h>
#include<time.h>
#include<io.h>
using namespace std;

std::string cmdPopen(const char* a);
//判断硬链接,传入的name有前缀
string check_hard(char* name,char* des);
std::string get_junc_des(char* junc);
bool create_symbolic(char* name,char* des);
bool check_harding(char* name,char* des);
bool change_file_time(char* src,char* des);
#endif