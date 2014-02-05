
#include <sys/time.h>
#include "MT.h"

#include "utl.h"

double rnd()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  init_genrand(tv.tv_usec);
  return genrand_real1();
}

