int super(int i)
{

#define INSIDE_FUN_SUPER(X,Y) X+100 + j

   //variable 'j' referenced in macro INSIDE_FUN_SUPER 
   //  //the transformation has to be carefully placed after the declaration of 'int j'
   int j = 10; 
   return INSIDE_FUN_SUPER(10000, 4000);
}

int j = 10;
#define OUTSIDE_FUN_SUPER(X,Y) X+100 + j


int k = OUTSIDE_FUN_SUPER(10000, 4000);



/*
template<typename T>
void f(T const& t)
{
  int S[] = {1, 2,3, 3,4,5};
#define S0(X) S[X]
#define SHIFT(X) X<<t

  int x, c;
  c = S0(x);
  x = SHIFT(c);
}

int main()
{
    return 0;
}
*/
