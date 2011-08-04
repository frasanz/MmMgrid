/*
 * =====================================================================================
 *
 *       Filename:  aux.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/08/11 15:22:00
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
#include "error_number.h"
#include "multigrid.h"

int debug=0;

void print_debug(int level, char * texto, const char * texto2) {
	if(debug>level)
		printf(texto, texto2);
}

int count_lines(const char * file_name)
{
	FILE *f;
	char char_line[1000];
	int lines=0;
	if(!(f=fopen(file_name,"r"))){
		print_debug(0,"\t[ERROR] The file %s doesn't exists, exiting...\n",file_name);
		exit(FILE_NOT_EXISTS);
	}
	while(fgets(char_line,1000,f)!=NULL)
		lines++;
	return lines;
}

void scan_elements(Element * element, const char * file_name)
{
	FILE * f;
	int i=0;
	if(!(f=fopen(file_name,"r"))){
		print_debug(0,"\t[ERROR] The file %s doesn't exists, exiting...\n", file_name);
		exit(FILE_NOT_EXISTS);
	}
	while(fscanf(f," %d %d %d %d\n",&element[i].id,
																	&element[i].node[0],
																	&element[i].node[1],
																	&element[i].node[2])!=EOF){

		if(debug>3)
			printf("\t[DEBUG] Element %d: %d %d %d\n",element[i].id,
			                                        element[i].node[0],
																							element[i].node[1],
																							element[i].node[2]);
		i++;
	}
}

void scan_vertex(Vertex * vertex, const char * file_name)
{
	FILE * f;
	int i=0;
	if(!(f=fopen(file_name,"r"))){
		print_debug(0,"\t[ERROR] The file %s doesn't exists, exiting...\n", file_name);
		exit(FILE_NOT_EXISTS);
	}
	while(fscanf(f,"%d %lf %lf",&vertex[i].id,&vertex[i].c[0], &vertex[i].c[1])!=EOF){
		if(debug>3)
			printf("\t[DEBUG] Vertex %d: %f %f\n",vertex[i].id, vertex[i].c[0], vertex[i].c[1]);
		i++;
	}
}

