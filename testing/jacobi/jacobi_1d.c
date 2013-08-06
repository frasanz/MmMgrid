/*
 * =====================================================================================
 *
 *       Filename:  jacobi_1d.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/08/13 20:24:58
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
#define SIZE 2048
#define ITER 100
#define STEP 10

void jacobi(float * u_new, float *u, float *f, float h){
	int i,j;
	for(i=1; i<SIZE-1;i++){
		for(j=1;j<SIZE-1;j++){
			u_new[i*SIZE+j]=0.25*(
				h*h*f[i*SIZE+j]
				   +u[(i-1)*SIZE+j]
					 +u[(i+1)*SIZE+j]
					 +u[i*SIZE+(j-1)]
					 +u[i*SIZE+(j+1)]
			);
		}
	}
}

float ratio(float *u, float ant, int iter){
	float tmp = 0.0;
	int i,j;
	for(i=0;i<SIZE;i++){
		for(j=0;j<SIZE;j++){
			if(u[i*SIZE+j]>tmp)
				tmp=u[i*SIZE+j];
		}
	}
	if(iter%STEP==0)
		printf("\titer =%d, ratio=%f, max=%f\n",iter, tmp/ant, tmp);
	return tmp;
}


int main(){
	float *u, *u_new, *temp, *f;
	float ant=1.0;
	int i,j;
	size_t size;
	float h = 1.0/SIZE;

	/* Memory allocation */
	size=SIZE*SIZE*sizeof(float);
	u      =(float*)malloc(size);
	u_new  =(float*)malloc(size);
	f      =(float*)malloc(size);

	printf("Necesitamos %d mb de memoria\n",3*size/1024/1024);
	size = SIZE*SIZE;
	
	/* Initialization */
	for(i=0; i<size; i++){
		f[i]=0.0;
		u[i]=rand();
	}

	/* Boundary conditions */
	for(i=0;i<SIZE;i++){
		u[i]              =0.0;
		u[(SIZE-1)*SIZE+i]=0.0;
		u[i*SIZE]         =0.0;
		u[i*SIZE+SIZE-1]  =0.0;
	}

	/* Principal loop, we can call jacobi */
	for(i=0; i< ITER ; i++){
		jacobi(u_new,u,f,h);
		ant = ratio(u_new, ant,i);
		temp=u_new;
		u_new=u;
		u=temp;
	}

	/* Free memory */
	free(u);
	free(u_new);
	free(f);
}
