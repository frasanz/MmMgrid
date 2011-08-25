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

int count_number_edges(Element * element, int number_elements)
{
	int i,j,k,l;
	int coincidencia=0;
	int number_edges=0;
	for(i=0;i<number_elements;i++){
		for(j=i+1;j<number_elements;j++){
			coincidencia=0;
			for(k=0;k<3;k++){
				for(l=0;l<3;l++){
					if(element[i].node[k]==element[j].node[l]){
						coincidencia++;
					}
				}
			}
			if(coincidencia==2)
				number_edges++;
		}
	}
	return number_edges;
}

void check_structure(Element * element, Edge * edge, int number_elements)
{
	int i,j,k,l;
	int coincidencia;
	int v1,v2,v3,v4;
	int edge_number=0;
	for(i=0;i<number_elements;i++){
		element[i].edge[0]=-1;
		element[i].edge[1]=-1;
		element[i].edge[2]=-1;
	}

	for(i=0;i<number_elements;i++){
		for(j=i+1;j<number_elements;j++){
			coincidencia=0;
			for(k=0;k<3;k++){
				for(l=0;l<3;l++){
					if(element[i].node[k]==element[j].node[l]){
						coincidencia++;
						if(coincidencia==1){
							v1=k;
							v2=l;
						} else if(coincidencia==2){
							v3=k;
							v4=l;
						}
					}
				}
			}
			if(coincidencia==2){
				edge[edge_number].id=edge_number;
				edge[edge_number].element[0]=i;
				edge[edge_number].element[1]=j;
				edge[edge_number].node[0]=element[i].node[v1];
				edge[edge_number].node[1]=element[i].node[v3];
				if(v1+v3==1)
					element[i].edge[0]=edge_number;
				else if(v1+v3==3)
					element[i].edge[1]=edge_number;
				else if(v1+v3==2)
					element[i].edge[2]=edge_number;
				if(v2+v4==1)
					element[j].edge[0]=edge_number;
				else if(v2+v4==3)
					element[j].edge[1]=edge_number;
				else if(v2+v4==2)
					element[j].edge[2]=edge_number;
				edge_number++;
			}
		}
	}
}

