#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include "BIELaplace.h"





void computeError(double * perrorvec, double * pu, double * pu_spec, double * pu_ana, double * pumax, int nbr_elements){
	int i;
	double errormax = 0;

	for (i = 0; i < nbr_elements; i++) {
		perrorvec[i] = fabs(pu_spec[i]-pu_ana[i])/(*pumax);
		if (errormax < perrorvec[i])
			errormax = perrorvec[i];
	}
	printf("Max error: %12.16e \n", errormax);
}
