#include"client.h"
#define BUFSIZE 512
//D:\\2021_software_class\\code\\test
//三个参数分别为e ip地址，端口号
int send_data(char* name)
{
    return 0;
}
int main_client(int argc,char* addr,char* por)
{
    WSADATA wsd;//一种数据结构，用来存储被WSAStartup函数调用后返回的socket数据
    SOCKET sClient;
    char Buffer[BUFSIZE]={0};
    char nameBuffer[BUFSIZE];
    char dirname[BUFSIZE];//需要发送的文件夹名
    int ret;
    FILE* send_file;
    struct sockaddr_in server;//用来处理网络通信的地址，它包括16位TCP端口号和32位IP地址
    unsigned short port;//端口号
    struct hostent *host=NULL;
    //加载winsock dll
    if(argc<3)
    {
     cout<<"Usage:"<<"IP Port"<<endl;
     return -1;
    }
    if (WSAStartup(MAKEWORD(2,2),&wsd)!=0)
    {
        cout<<"Winsock 初始化失败"<<endl;
        return -1;

    }
    sClient=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);//AF_INET（TCP/IP – IPv4）sock_stream提供面向连接的稳定数据传输，最后一个参数表示是TCP协议
    int nNetTimeout=1000;
    setsockopt(sClient,SOL_SOCKET,SO_RCVTIMEO,(char *)&nNetTimeout,sizeof(int));
    if(sClient ==INVALID_SOCKET)
    {
        cout<<"socket() failed: "<<WSAGetLastError()<<endl;
        return 1;
    }
    server.sin_family=AF_INET;//TCP/IP – IPv4
    port=atoi(por);//把字符串转换为整型
    server.sin_port=htons(port);//将整形变量从主机字节顺序变为网络字节顺序
    server.sin_addr.s_addr=inet_addr(addr);//将一个点分十进制的IP转换为一个常整数型数
    
    if(server.sin_addr.s_addr==INADDR_NONE)
    {
     host=gethostbyname(addr);//gethostbyname,返回一个hostent结构体结构体包括
     //h_name:官方域名代表某一主页
     //h_aliases:别名，可以通过多个域名访问同一主机，同一IP地址可以绑定多个域名
     //h_addrtype:2 表示TCP+IPV4
     //aliases:www.baidu.com
     if(host==NULL)
        {
            cout<<"cannot analyze the server addr"<<addr<<endl;
            return 1;
        }   
     CopyMemory(&server.sin_addr ,
                    host->h_addr_list[0] ,
                    host->h_length);
    }
    //建立连接
    if(connect(sClient,(struct sockaddr*)&server,sizeof(server))==SOCKET_ERROR)
    {
        cout<<"connect() failed: "<<WSAGetLastError()<<endl;
        return 1;
    }
    FILE* info_file;
    info_file=fopen("D:\\info.txt","rb");
    info tt;
    fread(&tt,sizeof(tt),1,info_file);
    strcpy(nameBuffer,tt.name);
    cout<<nameBuffer<<endl;
    itoa(strlen(nameBuffer),Buffer,10);
    ret=send(sClient,Buffer,2,0);
    cout<<nameBuffer<<endl;
    if(ret=send(sClient,nameBuffer,strlen(nameBuffer),0)<=0)//发送用户名
    {
        cout<<"usrname send failed"<<endl;
    }
    //recv(sClient,Buffer,sizeof(Buffer),0);//接受需要发送文件的请求
    //cout<<Buffer;//please select the dir you want to transport:
    cout<<"please select the dir you want to transport:"<<endl;//please input your usrname
    strcpy(dirname,tt.dir);
    cout<<dirname;
    fclose(info_file);
    tar_unit tar;
    tar.tar(dirname);
    strcat(dirname,"\\");
    strcat(dirname,".tar");
    send_file=fopen(dirname,"rb");
    //开始发送文件惹
    while(!feof(send_file))
    {
        fread(&Buffer,HEADER,1,send_file);
        ret=send(sClient,Buffer,sizeof(Buffer),0);
    }
    //recv(sClient,Buffer,strlen(Buffer),0);//获取返回信息
    //cout<<Buffer<<endl;
    fclose(send_file);
    remove(dirname);
    closesocket(sClient);
    WSACleanup();    //清理
    return 0;
}
int main_send(int argc,char* addr,char* por)
{
    WSADATA wsd;//一种数据结构，用来存储被WSAStartup函数调用后返回的socket数据
    SOCKET sClient;
    char Buffer[BUFSIZE]={0};
    char nameBuffer[BUFSIZE];
    char dirname[BUFSIZE];//需要发送的文件夹名
    int ret;
    FILE* send_file;
    struct sockaddr_in server;//用来处理网络通信的地址，它包括16位TCP端口号和32位IP地址
    unsigned short port;//端口号
    struct hostent *host=NULL;
    //加载winsock dll
    if(argc<3)
    {
     cout<<"Usage:"<<"IP Port"<<endl;
     return -1;
    }
    if (WSAStartup(MAKEWORD(2,2),&wsd)!=0)
    {
        cout<<"Winsock 初始化失败"<<endl;
        return -1;

    }
    sClient=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);//AF_INET（TCP/IP – IPv4）sock_stream提供面向连接的稳定数据传输，最后一个参数表示是TCP协议
    
    if(sClient ==INVALID_SOCKET)
    {
        cout<<"socket() failed: "<<WSAGetLastError()<<endl;
        return 1;
    }
    server.sin_family=AF_INET;//TCP/IP – IPv4
    port=atoi(por);//把字符串转换为整型
    server.sin_port=htons(port);//将整形变量从主机字节顺序变为网络字节顺序
    server.sin_addr.s_addr=inet_addr(addr);//将一个点分十进制的IP转换为一个常整数型数
    
    if(server.sin_addr.s_addr==INADDR_NONE)
    {
     host=gethostbyname(addr);//gethostbyname,返回一个hostent结构体结构体包括
     //h_name:官方域名代表某一主页
     //h_aliases:别名，可以通过多个域名访问同一主机，同一IP地址可以绑定多个域名
     //h_addrtype:2 表示TCP+IPV4
     //aliases:www.baidu.com
     if(host==NULL)
        {
            cout<<"cannot analyze the server addr"<<addr<<endl;
            return 1;
        }   
     CopyMemory(&server.sin_addr ,
                    host->h_addr_list[0] ,
                    host->h_length);
    }
    //建立连接
    if(connect(sClient,(struct sockaddr*)&server,sizeof(server))==SOCKET_ERROR)
    {
        cout<<"connect() failed: "<<WSAGetLastError()<<endl;
        return 1;
    }
    FILE* file;
    file=fopen("D:\\info1.txt","rb");
    info tt;
    fread(&tt,sizeof(tt),1,file);
    fclose(file);
    itoa(0,Buffer,10);
    strcpy(nameBuffer,tt.name);
    ret=send(sClient,Buffer,2,0);//发送00，表示需要文件
    memset(Buffer,0,sizeof(Buffer));
    itoa(strlen(nameBuffer),Buffer,10);
    ret=send(sClient,Buffer,2,0);//发送名称大小
    if(ret=send(sClient,nameBuffer,strlen(nameBuffer),0)<=0)//发送用户名
    {
        cout<<"usrname send failed"<<endl;
    }
    //接收文件
    char tar_dir[256]={0};
    strcpy(tar_dir,tt.dir);
    strcat(tar_dir,"\\");
    strcat(tar_dir,".tar");
    file=fopen(tar_dir,"wb");
    memset(Buffer,0,sizeof(Buffer));
    
    while(1)
    {
        ret=recv(sClient,Buffer,512,0);
        cout<<"receiving "<<ret<<" bytes"<<endl;
        if(strstr(Buffer,"bye")!=NULL&&ret==3)
        {
            break;
        }
        
        fwrite(&Buffer,sizeof(Buffer),1,file);    
    }
    fclose(file);
    tar_unit tar;
    tar.untar(tar_dir,tt.dir);
    DeleteFileA(tar_dir);
    return 0;
}