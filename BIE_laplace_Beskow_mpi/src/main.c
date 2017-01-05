#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include "BIELaplace.h"
#include "mpi.h"
//#include <clapack.h>
#include "mpi.h" 

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

	int root = 0;
  
	int rank, numproc, nbr_elements_proc; // numproc --> nbr_proc
	
	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&numproc);

	double tmp,tmp2; 
	tmp = NBR_DOMAIN_POINTS/(double) numproc;
	printf("tmp = %f\n",tmp);
	printf("numproc = %d\n",numproc);
	printf("nbrdomain = %d\n",NBR_DOMAIN_POINTS);
	

	// modf & mod
	tmp2 = tmp-floor(tmp);
	if (tmp2 != 0) {
		printf("ERROR: not suitable number of domain points.\n");
		MPI_Finalize();
		exit(EXIT_FAILURE);
	}
	nbr_elements_proc = floor(tmp);



   	// Allocate memory for complex double pointers.
	double complex * pzDrops = malloc(NBR_PANEL_POINTS * sizeof(complex double));
	double complex * pzDropsp = malloc(NBR_PANEL_POINTS * sizeof(complex double));
	double complex * pzDropspp = malloc(NBR_PANEL_POINTS * sizeof(complex double));
	double complex * ppanels = malloc((NBR_PANELS + 1) * sizeof(complex double));
	double complex * pz_proc = malloc(nbr_elements_proc * sizeof(complex double));

	// Allocate memory for double pointers.
	double * pwDrops = malloc(NBR_PANEL_POINTS * sizeof(double));
	double * pmu = malloc(NBR_PANEL_POINTS * sizeof(double));
        double * pu_proc = malloc(nbr_elements_proc * sizeof(double));
        double * pu_spec_proc = malloc(nbr_elements_proc * sizeof(double));
        double * pu_ana_proc = malloc(nbr_elements_proc * sizeof(double));
        double * perrorvec_proc = malloc(nbr_elements_proc * sizeof(double));
        double * pumax;	

	//Only for root.
	double umax;
	umax = 0; // Used to obtain relative error.
	pumax = &umax;

	// a,b,c = NULL;
	double complex *pz = NULL;
	double * pu_spec = NULL;
	double * pu_ana = NULL;
	double * perrorvec = NULL;
	double * pu = NULL;
	double * RHS = NULL;
	double * ptpar = NULL;
	if (rank == 0) {


		//Initialize domain arrays
		pz = malloc(NBR_DOMAIN_POINTS * sizeof(complex double));
//		double * pu = malloc(NBR_DOMAIN_POINTS * sizeof(double));
		pu_spec = malloc(NBR_DOMAIN_POINTS * sizeof(double));
		pu_ana = malloc(NBR_DOMAIN_POINTS * sizeof(double));
		perrorvec = malloc(NBR_DOMAIN_POINTS * sizeof(double));
		pu = malloc(NBR_DOMAIN_POINTS *sizeof(double));
		RHS = malloc(NBR_PANEL_POINTS * sizeof(double));
		ptpar = malloc(NBR_PANEL_POINTS * sizeof(double));

		//Initialize the domain.
		init_domain(pz,pzDrops,pzDropsp,pzDropspp,ppanels,ptpar,pwDrops);
		//Evaluate the given right hand side and obtain the analytial solution.
		init_function(RHS, pu_ana, pzDrops, pz, pumax);
		//Solve for density pmu.
		solveDensity(pzDrops, pzDropsp, pzDropspp, pwDrops, RHS, pmu);
        }

	
	//Distribute mu etc to all process from master
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Bcast(pmu,NBR_PANEL_POINTS,MPI_DOUBLE,0,MPI_COMM_WORLD);
	MPI_Bcast(pwDrops,NBR_PANEL_POINTS,MPI_DOUBLE,0,MPI_COMM_WORLD);
	MPI_Bcast(pzDrops,NBR_PANEL_POINTS,MPI_C_DOUBLE_COMPLEX,0,MPI_COMM_WORLD);
	MPI_Bcast(pzDropsp,NBR_PANEL_POINTS,MPI_C_DOUBLE_COMPLEX,0,MPI_COMM_WORLD);
	MPI_Bcast(pzDropspp,NBR_PANEL_POINTS,MPI_C_DOUBLE_COMPLEX,0,MPI_COMM_WORLD);
	MPI_Bcast(ppanels,NBR_PANELS+1,MPI_C_DOUBLE_COMPLEX,0,MPI_COMM_WORLD);
	MPI_Bcast(pumax,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);


        MPI_Scatter(pu,nbr_elements_proc,MPI_DOUBLE,pu_proc,nbr_elements_proc,MPI_DOUBLE,0,MPI_COMM_WORLD);
        MPI_Scatter(pz,nbr_elements_proc,MPI_C_DOUBLE_COMPLEX,pz_proc,nbr_elements_proc,MPI_C_DOUBLE_COMPLEX,0,MPI_COMM_WORLD);
	MPI_Scatter(pu_spec,nbr_elements_proc,MPI_DOUBLE,pu_spec_proc,nbr_elements_proc,MPI_DOUBLE,0,MPI_COMM_WORLD);
	MPI_Scatter(perrorvec,nbr_elements_proc,MPI_DOUBLE,perrorvec_proc,nbr_elements_proc,MPI_DOUBLE,0,MPI_COMM_WORLD);
	MPI_Scatter(pu_ana,nbr_elements_proc,MPI_DOUBLE,pu_ana_proc,nbr_elements_proc,MPI_DOUBLE,0,MPI_COMM_WORLD);

	//	double time = getTime();
        computeSolution(pmu, pz_proc, pwDrops, pzDrops, pzDropsp, pu_proc,nbr_elements_proc);
	//	printf("%-20s : %lf s\n","Time",getTime()-time);
      
	//Evaluate the solution pu_spec with special quadrature.
	specialquadlapl(pu_spec_proc, pu_proc, pmu, pz_proc, pzDrops, pzDropsp, pwDrops, ppanels,nbr_elements_proc);	


	//Compute the error perrorvec.
	computeError(perrorvec_proc, pu_proc, pu_spec_proc, pu_ana_proc, pumax,nbr_elements_proc);


	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Gather(pu_proc,nbr_elements_proc,MPI_DOUBLE,pu,nbr_elements_proc,MPI_DOUBLE,0,MPI_COMM_WORLD);
	MPI_Gather(pu_spec_proc,nbr_elements_proc,MPI_DOUBLE,pu_spec,nbr_elements_proc,MPI_DOUBLE,0,MPI_COMM_WORLD);
	MPI_Gather(perrorvec_proc,nbr_elements_proc,MPI_DOUBLE,perrorvec,nbr_elements_proc,MPI_DOUBLE,0,MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);



	//Free allocated space.
	free(pzDrops);
	free(pzDropsp);
	free(pzDropspp);
	free(ppanels);
	free(pwDrops);
	free(pmu);
	free(pu_proc); 
	free(pz_proc);
	free(pu_spec_proc);
	free(pu_ana_proc);
	free(perrorvec_proc);
	free(RHS);
	free(ptpar);
	free(pu_spec);
	free(pu_ana);
	free(perrorvec);
	free(pu);
	free(pz);
       


	MPI_Finalize();

	

	return 0;
}

