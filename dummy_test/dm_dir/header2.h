#if defined(__cplusplus) && defined(__GXX_EXPERIMENTAL_CXX0X__) && defined(USE_header1_macro_header2h_1_9)
template <class _T1, class _T2> 
void header1_macro(_T1 && x, _T2 && y)
{
x = x+y;
}
#else
#define header1_macro(x,y) x = x+y
#endif

#if defined(__cplusplus) && defined(__GXX_EXPERIMENTAL_CXX0X__) && defined(USE_header3_macro_header2h_2_9)
template <class _T1, class _T2> 
auto header3_macro(_T1 x, _T2 y) -> decltype(x[y])
{
  return x[y];
}
#else
#define header3_macro(x,y) x[y]
#endif



