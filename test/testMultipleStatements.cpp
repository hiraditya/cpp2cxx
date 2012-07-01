//test multiple statements
#define proc(X,Y) {\
  FXY(X,Y,0);\
  }

#define mu(a0, a1, a2)        \
{                             \
  a1 = reverseBits(a1);       \
  word32 t = reverseBits(a0); \
  a0 = reverseBits(a2);       \
  a2 = t;                     \
}

//OUTPUT-Date: July 07, 2011
//test multiple statements

// /*
// #define proc(X,Y) {  FXY(X,Y,0);  }
// */
// template <class T1, class T2> 
// void proc(T1 X, T2 Y)
// {  FXY(X,Y,0);  }
// 
// 
// /*
// #define mu(a0, a1, a2) {                               a1 = reverseBits(a1);         word32 t = reverseBits(a0);   a0 = reverseBits(a2);         a2 = t;                     }
// */
// template <class T1, class T2, class T3> 
// void mu(T1 a0, T2 a1, T3 a2)
// {                               a1 = reverseBits(a1);         word32 t = reverseBits(a0);   a0 = reverseBits(a2);         a2 = t;                     }
