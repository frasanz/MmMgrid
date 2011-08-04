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
#include "io.h"
#include "multigrid.h"
#include "error_number.h"
#include "operator.h"
extern int debug;

void multigrid(const char * element_file_name, const char * vertex_file_name)
{
	print_debug(3,"\t[WHERE] In function multigrid\n","");
	int i;

	/* Definitions */
	Element * element;
	Vertex * vertex;
	int number_elements;
	int number_vertex;

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

	/* Build operator for each element */
	for(i=0;i<number_elements;i++)
		build_operator(i,element,vertex);

}

