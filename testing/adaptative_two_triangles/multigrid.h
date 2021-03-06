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
							 int smooth_check,    // If 1, we will run the smooth_check
							 int restrict_check , // If 1, we will run the restrict_check
							 int interpolate_check); // If 1, we will run the interpolate_check

/* Structures */

typedef struct{
	double ** u;
	double ** u_gh;								//Ghosts nodes, will be allocated if needed
	double ** v;
	double ** v_gh;               //Ghosts nodes 
	double ** d;
	double ** d_gh;               //...
	double ** f; 
	double ** f_gh;								//...
	int number_nodes;							//Number of nodes of the mesh (depends of the level)
	int number_nodes_base;				//Number of nodes in the mesh in the base
} Mesh;

typedef struct{
	double ** u;
	double ** v;
	double ** d;
	double ** f;
	int number_nodes_base;
} One_edge;

typedef struct{
	double v[3][3];
} Operator;

typedef struct{
	int id;
	int node[3];
	int edge[3];
	int nei[3][3];
	int n_levels;
	Operator * operator;
	Mesh * mesh;
	double max_error;
} Element;

typedef struct{
	int id;
	int node[2];
	int element[2];
	int n_levels;
	One_edge * oe;
} Edge;

typedef struct{
	int id;
	double c[2];
} Vertex;

#endif /* __MULTIGRID_H_ */



