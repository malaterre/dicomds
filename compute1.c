#include <time.h>
#include <stdint.h>
#include <inttypes.h> // PRIuFAST32
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

static void clean(char *mant) {
    char *ix = mant + strlen(mant) - 1;
    while(('0' == *ix) && (ix > mant)) {
        *ix-- = '\0';
    }
    if ('.' == *ix) {
        *ix = '\0';
    }
}

static int add1(char *buf, int n) {
    if (n < 0) return 1;
    if (buf[n] == '9') {
        buf[n] = '0';
        return add1(buf, n-1);
    }
    else {
        buf[n] += 1;
    }
    return 0;
}

static int doround(char *buf, unsigned int n) {
    char c;
    if (n >= strlen(buf) - 1) return 0;
    c = buf[n];
    buf[n] = 0;
    if ((c >= '5') && (c <= '9')) return add1(buf, n-1);
    return 0;
}

static int myround(char *buf, unsigned int i, int iexp) {
    if (doround(buf, i) != 0) {
        iexp += 1;
        switch(iexp) {
            case -2:
                strcpy(buf, ".01");
                break;
            case -1:
                strcpy(buf, ".1");
                break;
            case 0:
                strcpy(buf, "1.");
                break;
            case 1:
                strcpy(buf, "10");
                break;
            case 2:
                strcpy(buf, "100");
                break;
            default:
                sprintf(buf, "1e%d", iexp);
        }
        return 1;
    }
    return 0;
}

static void encode1(double f, char *buf, int size) {
    char line[40];
    char *mant = line + 1;
    int iexp, lexp, i;
    char exp[6];

    if (f < 0) {
        f = -f;
        size -= 1;
        *buf++ = '-';
    }

    sprintf(line, "%1.16e", f);
    *mant = line[0];
    i = strcspn(mant, "eE");
    mant[i] = '\0';
    iexp = strtol(mant + i + 1, NULL, 10);
    lexp = sprintf(exp, "e%d", iexp);
    if ((iexp >= size) || (iexp < -3)) {
        i = myround(mant, size - 1 -lexp, iexp);
        if(i == 1) {
            strcpy(buf, mant);
            return;
        }
        buf[0] = mant[0];
        buf[1] = '.';
        strncpy(buf + i + 2, mant + 1, size - 2 - lexp);
        buf[size-lexp] = 0;
        clean(buf);
        strcat(buf, exp);
    }
    else if (iexp >= size - 2) {
        myround(mant, iexp + 1, iexp);
        strcpy(buf, mant);
    }
    else if (iexp >= 0) {
        i = myround(mant, size - 1, iexp);
        if (i == 1) {
            strcpy(buf, mant);
            return;
        }
        strncpy(buf, mant, iexp + 1);
        buf[iexp + 1] = '.';
        strncpy(buf + iexp + 2, mant + iexp + 1, size - iexp - 1);
        buf[size] = 0;
        clean(buf);
    }
    else {
        int j;
        i = myround(mant, size + 1 + iexp, iexp);
        if (i == 1) {
            strcpy(buf, mant);
            return;
        }
        buf[0] = '.';
        for(j=0; j< -1 - iexp; j++) {
            buf[j+1] = '0';
        }
        if ((i == 1) && (iexp != -1)) {
            buf[-iexp] = '1';
            buf++;
        }
        strncpy(buf - iexp, mant, size + 1 + iexp);
        buf[size] = 0;
        clean(buf);
    }
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
        encode1( u.f, buf, sizeof(buf) );
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
      encode1( u.f, buf );
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
