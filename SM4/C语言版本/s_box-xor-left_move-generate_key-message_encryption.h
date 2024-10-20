#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // ���ڻ�ȡ unint32 �����ֵ
// Encrypt_Flag    �ӽ��ܱ�־
#define ENCRYPT 1   //���ܱ�־  ENCRYPT
#define DECRYPT 0   //���ܱ�־  DECRYPT
typedef unsigned int unint32;  //������ unsigned int 
typedef unsigned char unchar8; //������ unsigned char 
// ����S����Ϊһ��һά���飬����0-255ֱ�Ӳ���ȡ���
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

// S�б任����������2λ16������������Sbox[index]
unchar8 Sbox_lookup(unchar8 input) 
{
    unchar8 row = (input & 0xF0) >> 4; //ȡ��4λ��Ϊ�к�
    unchar8 col = (input & 0x0F); //ȡ��4λ��Ϊ�к�
    unchar8 index = 16 * row + col ;//index�±���㹫ʽ����16���кţ�+�к�
    return Sbox[index]; //����Sbox���������±�λindex������
}

// ����һ���������ʹ�� unint32 ��Ϊ�������ͣ����������������Ľ��
unint32 xor_funtion(unint32 a, unint32 b) 
{
    return a ^ b;
}

// ѭ�����ƺ���������һ��������λ��32λ��������һ�������ڶ����ƶ�����λ��32Ϊ��������λ���ܳ���32�Σ� ������������
unint32 cyclic_shift(unint32 a,unint32 b)
{
    b %=32;
    return (a << b ) | (a >> (32-b));
}

// ϵͳ���� FK
static const unint32 FK[4] = 
{
    0xa3b1bac6,
    0x56aa3350,
    0x677d9197,
    0xb27022dc
};

// ϵͳ���� CK
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

//������Կ����
unint32* generate_key(unint32 Encrypt_Flag)
{
    char ch;  //���ڼ���Y/N
    const unchar8 key[16]; // 128λ��Կ
    unint32* rk_array = (unint32*)malloc(32 * sizeof(unint32)); // ��̬�����ڴ�
    if (rk_array == NULL) 
    {
    // �����ڴ����ʧ��
        return NULL;
    }
    if (Encrypt_Flag)
    {
        int count = 0;
        int judge = 1;
        while (judge)
        {
            printf("�������ʼ��ԿMK(128bit)\n");
            for (int i = 0; i < 16; i++)
            {
            if (scanf("%02hhX", &key[i]) == 1)
            {
                count++; // �ɹ���ȡһ���ַ�����������1
            }
            else
            {
                while (getchar() != '\n') continue; // ������������
                printf("����������\n");
                count = 0; // ���ü�����
                break; // ����ѭ�������û���������
            }
            }
            if (count == 16) // ����ɹ���ȡ��16���ַ�
            {
                judge = 0; // ����judgeΪ0���˳�ѭ��
            }
        }
    // ����Կ��Ϊ4��32λ�Ĳ���
        unint32 part0 = ((unint32)key[0] << 24) | ((unint32)key[1] << 16) | ((unint32)key[2] << 8) | key[3];
        unint32 part1 = ((unint32)key[4] << 24) | ((unint32)key[5] << 16) | ((unint32)key[6] << 8) | key[7];
        unint32 part2 = ((unint32)key[8] << 24) | ((unint32)key[9] << 16) | ((unint32)key[10] << 8) | key[11];
        unint32 part3 = ((unint32)key[12] << 24) | ((unint32)key[13] << 16) | ((unint32)key[14] << 8) | key[15];
        printf("��ʼ��ԿMK�Լ�K0 K1 K2 K3��ֵ\n");     //�������4��32λ�Ĳ���
        printf("MK=");
        for (int i = 0; i < 16; i++)
        {
            printf("%02hhX",key[i]);
        }
        printf("\n");
        printf("K0=0x%08X K1=0x%08X K2=0x%08X K3=0x%08X\n",part0,part1,part2,part3);
        printf("�Ƿ�������չ��Կ Y/N\n");
        scanf(" %c",&ch);
        unint32 k0 = xor_funtion(part0,FK[0]);  //���k0
        unint32 k1 = xor_funtion(part1,FK[1]);  //���k1
        unint32 k2 = xor_funtion(part2,FK[2]);  //���k2
        unint32 k3 = xor_funtion(part3,FK[3]);  //���k3
        unint32 K_array[36] = {k0,k1,k2,k3}; //��ó�ʼ��4��kֵ
        if (ch == 'y' || ch == 'Y') 
        {
            for (int i = 0; i < 32; i++)
            {
            //��һ�� ��������������Ⱥ�ck�������õ�һ���µ�32bit������
            unint32 first_step = xor_funtion((xor_funtion((xor_funtion(K_array[i+1],K_array[i+2])),K_array[i+3])),CK[i]);
            //�ڶ��� ������µ�32bit��Ϊ4��8bit      T����
            unchar8 a0 = (unchar8)(first_step >> 24);
            unchar8 a1 = (unchar8)((first_step >> 16)&0xFF);
            unchar8 a2 = (unchar8)((first_step >> 8)&0xFF);
            unchar8 a3 = (unchar8)(first_step & 0xFF);
            //������ ��4��8bit����S���н����û�
            unchar8 b0 = Sbox_lookup(a0);
            unchar8 b1 = Sbox_lookup(a1); 
            unchar8 b2 = Sbox_lookup(a2);
            unchar8 b3 = Sbox_lookup(a3);
            //���Ĳ� ���û�����������ºϲ�λ32bit ��������λ�������� 
            unint32 B = ((unint32)b0 << 24) | // ��byte0���������8λ
                        ((unint32)b1 << 16) | // ��byte1�����ڴθ�8λ
                        ((unint32)b2 << 8)  | // ��byte2�����ڴε�8λ
                        (unint32)b3;
            unint32 B_left = cyclic_shift(B,13);   //��B������13λ
            unint32 B_right = cyclic_shift(B,23);  //��B������23λ01
            unint32 k = xor_funtion((xor_funtion(xor_funtion(B,B_left),B_right)),K_array[i]);  //�õ�����Կrk i
            K_array[i+4] = k;  //�������ݴ洢
            rk_array[i] = K_array[i+4];   //�������ݴ洢
            }

            for (int i = 0; i < 32; i++)
            {
                printf("rk_array[%d]%08X\n",i,rk_array[i]);   //���Դ�ӡ32������Կ rk[i] ���Գɹ�
            }
        } 
        else 
        {
            exit(0);
        }

    }
    return rk_array;  //�������յĽ��
    
}

