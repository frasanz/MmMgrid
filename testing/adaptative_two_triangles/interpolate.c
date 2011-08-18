/*
 * =====================================================================================
 *
 *       Filename:  interpolate.c
 *
 *    Description:  ç
 *
 *        Version:  1.0
 *        Created:  15/08/11 16:44:07
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
extern int debug;

#include <stdio.h>
#include <stdlib.h>
#include "interpolate.h"
#include "multigrid.h"
#include "smoothers.h"
#include "io.h"
#include "error_number.h"
#include "aux_multigrid.h"

void interpolatecheck(Element * element)
{
	int i,j;
	int this_level = element[0].n_levels-1;
	FILE * f;

	initialize_sub_mesh(element[0].mesh[this_level-1].u,element[0].mesh[this_level-1].number_nodes_base,43.0);
	bound_sub_mesh(element[0].mesh[this_level-1].u,element[0].mesh[this_level-1].number_nodes_base,0.0);
	initialize_sub_mesh(element[0].mesh[this_level].v,element[0].mesh[this_level].number_nodes_base,0.0);
	interpolate_one(element[0],this_level);

	/*Write the original sub mesh*/
	if(!(f=fopen("original_sub_mesh","w"))){
		print_debug(0,"\t[ERROR] Can't create file \"original_sub_mesh\"\n","");
		exit(FILE_OPEN_ERROR);
	}
	for(i=0;i<element[0].mesh[this_level-1].number_nodes_base;i++){
		for(j=0;j<=i;j++){
			fprintf(f,"%d %d %f\n",2*i,2*j,element[0].mesh[this_level-1].u[i][j]);
		}
		fprintf(f,"\n");
	}
	fclose(f);
	/* Write interpolate_sub_mesh */
	if(!(f=fopen("interpolate_sub_mesh","w"))){
		print_debug(0,"\t[ERROR] Can't create file \"interpolate_sub_mesh\"\n","");
		exit(FILE_OPEN_ERROR);
	}
	for(i=0;i<element[0].mesh[this_level].number_nodes_base;i++){
		for(j=0;j<=i;j++){
			fprintf(f,"%d %d %f\n",i,j,element[0].mesh[this_level].v[i][j]);
		}
		fprintf(f,"\n");
	}
	fclose(f);

}

