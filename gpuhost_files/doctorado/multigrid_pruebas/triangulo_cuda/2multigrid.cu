#include <multigrid_kernel.cu>
#include <stdio.h>
#define N_MALLAS 4
#define BLOCK_SIZE 16

void g_imprime(Grid g);
void multigrid(Grid *u, Grid *f, Grid *v, Grid *d,int nivel, float * op, float *max);

int main()
{
	int i;
	int dim;
	int size;
	float max;
	float max_ant;

	/* Definicion de las Mallas */
	Grid u[N_MALLAS];
	Grid f[N_MALLAS];
	Grid v[N_MALLAS];
	Grid d[N_MALLAS];

	/* Reservamos la memoria */
	for(i=2;i<N_MALLAS;i++)
	{
		dim=pow(2,i)+1; //Dim es el número de elementos de la "diagonal"
		size=((dim-1)*(dim-1)+3*(dim-1))/2+1;
		u[i].dim=dim;
		f[i].dim=dim;
		v[i].dim=dim;
		d[i].dim=dim;
		u[i].size=size;
		f[i].size=size;
		v[i].size=size;
		d[i].size=size;

		cudaMalloc(&u[i].v,size*sizeof(float));
		cudaMalloc(&f[i].v,size*sizeof(float));
		cudaMalloc(&v[i].v,size*sizeof(float));
		cudaMalloc(&d[i].v,size*sizeof(float));
	}
	/* Para el operador en el device */
	float * d_op;
	cudaMalloc(&d_op,9*sizeof(float));


	/* Para la llamada a CUDA */
	int m=N_MALLAS-1;
	dim=(int)pow(2,m)+1;
	dim3 dimBlock(BLOCK_SIZE,BLOCK_SIZE);
	dim3 dimGrid((dim+BLOCK_SIZE-1)/dimBlock.x,(dim+BLOCK_SIZE-1)/dimBlock.y);
	printf("%d %d %d %d\n",dimBlock.x, dimBlock.y,dimGrid.x,dimGrid.y);

	/* Inicializamos la malla de la funcion */
	inicializa_f<<<dimGrid,dimBlock>>>(f[m]);

	/* Initialize u with random values */
	random<<<dimGrid,dimBlock>>>(u[m]);

	/* Principal */
	for(i=0;i<1;i++)
	{
		max_ant=max;
		max=0.0;
		multigrid(&u[0],&f[0],&v[0],&d[0],m,d_op,&max);
		printf("Iteracion %d nd=%f ratio=%f\n",i,max,max/max_ant);
	}

	/* Liberamos la memoria */
	for(i=2;i<N_MALLAS;i++)
	{
		cudaFree(&u[i].v);
		cudaFree(&f[i].v);
		cudaFree(&v[i].v);
		cudaFree(&d[i].v);

	}
	cudaFree(d_op);

	return 0;
}

