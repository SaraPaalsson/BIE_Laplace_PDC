#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include "BIELaplace.h"



void init_function(double * RHS, double * pu_ana, double complex * pzDrops, double complex * pz, double * pumax){

	int i;

	double complex zsrc1 = 1.5 + 1.5 * I;
	double complex zsrc2 = -0.25 + 1.5 * I;
	double complex zsrc3 = -0.5 - 1.5 * I;

	for (i = 0; i < NBR_PANEL_POINTS; ++i)
		RHS[i] = creal( 1 / (pzDrops[i] - zsrc1) + 1 / (pzDrops[i] - zsrc2) + 1 / (pzDrops[i] - zsrc3));
	
	*pumax = 0; // Used to obtain relative error.
	for (i = 0; i < NBR_DOMAIN_POINTS; i++){
		pu_ana[i] = creal( 1 / (pz[i] - zsrc1) + 1 / (pz[i] - zsrc2) + 1 / (pz[i] - zsrc3));
		if (fabs(*pumax) < fabs(pu_ana[i]))
			*pumax = fabs(pu_ana[i]);
	}
}