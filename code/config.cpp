#include"config.h"

std::string cmdPopen(const char* a) {
	char buffer[1024] = { '\0' };
	FILE* pf = NULL;
	pf = _popen(a, "r");
	if (NULL == pf) {
		printf("open pipe failed\n");
		return std::string("");
	}
	std::string ret;
	while (fgets(buffer, sizeof(buffer), pf)) {
		ret += buffer;
	}
	_pclose(pf);
	return ret;
}
/*
std::string cmdPopen(const char* a) {
    string s("cmd.exe /c ");
    
    s+=a;
    s+=" > d:\\abc.txt";
	WinExec(s.c_str(), SW_HIDE);
    FILE* file;
    file=fopen("d:\\abc.txt","rb");
    string res;
    while(!feof(file))
    {
        res+=fgetc(file);
    }
    fclose(file);
    DeleteFile("d:\\abc.txt");
    return res;
}
*/
//判断硬链接,传入的name有前缀
string check_hard(char* name,char* des)//是硬链接返回链接对象，不是返回空字符串
{
    string prefix("findlinks ");
    prefix+=name;
    string a=cmdPopen(prefix.c_str());
    vector<string>group;   
    string tmp;
    int pos=a.find("Linking file:");
    if(pos==a.npos)
    {
        pos=a.find("Linking files:");
        if(pos!=a.npos)
            pos++;
    }
    if(pos==a.npos)//不是嗯链接
    {
        string res("");
        return res;
    }
    for(int i=pos+14;i<a.size();++i)
    {
        if(a[i]!='\n')
        {
            tmp+=a[i];
        }
        else
        {
            break;
        }
    }
    return tmp;
}
bool create_symbolic(char* name,char* des)
{
    string prefix("mklink /J ");
    prefix+=des;
    prefix+=" ";
    prefix+=name;
    string res=cmdPopen(prefix.c_str());
    cout<<res<<endl;
    return true;
}
std::string get_junc_des(char* junc)
{
       string a("junction.exe ");
       a+=junc;
       string pos=cmdPopen(a.c_str());
       string res;
       int s=pos.find("Substitute Name:");
       for(int i=s+16;i<pos.size();++i)
       {
           if(pos[i]!=' '&&pos[i]!='\n')
           {
                res+=pos[i];
           }
       }
    return res;
}
bool check_harding(char* name,char* des)
{
    string prefix("fsutil.exe hardlink list ");
    prefix+=name;
    string a=cmdPopen(prefix.c_str());
    vector<string>group;   
    string tmp;
    for(int i=0;i<a.size();++i)
    {
        if(a[i]!='\n')
        {
            tmp+=a[i];
        }
        else
        {
            string c=tmp;
            c="D:"+c;
            group.push_back(c);
            tmp.clear();
        }
    }
    string b(name);
    if (group[0]==b)//不是嗯链接
    {
        return true;
    }
    else//直接创建嗯链接
    {
       CreateHardLinkA(des,group[0].c_str(),0);
       
       return false;
    }
}
bool change_file_time(char* src,char* des)
{
    _finddata_t info;
    _finddata_t info_des;
    long flag=_findfirst(src,&info);
    _findfirst(des,&info_des);
    tm s;
    localtime_s(&s,&info.time_create);
    SYSTEMTIME createTime;
    GetSystemTime(&createTime);
    createTime.wDay = (short)(s.tm_mday);            //changes the day
    createTime.wMonth = (short)(s.tm_mon+1);         //changes the month
    createTime.wYear = (short)(s.tm_year+1900);        //changes the year
    createTime.wHour = (short)((s.tm_hour+24-8)%24);           //changes the hour
    createTime.wMinute = (short)s.tm_min;         //changes the minute
    createTime.wSecond = (short)s.tm_sec;         //changes the second
    localtime_s(&s,&info.time_write);
    SYSTEMTIME lastWriteTime;
    GetSystemTime(&lastWriteTime);
    lastWriteTime.wDay = (short)(s.tm_mday);        //changes the day
    lastWriteTime.wMonth = (short)(s.tm_mon+1);      //changes the month
    lastWriteTime.wYear = (short)(s.tm_year+1900);     //changes the year
    lastWriteTime.wHour = (short)((s.tm_hour+24-8)%24);       //changes the hour
    lastWriteTime.wMinute = (short)s.tm_min;     //changes the minute
    lastWriteTime.wSecond = (short)s.tm_sec;     //changes the second
    localtime_s(&s,&info.time_access);
    SYSTEMTIME lastAccessTime;
    GetSystemTime(&lastAccessTime);
    lastAccessTime.wDay = (short)(s.tm_mday);       //changes the day
    lastAccessTime.wMonth = (short)(s.tm_mon+1);     //changes the month
    lastAccessTime.wYear = (short)(s.tm_year+1900);    //changes the year
    lastAccessTime.wHour = (short)((s.tm_hour+24-8)%24);      //changes the hour
    lastAccessTime.wMinute =(short)s.tm_min;    //changes the minute
    lastAccessTime.wSecond =(short)s.tm_sec;     //changes the second
    FILETIME lastWriteFiletime;
    SystemTimeToFileTime(&lastWriteTime, &lastWriteFiletime);

    FILETIME createFiletime;
    SystemTimeToFileTime(&createTime, &createFiletime);

    FILETIME lastAccessFileTime;
    SystemTimeToFileTime(&lastAccessTime, &lastAccessFileTime);
    HANDLE filename;
    if(info.attrib==16&&info.attrib==info_des.attrib)
    {
       //ilename =  CreateFileA(des, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS, NULL); 
        filename =  CreateFileA(des, GENERIC_READ | GENERIC_WRITE,
                               FILE_SHARE_READ | FILE_SHARE_WRITE,
                               NULL, OPEN_EXISTING,
                               FILE_FLAG_OPEN_REPARSE_POINT | FILE_FLAG_BACKUP_SEMANTICS, NULL);
    }
    else if(info.attrib==1040||info.attrib==16)
    {
        filename =  CreateFileA(des, GENERIC_READ | GENERIC_WRITE,
                               FILE_SHARE_READ | FILE_SHARE_WRITE,
                               NULL, OPEN_EXISTING,
                               FILE_FLAG_OPEN_REPARSE_POINT | FILE_FLAG_BACKUP_SEMANTICS, NULL);
        if (filename==INVALID_HANDLE_VALUE)
            cout<<"a"<<endl;
    }
    else
    {
        filename = CreateFileA(des, FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        
    }
    bool a=SetFileTime(filename, &createFiletime, &lastAccessFileTime, &lastWriteFiletime);
    if(a==false)
    {
        return false;
    }
    CloseHandle(filename);
    return true;
}