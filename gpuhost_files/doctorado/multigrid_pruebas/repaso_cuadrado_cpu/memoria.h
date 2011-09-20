/*
 * =====================================================================================
 *
 *       Filename:  memoria.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/08/10 11:52:10
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

double *** 
	allocate3D(int , int , int );
double **
	allocate2D(int, int);
double *
	allocate1D(int);

void
	deallocate3D(double ***,int , int);
void
	deallocate2D(double **, int);
void
	deallocate1D(double *);


