#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include "BIELaplace.h"


void computeSolution(double * pmu, double complex * pz, double * pwDrops, double complex * pzDrops, double complex * pzDropsp, double * pu, int nbr_elements)
{
	int i, j;
	for(i = 0; i < nbr_elements; ++i){
		        for (j = 0; j < NBR_PANEL_POINTS; ++j){
			  pu[i] =  pu[i] + pmu[j] * pwDrops[j] * cimag(pzDropsp[j] / (pzDrops[j] - pz[i])) * 1.0 / (2.0 * M_PI);
			}
	}
}

