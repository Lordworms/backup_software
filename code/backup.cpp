#include"backup.h"
//NTFS文件系统只支持对目录创建软链接，对文件创建硬链接
//思路：CreateSymbolicLinkA创建符号链接
//访问某个文件夹的软链接:区分软链接：fileinfo.attri=1040,正常文件夹是16（用于判断软链接）获取软链接的具体位置
//对于硬链接而言，备份时候只备份一次
int copyFile(const char * pSrc,const char *pDes)
{
    FILE *in_file, *out_file;
    char data[BUF_SIZE];
    size_t bytes_in, bytes_out;
    long len = 0;
    if ( (in_file = fopen(pSrc, "rb")) == NULL )
    {
        perror(pSrc);
        return -2;
    }
    if ( (out_file = fopen(pDes, "wb")) == NULL )
    {
        perror(pDes);
        return -3;
    }
    while ( (bytes_in = fread(data, 1, BUF_SIZE, in_file)) > 0 )
    {
        bytes_out = fwrite(data, 1, bytes_in, out_file);
        if ( bytes_in != bytes_out )
        {
            perror("Fatal write error.\n");
            return -4;
        }
        len += bytes_out;
        printf("copying file .... %d bytes copy\n", len);
    }
    fclose(in_file);
    fclose(out_file);
    return 1;
}

int copyDir(const char * pSrc,const char *pDes)
{
    if (NULL == pSrc || NULL == pDes)	return -1;
    mkdir(pDes);
    

    char dir[BUF_SIZE] = {0};
    char srcFileName[BUF_SIZE] = {0};
    char desFileName[BUF_SIZE] = {0};
    char *str = (char*)"\\*.*";
    strcpy(dir,pSrc);
    strcat(dir,str);
    //首先查找dir中符合要求的文件
    long hFile;
    _finddata_t fileinfo;
    if ((hFile = _findfirst(dir,&fileinfo)) != -1)
    {
        do
        {
            strcpy(srcFileName,pSrc);
            strcat(srcFileName,"\\");
            strcat(srcFileName,fileinfo.name);
            strcpy(desFileName,pDes);
            strcat(desFileName,"\\");
            strcat(desFileName,fileinfo.name);

            if (!(fileinfo.attrib & _A_SUBDIR))
            {
                if(check_hard(srcFileName,desFileName)=="")
                {
                    copyFile(srcFileName,desFileName);
                    change_file_time(srcFileName,desFileName);
                }
                else
                {
                    CreateHardLinkA(desFileName,check_hard(srcFileName,desFileName).c_str(),0);
                    change_file_time(srcFileName,desFileName);
                }
               
            }
            else
            {
                if ( strcmp(fileinfo.name, "." ) != 0 && strcmp(fileinfo.name, ".." ) != 0 )
                {
                    if(fileinfo.attrib==16)//非软链接
                    {
                        copyDir(srcFileName,desFileName);
                        change_file_time(srcFileName,desFileName);
                    }
                    else
                    {
                        create_symbolic(srcFileName,desFileName);
                        change_file_time(srcFileName,desFileName);
                    }
                }
            }
        } while (_findnext(hFile,&fileinfo) == 0);
        _findclose(hFile);
        return 1;
    }
    return -3;
}

int copy_all(char * pSrc,char *pDes)
{
    if(check_hard(pSrc,pDes)=="")
    {
        copyFile(pSrc,pDes);
        change_file_time(pSrc,pDes);
    }
    else
    {
        CreateHardLinkA(pDes,check_hard(pSrc,pDes).c_str(),0);
        change_file_time(pSrc,pDes);
    }
    return 1;
}
