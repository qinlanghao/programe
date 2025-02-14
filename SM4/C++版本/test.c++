
// 变量命名不规范（使用下划线命名法）
void example_function_2() {
   int my_variable = 0; // 使用下划线，不符合 LLVM 风格
   // 函数体内有多个连续空行

   // 不一致的缩进
       int x = 0;
   int y = 1;

   // 不一致的括号风格
   if (x == 0) { y = 1; }
   else { z = 2; }

   // 缺少空格
   int a=0,b=1,c=2;

   // 多余的空格
   int d =  3;

   // 不一致的空行
   int e = 4;

   int f = 5;

   // 文件末尾有空行
}