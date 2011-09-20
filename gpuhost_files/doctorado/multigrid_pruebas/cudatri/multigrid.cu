#include <multigrid_kernel.cu>
#include <stdio.h>
#include <stdlib.h>
#include "multigrid.h"
#include "auxiliares.h"
#include "io.h"
#include "operador.h"
#define BLOCK_SIZE 16

int main(int argc, char **argv)
{

	int i;
	int n_mallas=5;
	const char *uso=
		"\nUso: %s \n\t -m número de mallas \
		 \n\t -tr archivo de triangulos \n\n";
	if((argc>1) && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))) {
		printf (uso, argv[0]);
		exit(1);
	}
	char *archivo_triangulo="triangulo";

	for(i=1; i<argc;i++)
	{
		if(!strcmp(argv[i],"-m"))
			n_mallas=atoi(argv[++i]);
		else if(!strcmp(argv[i],"-tr"))
			archivo_triangulo=argv[++i];
	}

	int dim;
	int size;
	int size_total=0;
	int malla_actual=n_mallas;	

	/* El triangulo a usar */
	Triangulo tr;

	/* Definimos las mallas */
	Grid u[n_mallas];
	Grid f[n_mallas];
	Grid v[n_mallas];
	Grid d[n_mallas];

	/* Para el cálculo del máximo del defecto */
	double * h_def; /* En el Host */ 
	double max;
	double max_anterior;

	/* Reservamos la memoria en la GPU*/
	for(i=2;i<=n_mallas;i++)
	{
		dim  = pow(2,i)+1;	/* Dim es el número de elementos de la "diagonal" */
		size = dim*dim;			/* Reservamos una matriz entera, aunque sólo usaremos por debajo de la diagonal */
		u[i].dim = dim;
		f[i].dim = dim;
		v[i].dim = dim;
		d[i].dim = dim;
		u[i].size = size;
		f[i].size = size;
		v[i].size = size;
		d[i].size = size;

		cudaMalloc(&u[i].v,size*sizeof(double));
		cudaMalloc(&f[i].v,size*sizeof(double));
		cudaMalloc(&v[i].v,size*sizeof(double));
		cudaMalloc(&d[i].v,size*sizeof(double));

		cudaMalloc(&u[i].op,9*sizeof(double));
		cudaMalloc(&f[i].op,9*sizeof(double));
		cudaMalloc(&v[i].op,9*sizeof(double));
		cudaMalloc(&d[i].op,9*sizeof(double));

		size_total=size_total+4*size+4*9;
	}
	h_def=(double*)malloc(dim*dim*sizeof(double));

	printf("Hemos reservado %d kb en la GPU\n",size_total*sizeof(double)/1024);

	/* Creacion de el operador */
	double  op[3][3]={{0,0,0},{0,0,0,},{0,0,0}};
	double  op_es[3][3]={{0,0,0},{0,0,0},{0,0,0}};
	tr=lee_fichero_triangulo(archivo_triangulo);
	crea_operador(tr,&op[0][0]);
	
	/* Y escalamos el operador para cada malla */
	for(i=2;i<=n_mallas;i++)
	{
		iguala_matriz(&op[0][0],&op_es[0][0],3);
		escala_matriz(&op_es[0][0],3,pow((u[i].dim-1),2));
		cudaMemcpy(u[i].op,&op_es[0][0],9*sizeof(double),cudaMemcpyHostToDevice);
		cudaMemcpy(f[i].op,&op_es[0][0],9*sizeof(double),cudaMemcpyHostToDevice);
		cudaMemcpy(v[i].op,&op_es[0][0],9*sizeof(double),cudaMemcpyHostToDevice);
		cudaMemcpy(d[i].op,&op_es[0][0],9*sizeof(double),cudaMemcpyHostToDevice);
	}
