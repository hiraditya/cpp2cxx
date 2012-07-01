#define a 100
#define b 200
#define c 300
#define THIS_FILE __FILE__
#if +a + (-b+c)
#define depth1 (1 + t1)
#if !defined(a) || defined(b) && defined(c)
#define depth2 (1 + depth1 + a + t1)
#ifdef a
#endif
#ifndef __DBL_MIN_EXP__
#define depth3 (1+depth2+c+__SHRT_MAX__)
#endif
#endif
#endif
#define ai (aPtr[10] + _abc)
#define FXY(X,Y,Z) ((X) + (Y))
#define fun() FXY(1,200,3)

#define _abc 1000
#define t1 (10*256)
