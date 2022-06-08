#include"server.h"
#define BUFFER_SIZE 4096
bool check_name(char* name)
{
    FILE* file;
    file=fopen("D:\\2021_software_class\\netdisk\\usrname.txt","rb");
    person_name tmp;
    fread(&tmp,sizeof(tmp),1,file);
    while(!feof(file))
    {
        if(strcmp(name,tmp.name)==0)
        {
            return true;
        }
        fread(&tmp,sizeof(tmp),1,file);
    }
    return false;
}

DWORD WINAPI ClientThread(LPVOID lpParam)//DWORD全程double word，每个word是两个子节的长度，LPVOID是一个没有类型的指针
{
    SOCKET sock = (SOCKET)lpParam;
    char Buffer[BUFFER_SIZE];
    char name_buf[BUFFER_SIZE];
    char dir_buf[BUFFER_SIZE];
    int ret,nLeft,idx;
    //strcpy(Buffer,"please input your usrname:");//发送需求用户名的字符
    //ret=send(sock,Buffer,strlen(Buffer),0);//发送请求
    //cout<<"send:"<<ret<<endl;
    ret=recv(sock,Buffer,2,0);
    int len=atoi(Buffer);
    if(len!=0)
    {
        if(ret=recv(sock,Buffer,len,0)>0){//接受用户名
            cout<<"the usrname is "<<Buffer<<endl;
        }

        strcpy(name_buf,DISK_BASE);
        strcat(name_buf,"\\");
        strcat(name_buf,Buffer);//生成对应的用户目录
        if(access(name_buf,0)!=0)
        {
            mkdir(name_buf);
        }
    //strcpy(Buffer,"please select the dir you want to transport:");//发送请求数据的要求
    //ret=send(sock,Buffer,strlen(Buffer),0);
    
        cout<<"waiting for the client to transport...."<<endl;
        FILE* outfile;
        strcpy(dir_buf,name_buf);
        strcat(name_buf,"\\");
        strcat(name_buf,".tar");
    
        outfile=fopen(name_buf,"wb");
        memset(Buffer,0,sizeof(Buffer));
        while(1)//接受数据包
        {
            ret=recv(sock,Buffer,512,0);
            cout<<"receivig "<<ret<<" bytes data"<<endl;
            if(ret!=512)break;
            fwrite(&Buffer,512,1,outfile);
        }
        fclose(outfile);
        tar_unit tar;
        tar.untar(name_buf,dir_buf);
        DeleteFileA(name_buf);
        return 0;
        }
    else//发送备份文件
    {
      ret=recv(sock,Buffer,2,0);//接受用户名长度
      len=atoi(Buffer);
      if(ret=recv(sock,Buffer,len,0)>0){//接受用户名
            cout<<"the usrname is "<<Buffer<<endl;
        }
        strcpy(name_buf,DISK_BASE);
        strcat(name_buf,"\\");
        strcat(name_buf,Buffer);//对应的用户目录 
        tar_unit tar;
        tar.tar(name_buf);
        cout<<"sending data to "<<Buffer;
        strcat(name_buf,"\\");
        strcat(name_buf,".tar");
        FILE* infile;
        infile=fopen(name_buf,"rb");
        fread(&Buffer,512,1,infile);
        while(!feof(infile))
        {
            ret=send(sock,Buffer,512,0);  
            cout<<"send "<<ret<<" bytes data"<<endl;
            if(ret!=512)break;
            fread(&Buffer,512,1,infile);           
        }
        strcpy(Buffer,"bye");
        ret=send(sock,Buffer,3,0);
        cout<<"end sending"<<endl;
        fclose(infile);
        DeleteFileA(name_buf);
        return 0;
    }
}

int main_server(int argc,char *por)
{
    WSADATA wsd;
    HANDLE hThread;
    DWORD dwThread;
    SOCKET sListen, sClient;
    int AddrSize;
    unsigned short port;
    struct sockaddr_in local,client;

    if(argc<2)
    {
        cout<<"USAGE:"<<"Port"<<endl;
        return -1;
    }
    if(WSAStartup(MAKEWORD(2,2),&wsd)!=0)
    {
        cout<<"WinSock 初始化失败！"<<endl;
        return -1;
    }
    sListen=socket(AF_INET,SOCK_STREAM,IPPROTO_IP);
    if(sListen==SOCKET_ERROR)
    {
        cout<<"socket() failed:"<<WSAGetLastError()<<endl;
        return 1;
    }
    local.sin_family=AF_INET;
    local.sin_addr.s_addr=htonl(INADDR_ANY);
    port=atoi(por);//字符转换为整型
    local.sin_port=htons(port);//将主机数转换成无符号长整型的网络字节顺序

    //绑定socket
    if(bind(sListen,(struct sockaddr*)&local,sizeof(local))==SOCKET_ERROR)
    {
        cout<<"bind() failed: "<<WSAGetLastError()<<endl;
        return 1;
    }
    listen(sListen,8);

    while(1)
    {
        AddrSize=sizeof(client);
        sClient=accept(sListen,(struct sockaddr*)&client,&AddrSize);
        if(sClient ==INVALID_SOCKET)
        {
            cout<<"accept() failed:"<<WSAGetLastError();
            break;
        }
        cout<<"receiving client connection:"<<inet_ntoa(client.sin_addr)<<":"<<ntohs(client.sin_port)<<endl;
        
        hThread=CreateThread(NULL,0,ClientThread,(LPVOID)sClient,0,&dwThread);
        if(hThread==NULL)
        {
            cout<<"CreateThread() failed:"<<GetLastError()<<endl;
            break;
        }
        CloseHandle(hThread);
    }
    closesocket(sListen);
    WSACleanup();
    return 0;
}
