#include <time.h>
#include <stdint.h>
#include <inttypes.h> // PRIuFAST32
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

static size_t shrink(char *fp_buffer) {
  int lead, expo;
  long long mant;
  int n0, n1;
  int n = sscanf(fp_buffer, "%d.%n%lld%ne%d", &lead, &n0, &mant, &n1, &expo);
  assert(n == 3);
  return sprintf(fp_buffer, "%d%0*llde%d", lead, n1 - n0, mant,
          expo - (n1 - n0));
}

int encode4(double value, char *dest, size_t width) {
  if (!isfinite(value)) return 1;

  if (width < 5) return 2;
  if (signbit(value)) {
    value = -value;
    strcpy(dest++, "-");
    width--;
  }
  int precision = width - 2;
  while (precision > 0) {
    char buffer[width + 10];
    // %.*e prints 1 digit, '.' and then `precision - 1` digits
    snprintf(buffer, sizeof buffer, "%.*e", precision - 1, value);
    size_t n = shrink(buffer);
    if (n <= width) {
      strcpy(dest, buffer);
      return 0;
    }
    if (n > width + 1) precision -= n - width - 1;
    else precision--;
  }
  return 3;
}

static void decode(const char in[16+1], double *out) {
  int n = sscanf( in, "%lg", out );
  assert( n == 1 );
}

static double delta( double ref, double cmp )
{
  assert( isfinite(cmp) );
  if( ref == 0. ) return fabs(cmp - ref);
  // else
  return fabs( (cmp - ref) / ref );
}

int main()
{
  clock_t start, end;
  double cpu_time_used;
  start = clock();

  double sum = 0;
  double cmp;
  char buf[16+1];
  unsigned int nmissed = 0;
#if 1
  union {
    float f;
    uint32_t i;
  } u;
  uint_fast32_t i = 0;
  int j;
  for( i = 0; i < UINT32_MAX; /*++i*/ )
  {
    fprintf(stderr,"iteration: %lf with %.17g\n", ((double)i / UINT32_MAX) * 100, sum );
    /*
     * The log length has exceeded the limit of 4 Megabytes (this usually means
     * that test suite is raising the same exception over and over).
     * The build has been terminated.
     * - 10 000 -> not ok !
     */
    for( j = 0; j < 10000000 && i < UINT32_MAX; ++j, ++i )
    {
      u.i = i;
      if( isfinite(u.f) )
      {
        encode4( u.f, buf, sizeof(buf) );
        decode( buf, &cmp );
        sum += delta( u.f, cmp );
      }
      else
      {
        nmissed++;
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
      encode4( u.f, buf );
      decode( buf, &cmp );
      sum += delta( u.f, cmp );
    }
  }
#endif
  printf( "sum: %.17g\n", sum);
  printf( "nmissed: %u\n", nmissed);

  end = clock();
  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  printf("Elapsed: %f seconds\n", cpu_time_used );

  return 0;
}
