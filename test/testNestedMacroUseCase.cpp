#define X 100
#define Y 200
#define F(A,B) A+B

int x = F(X,Y);
//test fails in this case
int y = F(X+Y,100);
