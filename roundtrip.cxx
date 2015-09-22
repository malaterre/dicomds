#include <cstdio>
#include <cstring>
#include <cmath>
#include <cassert>
#include <cfloat>
#include <stdint.h>
// C++
#include <string>
#include <iostream>
#include <limits>
// http://stackoverflow.com/questions/32631178/writing-ieee-754-1985-double-as-ascii-on-a-limited-16-bytes-string

static void encode1(double f, char *buf)
{
  char line[40];
  //char format[8];
  int prec;
  int l;

  l = sprintf(line, "%.17g", f);
  if (l > 16) {
    prec = 33 - strlen(line);
    l = sprintf(line, "%.*g", prec, f);
    while(l > 16) {
      /* putc('.', stdout);*/
      prec -=1;
      l = sprintf(line, "%.*g", prec, f);
    }
  }
  assert(strlen(line) <= 16);
  strcpy(buf, line);
}

static void encode2(double x, char *out) {
  // Room for 16 printable characters.
  char buf[16+1];
  assert(std::isfinite(x)); // for now, only address finite numbers

  int precision = 8+1+1;
  if (std::signbit(x)) precision--;  // Or simply `if (x <= 0.0) precision--;`
  if (std::fabs(x) >= 9.99999999e99) precision--; // some refinement possible here.

  int n = std::snprintf(buf, sizeof buf, "%.*e", precision, x);
  (void)n;
  int n2 = strlen( buf );
  //assert(n >= 0 && (unsigned int)n < sizeof buf);
  assert(n2 >= 0 && (unsigned int)n2 < sizeof buf);
  //puts(buf);
  std::strcpy(out, buf );
}

static void encode3(double f, char *out) {
  char buf[16+1];
  int n = std::snprintf(buf, sizeof buf, "%+16.8e", f ) ;
  assert(n >= 0 && (unsigned int)n < sizeof buf);
  std::strcpy(out, buf );
}

static void TestPrintOneFloat(const float f)
{
  double diff; float ff = -1;
  char out[16+1];
  encode1(f, out);
  sscanf( out, "%f", &ff ); diff = f - ff; printf( "diff: %.17g\n", diff);
  encode2(f, out);
  sscanf( out, "%f", &ff ); diff = f - ff; printf( "diff: %.17g\n", diff);
  encode3(f, out);
  sscanf( out, "%f", &ff ); diff = f - ff; printf( "diff: %.17g\n", diff);
}

static void TestPrintOneDouble(const double f)
{
  double diff; double ff = -1;
  char out[16+1];
  encode1(f, out);
  sscanf( out, "%lg", &ff ); diff = (f - ff) / f; printf( "diff: %s -> %.17g\n", out, diff);
  encode2(f, out);
  sscanf( out, "%lg", &ff ); diff = (f - ff) / f; printf( "diff: %s -> %.17g\n", out, diff);
  encode3(f, out);
  sscanf( out, "%lf", &ff ); diff = (f - ff) / f; printf( "diff: %s -> %.17g\n", out, diff);
}

static std::string PrintDouble( const double f)
{
  double diff; double ff = -1;
  char out[16+1];
  encode1(f, out);
  sscanf( out, "%lf", &ff ); diff = f - ff; printf( "diff: %s -> %.17g\n", out, diff);
  encode2(f, out);
  sscanf( out, "%lf", &ff ); diff = f - ff; printf( "diff: %s -> %.17g\n", out, diff);
  encode3(f, out);
  sscanf( out, "%lf", &ff ); diff = f - ff; printf( "diff: %s -> %.17g\n", out, diff);
  return "";
}


int main()
{
//	printf("0.1f:\n");
//	TestPrintOneFloat(0.1f);
//
//	printf("Smallest subnormal float. Integer representation 0x1, pow(0.5, 126 + 23):\n");
//	TestPrintOneFloat(powf(0.5, 126 + 23));
//
//	printf("Largest subnormal float. One ULP below FLT_MIN, 112 non-leading-zero mantissa digits:\n");
//	TestPrintOneFloat(pow(0.5f, 126) - pow(0.5f, 126 + 23));
//
//	printf("Smallest normalized float. pow(0.5, 126), or FLT_MIN:\n");
//	TestPrintOneFloat(powf(0.5, 126));
//
//	printf("Largest float power-of-two. pow(2.0, 127):\n");
//	TestPrintOneFloat(powf(2.0, 127));
//
//	printf("Largest float. FLT_MAX, just below pow(2.0, 128):\n");
//	TestPrintOneFloat(FLT_MAX);
//
//	printf("Now let's print some interesting doubles:\n");
//
//
//	printf("Smallest subnormal double. Integer represenation 0x1, pow(0.5, 1022 + 52):\n");
//	TestPrintOneDouble(pow(0.5, 1022 + 52));
//
//	printf("Largest subnormal double. pow(0.5, 1022) - pow(0.5, 1022 + 52), one ULP below DBL_MIN, a lot of mantissa digits:\n");
//	TestPrintOneDouble(pow(0.5, 1022) - pow(0.5, 1022 + 52));

	printf("Largest double. DBL_MAX, just below pow(2.0, 1024):\n");
	TestPrintOneDouble(DBL_MAX);

	// This is as accurately as pi can be represented in a double.
	double pi = 3.14159265358979323846;
	printf("Note that double(pi) + sin(double(pi)) equals pi to 33 digits.\n");
	printf("double(pi) =      %s\n", PrintDouble(pi).c_str());
	printf("sin(double(pi)) = %s\n", PrintDouble(sin(pi)).c_str());
	printf("pi =              +3.141592653589793238462643383279502884197169399375105820974945...\n");
	printf("                             Start adding from here ^\n");
	printf("                                                    |\n");

  union {
    float f;
    uint32_t u;
  } u;
  double sum;
  //for( uint_fast64_t i = 0; std::numeric_limits<uint64_t>::max(); ++i)
  for( uint_fast32_t i = 0; i < std::numeric_limits<uint32_t>::max(); ++i )
  {
    sum += i;
  }
  std::cout << sum << std::endl;
  
  return 0;
}
