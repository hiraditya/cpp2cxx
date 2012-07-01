#define depth0 0
#define shift2 {bit = bit << 2}
#define a 100
#define b 200
#define INTERFACE_IArchiveUpdateCallback(x) \
  INTERFACE_IProgress(x); \
  STDMETHOD(GetUpdateItemInfo)(UInt32 index,  \
  Int32 *newData, /*1 - new data, 0 - old data */ \
  Int32 *newProperties, /* 1 - new properties, 0 - old properties */ \
  UInt32 *indexInArchive /* -1 if there is no in archive, or if doesn't matter */ \
  )  x; \
  STDMETHOD(GetProperty)(UInt32 index, PROPID propID, PROPVARIANT *value) x; \
  STDMETHOD(GetStream)(UInt32 index, ISequentialInStream **inStream) x; \
  STDMETHOD(SetOperationResult)(Int32 operationResult) x; \

#define RB1(i, edx, edi)	\
	AS2(	mov AS_REG_7d, [Wt_2(i)]	)\
	AS2(	mov edi, [Wt_15(i)])\
	AS2(	mov ebx, AS_REG_7d	)\
	AS2(	shr AS_REG_7d, 10		)\
	AS2(	ror ebx, 17		)\
	AS2(	xor AS_REG_7d, ebx	)\
	AS2(	ror ebx, 2		)\
	AS2(	xor ebx, AS_REG_7d	)/* s1(W_t-2) */\
	AS2(	add ebx, [Wt_7(i)])\
	AS2(	mov AS_REG_7d, edi	)\
	AS2(	shr AS_REG_7d, 3		)\
	AS2(	ror edi, 7		)\
	AS2(	add ebx, [Wt(i)])/* s1(W_t-2) + W_t-7 + W_t-16 */\
	AS2(	xor AS_REG_7d, edi	)\
	AS2(	add edx, [Kt(i)])\
	AS2(	ror edi, 11		)\
	AS2(	add edx, H(i)	)\
	AS2(	xor AS_REG_7d, edi	)/* s0(W_t-15) */\
	AS2(	add AS_REG_7d, ebx	)/* W_t = s1(W_t-2) + W_t-7 + s0(W_t-15) W_t-16*/\
	AS2(	mov [Wt(i)], AS_REG_7d)\
	AS2(	add edx, AS_REG_7d	)\

#define the "MacID230984230982304"
#define the "SOmething else"

#define c 300
#define THIS_FILE __FILE__
#if +a + (-b+c)
#define depth1 1 + t1
#if !defined(a) || defined(b) && defined(c)
#define depth2 1 + depth1 + a + t1
#ifdef a
#endif
#ifndef __DBL_MIN_EXP__
#define depth3 1+depth2+c+__SHRT_MAX__
#endif
#endif
#endif

#define CRYPTOPP_X86_ASM_AVAILABLE
#define FXY(X,Y,Z) ((X) + (Y))
#define fun() FXY(1,200,3)

int i = fun();
int super(int i,int j)
{
#define INSIDE_FUN_SUPER(X,Y) X+100 + i


  return INSIDE_FUN_SUPER(10000, 4000);
}
int l = FXY((i+i),super(10,12),100);

int aPtr[] = {1,2,3,4,5,6,7,8,9,10};
int cPtr[] = {1,2,3,4,5,6,7,8,9,10};

#define _a(i) aPtr[i%9] + _abc // 13 is inverse of 4 mod 17
#define _c(i) cPtr[((i)*13+16) % 7]
#define _b(i, j) b##i[(j)*2%8 + (j)/4]
#define _abc 1000
#define t12 10*256
#define TYPE_CHAR (char *)
#define Ptr(X) X*

#define proc(X,Y) {\
  FXY(X,Y,0);\
  }
