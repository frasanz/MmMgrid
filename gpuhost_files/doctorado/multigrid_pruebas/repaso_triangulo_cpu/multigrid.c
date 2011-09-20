/*
 * =====================================================================================
 *
 *       Filename:  multigrid.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/09/10 19:01:59
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "memoria.h"
#include "pantalla.h"
#include "tipos.h"
#include "auxiliares.h"
#include "operador.h"
#include "suavizador.h"
void multigrid(malla * u, malla *f, matriz *op, double * max)
{
	int i,j;
	if(u->nivel==1)
	{

		/* Escalar el operador al nivel */
		matriz op_n;

		/* Inicializamos el operador */
		inicializa_matriz(&op_n,3);
		copia_matriz(op,&op_n);
		escala_matriz(&op_n,pow((u->dimension-1),2));

		/* Construimos el sistema a resolver */
		double A[3][3];
		A[0][0]=op_n.v[1][1];
		A[0][1]=op_n.v[2][1];
		A[0][2]=op_n.v[2][2];
		A[1][0]=op_n.v[0][1];
		A[1][1]=op_n.v[1][1];
		A[1][2]=op_n.v[1][2];
		A[2][0]=op_n.v[0][0];
		A[2][1]=op_n.v[1][0];
		A[2][2]=op_n.v[1][1];

		double B[3];
		B[0] = f->v[2][1]-op_n.v[1][2]*u->v[2][2]
				-op_n.v[0][1]*u->v[1][1]
				-op_n.v[0][0]*u->v[1][0]
				-op_n.v[1][0]*u->v[2][0];
		B[1] = f->v[3][1]-op_n.v[0][0]*u->v[2][0]
				 -op_n.v[1][0]*u->v[3][0]
				 -op_n.v[2][1]*u->v[4][1];
		B[2] = f->v[3][2]-op_n.v[0][1]*u->v[2][2]
				 -op_n.v[1][2]*u->v[3][3]
				 -op_n.v[2][2]*u->v[4][3]
				 -op_n.v[2][1]*u->v[4][2];

		/* Hacemos eliminación gausiana */
		A[1][1]=A[1][1]-A[0][1]*A[1][0]/A[0][0];
		A[1][2]=A[1][2]-A[0][2]*A[1][0]/A[0][0];
		B[1]=B[1]-B[0]*A[1][0]/A[0][0];
		A[2][1]=A[2][1]-A[0][1]*A[2][0]/A[0][0];
		A[2][2]=A[2][2]-A[0][2]*A[2][0]/A[0][0];
		B[2]=B[2]-B[0]*A[2][0]/A[0][0];
		A[2][2]=A[2][2]-A[1][2]*A[2][1]/A[1][1];
		B[2]=B[2]-B[1]*A[2][1]/A[1][1];

		/* Resolvemos */
		u->v[3][2]=B[2]/A[2][2];
		u->v[3][1]=(B[1]-A[1][2]*u->v[3][2])/A[1][1];
		u->v[2][1]=(B[0]-A[0][2]*u->v[3][2]-A[0][1]*u->v[3][1])/A[0][0];

	}
	else
	{
		/* Escalar el operador al nivel */
		matriz op_n;
		/* Definicion de las mallas */
		malla d; /* Para almacenar el defecto */
		malla d_; /* Para almacenar el defecto restringido */
		malla v;  
		malla v_;


		/* Inicializacióm mallas*/
		inicializa_malla_c(&d,u->nivel);
		inicializa_malla_c(&d_,u->nivel-1);
		inicializa_malla_c(&v,u->nivel);
		inicializa_malla_c(&v_,u->nivel-1);

		inicializa_matriz(&op_n,3);
		copia_matriz(op,&op_n);
		printf("sf=pow(%d,2)",u->dimension-1);
		escala_matriz(&op_n,pow((u->dimension-1),2));


		/* Suavizado */
		muestra_malla(u);
		printf("After smooth %f\n ",op_n.v[1][1]);
		suaviza_r(u,f,&op_n);
		suaviza_g(u,f,&op_n);
		suaviza_b(u,f,&op_n);
		muestra_malla(u);

		/* Defecto */
		defecto(u,f,&d,&op_n);
		printf("After defect %f\n ",op_n.v[1][1]);
		muestra_malla(&d);

		/* Restringimos */
		restringe(&d,&d_);

		/* Rellamada a multigrid */
		multigrid(&v_,&d_,op,max);

		/* Interpolamos */
		interpola(&v_,&v);

		/* Sumamos */
		suma_mallas(u,&v);

		/* Postsuavizamos */
		suaviza_r(u,f,&op_n);
		suaviza_g(u,f,&op_n);
		suaviza_b(u,f,&op_n);



		/* En la malla mas alta, comprobamos cómo va la cosa */
		if(u->nivel==2)
		{
			/* Calculamos el defecto */
			defecto(u,f,&d,&op_n);
			for(i=0;i<u->dimension;i++)
			{
				for(j=0;j<i;j++)
				{

					if(fabs(d.v[i][j])>max[0])
					{
						max[0]=fabs(d.v[i][j]);
					}
				}
			}
		}

		/* Liberacion de recursos */
		libera_matriz(&op_n);
		libera_malla(&d);
		libera_malla(&d_);
		libera_malla(&v);
		libera_malla(&v_);

	}
}

int main()
{
	int total_niveles=2;
	int nivel=total_niveles;
	int i,j;
	malla f;
	malla u;
	malla solucion;
	matriz  op;
	double max=1.0;
	double max_ant;
	inicializa_matriz_c(&op,3);
	triangulo basico;
	basico.v[0][0]=0.0;
	basico.v[0][1]=0.0;
	basico.v[1][0]=1.0;
	basico.v[1][1]=0.0;
	basico.v[2][0]=0.5;
	basico.v[2][1]=sin(M_PI/3);
	crea_operador(&basico, &op);
	printf("Operador:\n");
	muestra_matriz(&op);
	printf("\n");

	inicializa_malla(&f,nivel);
	inicializa_malla(&u,nivel);
	inicializa_malla(&solucion,nivel);
	malla_solucion(&solucion,basico);
/* 
	malla_cero(&u,basico);
	malla_efe(&f); */
	malla_one(&u);
	malla_cero2(&f);

	/* Bucle principal */
	for(i=1;i<=1;i++)
	{
		max_ant=max;
		max=0.0;
		printf("[ Iteración #%d ] ",i);
		multigrid(&u,&f,&op,&max);
		printf("max=%0.10e ratio=%f\n",max,max/max_ant);

	}
	max=0.0;
	for(i=0;i<u.dimension;i++)
		for(j=0;j<i;j++)
		{
			if(fabs(u.v[i][j]-solucion.v[i][j])>max)
				max=fabs(u.v[i][j]-solucion.v[i][j]);
		}
	printf("MAX=%0.30f\n",max);
	libera_malla(&f);
	libera_malla(&u);
	libera_malla(&solucion);
	return 0;
}
