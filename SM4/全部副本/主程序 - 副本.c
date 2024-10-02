#include<stdio.h>
#include "s_box-xor-left_move-generate_key-message_encryption.h"
typedef unsigned int unint32;  //重命名 unsigned int 
typedef unsigned char unchar8; //重命名 unsigned char 


int main()
{
    int choice,flag=1;
    unint32 *rk_array;
    unint32 *reverse_X_array;
    while (flag)
    {   
        puts("=============================================================\n");
        puts("                 SMS4算法加解密算法演示\n");
        puts("=============================================================\n");
        puts("=====================1.加密输入(1)===========================\n");
        puts("=====================2.解密输入(2)===========================\n");
        puts("=====================3.退出程序(3)===========================\n");
        puts("=============================================================\n");
        scanf("%d",&choice);
        switch (choice)
        {
        case 1:
            rk_array = generate_key();   //调用生成密钥函数
            reverse_X_array = message_encryption(rk_array); //调用明文加密函数
            printf("密文:");
            for (int i = 0; i < 4; i++)
            {
                printf("%08X ",reverse_X_array[i]);
                /* code */
            }
            printf("\n");
            break;
        case 2:
            printf("解密程序\n");
            break;
        case 3:
            printf("退出程序\n");
            flag = 0;
            break;
        default:
            printf("输入有误请重新输入\n");
            break;
        }

    }
    
    // for (int i = 0; i < 32; i++)
    // {
    //     printf("rk_array[%d]%08X\n",i,rk_array[i]);   //测试打印32次子密钥 rk[i] 测试成功
    //     /* code */
    // }
    
    // for (int i = 0; i < 32; i++)
    // {
    //     printf("X_array[%d]:%08X\n",i+4,X_array[i+4]);   //测试打印32次密文输出 X_array[i] 测试成功
    //     /* code */
    // }
    
    return 0;
}