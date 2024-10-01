#include<stdio.h>
#include "s_box-xor-left_move-generate_key-message_encryption.h"
typedef unsigned int unint32;  //重命名 unsigned int 
typedef unsigned char unchar8; //重命名 unsigned char 


int main()
{
    unint32 *rk_array = generate_key();   //调用生成密钥函数
    unint32 *reverse_X_array = message_encryption(rk_array); //调用明文加密函数

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

    for (int i = 0; i < 4; i++)
    {
        printf("密文:%08X\n",reverse_X_array[i]);
        /* code */
    }
    
    return 0;
}