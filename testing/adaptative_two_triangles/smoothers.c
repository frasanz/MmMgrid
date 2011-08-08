/*
 * =====================================================================================
 *
 *       Filename:  smoothers.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/08/11 01:11:59
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
#include "smoothers.h"
#include "io.h"

/* TODO: You have to check this*/
void smooth(Element element, int this_level)
{
	int i,j;
	for(i=1;i<element.mesh[this_level].number_nodes_base-1;i++){
		for(j=1;j<i;j++){
			element.mesh[this_level].u[i][j]=
				(element.mesh[this_level].u[i][j]-
					(element.operator[this_level].v[0][0] * element.mesh[this_level].u[i-1][j-1] +
					 element.operator[this_level].v[0][1] * element.mesh[this_level].u[i-1][j  ] +
					 element.operator[this_level].v[1][0] * element.mesh[this_level].u[i  ][j-1] +
					 element.operator[this_level].v[1][2] * element.mesh[this_level].u[i  ][j+1] +
					 element.operator[this_level].v[2][1] * element.mesh[this_level].u[i+1][j]   +
					 element.operator[this_level].v[2][2] * element.mesh[this_level].u[i+1][j+1])
				)/element.operator[this_level].v[1][1];
		}
	}
}

