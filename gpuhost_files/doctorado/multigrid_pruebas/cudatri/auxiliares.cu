/*
 * =====================================================================================
 *
 *       Filename:  auxiliares.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  30/11/10 19:42:36
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#include "auxiliares.h"
#include "multigrid.h"
#include <stdio.h>

void imprime_grid(Grid g, const char *nombre)
{
	int i,j;
	double *hg;
	double *opg;
	/* Reservamos memoria para traer el grid de la GPU al host */
	hg = (double *)malloc(g.size*sizeof(double));
	opg= (double *)malloc(9*sizeof(double));
	cudaMemcpy(hg,g.v,g.size*sizeof(double),cudaMemcpyDeviceToHost);
	cudaMemcpy(opg,g.op,9*sizeof(double),cudaMemcpyDeviceToHost);
	printf("%s(tamaño=%d)=\n",nombre,g.size);
	for(i=0;i<g.dim;i++)
	{
		for(j=0;j<g.dim;j++)
		{
			printf("(%d %d)%e ",i,j,hg[I(g.dim,i,j)]);
		}
		printf("\n");
	}
	printf("operador=\n");
	for(i=0;i<9;i++)
		printf("%e ",opg[i]);
	printf("\n");
	free(opg);
	free(hg);
}

