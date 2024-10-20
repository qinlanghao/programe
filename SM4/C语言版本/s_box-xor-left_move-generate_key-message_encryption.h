#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // 用于获取 unint32 的最大值
// Encrypt_Flag    加解密标志
#define ENCRYPT 1   //加密标志  ENCRYPT
#define DECRYPT 0   //解密标志  DECRYPT
typedef unsigned int unint32;  //重命名 unsigned int 
typedef unsigned char unchar8; //重命名 unsigned char 
// 定义S盒作为一个一维数组，输入0-255直接查表获取输出
const unchar8 Sbox[256] = {
0xd6,0x90,0xe9,0xfe,0xcc,0xe1,0x3d,0xb7,0x16,0xb6,0x14,0xc2,0x28,0xfb,0x2c,0x05,
0x2b,0x67,0x9a,0x76,0x2a,0xbe,0x04,0xc3,0xaa,0x44,0x13,0x26,0x49,0x86,0x06,0x99,
0x9c,0x42,0x50,0xf4,0x91,0xef,0x98,0x7a,0x33,0x54,0x0b,0x43,0xed,0xcf,0xac,0x62,
0xe4,0xb3,0x1c,0xa9,0xc9,0x08,0xe8,0x95,0x80,0xdf,0x94,0xfa,0x75,0x8f,0x3f,0xa6,
0x47,0x07,0xa7,0xfc,0xf3,0x73,0x17,0xba,0x83,0x59,0x3c,0x19,0xe6,0x85,0x4f,0xa8,
0x68,0x6b,0x81,0xb2,0x71,0x64,0xda,0x8b,0xf8,0xeb,0x0f,0x4b,0x70,0x56,0x9d,0x35,
0x1e,0x24,0x0e,0x5e,0x63,0x58,0xd1,0xa2,0x25,0x22,0x7c,0x3b,0x01,0x21,0x78,0x87,
0xd4,0x00,0x46,0x57,0x9f,0xd3,0x27,0x52,0x4c,0x36,0x02,0xe7,0xa0,0xc4,0xc8,0x9e,
0xea,0xbf,0x8a,0xd2,0x40,0xc7,0x38,0xb5,0xa3,0xf7,0xf2,0xce,0xf9,0x61,0x15,0xa1,
0xe0,0xae,0x5d,0xa4,0x9b,0x34,0x1a,0x55,0xad,0x93,0x32,0x30,0xf5,0x8c,0xb1,0xe3,
0x1d,0xf6,0xe2,0x2e,0x82,0x66,0xca,0x60,0xc0,0x29,0x23,0xab,0x0d,0x53,0x4e,0x6f,
0xd5,0xdb,0x37,0x45,0xde,0xfd,0x8e,0x2f,0x03,0xff,0x6a,0x72,0x6d,0x6c,0x5b,0x51,
0x8d,0x1b,0xaf,0x92,0xbb,0xdd,0xbc,0x7f,0x11,0xd9,0x5c,0x41,0x1f,0x10,0x5a,0xd8,
0x0a,0xc1,0x31,0x88,0xa5,0xcd,0x7b,0xbd,0x2d,0x74,0xd0,0x12,0xb8,0xe5,0xb4,0xb0,
0x89,0x69,0x97,0x4a,0x0c,0x96,0x77,0x7e,0x65,0xb9,0xf1,0x09,0xc5,0x6e,0xc6,0x84,
0x18,0xf0,0x7d,0xec,0x3a,0xdc,0x4d,0x20,0x79,0xee,0x5f,0x3e,0xd7,0xcb,0x39,0x48
};

// S盒变换函数，输入2位16进制数，返回Sbox[index]
unchar8 Sbox_lookup(unchar8 input) 
{
    unchar8 row = (input & 0xF0) >> 4; //取高4位作为行号
    unchar8 col = (input & 0x0F); //取低4位作为列号
    unchar8 index = 16 * row + col ;//index下标计算公式，（16×行号）+列号
    return Sbox[index]; //返回Sbox数组里面下标位index的数据
}

// 定义一个异或函数，使用 unint32 作为参数类型，返回两个数据异或的结果
unint32 xor_funtion(unint32 a, unint32 b) 
{
    return a ^ b;
}

// 循环左移函数，输入一个用于移位的32位整数，另一个是用于定义移动多少位的32为整数（移位不能超过32次） （测试正常）
unint32 cyclic_shift(unint32 a,unint32 b)
{
    b %=32;
    return (a << b ) | (a >> (32-b));
}

// 系统参数 FK
static const unint32 FK[4] = 
{
    0xa3b1bac6,
    0x56aa3350,
    0x677d9197,
    0xb27022dc
};

