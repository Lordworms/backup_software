#include"file.h"
file::file()
{
    for(size_t i=0;i<256;++i)
    {   
        char_256[i]=new count_union;
        char_256[i]->count=0;
        char_256[i]->val=(char)i;
        huf_code[i]=new huff_code;
    }
    
}
file::file(const char* n)
{
    for(size_t i=0;i<256;++i)
    {   
        char_256[i]=new count_union;
        char_256[i]->count=0;
        char_256[i]->val=(char)i;
        huf_code[i]=new huff_code;
    }
    this->name=n;
    frequency();
}
void file::frequency()//统计词频
{
    FILE* file;
    file=fopen(this->name,"rb");
    unsigned char tmp=fgetc(file);
    int index;
    
    while(!feof(file))
    {   
        bool c=(unsigned int)tmp<0;
        if(c==true)
        {
            index=(int)tmp+256;
        }
        else
        {
            index=(int)tmp;
        }
        char_256[index]->count++;
        tmp=fgetc(file);
        file_count++;
    }
    fclose(file);
    huffman_tree* huf=new huffman_tree(char_256,256);//文件词频的哈夫曼树建立
    huf->print();
    this->huf=huf;
    string s;
    huffcode_gen(this->huf->root,s);
}
void file::print()
{   
    int t=0;
    
    for(size_t i=0;i<256;++i)
    {
        if(huf_code[i]->flag)
        {
            bitset<9>tmp(i);
            cout<<tmp.to_string()<<" : "<<huf_code[i]->code<<endl;
           
        }
    }
    
}
bool file::huffcode_gen(node* now,string par)//遍历文件的哈夫曼树，得到哈夫曼编码,true表示是右边，false表示是左边
{
    if(now->flag==true)//是字符节点
    {
        huf_code[now->name]->flag=true;
        huf_code[now->name]->code=par;
        return true;
    }
    if(now->left != NULL)
    {   
        
        huffcode_gen(now->left,(par+'0'));
    }
    if(now->right!=NULL)
    {
        huffcode_gen(now->right,(par+'1'));
    }
    return true;    

}
bool file::compress()
{
    if(this->name==NULL)
    {
        return false;
    }
    
    char* des_name=new char;
    strcpy(des_name,(char*)this->name);
    strcat(des_name,".compressed");
    FILE* infile;
    FILE* outfile;
    
    outfile=fopen(des_name,"wb");
    int word_count=file_count;
    fwrite(&word_count,sizeof(int),1,outfile);
    //文件的组织形式为字符:压缩编码
    //将编码写入压缩文件
    count_union tmp_cou;
    int a=0;
    for(int i=0;i<256;++i)
    {
        if(char_256[i]->count>0)
        {
            ++a;
            tmp_cou.count=char_256[i]->count;
            tmp_cou.val=char_256[i]->val;
            fwrite(&tmp_cou,sizeof(tmp_cou),1,outfile);
        }
    }
    cout<<"all:"<<a;
    tmp_cou.count=0;
    fwrite(&tmp_cou,sizeof(tmp_cou),1,outfile);
    //重新读原文件，进行压缩
    infile=fopen(this->name,"rb");
    unsigned char read_char=fgetc(infile);
    int index;
    int pos=0;
    int kk=0;
    char data_in=0x00;
    while(!feof(infile))
    {   
        //cout<<read_char<<endl;
        index=(int)read_char;
        for(int i=0;i<huf_code[index]->code.size();++i)
        {
            data_in=data_in<<1;
            data_in|=huf_code[index]->code[i]-'0';
            pos++;
            if(pos==8)
            {
                fputc(data_in,outfile);
                pos=0;
                data_in=0x00;
                
            }
        }
        read_char=fgetc(infile);
        ++kk;  
    }
    
    if(pos>0){
        data_in=data_in<<(8-pos);
        cout<<pos<<endl;
        bitset<8>b(data_in);
        fputc(data_in,outfile);
        cout<<b.to_string()<<endl;
    }
    
    fclose(infile);
    fclose(outfile);
    return true;
}
bool file::uncompress(const char* name)
{
    if(strstr(name,".compressed")==NULL)
    {
        cout<<"invalid file name"<<endl;
        return false;
    }
    FILE* file;
    file=fopen(name,"rb");
    int file_words_count=0;
    fread(&file_words_count,sizeof(int),1,file);
    count_union tmp;
    fread(&tmp,sizeof(tmp),1,file);
    while(tmp.count!=0)
    {   
        this->char_256[tmp.val]->count=tmp.count;
        fread(&tmp,sizeof(tmp),1,file);
    }
    huffman_tree* huf=new huffman_tree(char_256,256);
    FILE* outfile;
    char des[MAX]={0};
    strcpy(des,name);
    strcat(des,".uncompressed");
    outfile=fopen(des,"wb");
    string previous(name);
    int pos=7;
    unsigned char pre=fgetc(file);
    node* cur=huf->root;
    while(!feof(file)&&file_words_count>0)
    {   
        for(char i=7;i>=0;i--)
        {
            if(cur->flag==true)//到数据点了
            {
                fputc(cur->name,outfile);
                cur=huf->root;
                file_words_count--;
                if(file_words_count==0)
                {
                    break;
                }
            }
            
                bitset<8>cur_a(1<<i);
                bitset<8>cur_b(pre);
                bitset<8>res=cur_a&cur_b;
                if(res.to_ullong()>0)
                {
                    cur=cur->right;
                }
                else
                {
                    cur=cur->left;
                }
            

        }
        pre=fgetc(file);
        
    }
    fclose(outfile);
    return true;
}