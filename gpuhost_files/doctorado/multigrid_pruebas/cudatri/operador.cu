#include <stdio.h>
#include "multigrid.h"
#include "io.h"
#include "operador.h"
void crea_operador(Triangulo tr,double * op)
{
	double C[2][2];
	double InvC[2][2];
	double Cb[2][2];
	double scale;
	double Sxx[3][3]={{0,0,0},{-1,2,-1},{0,0,0}};
	double Sxy[3][3]={{-1,1,0},{1,-2,1},{0,1,-1}};
	double Syy[3][3]={{0,-1,0},{0,2,0},{0,-1,0}};

	/* Calculo de la matriz afín */
	C[0][0]=tr.v[1][0]-tr.v[0][0];
	C[0][1]=tr.v[2][0]-tr.v[1][0];
	C[1][0]=tr.v[1][1]-tr.v[0][1];
	C[1][1]=tr.v[2][1]-tr.v[1][1];
	imprime_matriz(&C[0][0],2,"Matriz afín=");

	/* Cálculo de la inversa de la matriz afín */
	InvC[0][0]=C[1][1];
	InvC[0][1]=-C[0][1];
	InvC[1][0]=-C[1][0];
	InvC[1][1]=C[0][0];
	scale=1.0/(C[0][0]*C[1][1]-C[0][1]*C[1][0]);
	escala_matriz(&InvC[0][0],2,scale);
	imprime_matriz(&InvC[0][0],2,"Matriz inversa=");

	/* Cálculo de la matriz Cb */
	Cb[0][0]=InvC[0][0]*InvC[0][0]+InvC[0][1]*InvC[0][1];
	Cb[0][1]=InvC[0][0]*InvC[1][0]+InvC[0][1]*InvC[1][1];
	Cb[1][0]=InvC[1][0]*InvC[0][0]+InvC[0][1]*InvC[1][1];
	Cb[1][1]=InvC[1][0]*InvC[1][0]+InvC[1][1]*InvC[1][1];
	imprime_matriz(&Cb[0][0],2,"Cb=");

	/* Comienza el cálculo del operador */
	escala_matriz(&Sxx[0][0],3,Cb[0][0]);
	escala_matriz(&Sxy[0][0],3,0.5*(Cb[1][0]+Cb[0][1]));
	escala_matriz(&Syy[0][0],3,Cb[1][1]);
	imprime_matriz(&Sxx[0][0],3,"Sxx");
	imprime_matriz(&Sxy[0][0],3,"Sxy");
	imprime_matriz(&Syy[0][0],3,"Syy");

	//suma_matrices(op,3,&Sxx[0][0]);
	suma_matrices(op,3,&Sxx[0][0]);
	suma_matrices(op,3,&Syy[0][0]);
	suma_matrices(op,3,&Sxy[0][0]);
	imprime_matriz(op,3,"operador=");

}
void escala_matriz(double * matriz, int dim, double scale)
{
	int i,j;
	for(i=0;i<dim;i++)
		for(j=0;j<dim;j++)
			matriz[i*dim+j]=matriz[i*dim+j]*scale;
}
void suma_matrices(double * m1, int dim, double * m2)
{
	int i,j;
	for(i=0;i<dim;i++)
		for(j=0;j<dim;j++)
			m1[i*dim+j]=m1[i*dim+j]+m2[i*dim+j];
}
void iguala_matriz(double * m1, double * m2, int dim)
{
	int i,j;
	for(i=0;i<dim;i++)
		for(j=0;j<dim;j++)
			m2[i*dim+j]=m1[i*dim+j];
}

