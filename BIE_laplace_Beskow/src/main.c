#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include "BIELaplace.h"
//#include <clapack.h>

double getTime() {

  struct timeval tv;

  gettimeofday(&tv, NULL);

  return (double) (tv.tv_sec+tv.tv_usec*1e-6);

}

int main(int argc, char const *argv[])
{

	/*
	pz: pointer to the points in the domain
	pzDrops: pointer to the points along the boundary.
	pzDropsp: pointer to the function values of the first derivative of the curvature, evaluated at the points stored in pzDrops.
	pzDropspp: pointer to the function values of the second derivative of the curvature, evaluated at the points stored in pzDrops.
	ppanels: pointer to the panels. Each element is the first point on the panel.
	
	RHS: contains the functional values of the given right hand side f. 
	ptpar: pointer to parameterized values of the panel points. Spans over [0,2*pi].
	pwDrops: pointer to the weights corresponding to each point on the panels.
	pmu: pointer to the density for each panel point.
	pu: pointer to the solution u in the points in pz.
	pu_spec: pointer to the solution from the special quadrature in the points in pz..
	pu_ana: pointer to the analytical solution in the points in pz.
	perrorvec: pointer to the error perrorvec.
	*/

	double umax;

	double * pumax;	

	umax = 0; // Used to obtain relative error.
	pumax = &umax;


   	// Allocate memory for complex double pointers.
   	double complex * pz = malloc(NBR_DOMAIN_POINTS * sizeof(complex double));
	double complex * pzDrops = malloc(NBR_PANEL_POINTS * sizeof(complex double));
	double complex * pzDropsp = malloc(NBR_PANEL_POINTS * sizeof(complex double));
	double complex * pzDropspp = malloc(NBR_PANEL_POINTS * sizeof(complex double));
	double complex * ppanels = malloc((NBR_PANELS + 1) * sizeof(complex double));
	
	// Allocate memory for double pointers.
	double * RHS = malloc(NBR_PANEL_POINTS * sizeof(double));
	double * ptpar = malloc(NBR_PANEL_POINTS * sizeof(double));
	double * pwDrops = malloc(NBR_PANEL_POINTS * sizeof(double));
	double * pmu = malloc(NBR_PANEL_POINTS * sizeof(double));
	double * pu = malloc(NBR_DOMAIN_POINTS * sizeof(double));
	double * pu_spec = malloc(NBR_DOMAIN_POINTS * sizeof(double));
	double * pu_ana = malloc(NBR_DOMAIN_POINTS * sizeof(double));
	double * perrorvec = malloc(NBR_DOMAIN_POINTS * sizeof(double));

	double timeStart = getTime();
	
	//Initialize the domain.
	init_domain(pz,pzDrops,pzDropsp,pzDropspp,ppanels,ptpar,pwDrops);
	//Evaluate the given right hand side and obtain the analytial solution.
	init_function(RHS, pu_ana, pzDrops, pz, pumax);

	double timeInit = getTime();

	//Solve for density pmu.
	solveDensity(pzDrops, pzDropsp, pzDropspp, pwDrops, RHS, pmu);
	
	double timeDensity = getTime();	

	//Evaluate the solution pu.
	computeSolution(pmu, pz, pwDrops, pzDrops, pzDropsp, pu);

	double timeSol = getTime();
	
	//Evaluate the solution pu_spec with special quadrature.
	specialquadlapl(pu_spec, pu, pmu, pz, pzDrops, pzDropsp, pwDrops, ppanels);	

	double timeSpecQ = getTime();

	//Compute the error perrorvec.
	computeError(perrorvec, pu, pu_spec, pu_ana, pumax);

	printf("Timings for run on starfish\n");
        printf("Parameters: \n");
        printf("Npanels = %d \n", NBR_PANELS);
        printf("NBR_R = %d \t NBR_T = %d\n",NBR_R,NBR_T);
        printf("Ndomain_points = %d \n", NBR_DOMAIN_POINTS);
        printf("Time for density: %lf\n", timeDensity-timeInit);
        printf("Time for solution: %lf\n",timeSol-timeDensity);
        printf("Time for special quad: %lf\n",timeSpecQ-timeSol);
        printf("\n");
        printf("Total time: %lf\n", timeSpecQ-timeInit);


//Free allocated space.
	free(pzDrops);
	free(pzDropsp);
	free(pzDropspp);
	free(ppanels);
	free(ptpar);
	free(pwDrops);
	free(RHS);
	free(pmu);
	free(pu);
	free(pu_spec);
	free(pu_ana);
	free(perrorvec);

	return 0;
}