//	imprime_grid(u[malla_actual],"grid");
	

	/* Datos para la llamada a CUDA */
	dim3 dimBlock(BLOCK_SIZE,BLOCK_SIZE);
	dim3 dimGrid((u[malla_actual].dim+BLOCK_SIZE-1)/dimBlock.x,(u[malla_actual].dim+BLOCK_SIZE-1)/dimBlock.y);

	/* Inicializamos la malla de la función f */
	cero<<<dimGrid,dimBlock>>>(f[malla_actual]);
	random<<<dimGrid,dimBlock>>>(u[malla_actual]);

	/* Principal */
	for(i=0;i<10;i++)
	{
		max_anterior = max;
		max=0.0;
		multigrid(&u[0],&f[0],&v[0],&d[0],malla_actual,&max,h_def,n_mallas);
		printf("[Iteracion #%d]\n",i);
		printf("max=%0.10e ratio=%f\n",max,max/max_anterior);
	}


	/* Liberacion de memoria */
	for(i=2;i<n_mallas;i++)
	{
		cudaFree(&u[i].v);
		cudaFree(&v[i].v);
		cudaFree(&f[i].v);
		cudaFree(&d[i].v);
		cudaFree(&u[i].op);
		cudaFree(&v[i].op);
		cudaFree(&f[i].op);
		cudaFree(&d[i].op);

	}
	free(h_def);
	return 0;
}

void multigrid(Grid *u, Grid *f, Grid *v, Grid *d, int m, double * max,double * h_def,int n_mallas)
{
	int dim;
	int dim_;
	int i,j;
	if(m==2) /* Resolver exacta */
	{
	}
	else
	{
		/* Datos para la llamada a CUDA */
		dim3 dimBlock(BLOCK_SIZE,BLOCK_SIZE);
		dim3 dimGrid((u[m].dim+BLOCK_SIZE-1)/dimBlock.x,(u[m].dim+BLOCK_SIZE-1)/dimBlock.y);
		dim3 dimGrid_((u[m-1].dim+BLOCK_SIZE-1)/dimBlock.x,(u[m-1].dim+BLOCK_SIZE-1)/dimBlock.y);

		/* Ponemos a 0 las mallas necesarias */
		cero<<<dimGrid,dimBlock>>>(v[m]);
		cero<<<dimGrid,dimBlock>>>(d[m]);
		cero<<<dimGrid_,dimBlock>>>(u[m-1]);
		cero<<<dimGrid_,dimBlock>>>(f[m-1]);

		/* Suavizamos */
		suaviza<<<dimGrid,dimBlock>>>(u[m],f[m],0);
		suaviza<<<dimGrid,dimBlock>>>(u[m],f[m],1);
		suaviza<<<dimGrid,dimBlock>>>(u[m],f[m],2);

		/* Calculamos el defecto */
		defecto<<<dimGrid,dimBlock>>>(u[m],f[m],d[m]);

		/* Lo restringimos */
		restringe<<<dimGrid_,dimBlock>>>(d[m],f[m-1]);

		/* Rellamamos a multigrid */
		multigrid(&u[0],&f[0],&v[0],&d[0],m-1,max,h_def,n_mallas);

		/* Interpolamos */
		interpola<<<dimGrid_,dimBlock>>>(u[m-1],v[m]);

		/* Sumamos */
		suma<<<dimGrid,dimBlock>>>(u[m],v[m]);

		/* Suavizamos */
		suaviza<<<dimGrid,dimBlock>>>(u[m],f[m],0);
		suaviza<<<dimGrid,dimBlock>>>(u[m],f[m],1);
		suaviza<<<dimGrid,dimBlock>>>(u[m],f[m],2);

		/* Si estamos en la malla superior, comprobamos el defecto */
		if(m==n_mallas)
		{
			printf("En comprobación...\n");
			/* Calculamos el defecto */
			defecto<<<dimGrid,dimBlock>>>(u[m],f[m],d[m]);

			/* Copiamos al host el valor para calcular su máximo */
			cudaMemcpy(h_def,d[m].v,d[m].dim*d[m].dim*sizeof(double),cudaMemcpyDeviceToHost);

			max[0] = 0.0;
			for(i=1;i<d[m].dim;i++)
			{
				for(j=1;j<i;j++)
				{
					if(max[0]<h_def[i])
						max[0]=h_def[i];
				}
			}
		}
	}
}
