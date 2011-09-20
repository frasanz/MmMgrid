#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <wilson_kernel.cu>

#define BLOCK_SIZE 32 
Representacion mul(Representacion a, Representacion b, u64 p)
{
	Representacion mul;
	mul.r2=(a.r2*b.r2)%p;
	mul.r1=(a.r2*b.r2)/p;
	mul.r1=(mul.r1+a.r1*b.r2+b.r1*a.r2)%p;
	return mul;
}

int main(int argc, char *argv)
{
	u64 primo=50000003;
	Representacion * salida;
	Representacion * g_salida;
	int i;

	/* Reservamos memoria */
	salida = (Representacion *)malloc(BLOCK_SIZE * sizeof(Representacion));
	cudaMalloc((void**)&g_salida,BLOCK_SIZE*sizeof(Representacion));

	/* A cero */
	//cero<<<1,BLOCK_SIZE>>>(g_salida);
	dim3 dimBlock(BLOCK_SIZE,BLOCK_SIZE);

	/* Calculamos */
	fac_mod_parll<<<1,dimBlock>>>(g_salida,primo,primo/(BLOCK_SIZE*BLOCK_SIZE-1));

	/* Copiamos la solucion */
	cudaMemcpy(salida,g_salida,BLOCK_SIZE*sizeof(Representacion),cudaMemcpyDeviceToHost);

	/* Los 256 los multiplicamos localmente */
	Representacion sol;
	sol.r1=1;
	sol.r2=1;
	for(i=0;i<BLOCK_SIZE;i++)
		sol=mul(sol,salida[i],primo);

	/* Imprimimos por pantalla */
	printf("%lld %lld\n",sol.r1,sol.r2);

	/* Liberamos memoria */
	cudaFree(g_salida);
	free(salida);

}
