/*  Original Macro with stringification */
#include<cstdio>

#define WARN_IF(EXP) \
     do { if (EXP) \
             fprintf (stderr, "Warning: " #EXP "\n"); } \
     while (0)

int main()
{
  int x = 0;
  WARN_IF(x==0);
  int i = 1;
  WARN_IF(i);
return 0;
}       

/*  Modified Alternative */
#include<cstdio>
#include<string>
//just need to copy the function argument as string and 
//pass that as a new argument
template <typename T1>
void WARN_IF(T1 EXP, std::string str)
{
  do { if (EXP)
             fprintf (stderr, std::string("Warning: " + str + "\n").c_str()); }
     while (0);
}

int main()
{
  int x = 0;
  WARN_IF(x==0,"x==0");
  int i = 1;
  WARN_IF(i,"i");
return 0;
}