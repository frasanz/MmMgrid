/*
 * =====================================================================================
 *
 *       Filename:  aux_multigrid.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/08/11 01:24:47
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef __AUX_MULTIGRID_H_
#define __AUX_MULTIGRID_H_
#include "multigrid.h"
void initialize_sub_mesh(double **,int number_nodes_base, double value);
void calculate_max(Element element);



#endif /* __AUX_MULTIGRID_H_*/