/*
 * =====================================================================================
 *
 *       Filename:  multigrid.h
 *
 *    Description:  :w
 *
 *        Version:  1.0
 *        Created:  29/11/10 20:32:54
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef _MULTIGRID_H_
#define _MULTIGRID_H_ 1

#define I(d,i,j) (i)*(d)+(j)

typedef struct {
	double *v;  /* Para almacenar los valores */
	double *op; /* Para almacenar el operador */
	int dim;
	int size;
} Grid;

typedef struct {
	double v[3][2];
} Triangulo;
void multigrid(Grid *u, Grid *f, Grid *v, Grid *d, int m, double * max,double * h_def,int n_mallas);
#endif /* multigrid.h */



