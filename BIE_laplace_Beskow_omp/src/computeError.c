#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include "BIELaplace.h"





void computeError(double * perrorvec, double * pu, double * pu_spec, double * pu_ana, double * pumax){
	int i;
	double errormax = 0;

	for (i = 0; i < NBR_DOMAIN_POINTS; i++) {
		perrorvec[i] = fabs(pu_spec[i]-pu_ana[i])/(*pumax);
		if (errormax < perrorvec[i])
			errormax = perrorvec[i];
	}
	printf("\n Max error: %12.16e \n", errormax);
}
