/*
 * =====================================================================================
 *
 *       Filename:  multigrid_kernel.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/08/11 02:16:10
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "multigrid.h"
#include "multigrid_kernel.h"
#include "aux_multigrid.h"
#include "io.h"
#include "smoothers.h"

extern int debug;

void multigrid_kernel(Element * element,
											int levels,            // Default number of levels
											int ele,							 // Special element
											int lev,							 // Number of levels of the special element
											char * mode,           // v,w or f
											int * smooth_levels,
											int this_level         // The next level
                      )
{
	int i=0;
	int e=0;         // Number of element

	/* Definitions */
	int next_level;

	if(this_level==0){

	}
	else{
		next_level=this_level-1;

		/* Initialize mesh */
		initialize_sub_mesh(element[e].mesh[this_level].d,
		                element[e].mesh[this_level].number_nodes_base,0.0);

		initialize_sub_mesh(element[e].mesh[this_level].v,
										element[e].mesh[this_level].number_nodes_base,0.0);

		/* First smooth */
		for(i=0;i<smooth_levels[0];i++)
			smooth(element[e],this_level);

	 //multigrid_kernel(element,levels,ele,lev,mode,smooth_levels,next_level);
	}
}