#define MSB(x) (((x) >> 24) & 0xffU)  /* most  significant byte */
#define SSB(x) (((x) >> 16) & 0xffU)  /* second in significance */
#define TSB(x) (((x) >>  8) & 0xffU)  /* third  in significance */
#define LSB(x) (((x)      ) & 0xffU)  /* least significant byte */

#define squareRound(text, temp, T0, T1, T2, T3, roundkey) \
{ \
  temp[0] = T0[MSB (text[0])] \
      ^ T1[MSB (text[1])] \
      ^ T2[MSB (text[2])] \
      ^ T3[MSB (text[3])] \
      ^ roundkey[0]; \
  temp[1] = T0[SSB (text[0])] \
      ^ T1[SSB (text[1])] \
      ^ T2[SSB (text[2])] \
      ^ T3[SSB (text[3])] \
      ^ roundkey[1]; \
  temp[2] = T0[TSB (text[0])] \
      ^ T1[TSB (text[1])] \
      ^ T2[TSB (text[2])] \
      ^ T3[TSB (text[3])] \
      ^ roundkey[2]; \
  temp[3] = T0[LSB (text[0])] \
      ^ T1[LSB (text[1])] \
      ^ T2[LSB (text[2])] \
      ^ T3[LSB (text[3])] \
      ^ roundkey[3]; \
} /* squareRound */

#define squareFinal(text, temp, S, roundkey) \
{ \
  text[0] = ((word32) (S[MSB (temp[0])]) << 24) \
      ^ ((word32) (S[MSB (temp[1])]) << 16) \
      ^ ((word32) (S[MSB (temp[2])]) <<  8) \
      ^  (word32) (S[MSB (temp[3])]) \
      ^ roundkey[0]; \
  text[1] = ((word32) (S[SSB (temp[0])]) << 24) \
      ^ ((word32) (S[SSB (temp[1])]) << 16) \
      ^ ((word32) (S[SSB (temp[2])]) <<  8) \
      ^  (word32) (S[SSB (temp[3])]) \
      ^ roundkey[1]; \
  text[2] = ((word32) (S[TSB (temp[0])]) << 24) \
      ^ ((word32) (S[TSB (temp[1])]) << 16) \
      ^ ((word32) (S[TSB (temp[2])]) <<  8) \
      ^  (word32) (S[TSB (temp[3])]) \
      ^ roundkey[2]; \
  text[3] = ((word32) (S[LSB (temp[0])]) << 24) \
      ^ ((word32) (S[LSB (temp[1])]) << 16) \
      ^ ((word32) (S[LSB (temp[2])]) <<  8) \
      ^  (word32) (S[LSB (temp[3])]) \
      ^ roundkey[3]; \
} /* squareFinal */

#include<iostream>

int main()
{
  int a1 = a;
  int b1 = b;
  int c1 = c;
  
  std::cout<<MSB(102400)<<"\n";
  std::cout<<LSB(102400)<<"\n";
  std::cout<<TSB(102400)<<"\n";
  std::cout<<SSB(102400)<<"\n";
 // statement like macro inside function 
  int bit = 1;
  #define shift3 bit = bit << 3
  shift3;
  #define shift4 bit = bit << 4; bit = bit >> 3
  shift4;
 //////
  int temp[4];
  int text[] = {1,2,4,8};
  int t0[] = {1,2,4,8};
  int t1[] = {1,2,4,8};
  int t2[] = {1,2,4,8};
  int t3[] = {10,2,4,8};
  int rk[] = {10,2,4,8};
  squareRound(text,temp,t0,t1,t2,t3,rk);
  std::cout<<temp[0]<<"\t"<<temp[1]<<"\t"<<temp[2]<<"\t"<<temp[3]<<"\n";
  std::cout<<the<<"\n";
  int p = _c(100);
  int q = _a(100);
#define INSIDE_FUN(X) X+100
  int infun = INSIDE_FUN(105);
  std::cout<<p<<"\t"<<q<<"\n";  
  return 0;  
}
