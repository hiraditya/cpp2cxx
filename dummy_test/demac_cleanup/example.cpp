#include "header1.h"


/** Demacrofication for the macro depth0 with unique identifier USE_depth0_examplecpp_3_9*/
constexpr auto depth0 = 0;

/** Demacrofication for the macro a with unique identifier USE_a_examplecpp_4_9*/
constexpr auto a = 100;

/** Demacrofication for the macro b with unique identifier USE_b_examplecpp_5_9*/
constexpr auto b = 200;


/** Demacrofication for the macro _c with unique identifier USE__c_examplecpp_7_9*/
template <class _T1> 
auto _c(_T1 i) -> decltype(i)
{
  return i;
}

/** Demacrofication for the macro _a with unique identifier USE__a_examplecpp_8_9*/
template <class _T1> 
auto _a(_T1 i) -> decltype(i*i)
{
  return i*i;
}

#define CRYPTOPP_X86_ASM_AVAILABLE 

/** Demacrofication for the macro FXY with unique identifier USE_FXY_examplecpp_11_9*/
template <class _T1, class _T2, class _T3> 
auto FXY(_T1 X, _T2 Y, _T3 Z) -> decltype(((X) + (Y)))
{
  return ((X) + (Y));
}

/** Demacrofication for the macro fun with unique identifier USE_fun_examplecpp_12_9*/

auto fun() -> decltype(FXY(1,200,3))
{
  return FXY(1,200,3);
}

int i = fun();
int super(int i)
{


  //variable 'j' referenced in macro INSIDE_FUN_SUPER 
  //the transformation has to be carefully placed after the declaration of 'int j'
  int j = 10;


/** Demacrofication for the macro INSIDE_FUN_SUPER with unique identifier USE_INSIDE_FUN_SUPER_examplecpp_18_9*/
auto INSIDE_FUN_SUPER = [&j](decltype(10000) X, decltype( 4000) Y) { return X+100 + j; };
  return INSIDE_FUN_SUPER(10000, 4000);
}
int l = FXY((i+i),super(10),100);

int aPtr[] = {1,2,3,4,5,6,7,8,9,10};
int cPtr[] = {1,2,3,4,5,6,7,8,9,10};



/** Demacrofication for the macro proc with unique identifier USE_proc_examplecpp_32_9*/
template <class _T1, class _T2> 
void proc(_T1 && X, _T2 && Y)
{
  FXY(X,Y,0);
  }


/** Demacrofication for the macro MSB with unique identifier USE_MSB_examplecpp_35_9*/
template <class _T1> 
auto MSB(_T1 x) -> decltype((((x) >> 24) & 0xffU)  /* most  significant byte */)
{
  return (((x) >> 24) & 0xffU)  /* most  significant byte */;
}


/** Demacrofication for the macro SSB with unique identifier USE_SSB_examplecpp_36_9*/
template <class _T1> 
auto SSB(_T1 x) -> decltype((((x) >> 16) & 0xffU)  /* second in significance */)
{
  return (((x) >> 16) & 0xffU)  /* second in significance */;
}


/** Demacrofication for the macro TSB with unique identifier USE_TSB_examplecpp_37_9*/
template <class _T1> 
auto TSB(_T1 x) -> decltype((((x) >>  8) & 0xffU)  /* third  in significance */)
{
  return (((x) >>  8) & 0xffU)  /* third  in significance */;
}


/** Demacrofication for the macro LSB with unique identifier USE_LSB_examplecpp_38_9*/
template <class _T1> 
auto LSB(_T1 x) -> decltype((((x)      ) & 0xffU)  /* least significant byte */)
{
  return (((x)      ) & 0xffU)  /* least significant byte */;
}


#include<iostream>

int main()
{
  int a1 = a;
  int b1 = b;
  int c1 = 'c';
  
  //std::cout<<MSB(102400)<<"\n";
  //std::cout<<LSB(102400)<<"\n";
  //std::cout<<TSB(102400)<<"\n";
  //std::cout<<SSB(102400)<<"\n";
  
  int p = _c(100);
  int q = _a(100);

/** Demacrofication for the macro INSIDE_FUN with unique identifier USE_INSIDE_FUN_examplecpp_55_9*/
auto INSIDE_FUN = [](decltype(105) X) { return X+100; };
  int infun = INSIDE_FUN(105)+a1+b1+c1;
  //std::cout<<p<<"\t"<<q<<"\n";  
  return 0;  
}

