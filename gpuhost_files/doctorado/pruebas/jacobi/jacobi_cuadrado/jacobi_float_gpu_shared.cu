/*
 * =====================================================================================
 *
 *       Filename:  jacobi_cpu.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/05/11 02:30:51
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
#include <cuda.h>
#define SIZE 8192
#define BLOCK_SIZE 16

float ratio(float**u,float ant,int iter)
{
	float tmp=0.0;
	int i,j;
	for(i=0;i<SIZE;i++)
	{
		for(j=0;j<SIZE;j++)
		{
			if(u[i][j]>tmp)
				tmp=u[i][j];
		}
	}
	if(iter%10==0)
		printf(" iter=%d ratio=%f max=%f\n",iter,tmp/ant,tmp);
	return tmp;
}

void muestra(float**u)
{
	int i,j;
	for(i=0;i<SIZE;i++)
	{
		for(j=0;j<SIZE;j++)
		{
			printf("%f ",u[i][j]);
		}
		printf("\n");
	}
}
__global__ void jacobi(float *d_u_new,float *d_u, float *d_f,float h, float pr)
{

	__shared__ float bf[BLOCK_SIZE][BLOCK_SIZE];

	int i = blockIdx.x*blockDim.x + threadIdx.x;
	int j = blockIdx.y*blockDim.y + threadIdx.y;

	int k = threadIdx.x;
	int l = threadIdx.y;
	
	bf[k][l] = d_u[i*SIZE+j];

	__syncthreads();

	if(i>=0 && i < SIZE && j>=0 && j<SIZE)
	{
		d_u_new[i*SIZE+j]=0.25*(
											h*h*d_f[i    *SIZE+j     ]+
											      bf[k-1][l]+
														bf[k+1][l]+
														bf[k][l-1]+
														bf[k][l+1]);
		d_u_new[i*SIZE+j]=pr;
	}
}
int main()
{
	float * h_u, * h_f;
	float * d_u, *d_u_new, *d_f;
	float * temp;
	float ant=1.0;
	int i,j;
	size_t size;
	float h = 1.0/SIZE;


	/* Reservamos memoria */
	size=SIZE*SIZE*sizeof(float);
	printf("Necesitamos %d Mb\n",3*size/1024/1024);
	h_u     = (float*)malloc(size);
	h_f     = (float*)malloc(size);

	/* REservamos memoria GPU*/
	cudaMalloc(&d_u,size);
	cudaMalloc(&d_u_new,size);
	cudaMalloc(&d_f,size);


	/* Inicializamos */
	for(i=0;i<SIZE;i++)
	{
		for(j=0;j<SIZE;j++)
		{
			h_f[i*SIZE+j]=0.0;
			h_u[i*SIZE+j]=rand();
		}
	}
	for(i=0;i<SIZE;i++)
	{
		h_u[i]=0.0;
		h_u[i*SIZE]=0.0;
		h_u[SIZE*(SIZE-1)+i]=0.0;
		h_u[i*SIZE+SIZE-1]=0.0;
	}

	/* Copiamos la memoria del host a la GPU */
	cudaMemcpy(d_f,h_f,size,cudaMemcpyHostToDevice);
	cudaMemcpy(d_u,h_u,size,cudaMemcpyHostToDevice);
	cudaMemcpy(d_u_new,h_u,size,cudaMemcpyHostToDevice);

	/* Creamos el grid para el cálculo */
	dim3 dimBlock(BLOCK_SIZE,BLOCK_SIZE);
	dim3 dimGrid(SIZE/BLOCK_SIZE,SIZE/BLOCK_SIZE);

	/* Bucle principal, llamamos a JACOBI */
	for(i=0;i<100;i++)
	{
		jacobi<<<dimGrid,dimBlock>>>(d_u_new,d_u,d_f,h,1.0*i);
		if(i%10==0) 
			printf("iter=%d\n",i); 
	}

	/* Copiamos */
	cudaMemcpy(h_u,d_u_new,size,cudaMemcpyDeviceToHost);

	/* Comprobamos */
	for(i=0;i<SIZE*SIZE;i++)
		if(h_u[i]!=1.0*99)
			printf("Error en %d, h_u[%d]=%f\n",i,i,h_u[i]);

	/* Liberamos memoria */
	free(h_u);
	free(h_f);
	cudaFree(d_u_new);
	cudaFree(d_u);
	cudaFree(d_f);
}

