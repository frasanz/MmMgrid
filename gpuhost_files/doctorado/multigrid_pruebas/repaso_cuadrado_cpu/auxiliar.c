/*
 * =====================================================================================
 *
 *       Filename:  auxiliar.c
 *
 *    Description:  :spli
 *
 *        Version:  1.0
 *        Created:  01/09/10 01:39:18
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <math.h>

void inicializa(double **f,int dimension)
{
	int i,j;
	int divisor=pow(dimension-1,2);
	for(i=1; i< dimension-1; i++)
	{
		for(j=1; j<dimension-1; j++)
		{
			f[i][j]=10.1+sin(i*cos(j));
		}
	}

}

void inicializa_cero(double **matriz, int dimension)
{
	int i,j;
	for(i=0;i<dimension;i++)
		for(j=0;j<dimension;j++)
			matriz[i][j]=0.0;
}
void calcula_defecto(double **d, double **f, double **u, int dimension)
{
	int i,j;
	double h2=pow(1.0/(dimension-1),2);
	for(i=1;i<dimension-1;i++)
	{
		for(j=1;j<dimension-1;j++)
		{
			d[i][j]=f[i][j]-(4*u[i][j]-u[i-1][j]-u[i+1][j]-u[i][j-1]-u[i][j+1])/h2;
		}
	}
}
void restringe(double **in, double **fin, int dim_final)
{
	int i,j;
	for(i=1;i<dim_final-1;i++)
	{
		for(j=1;j<dim_final-1;j++)
		{
			fin[i][j]=(4*in[2*i][2*j]+
			           2*(in[2*i-1][2*j]+
								    in[2*i+1][2*j]+
										in[2*i][2*j-1]+
										in[2*i][2*j+1])+
									   in[2*i-1][2*j-1]
										 +in[2*i-1][2*j+1]
										 +in[2*i+1][2*j-1]
										 +in[2*i+1][2*j+1])/16;
		}
	}
}
void interpola(double **in, double **fin, int dim_inicial,int dim_final)
{
	int i,j;
	for(i=0;i<dim_inicial;i++)
	{
		for(j=0;j<dim_inicial;j++)
		{
			fin[2*i][2*j]=in[i][j]; /* Aquí coincide */
			if(2*i+1<dim_final)
				fin[2*i+1][2*j]=(in[i][j]+in[i+1][j])/2;
			if(2*j+1<dim_final)
				fin[2*i][2*j+1]=(in[i][j]+in[i][j+1])/2;
			if(2*i+1<dim_final && 2*j+1<dim_final)
				fin[2*i+1][2*j+1]=(in[i][j]+in[i][j+1]+in[i+1][j]+in[i+1][j+1])/4;
		}
	}
}


void exacta(double **d, double **v)
{
	v[1][1]=d[1][1]/16;
}
void suma_matrices(double ** m1, double **m2, int dim)
{
	int i,j;
	for(i=0;i<dim;i++)
	{
		for(j=0;j<dim;j++)
		{
			m1[i][j]=m1[i][j]+m2[i][j];
		}
	}
}

void copia_matriz(double **d1, double **d2, int dim)
{
	int i,j;
	for(i=0;i<dim;i++)
	{
		for(j=0;j<dim;j++)
		{
			d2[i][j]=d1[i][j];
		}
	}
}



