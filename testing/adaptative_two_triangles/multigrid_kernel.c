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
		int this_level,         // The next level
		int number_elements    // The number of elements
		)
{
	if(debug>8)
		printf("\t\tIn level %d\n",this_level);
	int i=0;
	int e;

	/* Definitions */
	int next_level;

	if(this_level==1){
		/* Solve exactly: We can solve exactly in level 2 or go beyond */
		//solve_exactly(element[e]);
	}
	else{
		next_level=this_level-1;

		/* Initialize mesh */
		for(e=0;e<number_elements;e++){
			initialize_sub_mesh(element[e].mesh[this_level].d,
					element[e].mesh[this_level].number_nodes_base,0.0);

			initialize_sub_mesh(element[e].mesh[this_level].v,
					element[e].mesh[this_level].number_nodes_base,0.0);

			initialize_sub_mesh(element[e].mesh[this_level-1].f,
					element[e].mesh[this_level-1].number_nodes_base,0.0);

			initialize_sub_mesh(element[e].mesh[this_level-1].u,
					element[e].mesh[this_level-1].number_nodes_base,0.0);
		}

		/* First smooth */
		for(e=0;e<number_elements;e++){
			if(debug>10)
				show_sub_mesh(element[e].mesh[this_level].u,element[e].mesh[this_level].number_nodes_base);
			for(i=0;i<smooth_levels[0];i++)
				smooth_rgb(element[e],this_level);
			if(debug>10)
				show_sub_mesh(element[e].mesh[this_level].u,element[e].mesh[this_level].number_nodes_base);
		}

		/* Calculate the defect */
		for(e=0;e<number_elements;e++){
			defect(element[e],this_level);
			if(debug>10)
				show_sub_mesh(element[e].mesh[this_level].d,element[e].mesh[this_level].number_nodes_base);
		}

		/* Restrict the defect */
		for(e=0;e<number_elements;e++){
			restrict_one(element[e],this_level);
			if(debug>10)
				show_sub_mesh(element[e].mesh[this_level-1].f,element[e].mesh[this_level-1].number_nodes_base);
		}

		/* Recall to multigrid */
		multigrid_kernel(element,levels,ele,lev,mode,smooth_levels,next_level,number_elements);
		if(debug>8)
			printf("\t\tReturn to level %d\n",this_level);

		/* Interpolate */
		for(e=0;e<number_elements;e++){
			interpolate_one(element[e],this_level);
			if(debug>10)
				show_sub_mesh(element[e].mesh[this_level].v,element[e].mesh[this_level].number_nodes_base);
		}

		/* Add error */
		for(e=0;e<number_elements;e++){
			if(debug>10)
				show_sub_mesh(element[e].mesh[this_level].u,element[e].mesh[this_level].number_nodes_base);
			add_error(element[e],this_level);
			if(debug>10)
				show_sub_mesh(element[e].mesh[this_level].u,element[e].mesh[this_level].number_nodes_base);
		}


		/* Second smooth */
		for(e=0;e<number_elements;e++){
			for(i=0;i<smooth_levels[1];i++)
				smooth_rgb(element[e],this_level);
			if(debug>10)
				show_sub_mesh(element[e].mesh[this_level].u,element[e].mesh[this_level].number_nodes_base);
		}

		/* In the top level, we recalculate the defect*/
		for(e=0;e<number_elements;e++){
			if(element[e].n_levels-1==this_level)
				defect(element[e],this_level);
			if(debug>10)
				show_sub_mesh(element[e].mesh[this_level].d,element[e].mesh[this_level].number_nodes_base);
		}
	}
}
