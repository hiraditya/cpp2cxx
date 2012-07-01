#define CRYPTOPP_X86_ASM_AVAILABLE
#define depth0 0
#if +a + (-b+c)
#define depth1 1
#if !defined(a) || defined(b) && defined(c)
#define depth2 2
#ifdef a
#endif
#ifndef __DBL_MIN_EXP__
#define depth3 3
#endif
#endif
#endif


#if defined(__GNUC__) && defined(CRYPTOPP_X86_ASM_AVAILABLE)
#elif defined(CRYPTOPP_BYTESWAP_AVAILABLE)
#elif defined(__MWERKS__) && TARGET_CPU_PPC
  return (word32)__lwbrx(&value,0);
#elif _MSC_VER >= 1400 || (_MSC_VER >= 1300 && !defined(_DLL))
  return _byteswap_ulong(value);
#elif CRYPTOPP_FAST_ROTATE(32)
// 5 instructions with rotate instruction, 9 without
  return (rotrFixed(value, 8U) & 0xff00ff00) | (rotlFixed(value, 8U) & 0x00ff00ff);
#else
// 6 instructions with rotate instruction, 8 without
  value = ((value & 0xFF00FF00) >> 8) | ((value & 0x00FF00FF) << 8);
  return rotlFixed(value, 16U);
#endif

#include<iostream>  
int main()
{
  std::cout<<"Hello World\n";
}
