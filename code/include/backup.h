#ifndef BACK_UP
#define BACK_UP
#include <stdlib.h>
#include <direct.h>
#include <string.h>
#include <io.h>
#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <process.h>
#include<time.h>
#include"config.h"
#define BUF_SIZE 256
using namespace std;
int copyFile(const char * pSrc,const char *pDes);
int copyDir(const char * pSrc,const char *pDes);
int copy_all(char * pSrc,char *pDes);//用于拷贝硬链接

#endif