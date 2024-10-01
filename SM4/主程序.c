#include<stdio.h>
#include "s_box-xor-left_move-generate_key.h"
typedef unsigned int unint32;  //重命名 unsigned int 
typedef unsigned char unchar8; //重命名 unsigned char 
// // 系统参数 FK
// static const unint32 FK[4] = {
//     0xa3b1bac6,
//     0x56aa3350,
//     0x677d9197,
//     0xb27022dc
// };
//     // 系统参数 CK
// static const unint32 CK[32] = {
//     0x00070e15, 0x1c232a31, 0x383f464d, 0x545b6269,
//     0x70777e85, 0x8c939aa1, 0xa8afb6bd, 0xc4cbd2d9,
//     0xe0e7eef5, 0xfc030a11, 0x181f262d, 0x343b4249,
//     0x50575e65, 0x6c737a81, 0x888f969d, 0xa4abb2b9,
//     0xc0c7ced5, 0xdce3eaf1, 0xf8ff060d, 0x141b2229,
//     0x30373e45, 0x4c535a61, 0x686f767d, 0x848b9299,
//     0xa0a7aeb5, 0xbcc3cad1, 0xd8dfe6ed, 0xf4fb0209,
//     0x10171e25, 0x2c333a41, 0x484f565d, 0x646b7279
// };
//     //产生密钥过程
// unint32* generate_key(unint32 K_array[36])
//     {
//         unint32 *rk_array = malloc(32 * sizeof(unint32));  //储存32个子密钥
//         for (int i = 0; i < 32; i++)
//         {
//             //第一步 将传入的数组首先和ck进行异或得到一个新的32bit的数据
//             unint32 first_step = xor_funtion((xor_funtion((xor_funtion(K_array[i+1],K_array[i+2])),K_array[i+3])),CK[i]);
//             //第二步 将这个新的32bit分为4个8bit      T函数
//             unchar8 a0 = (unchar8)(first_step >> 24);
//             unchar8 a1 = (unchar8)((first_step >> 16)&0xFF);
//             unchar8 a2 = (unchar8)((first_step >> 8)&0xFF);
//             unchar8 a3 = (unchar8)(first_step & 0xFF);
//             //第三步 将4个8bit放入S盒中进行置换
//             unchar8 b0 = Sbox_lookup(a0);
//             unchar8 b1 = Sbox_lookup(a1); 
//             unchar8 b2 = Sbox_lookup(a2);
//             unchar8 b3 = Sbox_lookup(a3);
//             //第四步 将置换后的数据重新合并位32bit 并进行移位和异或操作 
//             unint32 B = ((unint32)b0 << 24) | // 将byte0放置在最高8位
//                         ((unint32)b1 << 16) | // 将byte1放置在次高8位
//                         ((unint32)b2 << 8)  | // 将byte2放置在次低8位
//                         (unint32)b3;  
//             unint32 B_left = cyclic_shift(B,13);   //将B向左移13位
//             unint32 B_right = cyclic_shift(B,23);  //将B向左移23位01
//             unint32 k = xor_funtion((xor_funtion(xor_funtion(B,B_left),B_right)),K_array[i]);  //得到子密钥rk i
//             K_array[i+4] = k;  //进行数据存储
//             rk_array[i] = K_array[i+4];   //进行数据存储
//         }
//     return rk_array;  //返回最终的结果
//     }

int main()
{
    
    unchar8 key[16]; // 128位密钥
    printf("请输入一个128位的初始密钥（16组共32个十六进制数字）：\n");
    for (int i = 0; i < 16; i++) {
        scanf("%02hhX", &key[i]);
    }

    // 将密钥分为4个32位的部分
    unint32 part0 = ((unint32)key[0] << 24) | ((unint32)key[1] << 16) | ((unint32)key[2] << 8) | key[3];
    unint32 part1 = ((unint32)key[4] << 24) | ((unint32)key[5] << 16) | ((unint32)key[6] << 8) | key[7];
    unint32 part2 = ((unint32)key[8] << 24) | ((unint32)key[9] << 16) | ((unint32)key[10] << 8) | key[11];
    unint32 part3 = ((unint32)key[12] << 24) | ((unint32)key[13] << 16) | ((unint32)key[14] << 8) | key[15];

    /*
    printf("第一部分：0x%08X\n", part0);   测试输出4个32位的部分
    printf("第二部分：0x%08X\n", part1);   测试输出4个32位的部分
    printf("第三部分：0x%08X\n", part2);   测试输出4个32位的部分
    printf("第四部分：0x%08X\n", part3);   测试输出4个32位的部分
    */
    unint32 k0 = xor_funtion(part0,FK[0]);  //获得k0
    unint32 k1 = xor_funtion(part1,FK[1]);  //获得k1
    unint32 k2 = xor_funtion(part2,FK[2]);  //获得k2
    unint32 k3 = xor_funtion(part3,FK[3]);  //获得k3
    unint32 K_num[4] = {k0,k1,k2,k3};
    // for (int i = 0; i < 4; i++)
    // {
    //     printf("0x%08X\n",K_num[i]);  //测试K_num[4]输出是否正常 测试正常
    //     /* code */
    // }
    unint32 *rk_array = generate_key(K_num);
    // for (int i = 0; i < 32; i++)
    // {
    //     printf("%08X\n",rk_array[i]);   //测试打印32个子密钥 rk[i] 测试成功
    //     /* code */
    // }
    
    return 0;
}