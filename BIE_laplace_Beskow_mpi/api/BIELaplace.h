#ifndef BIELAPLACE_H_INCLUDED
#define BIELAPLACE_H_INCLUDED

#define tStart 0.0
#define NBR_R 33 // Resolution in radial direction
#define NBR_T 10 // Resolution in boundary parameterisation
#define NBR_DOMAIN_POINTS NBR_R  * NBR_T // Number of grid points in domain
#define NBR_PANELS 30 // Number of GL-panels
#define NBR_POINTS_PER_PANEL 16 // Number of points per GL-panel
#define NBR_PANEL_POINTS  NBR_POINTS_PER_PANEL *  NBR_PANELS // Useful abbrevation

void init_domain(double complex * pz, double complex * pzDrops, double complex * pzDropsp, double complex * pzDropspp, double complex * ppanels,double * ptpar, double * pwDrops);
void init_function(double * RHS, double * pu_ana, double complex * pzDrops, double complex * pz, double * pumax);
void solveDensity(double complex * pzDrops, double complex * pzDropsp, double complex * pzDropspp, double * pwDrops, double * RHS, double * pmu);
void computeSolution(double * pmu, double complex * pz, double * pwDrops, double complex * pzDrops, double complex * pzDropsp, double * pu, int nbr_elements);
void specialquadlapl(double * u_specq, double * u_standardq, double * pmu, double complex *zDom, double complex * pzDrops, double complex * pzDropsp, double * wDrops, double complex * ppanels, int nbr_elements);
void computeError(double * perrorvec, double * pu, double * pu_spec, double * pu_ana, double * pumax,int nbr_elements);

#endif
