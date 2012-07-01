int fun()
{
  int i = 10;
#define INSIDE_FUN(A,B,C) A+B+C+i
  return INSIDE_FUN(1,2,3);
}


int main()
{
  int i = fun();
  return 0;
}
