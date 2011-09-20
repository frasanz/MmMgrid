/*
 * =====================================================================================
 *
 *       Filename:  ghost.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  28/08/11 00:18:53
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef __GHOST_H_
#define __GHOST_H_
#include "multigrid.h"
void u_to_ghost(Element * element, int level, int number_elements);
void print_ghost(Element * element, int number_elements, int level);

#endif /* __GHOST_H_ */

