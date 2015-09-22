#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>

static void encode3(double f, char *out, int len) {
  int n = snprintf(out, len, "%+16.8e", f ) ;
  assert(n >= 0 && n < len);
}

static void decode(const char in[16+1], double *out) {
  sscanf( in, "%lg", out );
}

static double delta( double ref, double cmp )
{
  return fabs(cmp - ref) / ref;
}

int main()
{
  clock_t start, end;
  double cpu_time_used;
  start = clock();

  double sum = 0;
  double cmp;
  char buf[16+1];
#if 1
  union {
    float f;
    uint32_t i;
  } u;
  uint_fast32_t i = 0;
  int j;
  for( i = 0; i < UINT32_MAX; /*++i*/ )
  {
    printf("iteration: %d\n", i );
    for( j = 0; j < 10000; ++j, ++i )
    {
      u.i = i;
      if( isfinite(u.f) )
      {
        encode3( u.f, buf, sizeof(buf) );
        decode( buf, &cmp );
        sum += delta( u.f, cmp );
      }
    }
  }
#else
  union {
    double f;
    uint64_t i;
  } u;
  uint_fast64_t i = 0;
  for( uint_fast64_t i = 0; i < UINT64_MAX; ++i)
  {
    u.i = i;
    if( isfinite(u.f) )
    {
      encode3( u.f, buf );
      decode( buf, &cmp );
      sum += delta( u.f, cmp );
    }
  }
#endif
  printf( "sum: %.17g\n", sum);

  end = clock();
  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  printf("Elapsed: %f seconds\n", cpu_time_used );

  return 0;
}
