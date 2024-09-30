#include <stdio.h>
#include <stdint.h>

int main() {
    uint32_t data = 0x12345678; // 一个32位的数据
    uint8_t bytes[4];

    bytes[0] = (data >> 24);  // 获取最高8位
    bytes[1] = ((data >> 16) & 0xFF); // 获取次高8位
    bytes[2] = (uint8_t)((data >> 8) & 0xFF);  // 获取次低8位
    bytes[3] = (uint8_t)(data & 0xFF);   // 获取最低8位

    printf("Byte 0: 0x%02X\n", bytes[0]);
    printf("Byte 1: 0x%02X\n", bytes[1]);
    printf("Byte 2: 0x%02X\n", bytes[2]);
    printf("Byte 3: 0x%02X\n", bytes[3]);

    return 0;
}