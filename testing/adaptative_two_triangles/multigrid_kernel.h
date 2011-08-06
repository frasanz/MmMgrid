/*
 * =====================================================================================
 *
 *       Filename:  multigrid.h
 *
 *    Description:  main multrigrid algorithm
 *
 *        Version:  1.0
 *        Created:  04/08/11 15:34:40
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef __MULTIGRID_KERNEL_H_
#define __MULTIGRID_KERNEL_H_

#include "multigrid.h"


/* Functions */
void multigrid_kernel(Element * element,
											int levels,            // Default number of levels
											int ele,							 // Special element
											int lev,							 // Number of levels of the special element
											char * mode,           // v,w or f
											int * smooth_levels,
											int next_level         // The next level
											);

#endif /* __MULTIGRID_H_ */



