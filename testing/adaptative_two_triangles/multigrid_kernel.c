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
#include "multigrid_kernel.h"
#include "io.h"

void multigrid_kernel(Element * element)
{
	int i;
	for(i=0;i<10;i++)
	{
		printf("\t[INFO] Iteracion # %d\n",i);
	}
}
