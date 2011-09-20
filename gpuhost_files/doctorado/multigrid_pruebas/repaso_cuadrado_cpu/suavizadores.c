/*
 * =====================================================================================
 *
 *       Filename:  suavizadores.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/09/10 02:19:36
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#include "suavizadores.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void suaviza(double **u, double **f, int iteraciones, const char *tipo, int dim)
{
	if(!strcmp(tipo,"gsrb"))
		suaviza_gsrb(u, f,iteraciones,dim);
	else
	{
		printf("[Erro] suavizador no encontrado\n");
		exit(-10);
	}
}

void suaviza_gsrb(double **u, double **f,int iteraciones,int dim)
{
	int i,j,k,l;
	double h2 = pow(1.0/(dim-1),2); // h^2
	for(k=0;k<iteraciones;k++)
	{
		for(l=0;l<2;l++) /* Una iteracion rojo, otra negro */
		{
			for(i=1;i<dim-1;i++) /* Para los puntos i interiores */
			{
				for(j=1+(i+l+1)%2;j<dim-1;j=j+2)
				{
					u[i][j]=0.25*(f[i][j]*h2+u[i-1][j]+u[i+1][j]+u[i][j-1]+u[i][j+1]);
				}
			}
		}
	}
}
