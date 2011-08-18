/*
 * =====================================================================================
 *
 *       Filename:  aux_multigrid.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/08/11 01:26:24
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
#include "io.h"
#include "multigrid.h"
#include "aux_multigrid.h"

extern int debug;

void initialize_sub_mesh(double ** sub_mesh, int number_nodes_base, double value)
{
	int i,j;
	for(i=0;i<number_nodes_base;i++){
		for(j=0;j<=i;j++){
			if(value==42.0)
				sub_mesh[i][j]=1.0*rand();
			else if(value==43.0)
				sub_mesh[i][j]=1.0*sin(0.01*i*j);
			else
				sub_mesh[i][j]=value;
		}
	}
}

void bound_sub_mesh(double ** sub_mesh, int number_nodes_base, double value)
{
	int i;
	for(i=0;i<number_nodes_base;i++){
		sub_mesh[i][i]=value;
		sub_mesh[i][0]=value;
		sub_mesh[number_nodes_base-1][i]=value;
	}
}

void calculate_max_defect(Element * element,int id)
{
	int i,j;
	element[id].max_error=0.0;
	for(i=0;i<element[id].mesh[element[id].n_levels-1].number_nodes_base;i++){
		for(j=0;j<=i;j++){
			if((fabs(element[id].mesh[element[id].n_levels-1].d[i][j]))>element[id].max_error){
				element[id].max_error=fabs(element[id].mesh[element[id].n_levels-1].d[i][j]);
			}
		}
	}
}

void show_sub_mesh(double ** sub_mesh, int number_nodes_base)
{
	int i,j;
	for(i=0;i<number_nodes_base;i++){
		for(j=0;j<=i;j++){
			printf("%1.1e ",sub_mesh[i][j]);
		}
		printf("\n");
	}
}
