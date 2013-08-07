/*
 * =====================================================================================
 *
 *       Filename:  test_01.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/08/13 19:29:35
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Francisco Sanz Garcia (fsg), francisco.sanz.g@gmail.com
 *        Company:  own
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define SIZE 8
#define VER 0
#define LH 1
#define LA 2
#define LO 3
#define TA 4
#define TO 5
#define NEL 8

typedef struct{
	double x;
	double y;
} punto;

int main(){
	int i,j,k;
	punto ***u;
	punto ***u_new;
	punto ***f;

	/* Lookup memory */
	u    =(punto ***)malloc(NEL*sizeof(punto**));
	u_new=(punto ***)malloc(NEL*sizeof(punto**));
	f    =(punto ***)malloc(NEL*sizeof(punto**));
	for(i=0;i<NEL;i++){
		u[i]      =(punto**)malloc(SIZE*sizeof(punto*));
		u_new[i]  =(punto**)malloc(SIZE*sizeof(punto*));
		f[i]      =(punto**)malloc(SIZE*sizeof(punto*));
		for(j=0;j<SIZE;j++){
			u[i][j]      =(punto*)malloc((SIZE-j)*sizeof(punto));
			u_new[i][j]  =(punto*)malloc((SIZE-j)*sizeof(punto));
			f[i][j]      =(punto*)malloc((SIZE-j)*sizeof(punto));
		}
	}
	printf("We need about %f mb of memory\n", 3.0*sizeof(punto)*NEL*SIZE*SIZE/2/1024/1024);

	/* Initialization */
	for(i=0;i<SIZE;i++){
		for(j=0;j<SIZE-i;j++){
			/* La componente VER*/
			f[VER][i][j].x=(i*cos(M_PI/3)+j*1.0)/(SIZE-1);
			f[VER][i][j].y=-sin(M_PI/3)*i*1.0/(SIZE-1);
			for(k=0;k<NEL;k++){
				u[k][i][j].x     =rand();
				u[k][i][j].y     =rand();
				u_new[k][i][j].x =0.0;
				u_new[k][i][j].y =0.0;
			}
		}
	}

	/* Show F components */
	for(i=SIZE-1; i>=0; i--){
		for(j=0; j<SIZE-i; j++){
			printf("(%.2f,%.2f)\t",f[0][i][j].x, f[0][i][j].y);
		}
		printf("\n");
	}


	/* Free memory */
	for(i=0;i<NEL;i++){
		for(j=0;j<SIZE;j++){
			free(u[i][j]);
			free(u_new[i][j]);
			free(f[i][j]);
		}
		free(u[i]);
		free(u_new[i][j]);
		free(f[i][j]);
	}

	return 0;
}

