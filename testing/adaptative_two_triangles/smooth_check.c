/*
 * =====================================================================================
 *
 *       Filename:  smooth_check.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/08/11 01:48:05
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
#include "smooth_check.h"
#include "multigrid.h"
#include "smoothers.h"
#include "io.h"
#include "error_number.h"

void smoothcheck(Element * element, int * smooth_levels,int iter)
{
	FILE *f;
	print_debug(3,"\t[WHERE] In function smoothcheck\n","");
	int i,j;
	int this_level=element[0].n_levels-1;
	char out_file[256];

	/* Write the pre smooth u*/
	sprintf(out_file,"original_sub_mesh_%d.out",iter);
	if(!(f=fopen(out_file,"w"))){
		print_debug(0,"\t[ERROR] Can't create file \"original_sub_mesh.out\"\n","");
		exit(FILE_OPEN_ERROR);
	}
	for(i=0;i<element[0].mesh[this_level].number_nodes_base;i++){
		for(j=0;j<=i;j++){
			fprintf(f,"%d %d %f\n",2*i,2*j,element[0].mesh[this_level].u[i][j]);
		}
		fprintf(f,"\n");
	}
	fclose(f);


	/* Do the smooth */
	for(i=0;i<smooth_levels[0];i++)
		smooth_rgb(element[0],this_level);

	/* Write the post smooth u*/
	sprintf(out_file,"post_smooth_sub_mesh_%d.out",iter);
	if(!(f=fopen(out_file,"w"))){
		print_debug(0,"\t[ERROR] Can't create file \"post_smooth_sub_mesh.out\"\n","");
		exit(FILE_OPEN_ERROR);
	}
	for(i=0;i<element[0].mesh[this_level].number_nodes_base;i++){
		for(j=0;j<=i;j++){
			fprintf(f,"%d %d %f\n",2*i,2*j,element[0].mesh[this_level].u[i][j]);
		}
		fprintf(f,"\n");
	}
	fclose(f);


	 defect(element[0],this_level);
}
