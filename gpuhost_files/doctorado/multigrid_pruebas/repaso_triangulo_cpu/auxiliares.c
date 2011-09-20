/*
 * =====================================================================================
 *
 *       Filename:  auxiliares.c
 *
 *    Description:   *
 *        Version:  1.0
 *        Created:  01/09/10 23:45:22
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
#include <stdlib.h>
#include "auxiliares.h"
#include "tipos.h"

void malla_cero(malla *tr,triangulo t)
{
	int i,j;
	double diferencia_x;
	double diferencia_y;
	double posicion_x;
	double posicion_y;
	for(i=0;i<tr->dimension;i++)
	{
		for(j=0;j<=i;j++)
		{
			tr->v[i][j]=0.0;
		}
	}
	diferencia_x=(t.v[1][0]-t.v[0][0])/(tr->dimension-1);
	diferencia_y=(t.v[1][1]-t.v[0][1])/(tr->dimension-1);
	for(i=0;i<tr->dimension;i++)
	{
		posicion_x=t.v[0][0]+i*diferencia_x;
		posicion_y=t.v[0][1]+i*diferencia_y;
		tr->v[tr->dimension-1][i]=posicion_x*posicion_x+posicion_y+posicion_y;
	}
	diferencia_x=(t.v[0][0]-t.v[2][0])/(tr->dimension-1);
	diferencia_y=(t.v[0][1]-t.v[2][1])/(tr->dimension-1);
	for(i=0;i<tr->dimension;i++)
	{
		posicion_x=t.v[2][0]+i*diferencia_x;
		posicion_y=t.v[2][1]+i*diferencia_y;
		tr->v[i][0]=posicion_x*posicion_x+posicion_y*posicion_y;
	}
	diferencia_x=(t.v[1][0]-t.v[2][0])/(tr->dimension-1);
	diferencia_y=(t.v[1][1]-t.v[2][1])/(tr->dimension-1);
	for(i=0;i<tr->dimension;i++)
	{
		posicion_x=t.v[2][0]+i*diferencia_x;
		posicion_y=t.v[2][1]+i*diferencia_y;
		tr->v[i][i]=posicion_x*posicion_x+posicion_y*posicion_y;
	}

}
void malla_cero2(malla *tr)
{
	int i,j;
	double diferencia_x;
	double diferencia_y;
	double posicion_x;
	double posicion_y;
	for(i=0;i<tr->dimension;i++)
	{
		for(j=0;j<=i;j++)
		{
			tr->v[i][j]=0.0;
		}
	}
}
void malla_solucion(malla *tr, triangulo t)
{
	int i,j;
	double diferencia_10_x;
	double diferencia_10_y;
	double diferencia_02_x;
	double diferencia_02_y;
	double posicion_x;
	double posicion_y;
	diferencia_10_x=(t.v[1][0]-t.v[0][0])/(tr->dimension-1);
	diferencia_10_y=(t.v[1][1]-t.v[0][1])/(tr->dimension-1);
	diferencia_02_x=(t.v[0][0]-t.v[2][0])/(tr->dimension-1);
	diferencia_02_y=(t.v[0][1]-t.v[2][1])/(tr->dimension-1);
	for(i=0;i<tr->dimension;i++)
	{
		for(j=0;j<=i;j++)
		{
			posicion_x=t.v[2][0]+j*diferencia_10_x+i*diferencia_02_x;
			posicion_y=t.v[2][1]+j*diferencia_10_y+i*diferencia_02_y;
			tr->v[i][j]=posicion_x*posicion_x+posicion_y*posicion_y;
		}
	}
}
void malla_random(malla *tr)
{
	int i,j;
	for(i=0;i<tr->dimension;i++)
	{
		for(j=0;j<=i;j++)
		{
			if(i==tr->dimension-1 || j==0 || j==i)
				tr->v[i][j]=0.0;
			else
				tr->v[i][j]=i+j;
		}
	}
}
void malla_one(malla *tr)
{
	int i,j;
	for(i=0;i<tr->dimension;i++)
	{
		for(j=0;j<=i;j++)
		{
			if(i==tr->dimension-1 || j==0 || j==i)
				tr->v[i][j]=0.0;
			else
				tr->v[i][j]=1.0;
		}
	}
}

void malla_efe(malla *tr)
{
	int i,j;
	for(i=0;i<tr->dimension;i++)
	{
		for(j=0;j<=i;j++)
		{
			tr->v[i][j]=-4.0;
		}
	}
}
void escala_matriz(matriz *m,double escala)
{
	int i,j;
	for(i=0;i<m->dimension;i++)
	{
		for(j=0;j<m->dimension;j++)
		{
			m->v[i][j]=m->v[i][j]*escala;
		}
	}
}
void suma_matrices(matriz *m, matriz *n)
{
	int i,j;
	for(i=0;i<m->dimension;i++)
	{
		for(j=0;j<m->dimension;j++)
		{
			m->v[i][j]=m->v[i][j]+n->v[i][j];
		}
	}
}
void copia_matriz(matriz *ori, matriz *dest)
{
	int i,j;
	for(i=0;i<ori->dimension;i++)
	{
		for(j=0;j<ori->dimension;j++)
		{
			dest->v[i][j]=ori->v[i][j];
		}
	}
}
void suma_mallas(malla *m, malla *n)
{
	int i,j;
	for(i=0;i<m->dimension;i++)
	{
		for(j=0;j<=i;j++)
		{
			m->v[i][j]=m->v[i][j]+n->v[i][j];
		}
	}
}


