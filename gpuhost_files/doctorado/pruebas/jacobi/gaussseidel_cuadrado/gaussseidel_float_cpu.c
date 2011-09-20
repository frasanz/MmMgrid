#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define SIZE 16

void jacobi_rojo(float *u, float *f, float h,float pr)
{
	int i,j;
	for(i=0;i<=SIZE-1;i++)
	{
		for(j=0;j<=SIZE-1;j++)
		{
			if((i+j)%2==0)
			{
			u[i*SIZE+j]=0.25*(
								h*h*f[i*SIZE+j]+
								u[(i-1)*SIZE+j]+
								u[(i+1)*SIZE+j]+
								u[i*SIZE+j-1]+
								u[i*SIZE+j+1]);
			u[i*SIZE+j]=pr;
			}
		}
	}
}
void jacobi_negro(float *u, float *f, float h,float pr)
{
	int i,j;
	for(i=0;i<SIZE*SIZE;i++)
	{
		u[i*SIZE+j]=0.25*(
								h*h*f[i*SIZE+j]+
								u[(i-1)*SIZE+j]+
								u[(i+1)*SIZE+j]+
								u[i*SIZE+j-1]+
								u[i*SIZE+j+1]);
			u[i*SIZE+j]=pr;

	}
}
void imprime(float *u)
{
	int i,j;
	for(i=0;i<SIZE;i++)
	{
		for(j=0;j<SIZE;j++)
		{
			printf("%f ",u[i*SIZE+j]);
		}
		printf("\n");
	}
}


int main()
{
	float * h_u, *h_f;
	float suma=0.0;
	int i,j;
	float h=1.0/SIZE;
	size_t size;


	/* Reservamos memoria */
	size=SIZE*SIZE*sizeof(float);
	printf("Necesitamos %d Mb\n",2*size/1024/1024);

	/* Reservamos memoria para la CPU */
	h_u     = (float*)malloc(size);
	h_f     = (float*)malloc(size);

	/* Inicializamos */
	for(i=0;i<SIZE;i++)
	{
		for(j=0;j<SIZE;j++)
		{
			h_f[i*SIZE+j]=0.0;
			h_u[i*SIZE+j]=1.0*i;
		}
	}
	for(i=0;i<SIZE;i++)
	{
		h_u[i]=0.0;
		h_u[i*SIZE]=0.0;
		h_u[SIZE*(SIZE-1)+i]=0.0;
		h_u[i*SIZE+SIZE-1]=0.0;
	}

	/* Bucle principal - llamamos a Jacobi  */
	for(i=0;i<700;i++)
	{
		jacobi_rojo(h_u,h_f,h,1.0*i);
		jacobi_negro(h_u,h_f,h,1.0*i);
		if(i%10==0) 
			printf("iter=%d\n",i); 
			
	} 
	/* Comprobamos */
	for(i=0;i<SIZE*SIZE;i++)
		if(h_u[i]!=1.0*699)
			printf("Error en %d, h_u[%d]=%f\n",i,i,h_u[i]);

	imprime(h_u);




	/* Liberamos memoria */
	free(h_u);
	free(h_f);
}
