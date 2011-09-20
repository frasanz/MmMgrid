#include <stdio.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <math.h>
#include <omp.h>


void muestra_matriz (gsl_matrix * matriz)
{
	int i,j;
	for(i=0; i< matriz->size1; i++)
	{
		for(j=0; j< matriz->size2; j++)
		{
			printf("(%d,%d)=%3.7f ",i,j,gsl_matrix_get(matriz,i,j));
		}
		printf("\n");
	}
}
void suavizador(gsl_matrix * var1, gsl_matrix * var2, int iteraciones)
{
	int i,j,k,l;
	float h2=pow(1.0/(var1->size1-1),2);
	for(k=0;k<iteraciones;k++)
	{
		for(l=0;l<2;l++)
			for(i=1;i<var1->size1-1;i++)
			{
				#pragma omp parallel for shared(var1,var2) private(j)
				for(j=1+(i+l+1)%2;j<var1->size1-1;j=j+2)
				{
					gsl_matrix_set(var1,i,j,
					0.25*(gsl_matrix_get(var2,i,j)*h2+
						  gsl_matrix_get(var1,i-1,j)+
						  gsl_matrix_get(var1,i+1,j)+
						  gsl_matrix_get(var1,i,j-1)+
						  gsl_matrix_get(var1,i,j+1)
						)
					);
				}
			}

	}
}


int main(int argc, char **argv)
{
	int i,j;
	int mallas=3;
	int m=mallas-1;
	int elementos = pow(2,mallas)+1;
	gsl_matrix * f = gsl_matrix_calloc(elementos,elementos);
	gsl_matrix * u = gsl_matrix_calloc(elementos,elementos);
	int divisor = pow(elementos-1,2);
	for(i=1; i< elementos-1; i++)
	{
		for(j=1; j<elementos-1; j++)
		{
			gsl_matrix_set(u,i,j,cos(12.0*i*j));
		}
	}
	for(i=0;i<10; i++)
	{
		suavizador(u,f,1);
		muestra_matriz(u);
		printf("%d\n");
	}


}
