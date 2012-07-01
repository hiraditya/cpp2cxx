#include<iostream>
#include<string>
#define CRYPTOPP_DUMMY_DLL_MAC "MAC_51f34b8db820ae8"
#define F(X,Y) X+Y
//constexpr auto CRYPTOPP_DUMMY_DLL_MAC = "MAC_51f34b8db820ae8";

//error: initializer fails to determine size of ‘mac’
int main()
{
unsigned char mac[] = CRYPTOPP_DUMMY_DLL_MAC;
int x = F(10,15);
return 0;
}
