#include "header1.h"

#if defined(__cplusplus) && defined(__GXX_EXPERIMENTAL_CXX0X__) && defined(USE_A_examplecpp_3_9)
constexpr auto A = 100;
#else
#define A 100
#endif


#if defined(__cplusplus) && defined(__GXX_EXPERIMENTAL_CXX0X__) && defined(USE__c_examplecpp_5_9)
template <class _T1>
auto _c(_T1 i) -> decltype(i)
{
  return i;
}
#else
#define _c(i) i
#endif

#if defined(__cplusplus) && defined(__GXX_EXPERIMENTAL_CXX0X__) && defined(USE__a_examplecpp_6_9)
template <class _T1>
auto _a(_T1 i) -> decltype(i*i)
{
  return i*i;
}
#else
#define _a(i) i*i
#endif


#define CRYPTOPP_X86_ASM_AVAILABLE 
#if defined(__cplusplus) && defined(__GXX_EXPERIMENTAL_CXX0X__) && defined(USE_FXY_examplecpp_9_9)
template <class _T1, class _T2, class _T3>
auto FXY(_T1 X, _T2 Y, _T3 Z) -> decltype(((X) + (Y)))
{
  return ((X) + (Y));
}
#else
#define FXY(X,Y,Z) ((X) + (Y))
#endif

#if defined(__cplusplus) && defined(__GXX_EXPERIMENTAL_CXX0X__) && defined(USE_fun_examplecpp_10_9)

auto fun() -> decltype(FXY(1,200,3))
{
  return FXY(1,200,3);
}
#else
#define fun() FXY(1,200,3)
#endif


int i = fun();
int super(int i)
{


  //variable 'j' referenced in macro INSIDE_FUN_SUPER
  //the transformation has to be carefully placed after the declaration of 'int j'
  int j = 10;

  return INSIDE_FUN_SUPER(10000, 4000);
}
int l = FXY((i+i),super(10),100);



#if defined(__cplusplus) && defined(__GXX_EXPERIMENTAL_CXX0X__) && defined(USE_proc_examplecpp_28_9)
template <class _T1, class _T2> 
void proc(_T1 && X, _T2 && Y)
{
  X = FXY(X,Y,0);
  }

#else
#define proc(X,Y) {  X = FXY(X,Y,0);  }
#endif

#if defined(__cplusplus) && defined(__GXX_EXPERIMENTAL_CXX0X__) && defined(USE_MSB_examplecpp_31_9)
template <class _T1>
auto MSB(_T1 x) -> decltype((((x) >> 24) & 0xffU)  /* most  significant byte */)
{
  return (((x) >> 24) & 0xffU)  /* most  significant byte */;
}
#else
#define MSB(x) (((x) >> 24) & 0xffU)  /* most  significant byte */
#endif



#include<iostream>

int main()
{
  int a1 = A;
  std::cout<<"\na1 = "<<a1;
  



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

