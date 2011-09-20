/*
 * =====================================================================================
 *
 *       Filename:  io.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  30/11/10 21:50:42
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include "multigrid.h"
#include "io.h"

Triangulo lee_fichero_triangulo(char * archivo)
{
	Triangulo tr;
	FILE * f;
	f=fopen(archivo,"r");
	if(f==NULL)
	{
		printf("Error en la apertura del archivo de triangulos %s\n",archivo);
		exit(-1);
	}
	fscanf(f,"%lf %lf\n",&tr.v[0][0],&tr.v[0][1]);
	fscanf(f,"%lf %lf\n",&tr.v[1][0],&tr.v[1][1]);
	fscanf(f,"%lf %lf\n",&tr.v[2][0],&tr.v[2][1]);
	fclose(f);
	return tr;
}

void imprime_matriz(double * matriz, int dim, const char *nombre)
{
	int i,j;
	printf("%s\n",nombre);
	for(i=0;i<dim;i++)
	{
		for(j=0;j<dim;j++)
		{
			printf("%f ",matriz[dim*i+j]);
		}
		printf("\n");
	}
}
