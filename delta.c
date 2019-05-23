#include <stdio.h>
#include <math.h>
#include <assert.h>

static double delta( double ref, double cmp )
{
  assert( isfinite(cmp) );
  if( ref == 0. ) return fabs(cmp - ref);
  // else
  return fabs( (cmp - ref) / ref );
}

int main()
{
  float f = -1.40129846e-45;
  double cmp = -1.4012984643e-45;
  double dlt = delta( f, cmp );
  printf( "%.17g\n", dlt );
  if( dlt < 0.0 )
    printf( "negative\n" );
  return 0;
}
