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

#ifndef __MULTIGRID_H_
#define __MULTIGRID_H_

/* Functions */
void multigrid(const char * element_file_name, 
               const char * vertex_file_name, 
						   int levels, 
							 int element,   // Special element
							 int level,     // Number of levels of the special element
							 char * mode,   // w,v or f
							 int iterations,// Iterations of the algorithm
							 int * smooth_levels,
							 int smooth_check); // If 1, we will run the smooth_check

/* Structures */

typedef struct{
	double ** u;
	double ** v;
	double ** d;
	double ** f;
	int number_nodes;							//Number of nodes of the mesh (depends of the level)
	int number_nodes_base;				//Number of nodes in the mesh in the base
} Mesh;

typedef struct{
	double v[3][3];
} Operator;

typedef struct{
	int id;
	int node[3];
	int n_levels;
	Operator * operator;
	Mesh * mesh;
	double max_error;
} Element;

typedef struct{
	int id;
	double c[2];
} Vertex;

#endif /* __MULTIGRID_H_ */



