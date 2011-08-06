/*
 * =====================================================================================
 *
 *       Filename:  multigrid.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/08/11 15:35:28
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
#include "io.h"
#include "multigrid.h"
#include "error_number.h"
#include "operator.h"
#include "multigrid_kernel.h"

extern int debug;

void multigrid(const char * element_file_name, 
               const char * vertex_file_name, 
							 int levels, 
							 int ele, 
							 int lev,
							 char * mode,
							 int iterations,
							 int * smooth_levels)
{
	print_debug(3,"\t[WHERE] In function multigrid\n","");
	int i,j,k,l;

	/* Definitions */
	Element * element;
	Vertex * vertex;
	int number_elements;
	int number_vertex;
	int n_bytes=0;  // To know the number of bytes used
	int next_level=(levels > lev ? levels : lev); // The next level to calculate in the algorithm

	/* Count the number of elements */
	number_elements = count_lines(element_file_name);
	if(debug>2)
		printf("\t[INFO] Number of elements = %d\n",number_elements);

	/* Count the number of vertex */
	number_vertex = count_lines(vertex_file_name);
	if(debug>2)
		printf("\t[INFO] Number of vertex = %d\n",number_vertex);

	/* Memory malloc */
	if(!(element = (Element*)malloc(number_elements*sizeof(Element)))){
		print_debug(0,"\t[ERROR] Can't allocate element","");
		exit(CAN_NOT_ALLOCATE_MEMORY);
	}
	if(!(vertex = (Vertex*)malloc(number_vertex*sizeof(Vertex)))){
		print_debug(0,"\t[ERROR] Can't allocate vertex","");
		exit(CAN_NOT_ALLOCATE_MEMORY);
	}

	/* Scan elements */
	scan_elements(element,element_file_name);

	/* Scan vertex */
	scan_vertex(vertex, vertex_file_name);

	/* Now, we need a hierarchy of mesh for each element */
	for(i=0;i<number_elements;i++) {
		if(i==ele)
			element[i].n_levels=lev;
		else
			element[i].n_levels=levels;
		if(!(element[i].mesh = (Mesh*)malloc(element[i].n_levels*sizeof(Mesh)))){
			print_debug(0,"\t[ERROR] Can't allocate mesh","");
			exit(CAN_NOT_ALLOCATE_MEMORY);
		}
		if(!(element[i].operator = (Operator*)malloc(element[i].n_levels*sizeof(Operator)))){
			print_debug(0,"\t[ERROR] Can't allocate operators","");
			exit(CAN_NOT_ALLOCATE_MEMORY);
		}
		for(j=0;j<element[i].n_levels;j++){
			k=pow(2,j)+1; // k is the number of nodes in the base ofthe triangle
			l=k*(k+1)/2;  // l is the number of nodes in the mesh (in this level)
			if(!(element[i].mesh[j].u = (double*)malloc(l*sizeof(double)))){
				print_debug(0,"\t[ERROR] Can't allocate nodes","");
				exit(CAN_NOT_ALLOCATE_MEMORY);
			}
			if(!(element[i].mesh[j].v = (double*)malloc(l*sizeof(double)))){
				print_debug(0,"\t[ERROR] Can't allocate nodes","");
				exit(CAN_NOT_ALLOCATE_MEMORY);
			}
			if(!(element[i].mesh[j].d = (double*)malloc(l*sizeof(double)))){
				print_debug(0,"\t[ERROR] Can't allocate nodes","");
				exit(CAN_NOT_ALLOCATE_MEMORY);
			}
			if(!(element[i].mesh[j].f = (double*)malloc(l*sizeof(double)))){
				print_debug(0,"\t[ERROR] Can't allocate nodes","");
				exit(CAN_NOT_ALLOCATE_MEMORY);
			}
			n_bytes=n_bytes+4*l*sizeof(double);
		}
	}

	/* Build operator for each element */
	for(i=0;i<number_elements;i++){
		build_operator(i,element,vertex);
	}

	/* Print the number of bytes used */
	if(debug>1)
		printf("\t[INFO] We are using %f Mb for the nodes of the mesh\n",1.0*n_bytes/1024/1024);

	/* We can now call the multigrid method */
	for(i=0;i<iterations;i++){
		printf("\t[INFO] Iteration # %d\n",i);
		multigrid_kernel(element,levels,ele,lev,mode,smooth_levels,next_level);
	}

	/* Free memory */
	for(i=0;i<number_elements;i++){
		for(j=0;j<element[i].n_levels;j++){
			free(element[i].mesh[j].u);
			free(element[i].mesh[j].v);
			free(element[i].mesh[j].d);
			free(element[i].mesh[j].f);
		}
		free(element[i].mesh);
		free(element[i].operator);
	}
	free(element);
	free(vertex);
}

