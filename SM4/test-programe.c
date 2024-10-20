#include<stdio.h>
#include<string.h>

void fileRead()
{
    //以只读的方式打开文件，文件必须存在，否则会报错
    FILE *fp = fopen("./touch.txt","r");
    if (fp == NULL)
    {
        perror("open file failed:");
        return;
        /* code */
    }
    
    //读取文件
    while (!feof(fp))
    {
        char ch = fgetc(fp);
        printf("%c",ch);   
        /* code */
    }
    fclose(fp); //关闭文件
}
void fileWrite()
{
    char *p = "你好，我是坏人";
    FILE *fp = fopen("./touch.txt","w");

    for (int i = 0; i < strlen(p); i++)
    {
        char ch = fputc(p[i],fp);
        printf("%3c ",ch);
        /* code */
    }
    
    fclose(fp);
}
void fileReadWrite()
{
    FILE *fp = fopen("./touch.txt","w+");
    if (fp == NULL)
    {
        perror("the failed:");
        return;
        /* code */
    }
    fputs("今天你吃饭了吗？",fp);
    rewind(fp);
    char str[128] = {0};
    fgets(str,sizeof(str),fp);
    printf("%s",str);
    
    fclose(fp);
}
void fileFormatRead()
{
    FILE *fp = fopen("./touch.txt","r");
    if (fp ==NULL)
    {
        perror("the file failed:");
        /* code */
    }
    

    fclose(fp);
}
void fileFormatWrite()
{
    
}





int main()
{
    // fileRead();
    // fileWrite();
    // fileReadWrite();
    // fileFormatRead();
    fileFormatWrite();
    return 0;
}