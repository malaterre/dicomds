#include <math.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

int main()
{
  const double d = pow(0.5, 1022 + 52);
  char buf[16+1];
  assert(isfinite(d)); // for now, only address finite numbers

  const int precision = 10;

  int n = snprintf(buf, sizeof buf, "%.*e", precision, d);
  int n2 = strlen( buf );
  printf( "%d vs %d vs %d\n", sizeof buf, n, n2 );
  return 0;
}
