#include <stdio.h>
#define BLOCK_SIZE 16
#define DIM 4096 
__global__ void calcula_max(double * max)
{
	int x = blockIdx.x*blockDim.x + threadIdx.x;
	if(x<DIM)
	{
			max[x]=x;
	}
}

int main()
{
	int i;
	dim3 dimBlock(BLOCK_SIZE,BLOCK_SIZE);
	double * g;
	double * h;
	cudaMalloc((void**)&g,DIM*sizeof(double));
	h=(double *)malloc(DIM*sizeof(double));
	calcula_max<<<(DIM+BLOCK_SIZE-1)/dimBlock.x,BLOCK_SIZE>>>(g);
	cudaMemcpy(h,g,DIM*sizeof(double),cudaMemcpyDeviceToHost);
	for(i=0;i<DIM;i++)
		printf("%d %f\n",i,h[i]); 

	cudaFree(g);
	free(h);
}
