#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include "BIELaplace.h"
//#include <clapack.h>


/* 
    Define precomputed gauss-legendre nodes and weights of order 16 on intervall [-1,1]
*/
static const double gl_nodes[16] = {-0.989400934991650,
                -0.944575023073233, -0.865631202387832, -0.755404408355003,
                -0.617876244402644, -0.458016777657227, -0.281603550779259,
                -0.095012509837637, 0.095012509837637, 0.281603550779259,
                0.458016777657227, 0.617876244402644, 0.755404408355003,
                0.865631202387832, 0.944575023073233, 0.989400934991650};
static const double gl_weights[16] = {0.027152459411754,
                0.062253523938648, 0.095158511682493,0.124628971255534,
                0.149595988816577, 0.169156519395003, 0.182603415044924,
                0.189450610455068, 0.189450610455068, 0.182603415044924,
                0.169156519395003, 0.149595988816577, 0.124628971255534,
                0.095158511682493, 0.062253523938648, 0.027152459411754};

/*
  Set up computational doimain. To change boundary, introduce new shape functions.
  ptau refers to the pointer corresponding to tau. taup and taupp refers to paramterization of boundary with first and second derivative.
*/

void create_grid(double complex * pz){        
    int i,j;
    double t,r;


for(i = 0; i < NBR_R; i++) {
    r = i * 0.999 / (NBR_R - 1);
    for (j = 0; j < NBR_T; j++)
      {
           t = 2.0 * M_PI * j /(NBR_T - 1);
           pz[i * NBR_T + j] = r * (1.0 + 0.3 *  ccos(5.0 * (t + tStart))) * cexp(I * (t + tStart));
      }
  }

}


void tau(double complex * ptau, double * t, int N)
{
  int i;
  for(i = 0; i<N; i++)
    ptau[i] = (1.0 + 0.3 *  ccos(5.0 * (t[i] + tStart))) * cexp(I * (t[i] + tStart));
}



void taup(double complex * ptaup, double * t, int N)
{
  int i;
  for(i = 0; i<N; i++)
    ptaup[i] = (-1.5 * csin(5.0 * (t[i] + tStart)) + I * (1.0 + 0.3 * ccos(5 * (t[i] + tStart)))) * cexp(I * (t[i] + tStart));
}



void taupp(double complex * ptaupp, double * t, int N)
{
  int i;
  for(i = 0; i<N; i++)
    ptaupp[i] = cexp(I * (t[i] + tStart)) * (-1.0 - 7.8 * ccos(5.0 * (t[i] + tStart)) - (3.0 * I) * csin(5.0 * (t[i] + tStart)));
}


//void glwt(double complex * pzDrops, double complex * pzDropsp, double complex * pzDropspp, double * pwDrops, double * ptpar, double first, double last){

void gl16(double complex * pzDrops, double complex * pzDropsp, double complex * pzDropspp, double * pwDrops, double * ptpar, double complex * ppanels)
{
    int i,j;


    double first, last;

    double ztmp;
  
    ppanels[0] = 0;

    for(i = 0; i < NBR_PANELS; ++i)
    {
        ppanels[i + 1] = (i + 1) * 2 * M_PI / (double) NBR_PANELS;
    }

    /*
        Go through all panels, and add 16 newly mapped points to pzDrops etc through tau, 
        and save in pwDrops
    */
    for(i = 0; i < NBR_PANELS; i ++) {
        first = creal(ppanels[i]);
        last = creal(ppanels[i+1]);
        for (j = 0; j < 16; j++) {
            pwDrops[i*NBR_POINTS_PER_PANEL + j] = 0.5*(last-first)*gl_weights[j];
            ztmp = 0.5 * (first * (1 - gl_nodes[j]) + last*(1 + gl_nodes[j]));
            tau(&pzDrops[i*NBR_POINTS_PER_PANEL + j],&ztmp,1);
            taup(&pzDropsp[i*NBR_POINTS_PER_PANEL + j],&ztmp,1);
            taupp(&pzDropspp[i*NBR_POINTS_PER_PANEL + j],&ztmp,1);
        }
    }

}




void init_domain(double complex * pz, double complex * pzDrops, double complex * pzDropsp, double complex * pzDropspp, double complex * ppanels,double * ptpar,double * pwDrops)
{

  int i;
  
  create_grid(pz);
    
  
  gl16(pzDrops,pzDropsp,pzDropspp,pwDrops,ptpar,ppanels);
  

  
  for(i = 0; i<(NBR_PANELS + 1); i++)
    ppanels[i] = (1.0 + 0.3 *  ccos(5.0 * (ppanels[i] + tStart))) * cexp(I * (ppanels[i] + tStart));
 
  
}