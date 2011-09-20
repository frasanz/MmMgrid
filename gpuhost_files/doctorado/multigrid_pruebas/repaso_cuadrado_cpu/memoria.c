/*
 * =====================================================================================
 *
 *       Filename:  memoria.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/08/10 11:53:38
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "memoria.h"

double *** 
	allocate3D( int dim1,
						 int dim2,
						 int dim3)
{
	int i,j;
	double *** matriz;
	matriz = (double***)malloc(dim1*sizeof(double**));
	if(matriz==NULL) {
		fprintf(stderr, "\nFallo en malloc 1\n");
		exit(EXIT_FAILURE);
	}
	for(i=0;i<dim1;i++)
	{
		matriz[i] = (double**)malloc(dim2*sizeof(double*));
		if(matriz[i]==NULL){
			fprintf(stderr,"\nFallo en malloc 2.\n ");
			exit(EXIT_FAILURE);
		}
		for(j=0;j<dim2;j++)
		{
			matriz[i][j] = (double*)malloc(dim3*sizeof(double));
			if(matriz[i][j]==NULL){
				fprintf(stderr,"\nFallo en malloc 3.\n ");
				exit(EXIT_FAILURE);
			}
		}
	}
	return matriz;
}
double **
	allocate2D( int dim1,
							int dim2)
{
	int i;
	double ** matriz;
	matriz = (double **)malloc(dim1*sizeof(double*));
	if(matriz==NULL) {
		fprintf(stderr,"\n Fallo en malloc (allocate2D)\n ");
		exit(EXIT_FAILURE);
	}
	for(i=0;i<dim1;i++)
	{
		matriz[i] = (double*)malloc(dim2*sizeof(double));
		if(matriz[i]==NULL){
			fprintf(stderr,"\n Fallo en malloc (allocate2D)\n ");
			exit(EXIT_FAILURE);
		}
	}
	return matriz;
}
double *
	allocate1D(int dim1)
{
	double * vector;
	vector = (double *)malloc(dim1*sizeof(double));
	if(vector==NULL){
		fprintf(stderr,"\n Fallo en malloc (allocate1D)\n ");
		exit(EXIT_FAILURE);
	}
	return vector;
}
void
	deallocate3D(double ***matriz,
							 int dim1,
							 int dim2)
{
	int i,j;
	for(i=0;i<dim1;i++)
	{
		for(j=0;j<dim2;j++)
			free(matriz[i][j]);
		free(matriz[i]);
	}
	free(matriz);

}
void deallocate2D (double **matriz, int dim1)
{
	int i;
	for(i=0;i<dim1;i++)
		free(matriz[i]);
	free(matriz);
}
void deallocate1D(double * vector)
{
	free(vector);
}

