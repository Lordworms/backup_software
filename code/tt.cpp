#include <windows.h>
#include <fstream>
#include <iostream>
#include<time.h>
#include"backup.h"
using namespace std;

int main() {
    
    std::cout << "A new project." << endl;

    //creates a fie as varuntest.txt
    //ofstream file("D:\\2021_software_class\\code\\test", ios::app);
    
    SYSTEMTIME createTime;
    GetSystemTime(&createTime);
    createTime.wDay = 3;            //changes the day
    createTime.wMonth = 01;         //changes the month
    createTime.wYear = 2021;        //changes the year
    createTime.wHour = 1;           //changes the hour
    createTime.wMinute = 1;         //changes the minute
    createTime.wSecond = 7;         //changes the second

    SYSTEMTIME lastWriteTime;
    GetSystemTime(&lastWriteTime);
    lastWriteTime.wDay = 07;        //changes the day
    lastWriteTime.wMonth = 04;      //changes the month
    lastWriteTime.wYear = 2012;     //changes the year
    lastWriteTime.wHour = 1;        //changes the hour
    lastWriteTime.wMinute = 37;     //changes the minute
    lastWriteTime.wSecond = 23;     //changes the second

    SYSTEMTIME lastAccessTime;
    GetSystemTime(&lastAccessTime);
    lastAccessTime.wDay = 20;       //changes the day
    lastAccessTime.wMonth = 7;     //changes the month
    lastAccessTime.wYear = 2017;    //changes the year
    lastAccessTime.wHour = 1;      //changes the hour
    lastAccessTime.wMinute = 31;    //changes the minute
    lastAccessTime.wSecond = 8;     //changes the second

    //creation of a filetimestruct and convert our new systemtime
    FILETIME lastWriteFiletime;
    SystemTimeToFileTime(&lastWriteTime, &lastWriteFiletime);

    FILETIME createFiletime;
    SystemTimeToFileTime(&createTime, &createFiletime);

    FILETIME lastAccessFileTime;
    SystemTimeToFileTime(&lastAccessTime, &lastAccessFileTime);

    //getthe handle to the file
    //HANDLE filename = CreateFileA("D:\\2021_software_class\\code\\test\\2.txt", FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE filename = CreateFileA("D:\\2021_software_class\\test2\\source_symboliclink", GENERIC_READ | GENERIC_WRITE,
                               FILE_SHARE_READ | FILE_SHARE_WRITE,
                               NULL, OPEN_EXISTING,
                               FILE_FLAG_OPEN_REPARSE_POINT | FILE_FLAG_BACKUP_SEMANTICS, NULL);
    //set the filetime on the file
    //HANDLE filename = CreateFileA("D:\\2021_software_class\\code\\test", GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if(filename==INVALID_HANDLE_VALUE)
        cout<<"cao!!!";
    bool a=SetFileTime(filename, &createFiletime, &lastAccessFileTime, &lastWriteFiletime);
    cout<<a<<endl;
    cout<<"failed "<<GetLastError()<<endl;
    //close our handle.
    CloseHandle(filename);
    
   
    _finddata_t info;
    long flag=_findfirst("D:\\2021_software_class\\test2\\source_symboliclink",&info);
    cout<<info.name<<endl;
    cout<<info.attrib<<endl;
    cout<<info.size<<endl;
    cout<<info.time_create<<endl;
    cout<<info.time_access<<endl;
    cout<<info.time_write<<endl;//time_t64
    tm s;
    localtime_s(&s,&info.time_create);
    cout<<s.tm_hour<<endl;//小时
    cout<<s.tm_isdst<<endl;
    cout<<s.tm_mday<<endl;//日
    cout<<s.tm_min<<endl;//分钟
    cout<<"month: "<<s.tm_mon<<endl;//
    cout<<s.tm_sec<<endl;//秒
    cout<<s.tm_wday<<endl;
    cout<<s.tm_year<<endl;//年份-1900
    return 0;

}