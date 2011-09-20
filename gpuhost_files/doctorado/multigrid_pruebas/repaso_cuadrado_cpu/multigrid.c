/*
 * =====================================================================================
 *
 *       Filename:  multigrid.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/09/10 01:07:34
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
#include <string.h>
#include "memoria.h"
#include "auxiliar.h"
#include "pantalla.h"
#include "suavizadores.h"
#include "normas.h"

void multigrid(int m, /* Malla en la que estamos */
		double ***d, /* Defecto */
		double ***v,
		int iteraciones_s1,
		int iteraciones_s2,
		const char * tipo) /* tipo de suavizador */
{
	int d_malla; /* dimension de la malla */
	int d_malla_inf; /* dimension de la malla inferior */
	double **fu;
	double **vg;
	if(m==0)
	{
		exacta(d[m],v[m]);
		printf("Exacta");
	}
	else
	{
		d_malla=pow(2,m+1)+1;
		d_malla_inf=pow(2,m)+1;
		inicializa_cero(v[m],d_malla);
		fu=allocate2D(d_malla,d_malla);
		vg=allocate2D(d_malla,d_malla);
		copia_matriz(d[m],fu,d_malla);


		suaviza(v[m],fu,iteraciones_s1,tipo,d_malla);
		copia_matriz(v[m],vg,d_malla);
		calcula_defecto(d[m],fu,v[m],d_malla);
		///muestra_matriz("d",d[m],d_malla,d_malla);
		restringe(d[m],d[m-1],d_malla_inf);
		multigrid(m-1,d,v,iteraciones_s1,iteraciones_s2,tipo);
		interpola(v[m-1],v[m],d_malla_inf,d_malla);
		suma_matrices(v[m],vg,d_malla);
		suaviza(v[m],fu,iteraciones_s2,tipo,d_malla);


		deallocate2D(fu,d_malla);
		deallocate2D(vg,d_malla);
	}

}


int main(int argc,char *argv[])
{
	int i,j;
	int n_mallas=4;
	int iteraciones_s1=1; /* Iteraciones del primer suavizador */
	int iteraciones_s2=1; /* Iteraciones del segundo suavizador */
	int dimension;
	int dim_inf; 					/* Dimension de la malla inferior */
	double norma_defecto=1.0;
	double norma_defecto_anterior;
	double **u; /* Solución */
	double **f;
	double ***v;
	double ***d;
	const char *tipo="gsrb";
	const char *uso="\n";

	///////////////////////////////////////////
	// escaneo de argumentos                 //
	if((argc>1) && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))) {
		printf (uso, argv[0]);
		exit(1);
	}
	for(i=1;i<argc;i++)
	{
		if(!strcmp(argv[i],"-mallas"))
		{
			n_mallas=atoi(argv[++i]);
		}
		else
		{
			fprintf(stderr,"Opción inválida '%s'\n `%s --help` para mas información\n",
					argv[i], argv[0]);
			exit(1);
		}
	}
	// Fin escaneo de argumentos             //
	///////////////////////////////////////////
	
	///////////////////////////////////////////
	// Generamos los datos necesarios        //
	//                                       //
	//   x . x . x                           //
	//   . . . . .                           //
	//   x . x . x                           //
	//   . . . . .                           //
	//   x . x . x n_mallas=2, dimension=5   //                                   
	dimension = pow(2,n_mallas)+1;

	u=allocate2D(dimension,dimension);
	f=allocate2D(dimension,dimension);
	inicializa_cero(f,dimension);
	//muestra_matriz("f",f,dimension,dimension);
	inicializa(u,dimension);
	//muestra_matriz("u",u,dimension,dimension);

	v=(double***)malloc(n_mallas*sizeof(double**)); // v y d son matrices p
	d=(double***)malloc(n_mallas*sizeof(double**)); // en cada malla
	for(i=0;i<n_mallas;i++)
	{
		j=pow(2,i+1)+1;
		v[i]=allocate2D(j,j);
		d[i]=allocate2D(j,j);
		inicializa_cero(v[i],j);
		inicializa_cero(d[i],j);
		//muestra_matriz("v",v[i],j,j);
	}
	//  Fin inicialización                    //
	////////////////////////////////////////////
	
	////////////////////////////////////////////
	// Bucle principal                        //
	for(i=0;i<20;i++)
	{
		dim_inf=(dimension+1)/2;

		/* Suavizado */
		suaviza(u,f,iteraciones_s1,tipo,dimension);
		//muestra_matriz("u",u,dimension,dimension);

		/* Cálculo del defecto */
		calcula_defecto(d[n_mallas-1],f,u,dimension);
		//muestra_matriz("d",d[n_mallas-1],dimension,dimension);

		/* Restringimos el defecto */
		restringe(d[n_mallas-1],d[n_mallas-2],(dimension+1)/2);
		//muestra_matriz("d_",d[n_mallas-2],(dimension+1)/2,(dimension+1)/2);

		/* Llamamos a multigrid */
		multigrid(n_mallas-2,d,v,iteraciones_s1,iteraciones_s2,tipo);
		//muestra_matriz("v_",v[n_mallas-2],(dimension+1)/2,(dimension+1)/2);

		/* Interpolamos */
		interpola(v[n_mallas-2],v[n_mallas-1],(dimension+1)/2,dimension);	
		//muestra_matriz("v",v[n_mallas-1],dimension,dimension);
		
		/* Sumamos */
		suma_matrices(u,v[n_mallas-1],dimension);
		//muestra_matriz("u",u,dimension,dimension);

		/* Volvemos a suavizar */
		suaviza(u,f,iteraciones_s2,tipo,dimension);
		//muestra_matriz("u",u,dimension,dimension);

		/* Comienza el test de convergencia */
		calcula_defecto(d[n_mallas-1],f,u,dimension);
		//muestra_matriz("d",d[n_mallas-1],dimension,dimension);

		norma_defecto_anterior=norma_defecto;
		norma_defecto=calcula_max(d[n_mallas-1],dimension);
		printf("Iter: %d max(defecto)=%e\tratio=%0.10f\n",i,norma_defecto,norma_defecto/norma_defecto_anterior);
	}
	
	// Fin bucle principal                    //
	////////////////////////////////////////////
	
	////////////////////////////////////////////
	// Liberamos memoria                      //
	deallocate2D(u,dimension);
	deallocate2D(f,dimension);
	for(i=0;i<n_mallas;i++)
	{
		j=pow(2,i+1)+1;
		deallocate2D(v[i],j);
		deallocate2D(d[i],j);
		}
	free(v);
	free(d);
	return 0;
}

