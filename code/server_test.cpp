#include"client.h"
#include"server.h"
#include<iostream>
using namespace std;
extern int main_client(int,char*,char*);
extern int main_server(int,char*);

int main()
{
    main_server(2,(char*)"9999");
    return 0;
}