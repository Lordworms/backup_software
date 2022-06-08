#include"monitor.h"

// 宽字节字符串转多字节字符串
void W2C(wchar_t *pwszSrc, int iSrcLen, char *pszDest, int iDestLen)
{
    ::RtlZeroMemory(pszDest, iDestLen);
    // 宽字节字符串转多字节字符串
    ::WideCharToMultiByte(CP_ACP,
        0,
        pwszSrc,
        (iSrcLen / 2),
        pszDest,
        iDestLen,
        NULL,
        NULL);
}
 
 
// 目录监控多线程
UINT MonitorFileThreadProc(LPVOID lpVoid)
{
    
    dir_unit* tmp=(dir_unit*)lpVoid;
    char* pszDirectory = tmp->src;
    char des[DIR_SIZE]={0};
    char src[DIR_SIZE]={0};
    
    // 打开目录, 获取文件句柄
    HANDLE hDirectory = ::CreateFileA(pszDirectory, FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS, NULL);
    if (INVALID_HANDLE_VALUE == hDirectory)
    {
        cout<<"hello"<<endl;
        cout<<"error:"<<GetLastError()<<endl;
        return 1;
    }
    char filebuf[MAX_PATH]={0};
    char szTemp[MAX_PATH] = { 0 };
    BOOL bRet = FALSE;
    DWORD dwRet = 0;
    DWORD dwBufferSize = 2048;
 
    // 申请一个足够大的缓冲区
    BYTE *pBuf = new BYTE[dwBufferSize];
    if (NULL == pBuf)
    {
        //ShowError("new");
        return 2;
    }
    FILE_NOTIFY_INFORMATION *pFileNotifyInfo = (FILE_NOTIFY_INFORMATION *)pBuf;
    
    // 开始循环设置监控
    do
    {
        ::RtlZeroMemory(pFileNotifyInfo, dwBufferSize);
        // 设置监控目录
        bRet = ::ReadDirectoryChangesW(hDirectory,
            pFileNotifyInfo,
            dwBufferSize,
            TRUE,
            FILE_NOTIFY_CHANGE_FILE_NAME |            // 修改文件名
            FILE_NOTIFY_CHANGE_ATTRIBUTES |            // 修改文件属性
            FILE_NOTIFY_CHANGE_LAST_WRITE,            // 最后一次写入
            &dwRet,
            NULL,
            NULL);
        
        if (FALSE == bRet)
        {
            //ShowError("ReadDirectoryChangesW");
            //break;
        }
        // 将宽字符转换成窄字符
        W2C((wchar_t *)(&pFileNotifyInfo->FileName), pFileNotifyInfo->FileNameLength, szTemp, MAX_PATH);
        strcpy(des,tmp->des);
        strcat(des,"\\");
        strcat(des,szTemp);
        strcpy(src,tmp->src);
        strcat(src,"\\");
        strcat(src,szTemp);
        // 判断操作类型并显示
        switch (pFileNotifyInfo->Action)
        {
        case FILE_ACTION_ADDED:
        {
            printf("[File Added Action]%s\n", szTemp);
            cout<<des<<endl;
            Sleep(3000);
            copy_all(src,des);
            
            break;
        }
        case FILE_ACTION_REMOVED:
        {
            // 移除文件
            DeleteFileA(des);
            cout<<des<<endl;
            printf("[File REMOVED Action]%s\n", szTemp);
            break;
        }
        case FILE_ACTION_MODIFIED:
        {
            // 修改文件
            copy_all(src,des);
            
            cout<<des<<endl;
            printf("[File MODIFIED Action]%s\n", szTemp);
            Sleep(1000);
            break;
        }
        default:
        {
            break;
        }
        }
 
    
    } while (1);
    // 关闭句柄, 释放内存
    
    
    ::CloseHandle(hDirectory);
    delete[] pBuf;
    pBuf = NULL;
 
    return 0;
}
 
 
// 创建目录监控多线程
void MonitorFile(dir_unit* pszDirectory)
{
    // 创建文件监控多线程
    ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MonitorFileThreadProc,pszDirectory, 0, NULL);
}
 
 
std::wstring GetWStringByChar(const char* szString)
{
    std::wstring wstrString;
    if (szString != NULL)
    {
        std::string str(szString);
        wstrString.assign(str.begin(), str.end());
    }

    return wstrString; 
}
