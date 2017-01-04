/*
Calculate density mu from the boundary integral formulation for Laplace
eq. with Dirichlet boundary condition (RHS)
Singular integrals calcualted with limit points
The system matrix is symmetric and positive definit.

*/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include "BIELaplace.h"
#include <gsl/gsl_linalg.h>
//#include <gsl/gsl_test.h>
//#include <gsl/gsl_splinalg.h>
//#include <clapack.h>

void call_gsl_gmres(gsl_matrix *A_gmres, double * RHS, double * X, const double epsrel, const int compress);


void solveDensity(double complex * pzDrops, double complex * pzDropsp, double complex * pzDropspp, double * pwDrops, double * RHS, double * pmu)
{
  int i,j;

  gsl_matrix *A_gmres = gsl_matrix_alloc(NBR_PANEL_POINTS, NBR_PANEL_POINTS); /* triplet format */
  
  //#pragma omp parallel for private(j) schedule(static,1)
  for (i = 0; i < NBR_PANEL_POINTS; ++i)
      for (j = 0; j < NBR_PANEL_POINTS; ++j)
          gsl_matrix_set(A_gmres, i, j, (i==j)?(1.0 / 2.0 + (1/(2 * M_PI)) * pwDrops[i] * cimag(pzDropspp[i]/(2 * pzDropsp[i]))):((1 / (2* M_PI)) * pwDrops[j] * cimag(pzDropsp[j]/(pzDrops[j] - pzDrops[i]))));


  const double epsrel = 1.0e-14;
  const int compress = 0;

  call_gsl_gmres(A_gmres, RHS, pmu, epsrel, compress);
  gsl_matrix_free(A_gmres);   
  
}






void call_gsl_gmres(gsl_matrix *A_gmres, double * RHS,double * pmu, const double epsrel, const int compress)
{
    int i;

    //Allocate space for structures need for int gsl_linalg_HH_solve (gsl_matrix * A, const gsl_vector * b, gsl_vector * x)
    gsl_vector *b = gsl_vector_alloc(NBR_PANEL_POINTS);        /* right hand side vector */
    gsl_vector *u = gsl_vector_calloc(NBR_PANEL_POINTS);       /* solution vector, u0 = 0 */
    
    for (i = 0; i < NBR_PANEL_POINTS; ++i)
      gsl_vector_set(b, i, RHS[i]);

    gsl_linalg_HH_solve(A_gmres,b,u); 


    for (i = 0; i < NBR_PANEL_POINTS; ++i)
      pmu[i] = gsl_vector_get(u, i);


  //Free allocated arrays.
  
  gsl_vector_free(b);
  gsl_vector_free(u);

}




