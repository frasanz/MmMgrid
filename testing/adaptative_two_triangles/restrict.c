/*
 * =====================================================================================
 *
 *       Filename:  restrict.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  15/08/11 16:07:39
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
#include "restrict.h"
#include "multigrid.h"
#include "smoothers.h"
#include "io.h"
#include "error_number.h"
#include "aux_multigrid.h"

void restrictcheck(Element * element){
	int i,j;
	int this_level = element[0].n_levels-1;
	FILE *f;

	initialize_sub_mesh(element[0].mesh[this_level].d,element[0].mesh[this_level].number_nodes_base,43.0);
	//bound_sub_mesh(element[0].mesh[this_level].d,element[0].mesh[this_level].number_nodes_base,0.0);
	initialize_sub_mesh(element[0].mesh[this_level-1].f,element[0].mesh[this_level-1].number_nodes_base,0.0);
	restrict_one(element[0],this_level);

	/*Write the original sub mesh*/
	if(!(f=fopen("original_sub_mesh","w"))){
		print_debug(0,"\t[ERROR] Can't create file \"original_sub_mesh\"\n","");
		exit(FILE_OPEN_ERROR);
	}
	for(i=0;i<element[0].mesh[this_level].number_nodes_base;i++){
		for(j=0;j<=i;j++){
			fprintf(f,"%d %d %f\n",i,j,element[0].mesh[this_level].d[i][j]);
		}
		fprintf(f,"\n");
	}
	fclose(f);
	/* Write restrict sub_mesh */
	if(!(f=fopen("restrict_sub_mesh","w"))){
		print_debug(0,"\t[ERROR] Can't create file \"restrict_sub_mesh\"\n","");
		exit(FILE_OPEN_ERROR);
	}
	for(i=0;i<element[0].mesh[this_level-1].number_nodes_base;i++){
		for(j=0;j<=i;j++){
			fprintf(f,"%d %d %f\n",2*i,2*j,element[0].mesh[this_level-1].f[i][j]);
		}
		fprintf(f,"\n");
	}
	fclose(f);
}


