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
  
	int rank, nbr_proc, nbr_el_proc, nbr_extra_el, i;


	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&nbr_proc);
	nbr_el_proc = floor(NBR_DOMAIN_POINTS /(double) nbr_proc);
	int *  pnbr_el_proc =  malloc(nbr_proc * sizeof(int));
	int * displs =  malloc(nbr_proc * sizeof(int));
	nbr_extra_el= NBR_DOMAIN_POINTS % nbr_proc;

	
	
	

	
	if(nbr_extra_el){
	  printf("Inside if\n");
	  for(i = 0; i < nbr_proc; i ++){
	    displs[i] = (i != 0)?pnbr_el_proc[i-1]+displs[i-1]:0;
	    pnbr_el_proc[i] = (i < nbr_extra_el)?nbr_el_proc+1:nbr_el_proc;
	  }
	    
	}


	
	




   	// Allocate memory for complex double pointers.
	double complex * pzDrops = malloc(NBR_PANEL_POINTS * sizeof(complex double));
	double complex * pzDropsp = malloc(NBR_PANEL_POINTS * sizeof(complex double));
	double complex * pzDropspp = malloc(NBR_PANEL_POINTS * sizeof(complex double));
	double complex * ppanels = malloc((NBR_PANELS + 1) * sizeof(complex double));
	double complex * pz_proc = malloc(nbr_el_proc * sizeof(complex double));
	double complex *pz = NULL;

	// Allocate memory for double pointers.
	double * pwDrops = malloc(NBR_PANEL_POINTS * sizeof(double));
	double * pmu = malloc(NBR_PANEL_POINTS * sizeof(double));
        double * pu_proc = malloc(nbr_el_proc * sizeof(double));
        double * pu_spec_proc = malloc(nbr_el_proc * sizeof(double));
        double * pu_ana_proc = malloc(nbr_el_proc * sizeof(double));
        double * perrorvec_proc = malloc(nbr_el_proc * sizeof(double));
        double * pumax;	
	
	double * pu_spec = NULL;
	double * pu_ana = NULL;
	double * perrorvec = NULL;
	double * pu = NULL;
	double * RHS = NULL;
	double * ptpar = NULL;
	//Only for root.


	double umax;
	umax = 0; // Used to obtain relative error.
	pumax = &umax;




	if(rank == root){

		//Initialize domain arrays
		pz = malloc(NBR_DOMAIN_POINTS * sizeof(complex double));
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
	MPI_Bcast(pmu,NBR_PANEL_POINTS,MPI_DOUBLE,root,MPI_COMM_WORLD);
	MPI_Bcast(pwDrops,NBR_PANEL_POINTS,MPI_DOUBLE,root,MPI_COMM_WORLD);
	MPI_Bcast(pzDrops,NBR_PANEL_POINTS,MPI_C_DOUBLE_COMPLEX,root,MPI_COMM_WORLD);
	MPI_Bcast(pzDropsp,NBR_PANEL_POINTS,MPI_C_DOUBLE_COMPLEX,root,MPI_COMM_WORLD);
	MPI_Bcast(pzDropspp,NBR_PANEL_POINTS,MPI_C_DOUBLE_COMPLEX,root,MPI_COMM_WORLD);
	MPI_Bcast(ppanels,NBR_PANELS+1,MPI_C_DOUBLE_COMPLEX,root,MPI_COMM_WORLD);
	MPI_Bcast(pumax,1,MPI_DOUBLE,root,MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);

	if(nbr_extra_el){
	  MPI_Scatterv(pu,pnbr_el_proc, displs, MPI_DOUBLE,pu_proc,pnbr_el_proc[rank], MPI_DOUBLE,root,MPI_COMM_WORLD);
	  MPI_Scatterv(pz,pnbr_el_proc,displs,MPI_C_DOUBLE_COMPLEX,pz_proc,pnbr_el_proc[rank], MPI_C_DOUBLE_COMPLEX,root,MPI_COMM_WORLD);
	  MPI_Scatterv(pu_spec,pnbr_el_proc,displs,MPI_DOUBLE,pu_spec_proc,pnbr_el_proc[rank], MPI_DOUBLE,root,MPI_COMM_WORLD);
	  MPI_Scatterv(perrorvec,pnbr_el_proc,displs,MPI_DOUBLE,perrorvec_proc,pnbr_el_proc[rank], MPI_DOUBLE,root,MPI_COMM_WORLD);
	  MPI_Scatterv(pu_ana,pnbr_el_proc,displs,MPI_DOUBLE,pu_ana_proc,pnbr_el_proc[rank], MPI_DOUBLE,root,MPI_COMM_WORLD);

	//	double time = getTime();
        computeSolution(pmu, pz_proc, pwDrops, pzDrops, pzDropsp, pu_proc,nbr_el_proc);
	//	printf("%-20s : %lf s\n","Time",getTime()-time);
      
	//Evaluate the solution pu_spec with special quadrature.
	specialquadlapl(pu_spec_proc, pu_proc, pmu, pz_proc, pzDrops, pzDropsp, pwDrops, ppanels,nbr_el_proc);	


	//Compute the error perrorvec.
	computeError(perrorvec_proc, pu_proc, pu_spec_proc, pu_ana_proc, pumax, nbr_el_proc);

	printf("Before gatherv\n");
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Gatherv(pu_proc, pnbr_el_proc[rank], MPI_DOUBLE, pu, pnbr_el_proc, displs, MPI_DOUBLE, root, MPI_COMM_WORLD);
	printf("Before gatherv pu_spec_proc \n");
	MPI_Gatherv(pu_spec_proc, pnbr_el_proc[rank], MPI_DOUBLE, pu_spec, pnbr_el_proc, displs, MPI_DOUBLE, root, MPI_COMM_WORLD);
	printf("Before gatherv perrorvec_proc \n");
	MPI_Gatherv(perrorvec_proc, pnbr_el_proc[rank], MPI_DOUBLE, perrorvec, pnbr_el_proc, displs, MPI_DOUBLE, root, MPI_COMM_WORLD);
	printf("Gatherv complete\n");
	MPI_Barrier(MPI_COMM_WORLD);
	}else{
	  
        MPI_Scatter(pu,nbr_el_proc,MPI_DOUBLE,pu_proc,nbr_el_proc,MPI_DOUBLE,root,MPI_COMM_WORLD);
        MPI_Scatter(pz,nbr_el_proc,MPI_C_DOUBLE_COMPLEX,pz_proc,nbr_el_proc,MPI_C_DOUBLE_COMPLEX,root,MPI_COMM_WORLD);
	MPI_Scatter(pu_spec,nbr_el_proc,MPI_DOUBLE,pu_spec_proc,nbr_el_proc,MPI_DOUBLE,root,MPI_COMM_WORLD);
	MPI_Scatter(perrorvec,nbr_el_proc,MPI_DOUBLE,perrorvec_proc,nbr_el_proc,MPI_DOUBLE,root,MPI_COMM_WORLD);
	MPI_Scatter(pu_ana,nbr_el_proc,MPI_DOUBLE,pu_ana_proc,nbr_el_proc,MPI_DOUBLE,root,MPI_COMM_WORLD);

	//	double time = getTime();
        computeSolution(pmu, pz_proc, pwDrops, pzDrops, pzDropsp, pu_proc,nbr_el_proc);
	//	printf("%-20s : %lf s\n","Time",getTime()-time);
      
	//Evaluate the solution pu_spec with special quadrature.
	specialquadlapl(pu_spec_proc, pu_proc, pmu, pz_proc, pzDrops, pzDropsp, pwDrops, ppanels,nbr_el_proc);	


	//Compute the error perrorvec.
	computeError(perrorvec_proc, pu_proc, pu_spec_proc, pu_ana_proc, pumax,nbr_el_proc);


	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Gather(pu_proc,nbr_el_proc,MPI_DOUBLE,pu,nbr_el_proc,MPI_DOUBLE,root,MPI_COMM_WORLD);
	MPI_Gather(pu_spec_proc,nbr_el_proc,MPI_DOUBLE,pu_spec,nbr_el_proc,MPI_DOUBLE,root,MPI_COMM_WORLD);
	MPI_Gather(perrorvec_proc,nbr_el_proc,MPI_DOUBLE,perrorvec,nbr_el_proc,MPI_DOUBLE,root,MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);

}


	//Free allocated space.
	printf("Before free\n");
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
       	printf("Free complete\n");
	MPI_Barrier(MPI_COMM_WORLD);

	printf("Before MPI_Finalize\n");
	MPI_Finalize();
	printf("MPI_Finalize complete \n");
	

	return 0;
}