/* This function prints a grid allocated in GPU */
void g_imprime(Grid g)
{
	float * dg;
	int i,j;
	size_t size=((g.dim-1)*(g.dim-1)+3*(g.dim-1))/2+1;
	dg=(float*)malloc(size*sizeof(float));
	cudaMemcpy(dg,g.v,size*sizeof(float),cudaMemcpyDeviceToHost);

	/* Mostramos por pantalla */
	printf("Grid size=%d dim=%d\n",size,g.dim);
	for(i=0;i<g.dim;i++)
	{
		for(j=0;j<=i;j++)
			printf("%f ",dg[IDT(i,j)]);
		printf("\n");
	}
	printf("\n");

	free(dg);
}
void multigrid(Grid *u, Grid *f, Grid *v, Grid *d,int m,float * d_op, float *max)
{
	int dim;
	int dim_;
	int i;
	/* Definimos h^2 */
	float h2=pow(u[m].dim-1,2);

	/* Definimos un operador (copiado de otro sitio) */
	float operador[9]={0.0,-1.0*h2,0.0,-1.0*h2,4.0*h2,-1.0*h2,0.0,-1.0*h2,0.0};
	cudaMemcpy(d_op,&operador[0],9*sizeof(float),cudaMemcpyHostToDevice);
	/* Para el operador en el device */

	if(m==2)
	{
		/* Dimension, necesitamos bajar la f al host para resolver el sistema lineal */
		float * hf;
		float * hu;
		dim=(int)pow(2,m)+1;
		size_t size=((f[m].dim-1)*(f[m].dim-1)+3*(f[m].dim-1))/2+1;
		hf=(float*)malloc(size*sizeof(float));
		cudaMemcpy(hf,f[m].v,size*sizeof(float),cudaMemcpyDeviceToHost);
		hu=(float*)malloc(size*sizeof(float));
		for(i=0;i<size;i++)
			hu[i]=0.0;

		/* Construimos el sistema a resolver */
		double A[3][3];
		A[0][0]=operador[4];
		A[0][1]=operador[7];
		A[0][2]=operador[8];
		A[1][0]=operador[2];
		A[1][1]=operador[4];
		A[1][2]=operador[5];
		A[2][0]=operador[0];
		A[2][1]=operador[3];
		A[2][2]=operador[4];

		double B[3];
		B[0]=hf[IDT(2,1)];
		B[1]=hf[IDT(3,1)];
		B[2]=hf[IDT(3,2)];

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
		hu[IDT(3,2)]=B[2]/A[2][2];
		hu[IDT(3,1)]=(B[1]-A[1][2]*hu[IDT(3,2)])/A[1][1];
		hu[IDT(2,1)]=(B[0]-A[0][2]*hu[IDT(3,2)]-A[0][1]*hu[IDT(3,1)])/A[0][0];

		/* Subimos la solución a la GPU */
		cudaMemcpy(u[m].v,hu,size*sizeof(float),cudaMemcpyHostToDevice);
		free(hf);
	}
	else
	{
		/* Para la llamada a CUDA */
		dim=(int)pow(2,m)+1;
		dim_=(int)pow(2,m-1)+1;
		dim3 dimBlock(BLOCK_SIZE,BLOCK_SIZE);
		dim3 dimGrid((dim+BLOCK_SIZE-1)/dimBlock.x,(dim+BLOCK_SIZE-1)/dimBlock.y);
		dim3 dimGrid_((dim_+BLOCK_SIZE-1)/dimBlock.x,(dim_+BLOCK_SIZE-1)/dimBlock.y);

		/* Ponemos a 0 las mallas necesarias */
		cero<<<dimGrid,dimBlock>>>(v[m]);
		cero<<<dimGrid,dimBlock>>>(d[m]);
		cero<<<dimGrid,dimBlock>>>(u[m-1]);
		cero<<<dimGrid,dimBlock>>>(f[m-1]);

		/* Suavizamos tres colores */
		g_imprime(u[m]);
		suaviza_r<<<dimGrid,dimBlock>>>(u[m],f[m],d_op);
		suaviza_g<<<dimGrid,dimBlock>>>(u[m],f[m],d_op);
		suaviza_b<<<dimGrid,dimBlock>>>(u[m],f[m],d_op);
		g_imprime(u[m]);

		/* Calculamos el defecto */
		defecto<<<dimGrid,dimBlock>>>(u[m],f[m],d[m],d_op);
		g_imprime(d[m]);

		/* Restringimos el defecto (de d[m] a f[m-1]) */
		restringe<<<dimGrid_,dimBlock>>>(d[m],f[m-1]);
		g_imprime(f[m-1]);

		/* Rellamamos a multigrid */
		multigrid(&u[0],&f[0],&v[0],&d[0],m-1,d_op,max);
		g_imprime(u[m-1]);

		/* Interpolate from u[m-1] to v[m] */
		interpola<<<dimGrid_,dimBlock>>>(u[m-1],v[m]);
		g_imprime(v[m]);

		/* Sumamos */
		suma<<<dimGrid,dimBlock>>>(u[m],v[m]);
		g_imprime(u[m]);

		/* Post-suavizamos tres colores */
		suaviza_r<<<dimGrid,dimBlock>>>(u[m],f[m],d_op);
		suaviza_g<<<dimGrid,dimBlock>>>(u[m],f[m],d_op);
		suaviza_b<<<dimGrid,dimBlock>>>(u[m],f[m],d_op);
		g_imprime(u[m]);

		/* Si estamos en la malla superior,comprobamos cómo va el defecto */
		if(m==N_MALLAS-1) 
		{
			defecto<<<dimGrid,dimBlock>>>(u[m],f[m],d[m],d_op);
			g_imprime(d[m]);
			float * def;
			size_t size=((f[m].dim-1)*(f[m].dim-1)+3*(f[m].dim-1))/2+1;
			def=(float*)malloc(size*sizeof(float));
			cudaMemcpy(def,d[m].v,size*sizeof(float),cudaMemcpyDeviceToHost);
			for(i=0;i<size;i++)
			{
				if(max[0]<fabs(def[i]))
					max[0]=fabs(def[i]);
			}
			free(def);
		}



	}
}