//���ļ��ܹ���
unint32* message_encryption(unint32 rk_array[32],unint32 Encrypt_Flag,unint32 *old_X_array)
{
    char ch;
    unchar8 message[16];
    unint32* X_array = (unint32*)malloc(36 * sizeof(unint32)); // ��̬�����ڴ�
    if (Encrypt_Flag)
    {   
        int count = 0;
        int judge = 1;
        while (judge)
        {
            printf("���������ķ���M(128bit)\n");
            for (int i = 0; i < 16; i++) 
            {
            if (scanf("%02hhX", &message[i]) == 1)
            {
                count++; // �ɹ���ȡһ���ַ�����������1
            }
            else
            {
                while (getchar() != '\n') continue; // ������������
                printf("����������\n");
                count = 0; // ���ü�����
                break; // ����ѭ�������û���������
            }
            }
            if (count == 16) // ����ɹ���ȡ��16���ַ�
            {
                judge = 0; // ����judgeΪ0���˳�ѭ��
            }
        }
        // �����ķ�Ϊ4��32λ�Ĳ���
        unint32 part_0 = ((unint32)message[0] << 24) | ((unint32)message[1] << 16) | ((unint32)message[2] << 8) | message[3];
        unint32 part_1 = ((unint32)message[4] << 24) | ((unint32)message[5] << 16) | ((unint32)message[6] << 8) | message[7];
        unint32 part_2 = ((unint32)message[8] << 24) | ((unint32)message[9] << 16) | ((unint32)message[10] << 8) | message[11];
        unint32 part_3 = ((unint32)message[12] << 24) | ((unint32)message[13] << 16) | ((unint32)message[14] << 8) | message[15];
        printf("���ķ���M�Լ�X0,X1,X2,X3��ֵ\n");
        printf("M=");
        for (int i = 0; i < 16; i++)
        {
            printf("%02hhX",message[i]);
        }
        printf("\n");
        printf("X0=0x%08X X1=0x%08X X2=0x%08X X3=0x%08X\n",part_0,part_1,part_2,part_3);
        if (X_array == NULL) 
            {
            // �����ڴ����ʧ��
                return NULL;
            }
        unint32 part[4] = {part_0,part_1,part_2,part_3}; //��ó�ʼ��4��Xֵ
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
        //�ֺ�������
        unint32 first_step = xor_funtion((xor_funtion((xor_funtion(X_array[i+1],X_array[i+2])),X_array[i+3])),rk_array[i]);
        //��һ�� ��������������Ⱥ�rk����Կ�������õ�һ���µ�32bit������
        //�ڶ��� ���µ�32bit���ݷ�Ϊ4����
        unchar8 a0 = (unchar8)(first_step >> 24);
        unchar8 a1 = (unchar8)((first_step >> 16)&0xFF);
        unchar8 a2 = (unchar8)((first_step >> 8)&0xFF);
        unchar8 a3 = (unchar8)(first_step & 0xFF);
        //������ ���ڶ�����4���ַ���S�н��з����Ա任
        unchar8 b0 = Sbox_lookup(a0);
        unchar8 b1 = Sbox_lookup(a1);
        unchar8 b2 = Sbox_lookup(a2);
        unchar8 b3 = Sbox_lookup(a3);
        //���Ĳ� ���û�����������ºϲ�λ32bit ��������λ�������� 
        unint32 B = ((unint32)b0 << 24) | // ��byte0���������8λ
                    ((unint32)b1 << 16) | // ��byte1�����ڴθ�8λ
                    ((unint32)b2 << 8)  | // ��byte2�����ڴε�8λ
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
            printf("������X1,X2,X3����1����չ��Կrk0���ɵ��м�ֵU? Y/N\n");
            scanf(" %c",&ch);
            if (ch == 'Y' || ch == 'y')
            {
                printf("�м�ֵU�Լ�u0,u1,u2,u3��ֵ\n");
                printf("U=0x%08X u0=0x%02X u1=0x%02X u2=0x%02X u3=0x%02X\n",first_step,a0,a1,a2,a3);
                printf("������Uͨ�������Ԧӱ任����м�ֵV? Y/N\n");
                scanf(" %c",&ch);
                if (ch == 'Y' || ch == 'y')
                {
                    printf("�м�ֵV�Լ�v0,v1,v2,v3��ֵ\n");
                    printf("V=0x%08X v0=0x%02X v1=0x%02X v2=0x%02X v3=0x%02X\n",B,b0,b1,b2,b3);
                    printf("�������м�ֵVͨ������L�任�õ����м�ֵW? Y/N\n");
                    scanf(" %c",&ch);
                    if (ch == 'Y' || ch == 'y')
                    {
                        printf("�м�ֵW=0x%08X\n",C);
                        printf("������X0���м�ֵW���ɵ�X4��ֵ? Y/N\n");
                        scanf(" %c",&ch);
                        if (ch == 'Y' || ch == 'y')
                        {
                            printf("��1��X4�Ľ��Ϊ:X4=0x%08X\n",X);
                            printf("�����2-32�ֵĽ��? Y/N\n");
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
            printf("X_array[%d]:%08X\n",i+4,X_array[i+4]);   //���Դ�ӡ32��������� X_array[i] ���Գɹ�
        }
        printf("������X32,X33,X34,X35����任�Ľ��? Y/N\n");
        
        scanf(" %c",&ch);
        if (ch == 'Y' || ch == 'y')
        {
        printf("Y0=0x%08X Y1=0x%08X Y2=0x%08X Y3=0x%08X\n",X_array[35],X_array[34],X_array[33],X_array[32]);
        printf("���ķ���C=");
        }
        else
            exit(0);
    }
    // ����任
        unint32 *reverse_X_array =(unint32*)malloc(4 * sizeof(unint32));
        if (X_array == NULL) 
        {
        // �����ڴ����ʧ��
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
    return reverse_X_array; //���ط���ֵ ��4�� ÿ��32bit �ܼ�128bit
}
//������
// int main() {
//     // printf("input=%x\n",input);   �����������  ��������
//     // printf("output=%x\n",output);  �����������  ��������

//     unint32 a = xor_funtion(3,5);           ������������  ��������
//     printf("%x",a);                         ������������  �������� a = 6;

//     // unint32 c = cyclic_shift(1,1);   ����ѭ����λ�������    ��������
//     // printf("%x",c);                  ����ѭ����λ�������    ��������

//     unchar8 input; // ����һ���޷��ŵ��ַ������ݣ����Դ洢0-255 char�����ʺϴ洢ASCII�ַ��Լ�����������
//     printf("������2λ16������:");
//     scanf("%X",&input); //%X��ʾ��16���ƽ��ܻ�������� 
//     unchar8 output = Sbox_lookup(input);//������ֵ����ת���±꺯������ʹ��output�������ܷ���ֵ   S�в������ ��������
//     printf("S������: 0x%X, ���: 0x%X\n", input, output);                                     S�в������ ��������
//     return 0;
// }