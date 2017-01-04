#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include "BIELaplace.h"
#include "mpi.h"
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
  
        MPI_Status status;
	int rank, size, j;
	int root = 0;
	//        MPI_Init(&argc, &argv);
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get rank for process
	MPI_Comm_size(MPI_COMM_WORLD, &size); // Get number of processes.


   	// Allocate memory for complex double pointers.
	double complex * pzDrops = malloc(NBR_PANEL_POINTS * sizeof(complex double));
	double complex * pzDropsp = malloc(NBR_PANEL_POINTS * sizeof(complex double));
	double complex * pzDropspp = malloc(NBR_PANEL_POINTS * sizeof(complex double));
	double complex * ppanels = malloc((NBR_PANELS + 1) * sizeof(complex double));
	
	// Allocate memory for double pointers.

	double * pwDrops = malloc(NBR_PANEL_POINTS * sizeof(double));
	double * pmu = malloc(NBR_PANEL_POINTS * sizeof(double));
	double * pu_proc = calloc(NBR_T,  sizeof(double));
	double * pu_spec_proc = calloc(NBR_T, sizeof(double));
	double * pu_ana_proc = calloc(NBR_T, sizeof(double));
	double * perrorvec_proc = calloc(NBR_T, sizeof(double));
	
	double umax;
        double * pumax;

        umax = 0; // Used to obtain relative error.                                                                                                                                                                 
        pumax = &umax;


        // Allocate memory for complex double pointers.                                                                                                                                                             
	

	double complex * pz_proc =  malloc(NBR_T * sizeof(complex double));
        // Allocate memory for double pointers.                                                                                                                                                                     
        double * RHS = malloc(NBR_PANEL_POINTS * sizeof(double));
        double * ptpar = malloc(NBR_PANEL_POINTS * sizeof(double));
        double * pu = calloc(NBR_DOMAIN_POINTS, sizeof(double));
        double * pu_spec = malloc(NBR_DOMAIN_POINTS * sizeof(double));
        double * pu_ana = malloc(NBR_DOMAIN_POINTS * sizeof(double));
        double * perrorvec = malloc(NBR_DOMAIN_POINTS * sizeof(double));

	
        if(rank == root){
	  double complex * pz = malloc(NBR_DOMAIN_POINTS * sizeof(complex double));
	//Initialize the domain. 
	  init_domain(pz,pzDrops,pzDropsp,pzDropspp,ppanels,ptpar,pwDrops);
	  //Evaluate the given right hand side and obtain the analytial solution.                                                                                                                                     
        init_function(RHS, pu_ana, pzDrops, pz, pumax);
        //Solve for density pmu.                                                                                                                                                                                    
	solveDensity(pzDrops, pzDropsp, pzDropspp, pwDrops, RHS, pmu);
		MPI_Scatter(pz, NBR_T, MPI_C_DOUBLE_COMPLEX, pz_proc, NBR_T, MPI_C_DOUBLE_COMPLEX, root, MPI_COMM_WORLD);


	//	MPI_Scatter(pz, NBR_T, MPI_C_DOUBLE_COMPLEX, pz_proc, NBR_T, MPI_C_DOUBLE_COMPLEX, root, MPI_COMM_WORLD);

	
	}

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Bcast(pz, NBR_DOMAIN_POINTS, MPI_C_DOUBLE_COMPLEX, root,MPI_COMM_WORLD);
	MPI_Bcast(pzDrops, NBR_PANEL_POINTS, MPI_C_DOUBLE_COMPLEX, root,MPI_COMM_WORLD);	
	MPI_Bcast(pzDropsp, NBR_PANEL_POINTS, MPI_C_DOUBLE_COMPLEX, root,MPI_COMM_WORLD);
	MPI_Bcast(ppanels, NBR_PANEL_POINTS, MPI_C_DOUBLE_COMPLEX, root,MPI_COMM_WORLD);
	MPI_Bcast(pwDrops, NBR_PANEL_POINTS, MPI_DOUBLE, root,MPI_COMM_WORLD);
	MPI_Bcast(pmu, NBR_PANEL_POINTS, MPI_DOUBLE, root, MPI_COMM_WORLD);
	MPI_Bcast(pu_ana, NBR_DOMAIN_POINTS, MPI_DOUBLE, root, MPI_COMM_WORLD);
	MPI_Bcast(pumax, 1, MPI_DOUBLE, root, MPI_COMM_WORLD);
 	MPI_Barrier(MPI_COMM_WORLD);

	double time = getTime();
	computeSolution(pmu, pz_proc, pwDrops, pzDrops, pzDropsp, pu_proc, NBR_T);

	//	printf("%-20s : %lf s\n","Time",getTime()-time);

	MPI_Gather(pu_proc, 10, MPI_DOUBLE, pu, 10, MPI_DOUBLE, root, MPI_COMM_WORLD);
	if(rank == root){
	specialquadlapl(pu_spec, pu, pmu, pz, pzDrops, pzDropsp, pwDrops, ppanels);	



	//Compute the error perrorvec.
	computeError(perrorvec, pu, pu_spec, pu_ana, pumax);
	}
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
	free(pu_proc);
	free(pu_spec_proc);
	free(pu_ana_proc);
	free(perrorvec_proc);
	free(pz_proc); 
	MPI_Finalize();
	return 0;
}

