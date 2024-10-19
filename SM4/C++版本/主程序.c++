#include "test.h"

using namespace std;

unint32* generate_key(unint32 Encrypt_Flag) {
    if (!Encrypt_Flag) return nullptr;
    char ch;
    vector<unchar8> key(16);
    cout << "�������ʼ��ԿMK(128bit)" << endl;

    string line;
    bool validInput = false;
    while (!validInput) {
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ������뻺�����еĲ�������
        getline(cin, line); // ��ȡ��������
        stringstream ss(line);
        validInput = true;
        for (int i = 0; i < 16; ++i) {
            string byteStr;
            if (i < 15) {
                getline(ss, byteStr, ' ');
            } else {
                getline(ss, byteStr); // ���һ�����󲻸��ո�
            }
            if (byteStr.length() != 2 || sscanf(byteStr.c_str(), "%hhX", &key[i]) != 1) {
                cout << "��Ч���룬ÿ��ʮ�������������������ַ���" << endl;
                validInput = false;
                break;
            }
        }
        if (!validInput) {
            cout << "�����������ʼ��ԿMK(128bit, ��16����λʮ��������)��ÿ������֮���ÿո�ָ���" << endl;
        }
    }

    unint32 part0 = ((key[0] << 24) | (key[1] << 16) | (key[2] << 8) | key[3]);
    unint32 part1 = ((key[4] << 24) | (key[5] << 16) | (key[6] << 8) | key[7]);
    unint32 part2 = ((key[8] << 24) | (key[9] << 16) | (key[10] << 8) | key[11]);
    unint32 part3 = ((key[12] << 24) | (key[13] << 16) | (key[14] << 8) | key[15]);

    cout << "��ʼ��ԿMK�Լ�k0,k1,k2,k3��ֵ" <<endl;
    cout << "MK=" ; 
    for (int i = 0; i < 16; ++i) {
        cout << hex << setw(2) << setfill('0') << uppercase << static_cast<int>(key[i]);
        if (i < 15) cout << " ";
    }

    cout << endl;

    cout << "K0=0x" << hex << setw(8) << setfill('0') << part0 
         << " K1=0x" << setw(8) << part1 
         << " K2=0x" << setw(8) << part2 
         << " K3=0x" << setw(8) << part3 << endl;

    unint32* rk_array = (unint32*)malloc(32 * sizeof(unint32));
    if (rk_array == nullptr) {
        return nullptr;
    }
    cout << "������չ��Կ? (Y/N): ";
    cin >> ch;

    unint32 k0 = xor_function(part0, FK[0]);
    unint32 k1 = xor_function(part1, FK[1]);
    unint32 k2 = xor_function(part2, FK[2]);
    unint32 k3 = xor_function(part3, FK[3]);

    unint32 K_array[36] = {k0, k1, k2, k3};
    if (ch == 'y' || ch == 'Y') 
    {
        for (int i = 0; i < 32; ++i) 
        {
        unint32 first_step = xor_function(xor_function(xor_function(K_array[i + 1], K_array[i + 2]), K_array[i + 3]), CK[i]);
        unchar8 a0 = static_cast<unchar8>(first_step >> 24);
        unchar8 a1 = static_cast<unchar8>((first_step >> 16) & 0xFF);
        unchar8 a2 = static_cast<unchar8>((first_step >> 8) & 0xFF);
        unchar8 a3 = static_cast<unchar8>(first_step & 0xFF);
        unchar8 b0 = Sbox_lookup(a0);
        unchar8 b1 = Sbox_lookup(a1);
        unchar8 b2 = Sbox_lookup(a2);
        unchar8 b3 = Sbox_lookup(a3);
        unint32 B = (static_cast<unint32>(b0) << 24) | (static_cast<unint32>(b1) << 16) | (static_cast<unint32>(b2) << 8) | static_cast<unint32>(b3);
        unint32 B_left = cyclic_shift(B, 13);
        unint32 B_right = cyclic_shift(B, 23);
        unint32 k = xor_function(xor_function(xor_function(B, B_left), B_right), K_array[i]);
        K_array[i + 4] = k;
        rk_array[i] = K_array[i + 4];
        }
        for (int i = 0; i < 32; i++) 
        {
            cout << "rk_array[" << dec << i<< "]:" << hex << uppercase << setw(8) << setfill('0') << rk_array[i] << endl;
        }
    }
    else
        exit(0);

    return rk_array;
}

