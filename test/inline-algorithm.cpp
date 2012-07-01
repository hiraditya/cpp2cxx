
// Handling these functions requires advanced techniques.
// You have to look at the usage of parameters within the
// the body of the defined function in order to determine
// the signature of the generated function. Mostly, you need
// to know when you should generate a reference type vs. a
// const reference type.

#define f(x)		\
{			\
	x += 2;		\
}

#define f(x,y)    \
{     \
  x += 2;   \
        y += 14;        \
}

template<typename T>
void f(T& x)
{
	x += 2;
}

