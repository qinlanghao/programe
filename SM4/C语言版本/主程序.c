#include<stdio.h>
#include "s_box-xor-left_move-generate_key-message_encryption.h"
typedef unsigned int unint32;  //������ unsigned int 
typedef unsigned char unchar8; //������ unsigned char 
// Encrypt_Flag    �ӽ��ܱ�־
// #define ENCRYPT 1   //���ܱ�־  ENCRYPT
// #define DECRYPT 0   //���ܱ�־  DECRYPT
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
        puts("                 SMS4�㷨�ӽ����㷨��ʾ\n");
        puts("=============================================================\n");
        puts("=====================1.��������(1)===========================\n");
        puts("=====================2.��������(2)===========================\n");
        puts("=====================3.�˳�����(3)===========================\n");
        puts("=============================================================\n");
        if (scanf("%d", &choice) != 1)
        {
        // ������������
            while (getchar() != '\n') continue;
            printf("����������\n");
            continue;
        }

        switch (choice)
        {
        case 1:
            rk_array = generate_key(ENCRYPT);   //����������Կ����
            X_array = message_encryption(rk_array,ENCRYPT,NULL); //�������ļ��ܺ���
            // printf("����:");
            // for (int i = 0; i < 4; i++)
            // {
            //     printf("%08X ",X_array[i]);
            //     /* code */
            // }
            // printf("\n");
            break;
        case 2:
            // ��ת��Կ˳���Ա����ڽ���
            if (rk_array == NULL || X_array == NULL) 
            {
                printf("���Ƚ��м��ܲ���(����1)�ٽ��н��ܡ�\n");
                break;
            }
            for (int i = 0; i < 16; i++)
            {
                temp = rk_array[i];
                rk_array[i] = rk_array[31 - i];
                rk_array[31 - i] = temp;
            }
            reverse_X_array = message_encryption(rk_array,DECRYPT,X_array); // ���ܵ���
            printf("��������:");
            for (int i = 0; i < 4; i++)
            {
                printf("%08X ", reverse_X_array[i]);
            }
            printf("\n");
            break;
        case 3:
            printf("�˳�����\n");
            flag = 0;
            break;
        default:
            printf("������������������\n");
            break;
        }

    }
    return 0;
}