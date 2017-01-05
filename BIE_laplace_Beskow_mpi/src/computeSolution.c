#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include "BIELaplace.h"


void computeSolution(double * pmu, double complex * pz, double * pwDrops, double complex * pzDrops, double complex * pzDropsp, double * pu, int nbr_elements)
{
	int i,k;

	for (i=0; i<nbr_elements; i++) {
		pu[i] = 0;
		for (k=0; k<NBR_PANEL_POINTS; k++) {
			pu[i] =  pu[i] + pmu[k] * pwDrops[k] * cimag(pzDropsp[k] / (pzDrops[k] - pz[i])) * 1.0 / (2.0 * M_PI);
		}
	}	

}

