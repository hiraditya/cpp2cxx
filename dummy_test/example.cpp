#include "header1.h"

#define A 100

#define _c(i) i
#define _a(i) i*i

#define CRYPTOPP_X86_ASM_AVAILABLE
#define FXY(X,Y,Z) ((X) + (Y))
#define fun() FXY(1,200,3)

int i = fun();
int super(int i)
{

#define INSIDE_FUN_SUPER(X,Y) X+100 + j

  //variable 'j' referenced in macro INSIDE_FUN_SUPER 
  //the transformation has to be carefully placed after the declaration of 'int j'
  int j = 10;

  return INSIDE_FUN_SUPER(10000, 4000);
}
int l = FXY((i+i),super(10),100);



#define proc(X,Y) {\
  X = FXY(X,Y,0);\
  }
#define MSB(x) (((x) >> 24) & 0xffU)  /* most  significant byte */

#include<iostream>

int main()
{
   int a1 = A;
  std::cout<<"\na1 = "<<a1;
  


#define INSIDE_FUN(X) X+100

  int infun = INSIDE_FUN(105)+a1;

  int p = _c(100);
  int q = _a(100);

  std::cout<<"\np = "<<p;
  std::cout<<"\nq = "<<q;

  std::cout<<"\nl = "<<l;
  std::cout<<"\ninfun = "<<infun;
  std::cout<<"\nMSB(167816141) = "<<MSB(167816141);

  int lval = 0;
  proc(lval, 2);

  std::cout<<"\nlval = "<<lval<<"\n";
  return 0;  
}
