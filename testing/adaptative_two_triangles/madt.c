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
#include "smooth_check.h"

extern int debug;                                // Debug level

int main(int argc, char ** argv)
{
	int i;
	char elements_file_name[256]="elements.txt";    // Default name of elements  input file
	char vertex_file_name[256]="vertex.txt";        // Default name of vertex    input file
	int levels = 5;																	// Default number of levels
	int element=-1;                                 // Special element
	int level=-1;                                   // number of levels of the special element
	char mode[1];                                   // The default multigrid mode
	int iterations=5;				   											// The default number of iterations
	int smooth_levels[2]={1,1};											// Default smooth levels
	int smooth_check=0;                             // If 1 run in smooth_check mode
	int restrict_check=0;														// If 1 run in restrict_check mode
	int interpolate_check=0;												// If 1 run in interpolate_check mode

	/* Variables de uso */
	const char *uso="Use: %s [Options]\n "
	 "Optional arguments:\n"
	 " -el elements_file_name     file where is defined the elements\n"
	 " -vx vertex_file_name       file where is defined the vertexs\n"
	 " -levels n                  default number of levels\n"
	 " -element m n               the element m has n levels\n"
	 " -mode f|v|w                multigrid mode, default v\n"
	 " -iter n                    the number of iterations\n"
	 " -sl n m                    smooth levels, default 1 1\n"
	 " -d int                     level of debug\n"
	 " -smooth_check              run in smooth_check mode\n"
	 " -restrict_check            check the restrict function\n"
	 " -interpolate_check         check the interpolate function\n"
	 " -h | --help                show this help\n\n";




	/* Default arguments */
	sprintf(mode,"%s","v");
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
		if(!strcmp(argv[i],"-levels")){
			levels=atoi(argv[++i]);
		}
		if(!strcmp(argv[i],"-element")){
			element=atoi(argv[++i]);
			level=atoi(argv[++i]);
			if(level<levels) // The special element has at least the default number of levels
				level=levels;
		}
		if(!strcmp(argv[i],"-mode")){
			sprintf(mode,"%s",argv[++i]);
		}
		if(!strcmp(argv[i],"-iter")){
			iterations=atoi(argv[++i]);
		}
		if(!strcmp(argv[i],"-sl")){
			smooth_levels[0]=atoi(argv[++i]);
			smooth_levels[1]=atoi(argv[++i]);
		}
		if(!strcmp(argv[i],"-smooth_check")){
			smooth_check=1;
		}
		if(!strcmp(argv[i],"-restrict_check")){
			restrict_check=1;
		}
		if(!strcmp(argv[i],"-interpolate_check")){
			interpolate_check=1;
		}
	}
	if(smooth_check){
		printf("**** Starting smooth_check ****\n\n");
		printf("[NOTE] This will run the desire smoother with the element\n");
		printf("[NOTE] in the level max(levels)\n");
		print_debug(0,"\t[INFO] elements file name: %s\n",elements_file_name);
		print_debug(0,"\t[INFO] vertex file name: %s\n",vertex_file_name);
	}
	else if(restrict_check){
		printf("**** Starting restrict_check ****\n\n");
		printf("[NOTE] This will run the restrict_check function\n");
		printf("[NOTE] in the level max(levels)\n");
		print_debug(0,"\t[INFO] elements file name: %s\n",elements_file_name);
		print_debug(0,"\t[INFO] vertex file name: %s\n",vertex_file_name);
	}
	else if(interpolate_check){
		printf("**** Starting interpolate_check ****\n\n");
		printf("[NOTE] This will run the interpolate_check function\n");
		printf("[NOTE] in the level max(levels)\n");
		print_debug(0,"\t[INFO] elements file name: %s\n",elements_file_name);
		print_debug(0,"\t[INFO] vertex file name: %s\n",vertex_file_name);
	}
	else{
		/* Print the program call */
		printf("**** Starting the multigrid algorithm ****\n"
				"\t[INFO] debug level=%d\n",debug);
		printf("\t[INFO] the default number of levels is %d\n",levels);
		print_debug(0,"\t[INFO] elements file name: %s\n",elements_file_name);
		print_debug(0,"\t[INFO] vertex file name: %s\n",vertex_file_name);
		print_debug(0,"\t[INFO] the mode is %s\n",mode);
		if(debug>0 && element >=0)
			printf("\t[INFO] The element %d will be refined until level %d\n",element, level);

	}
		/* Call multigrid */
		multigrid(elements_file_name,
		          vertex_file_name,
							levels,
							element,
							level,
							mode,
							iterations,
							smooth_levels,
							smooth_check,
							restrict_check,
							interpolate_check);

	return 0;
}
