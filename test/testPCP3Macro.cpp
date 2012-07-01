#include<iostream>
//this macro has been taken from the pcp3 paper by Ernst Et al.
/*
#define FOO 100
#define TA3(x,y) y + x
#define TA4(x,y) x + y
#define PA2(x) x + FOO
#define PA3(x) x + 4
int main()
{
  int a = 10;
  int b = 20;
  int c = TA4(TA3(PA2(a),PA3(b)),FOO);
  std::cout<<c;
  return 0;
}*/
/* OUTPUT OF THE DEMACROFIER */
#include<iostream>
constexpr auto FOO = 100;

template <class _T1, class _T2> 
inline auto TA3(_T1 x, _T2 y) -> decltype(y + x)
{
  return y + x;
}

template <class _T1, class _T2> 
inline auto TA4(_T1 x, _T2 y) -> decltype(x + y)
{
  return x + y;
}

template <class _T1> 
inline auto PA2(_T1 x) -> decltype(x + FOO)
{
  return x + FOO;
}

template <class _T1> 
inline auto PA3(_T1 x) -> decltype(x + 4)
{
  return x + 4;
}

int main()
{
  int a = 10;
  int b = 20;
  int c = TA4(TA3(PA2(a),PA3(b)),FOO);
  std::cout<<c;
  return 0;
}

