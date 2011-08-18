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
#include "smooth_check.h"
#include "aux_multigrid.h"
#include "restrict.h"
#include "interpolate.h"
extern int debug;

void multigrid(const char * element_file_name, 
               const char * vertex_file_name, 
							 int levels, 
							 int ele, 
							 int lev,
							 char * mode,
							 int iterations,
							 int * smooth_levels,
							 int smooth_check,
							 int restrict_check,
							 int interpolate_check)
{
	print_debug(3,"\t[WHERE] In function multigrid\n","");
	int e,i,j,k,l,m;

	/* Definitions */
	Element * element;
	Vertex * vertex;
	int number_elements;
	int number_vertex;
	int n_bytes=0;  // To know the number of bytes used
	int next_level=(levels > lev ? levels-1 : lev-1); // The next level to calculate in the algorithm
	int this_level;
	double previous_max;

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
			element[i].mesh[j].number_nodes=l; //We save here the number of nodes
			element[i].mesh[j].number_nodes_base=k;

			if(!(element[i].mesh[j].u = (double**)malloc(k*sizeof(double*)))){
				print_debug(0,"\t[ERROR] Can't allocate nodes","");
				exit(CAN_NOT_ALLOCATE_MEMORY);
			}
			if(!(element[i].mesh[j].v = (double**)malloc(k*sizeof(double*)))){
				print_debug(0,"\t[ERROR] Can't allocate nodes","");
				exit(CAN_NOT_ALLOCATE_MEMORY);
			}
			if(!(element[i].mesh[j].d = (double**)malloc(l*sizeof(double*)))){
				print_debug(0,"\t[ERROR] Can't allocate nodes","");
				exit(CAN_NOT_ALLOCATE_MEMORY);
			}
			if(!(element[i].mesh[j].f = (double**)malloc(l*sizeof(double*)))){
				print_debug(0,"\t[ERROR] Can't allocate nodes","");
				exit(CAN_NOT_ALLOCATE_MEMORY);
			}
			for(m=0;m<k;m++){
				if(!(element[i].mesh[j].u[m] = (double*)malloc((m+1)*sizeof(double)))){
					print_debug(0,"\t[ERROR] Can't allocate nodes","");
					exit(CAN_NOT_ALLOCATE_MEMORY);
				}
				if(!(element[i].mesh[j].v[m] = (double*)malloc((m+1)*sizeof(double)))){
					print_debug(0,"\t[ERROR] Can't allocate nodes","");
					exit(CAN_NOT_ALLOCATE_MEMORY);
				}
				if(!(element[i].mesh[j].d[m] = (double*)malloc((m+1)*sizeof(double)))){
					print_debug(0,"\t[ERROR] Can't allocate nodes","");
					exit(CAN_NOT_ALLOCATE_MEMORY);
				}
				if(!(element[i].mesh[j].f[m] = (double*)malloc((m+1)*sizeof(double)))){
					print_debug(0,"\t[ERROR] Can't allocate nodes","");
					exit(CAN_NOT_ALLOCATE_MEMORY);
				}
			}
			n_bytes=n_bytes+4*k*k*sizeof(double);
		}
	}

	/* Build operator for each element */
	for(i=0;i<number_elements;i++){
		build_operator(i,element,vertex);
	}

	/* Print the number of bytes used */
	if(debug>1)
		printf("\t[INFO] We are using %f Mb for the nodes of the mesh\n",1.0*n_bytes/1024/1024);

	/* Initialize u y f in the finest level */
	for(e=0;e<number_elements;e++){
		this_level = element[e].n_levels-1;
		initialize_sub_mesh(element[e].mesh[this_level].f,
				element[e].mesh[this_level].number_nodes_base,0.0);

		initialize_sub_mesh(element[e].mesh[this_level].u,
				element[e].mesh[this_level].number_nodes_base,1.0);

		bound_sub_mesh(element[e].mesh[this_level].u,
				element[e].mesh[this_level].number_nodes_base,0.0);

	}


  /* Some checks modes */
	if(restrict_check){
		restrictcheck(element);
		exit(0);
	}
	else if(interpolate_check){
		interpolatecheck(element);
		exit(0);
	}
	


	/* We can now call the multigrid method */
	for(i=0;i<iterations;i++){

		printf("\t[INFO] Iteration # %d\n",i);
		if(!smooth_check){
			multigrid_kernel(element,levels,ele,lev,mode,smooth_levels,next_level);

			/* Check the errors */
			previous_max=element[0].max_error;
			calculate_max_defect(element,0);
			printf("\t\tIn level %d max_error=%f\n\t\tratio= %f\n",element[0].n_levels-1, element[0].max_error,element[0].max_error/previous_max);
		}else{
			if(debug>10){
				show_sub_mesh(element[0].mesh[element[0].n_levels-1].u,
						element[0].mesh[element[0].n_levels-1].number_nodes_base);
			}
			smoothcheck(element,smooth_levels,i);

			/* Check the errors */
			previous_max=element[0].max_error;
			calculate_max_defect(element,0);
			printf("\t[INFO] ratio= %f\n",element[0].max_error/previous_max);
		}
	}
	/* Free memory */
	for(i=0;i<number_elements;i++){
		for(j=0;j<element[i].n_levels;j++){
			for(m=0;m<element[i].mesh[j].number_nodes_base;m++){
				free(element[i].mesh[j].u[m]);
				free(element[i].mesh[j].v[m]);
				free(element[i].mesh[j].d[m]);
				free(element[i].mesh[j].f[m]);
			}
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

