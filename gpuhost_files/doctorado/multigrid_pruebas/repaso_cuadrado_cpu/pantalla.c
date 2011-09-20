/*
 * =====================================================================================
 *
 *       Filename:  pantalla.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/09/10 01:48:18
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#include <stdio.h>

void muestra_matriz(char * nombre, double ** matriz, int dim1, int dim2)
{
	int i,j;
	printf("%s=\n",nombre);
	for(i=0;i<dim1;i++)
	{
		for(j=0;j<dim1;j++)
		{
			printf("%1.4f\t",matriz[i][j]);
		}
		printf("\n");
	}
}