unint32* message_encryption(unint32 rk_array[32], unint32 Encrypt_Flag, unint32* old_X_array) {
    char ch;
    unchar8 message[16];
    unint32* X_array = (unint32*)malloc(36 * sizeof(unint32));
    if (X_array == NULL) {
        return NULL;
    }
if (Encrypt_Flag) {
    unchar8 message[16];
    cout << "������128λ��ϢM(16�ֽ�):\n";

    string line;
    bool validInput = false;
    while (!validInput) {
        // ������������뻺�����еĲ������ݣ�ȷ����һ������ʱ�������ǿյ�
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        getline(cin, line); // ��ȡ��������
        stringstream ss(line);
        validInput = true;
        for (int i = 0; i < 16; ++i) {
            string byteStr;
            if (i < 15) {
                getline(ss, byteStr, ' ');
            } else {
                getline(ss, byteStr); // ���һ�����󲻸��ո�
            }
            if (byteStr.length() != 2 || sscanf(byteStr.c_str(), "%hhX", &message[i]) != 1) {
                cout << "��Ч���룬ÿ��ʮ�������������������ַ���\n";
                validInput = false;
                break;
            }
        }
        if (!validInput) {
            cout << "����������128λ��ϢM(16�ֽ�):\n";
        }
    }

        unint32 part_0 = ((message[0] << 24) | (message[1] << 16) | (message[2] << 8) | message[3]);
        unint32 part_1 = ((message[4] << 24) | (message[5] << 16) | (message[6] << 8) | message[7]);
        unint32 part_2 = ((message[8] << 24) | (message[9] << 16) | (message[10] << 8) | message[11]);
        unint32 part_3 = ((message[12] << 24) | (message[13] << 16) | (message[14] << 8) | message[15]);

        cout << "��ϢM��Ϊ�ĸ���:X0, X1, X2, X3\n";
        cout << "M=";
        for (int i = 0; i < 16; i++) {
            cout << hex << setw(2) << setfill('0') << uppercase <<static_cast<int>(message[i]);
            if (i<15)
            {
                cout << " ";
            }
            
        }
        cout << "\nX0=0x" << hex << part_0 << " X1=0x" << hex << part_1 << " X2=0x" << hex << part_2 << " X3=0x" << hex << part_3 << uppercase <<endl;
        unint32 part[4] = {part_0, part_1, part_2, part_3};
        for (int i = 0; i < 4; i++) {
            X_array[i] = part[i];
        }
    } 
    else 
    {
        for (int i = 0; i < 4; i++) {
            X_array[i] = old_X_array[i];
        }
    }

    for (int i = 0; i < 32; i++) {
        unint32 first_step = xor_function(xor_function(xor_function(X_array[i + 1], X_array[i + 2]), X_array[i + 3]), rk_array[i]);
        unchar8 a0 = static_cast<unchar8>(first_step >> 24);
        unchar8 a1 = static_cast<unchar8>((first_step >> 16) & 0xFF);
        unchar8 a2 = static_cast<unchar8>((first_step >> 8) & 0xFF);
        unchar8 a3 = static_cast<unchar8>(first_step & 0xFF);
        unchar8 b0 = Sbox_lookup(a0);
        unchar8 b1 = Sbox_lookup(a1);
        unchar8 b2 = Sbox_lookup(a2);
        unchar8 b3 = Sbox_lookup(a3);
        unint32 B = (static_cast<unint32>(b0) << 24) | (static_cast<unint32>(b1) << 16) | (static_cast<unint32>(b2) << 8) | static_cast<unint32>(b3);
        unint32 C = xor_function(xor_function(xor_function(xor_function(B, cyclic_shift(B, 2)), cyclic_shift(B, 10)), cyclic_shift(B, 18)), cyclic_shift(B, 24));
        unint32 X = xor_function(C, X_array[i]);
        X_array[i + 4] = X;
            if (i == 0 && Encrypt_Flag == 1) {
        cout << "������X1,X2,X3����1����չ��Կrk0���ɵ��м�ֵU? (Y/N): ";
        cin >> ch;
        if (ch == 'Y' || ch == 'y') {
            cout << "��һ���м���U=0x" << hex << first_step << " u0=0x" << hex << static_cast<int>(a0) << " u1=0x" << hex << static_cast<int>(a1) << " u2=0x" << hex << static_cast<int>(a2) << " u3=0x" << hex << uppercase << static_cast<int>(a3) << endl;
            cout << "������Uͨ�������Ԧӱ任����м�ֵV? (Y/N): ";
            cin >> ch;
            if (ch == 'Y' || ch == 'y') {
                cout << "U��Sbox�任���V=0x" << hex << B << " v0=0x" << hex << static_cast<int>(b0) << " v1=0x" << hex << static_cast<int>(b1) << " v2=0x" << hex << static_cast<int>(b2) << " v3=0x" << hex << uppercase << static_cast<int>(b3) << endl;
                cout << "�������м�ֵVͨ������L�任�õ����м�ֵW? (Y/N): ";
                cin >> ch;
                if (ch == 'Y' || ch == 'y') {
                    cout << "�м�ֵW=0x" << hex << C << uppercase << endl;
                    cout << "������X0���м�ֵW���ɵ�X4��ֵ? (Y/N): ";
                    cin >> ch;
                    if (ch == 'Y' || ch == 'y') {
                        cout << "��һ�ֵ����ս��X4=0x" << hex << X << uppercase <<endl;
                        cout << "�����2-32�ֽ��? (Y/N): ";
                        cin >> ch;
                        if (ch == 'Y' || ch == 'y') {}
                        else {
                            free(X_array);
                            exit(0);
                        }
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

if (Encrypt_Flag == 1) {
    for (int i = 0; i < 32; i++) {
        cout << "X_array[" << dec <<i + 4 << "]:0x" << hex << X_array[i + 4] << uppercase <<endl;
    }
    cout << "������X32,X33,X34,X35����任�Ľ�������ķ���? (Y/N): ";
    cin >> ch;
    if (ch == 'Y' || ch == 'y') {
        cout << "Y0=0x" << hex << X_array[35] << " Y1=0x" << hex << X_array[34] << " Y2=0x" << hex << X_array[33] << " Y3=0x" << hex << X_array[32] << uppercase << endl;
        cout << "���ķ���C=";
    } 
    else {
        free(X_array);
        exit(0);
    }
}

unint32* reverse_X_array = (unint32*)malloc(4 * sizeof(unint32));
for (int i = 0; i < 4; i++) {
    reverse_X_array[i] = X_array[35 - i];
    if (Encrypt_Flag == 1) {
        cout << hex << setw(8) << reverse_X_array[i] << " ";
    }
}
cout << endl;
free(X_array);
return reverse_X_array;
}
int main() {
int choice, flag = 1;
unint32* rk_array = nullptr;
unint32* X_array = nullptr;
unint32* reverse_X_array = nullptr;
unint32 temp;
while (flag) {
    cout << "=============================================================\n";
    cout << "                 SMS4���ܽ��ܳ���\n";
    cout << "=============================================================\n";
    cout << "=====================1.����==========================\n";
    cout << "=====================2.����==========================\n";
    cout << "=====================3.�˳�==========================\n";
    cout << "=============================================================\n";

    cin >> choice;
    if (choice < 1 || choice > 3) {
        cout << "��Ч���룬������ѡ��:\n";
        continue;
    }

    switch (choice) {
    case 1:
        rk_array = generate_key(ENCRYPT);
        X_array = message_encryption(rk_array, ENCRYPT, nullptr);
        break;
    case 2:
        if (!rk_array || !X_array) {
            cout << "����ִ�м��ܲ���(ѡ��1)��ִ�н��ܲ�����\n";
            break;
        }
        for (int i = 0; i < 16; i++) {
            temp = rk_array[i];
            rk_array[i] = rk_array[31 - i];
            rk_array[31 - i] = temp;
        }
        reverse_X_array = message_encryption(rk_array, DECRYPT, X_array);
        cout << "���ܺ������:";
        for (int i = 0; i < 4; i++) {
            cout << hex << setw(8) << reverse_X_array[i] << " "<< uppercase ;
        }
        cout << endl;
        free(reverse_X_array);
        break;
    case 3:
        cout << "�˳�����\n";
        flag = 0;
        break;
    }
}

if (rk_array) free(rk_array);
if (X_array) free(X_array);

return 0;
}