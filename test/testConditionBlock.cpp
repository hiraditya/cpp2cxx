#if !defined(a) || defined(b) && defined(c)
#define depth1 __LINE__
#elif CRYPTOPP_FAST_ROTATE(32)
#define depth1 __FILE__
#endif

/*
 * #define  CRYPTOPP_COMPILE_ASSERT(assertion) CRYPTOPP_COMPILE_ASSERT_INSTANCE(assertion, __LINE__)
 * */
template <class T1>
 CRYPTOPP_COMPILE_ASSERT(T1 assertion) -> decltype(CRYPTOPP_COMPILE_ASSERT_INSTANCE(assertion, __LINE__))
{
    return CRYPTOPP_COMPILE_ASSERT_INSTANCE(assertion, __LINE__);
}
//----------------------------------------------------------------------------------------
 template <class U>
   inline GetBlock<T, B, A> & operator()(U &x)
    {
       CRYPTOPP_COMPILE_ASSERT(sizeof(U) >= sizeof(T));
       x = GetWord<T>(A, B::ToEnum(), m_block);
       m_block += sizeof(T);
       return *this;
    }