// 系统参数 CK
static const unint32 CK[32] = 
{
    0x00070e15, 0x1c232a31, 0x383f464d, 0x545b6269,
    0x70777e85, 0x8c939aa1, 0xa8afb6bd, 0xc4cbd2d9,
    0xe0e7eef5, 0xfc030a11, 0x181f262d, 0x343b4249,
    0x50575e65, 0x6c737a81, 0x888f969d, 0xa4abb2b9,
    0xc0c7ced5, 0xdce3eaf1, 0xf8ff060d, 0x141b2229,
    0x30373e45, 0x4c535a61, 0x686f767d, 0x848b9299,
    0xa0a7aeb5, 0xbcc3cad1, 0xd8dfe6ed, 0xf4fb0209,
    0x10171e25, 0x2c333a41, 0x484f565d, 0x646b7279
};

//产生密钥过程
unint32* generate_key(unint32 Encrypt_Flag)
{
    char ch;  //用于检验Y/N
    const unchar8 key[16]; // 128位密钥
    unint32* rk_array = (unint32*)malloc(32 * sizeof(unint32)); // 动态分配内存
    if (rk_array == NULL) 
    {
    // 处理内存分配失败
        return NULL;
    }
    if (Encrypt_Flag)
    {
        int count = 0;
        int judge = 1;
        while (judge)
        {
            printf("请输入初始密钥MK(128bit)\n");
            for (int i = 0; i < 16; i++)
            {
            if (scanf("%02hhX", &key[i]) == 1)
            {
                count++; // 成功读取一个字符，计数器加1
            }
            else
            {
                while (getchar() != '\n') continue; // 清除错误的输入
                printf("请重新输入\n");
                count = 0; // 重置计数器
                break; // 跳出循环，让用户重新输入
            }
            }
            if (count == 16) // 如果成功读取了16个字符
            {
                judge = 0; // 设置judge为0，退出循环
            }
        }
    // 将密钥分为4个32位的部分
        unint32 part0 = ((unint32)key[0] << 24) | ((unint32)key[1] << 16) | ((unint32)key[2] << 8) | key[3];
        unint32 part1 = ((unint32)key[4] << 24) | ((unint32)key[5] << 16) | ((unint32)key[6] << 8) | key[7];
        unint32 part2 = ((unint32)key[8] << 24) | ((unint32)key[9] << 16) | ((unint32)key[10] << 8) | key[11];
        unint32 part3 = ((unint32)key[12] << 24) | ((unint32)key[13] << 16) | ((unint32)key[14] << 8) | key[15];
        printf("初始密钥MK以及K0 K1 K2 K3的值\n");     //测试输出4个32位的部分
        printf("MK=");
        for (int i = 0; i < 16; i++)
        {
            printf("%02hhX",key[i]);
        }
        printf("\n");
        printf("K0=0x%08X K1=0x%08X K2=0x%08X K3=0x%08X\n",part0,part1,part2,part3);
        printf("是否生成拓展密钥 Y/N\n");
        scanf(" %c",&ch);
        unint32 k0 = xor_funtion(part0,FK[0]);  //获得k0
        unint32 k1 = xor_funtion(part1,FK[1]);  //获得k1
        unint32 k2 = xor_funtion(part2,FK[2]);  //获得k2
        unint32 k3 = xor_funtion(part3,FK[3]);  //获得k3
        unint32 K_array[36] = {k0,k1,k2,k3}; //获得初始的4个k值
        if (ch == 'y' || ch == 'Y') 
        {
            for (int i = 0; i < 32; i++)
            {
            //第一步 将传入的数组首先和ck进行异或得到一个新的32bit的数据
            unint32 first_step = xor_funtion((xor_funtion((xor_funtion(K_array[i+1],K_array[i+2])),K_array[i+3])),CK[i]);
            //第二步 将这个新的32bit分为4个8bit      T函数
            unchar8 a0 = (unchar8)(first_step >> 24);
            unchar8 a1 = (unchar8)((first_step >> 16)&0xFF);
            unchar8 a2 = (unchar8)((first_step >> 8)&0xFF);
            unchar8 a3 = (unchar8)(first_step & 0xFF);
            //第三步 将4个8bit放入S盒中进行置换
            unchar8 b0 = Sbox_lookup(a0);
            unchar8 b1 = Sbox_lookup(a1); 
            unchar8 b2 = Sbox_lookup(a2);
            unchar8 b3 = Sbox_lookup(a3);
            //第四步 将置换后的数据重新合并位32bit 并进行移位和异或操作 
            unint32 B = ((unint32)b0 << 24) | // 将byte0放置在最高8位
                        ((unint32)b1 << 16) | // 将byte1放置在次高8位
                        ((unint32)b2 << 8)  | // 将byte2放置在次低8位
                        (unint32)b3;
            unint32 B_left = cyclic_shift(B,13);   //将B向左移13位
            unint32 B_right = cyclic_shift(B,23);  //将B向左移23位01
            unint32 k = xor_funtion((xor_funtion(xor_funtion(B,B_left),B_right)),K_array[i]);  //得到子密钥rk i
            K_array[i+4] = k;  //进行数据存储
            rk_array[i] = K_array[i+4];   //进行数据存储
            }

            for (int i = 0; i < 32; i++)
            {
                printf("rk_array[%d]%08X\n",i,rk_array[i]);   //测试打印32次子密钥 rk[i] 测试成功
            }
        } 
        else 
        {
            exit(0);
        }

    }
    return rk_array;  //返回最终的结果
    
}

