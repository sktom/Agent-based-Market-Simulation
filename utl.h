#ifndef __UTL__
#define __UTL__
#include <stdio.h>

#define p(x, y) printf(x, y)
#define puts(x) printf("%s\n",x)
#define puti(x) printf("%d\n",x)
#define putl(x) printf("%ld\n",x)
#define putd(x) printf("%lf\n",x)

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

