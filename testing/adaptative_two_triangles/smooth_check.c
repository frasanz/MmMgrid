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

void smoothcheck(Element * element, int * smooth_levels)
{
	print_debug(3,"\t[WHERE] In function smoothcheck\n","");
	int i;
	int this_level=element[0].n_levels-1;

	/* Do the smooth */
	for(i=0;i<smooth_levels[0];i++)
		smooth(element[0],this_level);
}
