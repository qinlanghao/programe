#include<stdio.h>
#include "s_box-xor-left_move-generate_key-message_encryption.h"
typedef unsigned int unint32;  //重命名 unsigned int 
typedef unsigned char unchar8; //重命名 unsigned char 
// Encrypt_Flag    加解密标志
// #define ENCRYPT 1   //加密标志  ENCRYPT
// #define DECRYPT 0   //解密标志  DECRYPT
int main()
{
    int choice,flag=1;
    unint32 *rk_array = NULL;
    unint32 *X_array = NULL;
    unint32 *reverse_X_array = NULL;
    unint32 temp;
    while (flag)
    {   
        puts("=============================================================\n");
        puts("                 SMS4算法加解密算法演示\n");
        puts("=============================================================\n");
        puts("=====================1.加密输入(1)===========================\n");
        puts("=====================2.解密输入(2)===========================\n");
        puts("=====================3.退出程序(3)===========================\n");
        puts("=============================================================\n");
        if (scanf("%d", &choice) != 1) 
        {
        // 清除错误的输入
            while (getchar() != '\n') continue;
            printf("请输入数字\n");
            continue;
        }

        switch (choice)
        {
        case 1:
            rk_array = generate_key(ENCRYPT);   //调用生成密钥函数
            X_array = message_encryption(rk_array,ENCRYPT,NULL); //调用明文加密函数
            printf("密文:");
            for (int i = 0; i < 4; i++)
            {
                printf("%08X ",X_array[i]);
                /* code */
            }
            printf("\n");
            break;
        case 2:
            // 反转密钥顺序以便用于解密
            if (rk_array == NULL || X_array == NULL) 
            {
                printf("请先进行加密操作(输入1)再进行解密。\n");
                break;
            }
            for (int i = 0; i < 16; i++)
            {
                temp = rk_array[i];
                rk_array[i] = rk_array[31 - i];
                rk_array[31 - i] = temp;
            }
            reverse_X_array = message_encryption(rk_array,DECRYPT,X_array); // 解密调用
            printf("解密明文:");
            for (int i = 0; i < 4; i++)
            {
                printf("%08X ", reverse_X_array[i]);
            }
            printf("\n");
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