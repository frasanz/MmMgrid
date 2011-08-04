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
#define SIZE 2048

double ratio(double**u,double ant,int iter)
{
	double tmp=0.0;
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

void muestra(double**u)
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

void jacobi(double **u_new, double **u, double **f, double h)
{
	int i,j;
	for(i=1;i<SIZE-1;i++)
	{
		for(j=1;j<SIZE-1;j++)
		{
			u_new[i][j]=0.25*(
								h*h*f[i][j]+
								u[i-1][j]+
								u[i+1][j]+
								u[i][j-1]+
								u[i][j+1]);
		}
	}
}

int main()
{
	double ** u, ** u_new, **temp, ** f;
	double ant=1.0;
	int i,j;
	size_t size;
	double h = 1.0/SIZE;


	/* Reservamos memoria */
	size=SIZE*SIZE*sizeof(double);
	printf("Necesitamos %d Mb\n",3*size/1024/1024);
	u     = (double**)malloc(SIZE*sizeof(double*));
	u_new = (double**)malloc(SIZE*sizeof(double*));
	f     = (double**)malloc(SIZE*sizeof(double*));

	for(i=0;i<SIZE;i++)
	{
		u[i]    =(double*)malloc(SIZE*sizeof(double));
		u_new[i]=(double*)malloc(SIZE*sizeof(double));
		f[i]    =(double*)malloc(SIZE*sizeof(double));
	}

	/* Inicializamos */
	for(i=0;i<SIZE;i++)
	{
		for(j=0;j<SIZE;j++)
		{
			f[i][j]=0.0;
			u[i][j]=rand();
		}
	}
	for(i=0;i<SIZE;i++)
	{
		u[0][i]=0.0;
		u[i][0]=0.0;
		u[SIZE-1][i]=0.0;
		u[i][SIZE-1]=0.0;
	}

	/* Bucle principal, llamamos a JACOBI */
	for(i=0;i<100;i++)
	{
		jacobi(u_new,u,f,h);
		ant=ratio(u_new,ant,i);
		temp=u_new;
		u_new=u;
		u=temp;
	}

	/* Liberamos memoria */
	for(i=0;i<SIZE;i++)
	{
		free(u[i]);
		free(u_new[i]);
		free(f[i]);

	}
	free(u);
	free(u_new);
	free(f);
}

