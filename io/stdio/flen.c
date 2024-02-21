unsigned long get_file_size(const char *filename)  
{  
    unsigned long size;   // 声明一个变量size，表示文件的大小 
    FILE* fp = fopen( filename, "rb" );      // 以只读方式打开文件，指针fp指向文件开头
    if(fp==NULL)          // 如果打开文件失败，则输出错误信息，返回0
    {  
        printf("ERROR: Open file [%s] failed.\n", filename);  
        return 0;  
    }  
    fseek( fp,0 , SEEK_END );        // 将指针fp指向文件末尾
    size=ftell(fp);       // 获取指针fp当前位置相对于文件开头的偏移量，即文件大小
    fclose(fp);           // 关闭文件
    return size;          // 返回文件大小
}
