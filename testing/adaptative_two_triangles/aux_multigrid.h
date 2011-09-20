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
void calculate_max_defect(Element * element, int id);
void show_sub_mesh(double **, int number_nodes_base);
void bound_sub_mesh(double **,int number_nodes_base, double value);
void show_structure(Element * element, Edge * edge, int number_element, int number_edges);
void show_structure_elements(Element * element, int number_element);
void copy_u_to_edges(Element * element, Edge * edge, int number_element, int number_edges,int level);





#endif /* __AUX_MULTIGRID_H_*/
