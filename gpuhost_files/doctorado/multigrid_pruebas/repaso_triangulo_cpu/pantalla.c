/*
 * =====================================================================================
 *
 *       Filename:  pantalla.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/09/10 21:55:52
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include "tipos.h"
#include "pantalla.h"

void muestra_malla(malla * tr)
{
	int i,j;
	for(i=0;i<tr->dimension;i++)
	{
		for(j=0;j<=i;j++)
		{
			printf("(%d, %d) %1.1e ",i,j,tr->v[i][j]);
		}
		printf("\n");
	}
}
void muestra_matriz(matriz *m)
{
	int i,j;
	for(i=0;i<m->dimension;i++)
	{
		for(j=0;j<m->dimension;j++)
		{
			printf("%f ",m->v[i][j]);
		}
		printf("\n");
	}
}

