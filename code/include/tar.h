#ifndef TAR_H
#define TAR_H
#define FIL_E 0//文件的mode
#define DIR 1//目录的mode
#define SYMBOLIC 2//软链接的mode
#define HARD 3//嗯链接的mode
#define MAX 256
#define BLOCK_SIZE 512
#include<string>
#include<iostream>
#include<stdio.h>
#include <conio.h>
#include <process.h>
#include <direct.h>
#include"config.h"
using namespace std;
struct posix_tar_header
{
		char name[100];//文件名
		char mode[8];//用作存储是否是硬链接
		char uid[8];//usr id
		char gid[8];//group id
		char size[12];//文件大小的八进制表示
		char mtime[12];//x修改时间
		char chksum[8];//checksum的计算方法为出去checksum字段其他所有的512-8共504个字节的ascii码相加的值再加上256(checksum当作八个空格，即8*0x20）
		char typeflag;
		char linkname[100]={0};//软/硬链接的文件名
		char magic[6];
		char version[2];
		char uname[14];
		SYSTEMTIME file_time[3];
		char gname[32];
		char devmajor[8];
		char devminor[8];
		char prefix[123];
        char padding[12];
};

class tar_unit
{
    public:
        tar_unit();
        tar_unit(string filename);
        int tar(const char* filename);//打包函数
        int untar(const char*  filename,const char* desname);//解包函数
        int untar_dir(posix_tar_header now,FILE* file);
		int untar_file(const char* filename,FILE* file);
		int tar_file(const char* filename,FILE* file,int size);//单个文件写入tar
        int tar_dir(const char* filename,FILE* file,const char* prefix);
    private:
        char buffer[MAX];
		char tar_file_name[MAX];
};
#endif