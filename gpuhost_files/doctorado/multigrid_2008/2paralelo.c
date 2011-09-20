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
	printf("\n\n");
}
void interpola(gsl_matrix * m_inicial, gsl_matrix * m_final)
{
	int i,j;
	for(i=0;i<m_inicial->size1;i++)
		for(j=0;j<m_inicial->size2;j++)
		{
			gsl_matrix_set(m_final,2*i,2*j,gsl_matrix_get(m_inicial,i,j));
			if(2*i+1<m_final->size1) /*TODO: tratar de quitar estas condicio*/
				gsl_matrix_set(m_final,2*i+1,2*j,
					(gsl_matrix_get(m_inicial,i,j)+
					 gsl_matrix_get(m_inicial,i+1,j)
					)/2);
			if(2*j+1<m_final->size2)
				gsl_matrix_set(m_final,2*i,2*j+1,
					(gsl_matrix_get(m_inicial,i,j)+
					 gsl_matrix_get(m_inicial,i,j+1)
					)/2);
			if(2*i+1<m_final->size1 && 2*j+1<m_final->size2)
				gsl_matrix_set(m_final,2*i+1,2*j+1,
					(gsl_matrix_get(m_inicial,i,j)+
					 gsl_matrix_get(m_inicial,i,j+1)+
					 gsl_matrix_get(m_inicial,i+1,j)+
					 gsl_matrix_get(m_inicial,i+1,j+1)
					)/4);
		}
}
void restringe(gsl_matrix * m_inicial, gsl_matrix * m_final)
{
	int i,j;
	#pragma omp parallel shared(m_final,m_inicial) private(i,j)
	for(i=1;i<m_final->size1-1;i++)
		#pragma omp parallel for
		for(j=1;j<m_final->size2-1;j++)
		{
			gsl_matrix_set(m_final,i,j,
				(4*gsl_matrix_get(m_inicial,2*i,2*j)+
				 2*gsl_matrix_get(m_inicial,2*i-1,2*j)+
				 2*gsl_matrix_get(m_inicial,2*i+1,2*j)+
				 2*gsl_matrix_get(m_inicial,2*i,2*j-1)+
				 2*gsl_matrix_get(m_inicial,2*i,2*j+1)+
				   gsl_matrix_get(m_inicial,2*i-1,2*j-1)+
				   gsl_matrix_get(m_inicial,2*i-1,2*j+1)+
				   gsl_matrix_get(m_inicial,2*i+1,2*j+1)+
				   gsl_matrix_get(m_inicial,2*i+1,2*j-1)
				)/16);
		}
}
void suavizador(gsl_matrix * var1, gsl_matrix * var2, int iteraciones, int tipos)
{
	if(tipos==1)/* GS - RB */
	{
		int i,j,k,l;
		float h2=pow(1.0/(var1->size1-1),2);
		for(k=0;k<iteraciones;k++)
		{
			for(l=0;l<2;l++)
			{
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
	}
	else if(tipos==2)/* GS -Lexicograph */
	{
		int i,j,k,l;
		float h2=pow(1.0/(var1->size1-1),2);
		for(k=0;k<iteraciones;k++)
		{
			for(l=1;l<=(var1->size1-2)*2;l++)
			{

				#pragma omp parallel for shared(var1,var2) private(i,j)
				for(i=1;i<var1->size1-1;i++)
				{
					j=l-i;
					if(j<var1->size1-1 && j>0)
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

	}
	else /* Jacobi */
	{
		int i,j,k,l;
		float h2=pow(1.0/(var1->size1-1),2);
		for(k=0;k<iteraciones;k++)
		{
			gsl_matrix * u_nue = gsl_matrix_calloc(var1->size1,var1->size1);
			for(i=1;i<var1->size1-1;i++)
			{
				for(j=1;j<var1->size1-1;j++)
				{
					gsl_matrix_set(u_nue,i,j,
					0.25*(gsl_matrix_get(var2,i,j)*h2+
						  gsl_matrix_get(var1,i-1,j)+
						  gsl_matrix_get(var1,i+1,j)+
						  gsl_matrix_get(var1,i,j-1)+
						  gsl_matrix_get(var1,i,j+1)
						)
					);
				}
			}
			gsl_matrix_scale(var1,0.2);
			gsl_matrix_scale(u_nue,0.8);
			gsl_matrix_add(var1,u_nue);
		}
	}
}
void defecto(gsl_matrix * defecto, gsl_matrix * funcion, gsl_matrix * vector)
{
	int i,j,k,l;
	float h2=pow(1.0/(funcion->size1-1),2);
	#pragma omp parallel for shared(defecto,funcion,vector) private(i,j)
	for(i=1;i<funcion->size1-1;i++)
		#pragma omp parallel for shared(defecto,funcion,vector) private(j)
		for(j=1;j<funcion->size1-1;j++)
		{
			gsl_matrix_set(defecto,i,j,
				 gsl_matrix_get(funcion,i,j)-
				(4*gsl_matrix_get(vector,i,j)-
				   gsl_matrix_get(vector,i-1,j)-
				   gsl_matrix_get(vector,i+1,j)-
				   gsl_matrix_get(vector,i,j-1)-
				   gsl_matrix_get(vector,i,j+1)
				)/h2);
		}

}
void sol_exacta(gsl_matrix * vector, gsl_matrix * defecto)
{
	gsl_matrix_set(vector,1,1,gsl_matrix_get(defecto,1,1)/16);
}
void multigrid(int m, /* Malla en la que estamos */
				gsl_matrix **d, /* defecto */
				gsl_matrix **v,  /* las v's */
				int s1,	/* iteraciones suavizador 1*/
				int s2,	/*iteraciones suavizador 2*/
				int tipo, /* 1-V 2-W */ 
				int tipos)	/*Tipo de suavizador*/
{
	int i;
	if(m==0)	/*Malla 0, solucion exacta */
		sol_exacta(v[m],d[m]);
	else
	{
		/* En primer lugar, inicializamos el vector v a cero */
		gsl_matrix_set_zero(v[m]);
		
		gsl_matrix * funcion = gsl_matrix_alloc(d[m]->size1,d[m]->size2);
		gsl_matrix_memcpy(funcion,d[m]);/*Copiamos el defecto a la funcion*/
		gsl_matrix * v_guard = gsl_matrix_alloc(v[m]->size1,v[m]->size2);


		for(i=0;i<tipo;i++)
		{
			suavizador(v[m],funcion,s1,tipos);
			/* Y guardamos el valor de v */
			gsl_matrix_memcpy(v_guard,v[m]);

			defecto(d[m],funcion,v[m]);
			restringe(d[m],d[m-1]);
			multigrid(m-1,d,v,s1,s2,tipo,tipos);
			interpola(v[m-1],v[m]);
			gsl_matrix_add(v[m],v_guard);

			/* Segundo suavizador*/
			suavizador(v[m],funcion,s2,tipos);
		}
		gsl_matrix_free(funcion);
		gsl_matrix_free(v_guard);
	}


}
int main(int argc, char **argv)
{
	if(argc <7 )
	{
		printf("Modo de empleo: %s mallas error iter_suavizador1 iter_suavizador2 tipo(V-1, 2-W) tipo_de_suavizador\n",argv[0]);
		exit(EXIT_FAILURE);
	}
	int i,j,k;
	int mallas=atoi(argv[1]);
	int m=mallas-1; /* m es simplemente mallas - 1*/
	double err=1.0*(pow(10,-atoi(argv[2])));
	int s1 = atoi(argv[3]);
	int s2 = atoi(argv[4]);
	int tipo = atoi(argv[5]);
	int tipos = atoi(argv[6]);
	printf("********************Multigrid*******************\n");
	printf("** Mallas: %d\n",mallas);
	printf("** Error: 10^-%d\n",atoi(argv[2]));
	if(tipo==1)
		printf("** Tipo V\n");
	else if(tipo==2)
		printf("** Tipo W\n");

	if(tipos==1)
		printf("** GS - RB %d %d\n",s1,s2);
	else if(tipos==2)
		printf("** GS - Lex %d %d\n",s1,s2);
	else if(tipos==3)
		printf("** Jacobi %d %d\n",s1,s2);
	printf("************************************************\n\n\n");

	/* elementos x elementos en la malla mas fina*/
	int elementos = pow(2,mallas)+1;
	printf("Elementos %d\n",elementos*elementos);

	/*Inicializamos u en la malla mas fina con valor 0*/
	gsl_matrix * u = gsl_matrix_calloc(elementos,elementos);

	/*Inicializamos f en la malla mas fina*/
	gsl_matrix * f = gsl_matrix_calloc(elementos,elementos);
	int divisor = pow(elementos-1,2);
	for(i=1; i< elementos-1; i++)
	{
		for(j=1; j<elementos-1; j++)
		{
			gsl_matrix_set(f,i,j,cos(1.0*i*j/divisor));
		}
	}
	/*Inicializamos las v's y d's*/
	gsl_matrix *v[mallas], *d[mallas];
	for(k=0;k<mallas;k++)
	{
		elementos = pow(2,k+1)+1;
		v[k] = gsl_matrix_calloc(elementos,elementos);
		d[k] = gsl_matrix_calloc(elementos,elementos);
	}
	/* Bucle principal */
	double norma_defecto_anterior;
	double norma_defecto=1;
	i=0;
	while(norma_defecto>err)
	{
		i++;
		suavizador(u,f,s1,tipos);
		defecto(d[m],f,u);
		restringe(d[m],d[m-1]);
		multigrid(m-1,d,v,s1,s2,tipo,tipos);
		interpola(v[m-1],v[m]);
		gsl_matrix_add(u,v[m]);
		suavizador(u,f,s2,tipos);
		defecto(d[m],f,u);
		norma_defecto_anterior = norma_defecto;
		norma_defecto=(gsl_matrix_max(d[m]) > - gsl_matrix_min(d[m])) ? 
			 gsl_matrix_max(d[m]) : - gsl_matrix_min(d[m]);
		printf("--------------- Iteracion %d ----------------\n",i);
		printf("Max defecto: %0.16f     ",norma_defecto);
		printf("Ratio: %0.16f\n",(norma_defecto/norma_defecto_anterior));
		printf("\n");
	}
	/* Fin bucle principal */

	/* Liberacion de los recursos */
	gsl_matrix_free(u);
	gsl_matrix_free(f);
	for(k=0;k<mallas;k++)
	{
		gsl_matrix_free(v[k]);
	}
	return 0;
}