//明文加密过程
unint32* message_encryption(unint32 rk_array[32],unint32 Encrypt_Flag,unint32 *old_X_array)
{
    char ch;
    unchar8 message[16];
    unint32* X_array = (unint32*)malloc(36 * sizeof(unint32)); // 动态分配内存
    if (Encrypt_Flag)
    {   
        int count = 0;
        int judge = 1;
        while (judge)
        {
            printf("请输入明文分组M(128bit)\n");
            for (int i = 0; i < 16; i++) 
            {
            if (scanf("%02hhX", &message[i]) == 1)
            {
                count++; // 成功读取一个字符，计数器加1
            }
            else
            {
                while (getchar() != '\n') continue; // 清除错误的输入
                printf("请重新输入\n");
                count = 0; // 重置计数器
                break; // 跳出循环，让用户重新输入
            }
            }
            if (count == 16) // 如果成功读取了16个字符
            {
                judge = 0; // 设置judge为0，退出循环
            }
        }
        // 将明文分为4个32位的部分
        unint32 part_0 = ((unint32)message[0] << 24) | ((unint32)message[1] << 16) | ((unint32)message[2] << 8) | message[3];
        unint32 part_1 = ((unint32)message[4] << 24) | ((unint32)message[5] << 16) | ((unint32)message[6] << 8) | message[7];
        unint32 part_2 = ((unint32)message[8] << 24) | ((unint32)message[9] << 16) | ((unint32)message[10] << 8) | message[11];
        unint32 part_3 = ((unint32)message[12] << 24) | ((unint32)message[13] << 16) | ((unint32)message[14] << 8) | message[15];
        printf("明文分组M以及X0,X1,X2,X3的值\n");
        printf("M=");
        for (int i = 0; i < 16; i++)
        {
            printf("%02hhX",message[i]);
        }
        printf("\n");
        printf("X0=0x%08X X1=0x%08X X2=0x%08X X3=0x%08X\n",part_0,part_1,part_2,part_3);
        if (X_array == NULL) 
            {
            // 处理内存分配失败
                return NULL;
            }
        unint32 part[4] = {part_0,part_1,part_2,part_3}; //获得初始的4个X值
        for (int i = 0; i < 4; i++)
            {
                X_array[i]=part[i];
            }
    }
    else
    {
        for (int i = 0; i < 4; i++)
        {
            X_array[i] = old_X_array[i];
        }
        
    }
    
    
    for (int i = 0; i < 32; i++)
    {   
        //轮函数部分
        unint32 first_step = xor_funtion((xor_funtion((xor_funtion(X_array[i+1],X_array[i+2])),X_array[i+3])),rk_array[i]);
        //第一步 将传入的数组首先和rk子密钥进行异或得到一个新的32bit的数据
        //第二步 将新的32bit数据分为4部分
        unchar8 a0 = (unchar8)(first_step >> 24);
        unchar8 a1 = (unchar8)((first_step >> 16)&0xFF);
        unchar8 a2 = (unchar8)((first_step >> 8)&0xFF);
        unchar8 a3 = (unchar8)(first_step & 0xFF);
        //第三步 将第二步的4部分放入S盒进行非线性变换
        unchar8 b0 = Sbox_lookup(a0);
        unchar8 b1 = Sbox_lookup(a1);
        unchar8 b2 = Sbox_lookup(a2);
        unchar8 b3 = Sbox_lookup(a3);
        //第四步 将置换后的数据重新合并位32bit 并进行移位和异或操作 
        unint32 B = ((unint32)b0 << 24) | // 将byte0放置在最高8位
                    ((unint32)b1 << 16) | // 将byte1放置在次高8位
                    ((unint32)b2 << 8)  | // 将byte2放置在次低8位
                    (unint32)b3;  
        // cyclic_shift(B,2);
        // cyclic_shift(B,10);
        // cyclic_shift(B,18);
        // cyclic_shift(B,24);
        unint32 C = xor_funtion(xor_funtion(xor_funtion(xor_funtion(B,cyclic_shift(B,2)),cyclic_shift(B,10)),cyclic_shift(B,18)),cyclic_shift(B,24));
        unint32 X = xor_funtion(C,X_array[i]);
        X_array[i+4] = X;
        if(i==0 && Encrypt_Flag == 1)
        {
            char ch;
            printf("计算由X1,X2,X3及第1个拓展密钥rk0生成的中间值U? Y/N\n");
            scanf(" %c",&ch);
            if (ch == 'Y' || ch == 'y')
            {
                printf("中间值U以及u0,u1,u2,u3的值\n");
                printf("U=0x%08X u0=0x%02X u1=0x%02X u2=0x%02X u3=0x%02X\n",first_step,a0,a1,a2,a3);
                printf("计算由U通过非线性τ变换后的中间值V? Y/N\n");
                scanf(" %c",&ch);
                if (ch == 'Y' || ch == 'y')
                {
                    printf("中间值V以及v0,v1,v2,v3的值\n");
                    printf("V=0x%08X v0=0x%02X v1=0x%02X v2=0x%02X v3=0x%02X\n",B,b0,b1,b2,b3);
                    printf("计算由中间值V通过线性L变换得到的中间值W? Y/N\n");
                    scanf(" %c",&ch);
                    if (ch == 'Y' || ch == 'y')
                    {
                        printf("中间值W=0x%08X\n",C);
                        printf("计算由X0和中间值W生成的X4的值? Y/N\n");
                        scanf(" %c",&ch);
                        if (ch == 'Y' || ch == 'y')
                        {
                            printf("第1轮X4的结果为:X4=0x%08X\n",X);
                            printf("计算第2-32轮的结果? Y/N\n");
                            scanf(" %c",&ch);
                            if (ch == 'Y' || ch == 'y')
                            {}
                            else
                                exit(0);
                            
                        }
                        else
                            exit(0);
                    }
                    else   
                        exit(0);
                    
                }
                else
                    exit(0);
                
            }
            else
                exit(0);
            
           
        }
        
    }
    if (Encrypt_Flag == 1)
    {
        for (int i = 0; i < 32; i++)
        {
            printf("X_array[%d]:%08X\n",i+4,X_array[i+4]);   //测试打印32次密文输出 X_array[i] 测试成功
        }
        printf("计算由X32,X33,X34,X35反序变换的结果? Y/N\n");
        
        scanf(" %c",&ch);
        if (ch == 'Y' || ch == 'y')
        {
        printf("Y0=0x%08X Y1=0x%08X Y2=0x%08X Y3=0x%08X\n",X_array[35],X_array[34],X_array[33],X_array[32]);
        printf("密文分组C=");
        }
        else
            exit(0);
    }
    // 反序变换
        unint32 *reverse_X_array =(unint32*)malloc(4 * sizeof(unint32));
        if (X_array == NULL) 
        {
        // 处理内存分配失败
            return NULL;
        }
        for (int i = 0; i < 4; i++)
        {
            reverse_X_array[i] = X_array[35-i];
            if (Encrypt_Flag == 1)
            {
                printf("%08X ",reverse_X_array[i]);
            }
        }
        printf("\n");
    return reverse_X_array; //返回反序值 共4个 每个32bit 总计128bit
}
//主程序
// int main() {
//     // printf("input=%x\n",input);   测试输出代码  （正常）
//     // printf("output=%x\n",output);  测试输出代码  （正常）

//     unint32 a = xor_funtion(3,5);           测试异或函数输出  （正常）
//     printf("%x",a);                         测试异或函数输出  （正常） a = 6;

//     // unint32 c = cyclic_shift(1,1);   测试循环移位函数输出    （正常）
//     // printf("%x",c);                  测试循环移位函数输出    （正常）

//     unchar8 input; // 定义一个无符号的字符型数据，可以存储0-255 char类型适合存储ASCII字符以及二进制数据
//     printf("请输入2位16进制数:");
//     scanf("%X",&input); //%X表示以16进制接受或输出变量 
//     unchar8 output = Sbox_lookup(input);//将输入值放入转换下标函数，并使用output变量接受返回值   S盒测试输出 （正常）
//     printf("S盒输入: 0x%X, 输出: 0x%X\n", input, output);                                     S盒测试输出 （正常）
//     return 0;
// }