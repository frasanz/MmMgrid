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
				sub_mesh[i][j]=1.0+1.0*sin(0.01*i*j);
			else if(value==44.0)
				sub_mesh[i][j]=j;
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
void show_structure(Element * element, Edge * edge, int number_element, int number_edges)
{
	int i;
	for(i=0;i<number_element;i++){
		printf("\t[INFO] Element %d[%d]  has edges %d %d %d\n",i,element[i].id,element[i].edge[0],element[i].edge[1],element[i].edge[2]);
	}
	for(i=0;i<number_edges;i++){
		printf("\t[INFO] Edge %d[%d] is used by elements %d %d and has vertex %d %d\n",i,edge[i].id,edge[i].element[0],edge[i].element[1],edge[i].node[0],edge[i].node[1]);
	}
}
void show_structure_elements(Element * element, int number_element)
{
	int i;
	for(i=0;i<number_element;i++){
		printf("\t[INFO] Element %d[%d]  has neighbor the elements  %d(%d %d) %d(%d %d) %d(%d %d)\n",i,element[i].id,
		element[i].nei[0][0],element[i].nei[0][1],element[i].nei[0][2],
		element[i].nei[1][0],element[i].nei[1][1],element[i].nei[1][2],
		element[i].nei[2][0],element[i].nei[2][1],element[i].nei[2][2]);
	}
}
