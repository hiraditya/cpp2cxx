//#include "/media/space/opt_149739_build/lib/clang/3.1/include/stddef.h"
#include <iostream>
#include <algorithm>
#define A 100
//#define F() A+100

int f1()
{
  #define INSIDE_FUN(i) i*100
  size_t i = INSIDE_FUN(100);
  return 0;
}
int f2()
{
#define INSIDE 100*100
  int i = 10;
  return 0;  
}
 
int f3()
{
#define WHATEVER_IN_F3() 1
  int i = 10;
  return 0;  
}

int f4()
{
  int i = 10;
  return 0;  
}

int main()
{
  int i = 10;
  return 0;  
}
#define pi 3.14
