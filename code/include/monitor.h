//用于实时备份和增量备份
#ifndef MONITOR_H
#define MONITOR_H
#include <iostream>
#include"backup.h"
#include <Windows.h>
#include <tchar.h>
using namespace std;
#define DIR_SIZE 128
struct dir_unit
{
    wstring source;
    char des[DIR_SIZE]={0};
    char src[DIR_SIZE]={0};
};
void W2C(wchar_t *pwszSrc, int iSrcLen, char *pszDest, int iDestLen);
UINT MonitorFileThreadProc(LPVOID lpVoid);
void MonitorFile(dir_unit* pszDirectory);
std::wstring GetWStringByChar(const char* szString);
#endif