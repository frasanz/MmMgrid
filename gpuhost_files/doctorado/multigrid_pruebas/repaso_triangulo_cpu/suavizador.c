/*
 * =====================================================================================
 *
 *       Filename:  suavizador.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/09/10 02:20:06
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
#include "suavizador.h"

void suaviza(malla *u, malla *f, matriz *op)
{
	int i,j;
	for(i=1;i<u->dimension-1;i++)
	{
		for(j=1;j<=i-1;j++)
		{
			u->v[i][j]=(f->v[i][j]-(op->v[0][0]*u->v[i-1][j-1] +
						op->v[0][1]*u->v[i-1][j]   +
						op->v[1][0]*u->v[i][j-1]   +
						op->v[1][2]*u->v[i][j+1]   +
						op->v[2][1]*u->v[i+1][j]   +
						op->v[2][2]*u->v[i+1][j+1]  ))/op->v[1][1];
		}
	}
}
void suaviza_r(malla *u, malla *f, matriz *op)
{
	int i,j;
	for(i=1;i<u->dimension-1;i++)
	{
		for(j=1;j<=i-1;j++)
		{
			if((i+j)%3==0)
			{
				u->v[i][j]=(f->v[i][j]-(op->v[0][0]*u->v[i-1][j-1] +
							op->v[0][1]*u->v[i-1][j]   +
							op->v[1][0]*u->v[i][j-1] +
							op->v[1][2]*u->v[i][j+1]   +
							op->v[2][1]*u->v[i+1][j] +
							op->v[2][2]*u->v[i+1][j+1]   ))/op->v[1][1];
			}
		}
	}
}
void suaviza_g(malla *u, malla *f, matriz *op)
{
	int i,j;
	for(i=1;i<u->dimension-1;i++)
	{
		for(j=1;j<=i-1;j++)
		{
			if((i+j)%3==1)
			{
				u->v[i][j]=(f->v[i][j]-(op->v[0][0]*u->v[i-1][j-1] +
							op->v[0][1]*u->v[i-1][j]   +
							op->v[1][0]*u->v[i][j-1] +
							op->v[1][2]*u->v[i][j+1]   +
							op->v[2][1]*u->v[i+1][j] +
							op->v[2][2]*u->v[i+1][j+1]   ))/op->v[1][1];

			}
		}
	}
}
void suaviza_b(malla *u, malla *f, matriz *op)
{
	int i,j;
	for(i=1;i<u->dimension-1;i++)
	{
		for(j=1;j<=i-1;j++)
		{
			if((i+j)%3==2)
			{
				u->v[i][j]=(f->v[i][j]-(op->v[0][0]*u->v[i-1][j-1] +
							op->v[0][1]*u->v[i-1][j]   +
							op->v[1][0]*u->v[i][j-1] +
							op->v[1][2]*u->v[i][j+1]   +
							op->v[2][1]*u->v[i+1][j] +
							op->v[2][2]*u->v[i+1][j+1]   ))/op->v[1][1];

			}
		}
	}
}
void defecto(malla *u, malla *f, malla *d, matriz *op)
{
	int i,j;
	for(i=1;i<u->dimension-1;i++)
	{
		for(j=1;j<=i-1;j++)
		{
			d->v[i][j]=(f->v[i][j]-(
						op->v[0][0]*u->v[i-1][j-1]   +
						op->v[0][1]*u->v[i-1][j] +
						op->v[1][0]*u->v[i][j-1]   +
						op->v[1][1]*u->v[i][j]     +
						op->v[1][2]*u->v[i][j+1]   +
						op->v[2][1]*u->v[i+1][j] +
						op->v[2][2]*u->v[i+1][j+1]   ));
		}
	}
}
void restringe(malla *sup, malla *inf)
{
	int i,j;
	for(i=1;i<inf->dimension-1;i++) /* puntos interiores */
	{
		for(j=1;j<=i-1;j++)
		{
			inf->v[i][j]=(sup->v[2*i][2*j]+
					0.5*(sup->v[2*i-1][2*j-1]+
						sup->v[2*i-1][2*j]  +
						sup->v[2*i][2*j-1]  +
						sup->v[2*i][2*j+1]  +
						sup->v[2*i+1][2*j]  +
						sup->v[2*i+1][2*j+1]))/4;

		}
	}
}
void interpola(malla *inf, malla *sup)
{
	int i,j;
	for(i=1;i<inf->dimension-1;i++)
	{
		for(j=1;j<=i-1;j++)
		{
			sup->v[2*i][2*j]     =inf->v[i][j];
			sup->v[2*i-1][2*j-1] =(inf->v[i][j]+inf->v[i-1][j-1])/2;
			sup->v[2*i-1][2*j]   =(inf->v[i][j]+inf->v[i-1][j])/2;
			sup->v[2*i][2*j+1]   =(inf->v[i][j]+inf->v[i][j+1])/2;
			sup->v[2*i][2*j-1]   =(inf->v[i][j]+inf->v[i][j-1])/2;
			sup->v[2*i+1][2*j+1] =(inf->v[i][j]+inf->v[i+1][j+1])/2;
			sup->v[2*i+1][2*j]   =(inf->v[i][j]+inf->v[i+1][j])/2;

		}
	}
}

