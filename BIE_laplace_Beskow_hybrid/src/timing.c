#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include "BIELaplace.h"


double getTime() {

  struct timeval tv;

  gettimeofday(&tv, NULL);

  return (double) (tv.tv_sec+tv.tv_usec*1e-6);

}

