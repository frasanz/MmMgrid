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
#include "io.h"
#include "multigrid.h"

extern int debug;

void initialize_sub_mesh(double ** sub_mesh, int number_nodes_base, double value)
{
	int i,j;
	for(i=0;i<number_nodes_base;i++){
		for(j=0;j<=i;j++){
			sub_mesh[i][j]=value;
		}
	}
}

void calculate_max(Element element)
{
	int i,j;
	for(i=0;i<element.mesh[element.n_levels-1].number_nodes_base;i++){
		for(j=0;j<=i;j++){
			if(fabs(element.mesh[element.n_levels-1].u[i][j])>element.max_error)
				element.max_error=element.mesh[element.n_levels-1].u[i][j];
		}
	}
}

