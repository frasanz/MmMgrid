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
#include "ghost.h"

extern int debug;

void multigrid_kernel(Element * element,
		int levels,            // Default number of levels
		int ele,							 // Special element
		int lev,							 // Number of levels of the special element
		char * mode,           // v,w or f
		int * smooth_levels,
		int this_level,        // The next level
		int number_elements   // The number of elements
		)
{
	if(debug>8)
		printf("\t\tIn level %d\n",this_level);
	int e;
	int k;

	/* Definitions */
	int next_level;

	if(this_level==0){
		/* Solve exactly: We can solve exactly in level 2 or go beyond */
		//solve_exactly(element[e]);
	}
	else{
		next_level=this_level-1;

		/* Initialize meshs */
		for(e=0;e<number_elements;e++){
			if(e==ele || this_level < levels){
			initialize_sub_mesh(element[e].mesh[this_level].d,
					element[e].mesh[this_level].number_nodes_base,0.0);

			initialize_sub_mesh(element[e].mesh[this_level].v,
					element[e].mesh[this_level].number_nodes_base,0.0);

			initialize_sub_mesh(element[e].mesh[this_level-1].f,
					element[e].mesh[this_level-1].number_nodes_base,0.0);

			initialize_sub_mesh(element[e].mesh[this_level-1].u,
					element[e].mesh[this_level-1].number_nodes_base,0.0);
			}
		}


		/* First smooth TODO: smooth levels*/
		if(debug>10){
			for(e=0;e<number_elements;e++){
				printf("Element %d: u\n",e);
				show_sub_mesh(element[e].mesh[this_level].u,element[e].mesh[this_level].number_nodes_base);
			}
		}
		for(k=0;k<3;k++){
			u_to_ghost(element,this_level,number_elements);
			for(e=0;e<number_elements;e++){
				if(e==ele || this_level < levels){
					smooth_rgb(element[e],this_level,k);
				}
			}
		}
		if(debug>10){
			for(e=0;e<number_elements;e++){
				printf("Element %d: u\n",e);
				show_sub_mesh(element[e].mesh[this_level].u,element[e].mesh[this_level].number_nodes_base);
			}
		}
		u_to_ghost(element,this_level,number_elements);
		if(debug>10)
			print_ghost(element,number_elements,this_level);



		/* Calculate the defect */
		for(e=0;e<number_elements;e++){
			if(e==ele || this_level < levels){
				defect(element[e],this_level);
				if(debug>10)
					show_sub_mesh(element[e].mesh[this_level].d,element[e].mesh[this_level].number_nodes_base);
			}
		}

		/* Restrict the defect */
		for(e=0;e<number_elements;e++){
			if(e==ele || this_level < levels){
				restrict_one(element[e],this_level);
				if(debug>10)
					show_sub_mesh(element[e].mesh[this_level-1].f,element[e].mesh[this_level-1].number_nodes_base);
			}
		}


		/* Recall to multigrid */
		multigrid_kernel(element,levels,ele,lev,mode,smooth_levels,next_level,number_elements);
		if(debug>8)
			printf("\t\tReturn to level %d\n",this_level);

		/* Interpolate */
		for(e=0;e<number_elements;e++){
			if(e==ele || this_level < levels){
				interpolate_one(element[e],this_level);
				if(debug>10){
					printf("Element %d v\n",e);
					show_sub_mesh(element[e].mesh[this_level].v,element[e].mesh[this_level].number_nodes_base);
				}
			}
		}

		/* Add error */
		for(e=0;e<number_elements;e++){
			if(e==ele || this_level < levels){
				if(debug>10){
					printf("Element %d, pre add\n",e);
					show_sub_mesh(element[e].mesh[this_level].u,element[e].mesh[this_level].number_nodes_base);
				}
				add_error(element[e],this_level);
				if(debug>10){
					printf("Element %d, post add\n",e);
					show_sub_mesh(element[e].mesh[this_level].u,element[e].mesh[this_level].number_nodes_base);
				}
			}
		}


		/* Second smooth TODO: smooth levels*/
		if(debug>10){
			for(e=0;e<number_elements;e++){
				printf("Element %d\n",e);
				show_sub_mesh(element[e].mesh[this_level].u,element[e].mesh[this_level].number_nodes_base);
			}
		}
		for(k=0;k<3;k++){
			u_to_ghost(element,this_level,number_elements);
			for(e=0;e<number_elements;e++){
				if(e==ele || this_level < levels){
					smooth_rgb(element[e],this_level,k);
				}
			}
		}

		if(debug>10)
			for(e=0;e<number_elements;e++)
				show_sub_mesh(element[e].mesh[this_level].u,element[e].mesh[this_level].number_nodes_base);
		u_to_ghost(element,this_level,number_elements);

		/* In the top level, we recalculate the defect*/
		for(e=0;e<number_elements;e++){
			if(element[e].n_levels-1==this_level)
				defect(element[e],this_level);
		}
	}
}
