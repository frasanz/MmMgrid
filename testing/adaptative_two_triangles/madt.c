/*
 * =====================================================================================
 *
 *       Filename:  madt.c
 *
 *    Description:  Multigrid adaptativo dos triángulos
 *
 *        Version:  1.0
 *        Created:  04/08/11 14:17:16
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Francisco Sanz <frasanz@bifi.es>
 *        Company:  
 *
 * =====================================================================================
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"
#include "multigrid.h"

extern int debug;                                    // Debug level

int main(int argc, char ** argv)
{
	int i;
	char elements_file_name[256]="elements.txt";    // Default name of elements  input file
	char vertex_file_name[256]="vertex.txt";        // Default name of vertex    input file

	/* Variables de uso */
	const char *uso="Use: %s [Options]\n "
	 "Optional arguments:\n"
	 " -el elements_file_name     file where is defined the elements\n"
	 " -vx vertex_file_name       file where is defined the vertes\n"
	 " -d int                     level of debug\n\n";


	/* Parse of arguments */
	if((argc>1) && (!strcmp(argv[1],"-h") || !strcmp(argv[1], "--help"))) {
		printf (uso, argv[0]);
		exit(1);
	}
	for(i=1; i<argc; i++){
		if(!strcmp(argv[i],"-el")) {
			sprintf(elements_file_name,"%s",argv[++i]);
		}
		if(!strcmp(argv[i],"-vx")) {
			sprintf(vertex_file_name,"%s",argv[++i]);
		}
		if(!strcmp(argv[i],"-d")) {
			debug=atoi(argv[++i]);
		}
	}

	/* Print the program call */
	printf("**** Starting the multigrid algorithm ****\n"
	       "\tdebug level=%d\n",debug);
	print_debug(0,"\telements file name: %s\n",elements_file_name);
	print_debug(0,"\tvertex file name: %s\n",vertex_file_name);

	/* Call multigrid */
	multigrid(elements_file_name, vertex_file_name);

	return 0;
}
