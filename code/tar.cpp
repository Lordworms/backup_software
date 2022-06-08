#include"tar.h"
tar_unit::tar_unit()
{

}
int tar_unit::tar(const char* filename)
{
    FILE* file;
    char out_name[MAX];
    if(strcmp(filename,".")!=0)//是个文件夹
    {
        strcpy(out_name,filename);
        strcat(out_name,"\\");
        strcat(out_name,".tar");
        strcpy(tar_file_name,".tar");
        file=fopen(out_name,"wb");
        tar_dir(filename,file,"");
    }
    fclose(file);
    return 0;
}
int tar_unit::tar_dir(const char* filename,FILE* file,const char* prefix)
{
    if(strlen(filename)==0)
    {
        return 0;
    }
    char *str = (char*)"\\*.*";
    char dir[MAX]={0};
    char now_dir[MAX]={0};
    strcpy(dir,filename);
    strcat(dir,str);
    _finddata_t fileinfo;
    long hFile;
    posix_tar_header tmp_header;
    const char* c;
    
    if((hFile = _findfirst(dir,&fileinfo))!=-1)
    {   
        do
        {
            strcpy(now_dir,filename);
            strcat(now_dir,"\\");
            strcat(now_dir,fileinfo.name);
            if(!(fileinfo.attrib&_A_SUBDIR)&&strcmp(tar_file_name,fileinfo.name)!=0&&strcmp(fileinfo.name,".tar")!=0)//是个文件，开始写
            {
                strcpy(tmp_header.name,prefix);
                if(prefix!="")
                {
                    strcat(tmp_header.name,"\\");
                }
                strcat(tmp_header.name,fileinfo.name);
                itoa(fileinfo.size,tmp_header.size,10);
                itoa(0,tmp_header.uid,10);//文件的uid=0;
                if(check_hard(now_dir,NULL)!="")//是硬链接
                {
                    itoa(HARD,tmp_header.mode,10);
                    strcpy(tmp_header.linkname,check_hard(now_dir,NULL).c_str());
                }
                else
                {
                    itoa(FIL_E,tmp_header.mode,10);
                    strcpy(tmp_header.linkname,now_dir);
                }
                fwrite(&tmp_header,sizeof(tmp_header),1,file);
                
                if(check_hard(now_dir,NULL)=="")//是个正经文件
                {
                    tar_file(now_dir,file,fileinfo.size);
                }
            }
            else
            { 
                if(strcmp(fileinfo.name, "." ) != 0 && strcmp(fileinfo.name, ".." ) != 0&&strcmp(tar_file_name,fileinfo.name)!=0)//是个目录，先建立目录的记录，再开始递归打包目录内的文件
                {
                    strcpy(tmp_header.name,prefix);
                    strcat(tmp_header.name,"\\");
                    strcat(tmp_header.name,fileinfo.name);
                    itoa(0,tmp_header.size,10);
                    if(fileinfo.attrib!=16)//软链接
                    {
                        itoa(SYMBOLIC,tmp_header.mode,10);
                        
                        strcpy(tmp_header.linkname,get_junc_des(now_dir).c_str());
                    }
                    else
                    {
                        itoa(DIR,tmp_header.mode,10);
                        strcpy(tmp_header.linkname,now_dir);
                    }
                    fwrite(&tmp_header,sizeof(tmp_header),1,file);
                    if(fileinfo.attrib==16)
                    {//正经文件夹，不是软链接
                        
                        char prefixx[MAX]={0};
                        strcpy(prefixx,prefix);
                        strcat(prefixx,"\\");
                        strcat(prefixx,fileinfo.name);
                        tar_dir(now_dir,file,prefixx);
                    }
                    
                }
            }

        }while(_findnext(hFile,&fileinfo)==0);
        _findclose(hFile);
    }
    return 0;
}
int tar_unit::tar_file(const char* filename,FILE*file,int size)
{
    FILE* infile;
    infile=fopen(filename,"rb");
    while(size>512)
    {
        fread(&buffer,512,1,infile);
        fwrite(&buffer,512,1,file);
        size-=512;
    }
    memset(buffer,0xff,sizeof(buffer));
    fread(&buffer,size,1,infile);
    fwrite(&buffer,512,1,file);
    return 0;
}
//存了一个name(+prefix) size没了
int tar_unit::untar(const char* filename,const char* desname)
{
   FILE* infile;
   infile=fopen(filename,"rb");
   posix_tar_header tmp;
   fread(&tmp,512,1,infile);
   char tmp_des[MAX];
   char last_name[MAX];
   int size;
   int last=0;//表示是不是多层嵌套
   while(!feof(infile))
   {
       strcpy(tmp_des,desname);
       if(atoi(tmp.mode)==DIR||atoi(tmp.mode)==SYMBOLIC)//表示是个文件夹,分类讨论是否是软链接或者真正的文件夹
       {
           strcat(tmp_des,"\\");
           strcat(tmp_des,tmp.name);
           if(atoi(tmp.mode)==SYMBOLIC)
           {
               create_symbolic(tmp.linkname,tmp_des);
               change_file_time(tmp.linkname,tmp_des);
           }
           else
           {
                
                mkdir(tmp_des);
                change_file_time(tmp.linkname,tmp_des);
           }
       }
       else//是个文件
       {
           strcat(tmp_des,"\\");
           strcat(tmp_des,tmp.name);
           if(atoi(tmp.mode)==HARD)//硬链接，创建一个新的
           {
               char linkname[256]={0};
               strcpy(linkname,tmp.linkname);
               char order[256]="mklink /H ";
               strcat(order,tmp_des);
               strcat(order," ");
               strcat(order,linkname);
               cout<<order<<endl;
               cmdPopen(order);
               change_file_time(tmp.linkname,tmp_des);
           }
           else{
           size=atoi(tmp.size);
           FILE* outfile;
           outfile=fopen(tmp_des,"wb");
           while(size>0)
           {
               fread(&buffer,BLOCK_SIZE,1,infile);
               if(size>BLOCK_SIZE)
               {
                  fwrite(&buffer,BLOCK_SIZE,1,outfile);
               }
                else
                    {
                        fwrite(&buffer,size,1,outfile);

                    }
                size-=BLOCK_SIZE;
            }
            fclose(outfile);
            change_file_time(tmp.linkname,tmp_des);
        }
           
       }
       fread(&tmp,512,1,infile);
    }
    
    fclose(infile);
    return 0;
}
