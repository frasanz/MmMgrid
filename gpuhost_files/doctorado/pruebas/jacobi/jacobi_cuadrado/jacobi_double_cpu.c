#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define SIZE 8192

void jacobi(double *u_new, double *u, double *f, double h,double pr)
{
	int i,j;
	for(i=0;i<=SIZE-1;i++)
	{
		for(j=0;j<=SIZE-1;j++)
		{
			u_new[i*SIZE+j]=0.25*(
								h*h*f[i*SIZE+j]+
								u[(i-1)*SIZE+j]+
								u[(i+1)*SIZE+j]+
								u[i*SIZE+j-1]+
								u[i*SIZE+j+1]);
			u_new[i*SIZE+j]=pr;
		}
	}
}




int main()
{
	double * h_u, *h_u_new, *h_f;
	double * temp;
	double suma=0.0;
	int i,j;
	double h=1.0/SIZE;
	size_t size;


	/* Reservamos memoria */
	size=SIZE*SIZE*sizeof(double);
	printf("Necesitamos %d Mb\n",3*size/1024/1024);

	/* Reservamos memoria para la CPU */
	h_u     = (double*)malloc(size);
	h_u_new = (double*)malloc(size);
	h_f     = (double*)malloc(size);

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
		jacobi(h_u_new,h_u,h_f,h,1.0*i);
		if(i%10==0) 
			printf("iter=%d\n",i); 
			
	} 
	/* Comprobamos */
	for(i=0;i<SIZE*SIZE;i++)
		if(h_u_new[i]!=1.0*699)
			printf("Error en %d, h_u[%d]=%f\n",i,i,h_u[i]);




	/* Liberamos memoria */
	free(h_u);
	free(h_f);
	free(h_u_new);


}
