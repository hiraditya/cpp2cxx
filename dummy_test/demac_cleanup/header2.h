
/** Demacrofication for the macro header1_macro with unique identifier USE_header1_macro_header2h_1_9*/
template <class _T1, class _T2> 
void header1_macro(_T1 && x, _T2 && y)
{
x+y;
;
}

/** Demacrofication for the macro header3_macro with unique identifier USE_header3_macro_header2h_2_9*/
template <class _T1, class _T2> 
auto header3_macro(_T1 x, _T2 y) -> decltype(x[y])
{
  return x[y];
}


