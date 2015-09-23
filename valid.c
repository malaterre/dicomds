#include <stdio.h>
int main()
{
  double d;
  const char str[] = "-.";
  int n = sscanf( str, "%lf", &d );
  printf( "%d %f\n", n , d );
  return 0;
}
