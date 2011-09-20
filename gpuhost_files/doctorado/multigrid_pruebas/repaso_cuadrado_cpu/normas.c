/*
 * =====================================================================================
 *
 *       Filename:  normas.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/09/10 03:34:27
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
double calcula_max(double **x, int dimension)
{
	int i,j;
	double max=0.0;
	for(i=0;i<dimension;i++)
	{
		for(j=0;j<dimension;j++)
		{
			if(max<fabs(x[i][j]))
				max=fabs(x[i][j]);
		}
	}
	return max;
}


