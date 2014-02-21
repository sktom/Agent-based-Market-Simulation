#ifndef __UTL__
#define __UTL__
#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include "MT.h"

#define p(x, y) printf(x, y)
#define puts(x) printf("%s\n",x)
#define puti(x) printf("%d\n",x)
#define putl(x) printf("%ld\n",x)
#define putd(x) printf("%lf\n",x)

double
urand()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  init_genrand(tv.tv_usec);
  return genrand_real1();
}

double
grand()
{
  return pow(-2 * log(urand()), 0.5) * cos(2 * M_PI * urand());
}

double
max(double * values, int size)
{
  double max;
  double * value;
  for(value = values, max = *value; size--; ++value)
    max = max > *value ? max : *value;

  return max;
}

double
min(double * values, int size)
{
  double min = *values;
  double * value;
  for(value = values, min = *value; size--; ++value)
    min = min < *value ? min : *value;

  return min;
}
#endif

