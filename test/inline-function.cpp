
#define f(x) x + 2
#define g(x) #x

//ultimately we would like to achive this for f(x)
template<typename T>
constexpr auto f(T const& x) -> decltype(x + 2)
{
	return x + 2;
}
