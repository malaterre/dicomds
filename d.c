#include <stdio.h>
#include <float.h>
#include <assert.h>

static void diff( const double d1, const double d2 )
{
  printf( "d1: %.17g\n", d1);
  printf( "d2: %.17g\n", d2);
  const double diff = d2 - d1;
  printf( "diff: %.17g\n", diff);
}

static void roundt( const double d1 )
{
  char buf[16+1];
  //int n = sprintf( buf, "%.17g", d1);
  int n = snprintf( buf, sizeof buf, "%.*e", 9, d1);
  assert( n >= 0 && n < 40 );
  double d2;
  sscanf( buf ,"%lf", &d2 );

  printf( "d1: %.17g\n", d1);
  printf( "d2: %.17g\n", d2);
  const double diff = d2 - d1;
  printf( "diff: %.17g\n", diff);
}


int main()
{
  double d1 = DBL_MAX;
  double d2 = DBL_MAX;
  //diff( d1, d2 );
  roundt( d1 );
  return 0;
}
