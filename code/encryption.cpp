#include"encryption.h"
psw::psw()//构造函数 
{
    for(int i=0;i<8;++i)
    {
        password[i]=0x11;
    }
}
psw::psw(char* pwd)
{
    for(int i=0;i<8;++i)
    {
        password[i]=pwd[i];
    }
}
bool psw::encode(char* filepath)
{
    FILE* file;
    FILE* outfile;
    char path[256]={0};
    file=fopen(filepath,"rb");
    strcpy(path,filepath);
    strcat(path,".encode");
    outfile=fopen(path,"wb");
    char tmp=fgetc(file);
    int now=0;
    while(!feof(file))
    {
        tmp^=password[now%8];
        fputc(tmp,outfile);
        tmp=fgetc(file);
    }
    fclose(file);
    fclose(outfile);
    return true;
}
bool psw::decode(char* filepath,char* pwd)
{
     FILE* file;
    FILE* outfile;
    char path[256]={0};
    file=fopen(filepath,"rb");
    strcpy(path,filepath);
    strcat(path,".decode");
    outfile=fopen(path,"wb");
    char tmp=fgetc(file);
    int now=0;
    while(!feof(file))
    {
        tmp^=pwd[now%8];
        fputc(tmp,outfile);
        tmp=fgetc(file);
    }
    fclose(file);
    fclose(outfile);
    return true;
}