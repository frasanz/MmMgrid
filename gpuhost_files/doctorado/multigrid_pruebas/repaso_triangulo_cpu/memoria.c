/*
 * =====================================================================================
 *
 *       Filename:  memoria.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/09/10 21:20:59
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
#include <math.h>
#include "memoria.h"
#include "tipos.h"


void inicializa_malla(malla  * tr, int nivel)
{
	int i;
	tr->nivel=nivel;
	tr->dimension=pow(2,nivel+1)+1;
	tr->v=(double**)malloc(tr->dimension*sizeof(double*));
	for(i=0;i<tr->dimension;i++)
	{
		tr->v[i]=(double*)malloc((i+1)*sizeof(double));
	}
}
void inicializa_malla_c(malla  * tr, int nivel)
{
	int i,j;
	tr->nivel=nivel;
	tr->dimension=pow(2,nivel+1)+1;
	tr->v=(double**)malloc(tr->dimension*sizeof(double*));
	for(i=0;i<tr->dimension;i++)
	{
		tr->v[i]=(double*)malloc((i+1)*sizeof(double));
	}
	for(i=0;i<tr->dimension;i++)
	{
		for(j=0;j<=i;j++)
		{
			tr->v[i][j]=0.0;
		}
	}
}

void libera_malla(malla  * tr)
{
	int i;
	for(i=0;i<tr->dimension;i++)
	{
		free(tr->v[i]);
	}
	free(tr->v);
}
void inicializa_matriz(matriz * m, int dim)
{
	int i;
	m->dimension=dim;
	m->v=(double**)malloc(dim*sizeof(double*));
	for(i=0;i<dim;i++)
		m->v[i]=(double*)malloc(dim*sizeof(double));
}
void inicializa_matriz_c(matriz * m, int dim)
{
	int i,j;
	m->dimension=dim;
	m->v=(double**)malloc(dim*sizeof(double*));
	for(i=0;i<dim;i++)
		m->v[i]=(double*)malloc(dim*sizeof(double));
	for(i=0;i<dim;i++)
	{
		for(j=0;j<dim;j++)
		{
			m->v[i][j]=0.0;
		}
	}
}
void libera_matriz(matriz *m)
{
	int i;
	for(i=0;i<m->dimension;i++)
		free(m->v[i]);
	free(m->v);
}

