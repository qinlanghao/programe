#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // 用于获取 unint32 的最大值

typedef unsigned int unint32;
typedef unsigned char unchar8;

// 循环左移函数，输入一个用于移位的32位整数，另一个是用于定义移动多少位的32为整数（移位不能超过32次）
unint32 cyclic_shift(unint32 a,unint32 b)
{
    b %=32;
    return (a << b ) | (a >> (32-b));
}
//主程序
int main() {
    unint32 c = cyclic_shift(1,1);
    printf("%d",c);
    return 0;
}
