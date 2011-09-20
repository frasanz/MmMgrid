#include <multigrid_kernel.cu>
#include <stdio.h>
#define N_MALLAS 12
#define BLOCK_SIZE 16
void g_imprime(Grid g,const char *);
void g_muestra(Grid g,const char *);
void multigrid(Grid *u, Grid *f, Grid *v, Grid *d, int m, float * max,float *def, float * hdef);

int main()
{
	int i;
	int dim;
	int size;
	int sizetotal=0;
	float max=1.0;
	float max_ant;

	/* Definimos los grids */
	Grid u[N_MALLAS];
	Grid f[N_MALLAS];
	Grid v[N_MALLAS];
	Grid d[N_MALLAS];

	/* un float para calcular el maximo del defecto de cada fila */
	float * def;  //En la GPU
	float * hdef; //En el host

	/* Reservamos memoria */
	for(i=0;i<N_MALLAS;i++)
	{
		dim  = (int)pow(2,i+1)+1;
		size = dim*dim;
		u[i].d = dim;
		f[i].d = dim;
		v[i].d = dim;
		d[i].d = dim;
		u[i].size = size;
		f[i].size = size;
		v[i].size = size;
		d[i].size = size;

		cudaMalloc(&u[i].v,size*sizeof(float));
		cudaMalloc(&f[i].v,size*sizeof(float));
		cudaMalloc(&v[i].v,size*sizeof(float));
		cudaMalloc(&d[i].v,size*sizeof(float));
		sizetotal+=4*size;
	}
	int m = N_MALLAS-1;
	dim = (int)pow(2,m+1)+1;
	size=dim*dim;
	cudaMalloc(&def,size*sizeof(float));
	sizetotal+=size;
	hdef=(float*)malloc(size*sizeof(float));

	printf("Necesitamos %d Kb\n",sizetotal*sizeof(float)/1024);

	/* Para las llamadas a CUDA */
	dim3 dimBlock(BLOCK_SIZE,BLOCK_SIZE);
	dim3 dimGrid((dim+BLOCK_SIZE-1)/dimBlock.x,(dim+BLOCK_SIZE-1)/dimBlock.y);

	/* Inicializamos la malla de la función */
	cero<<<dimGrid, dimBlock>>>(f[m]);

	/* Inicializamos u[m] con valores "random" */
	cero<<<dimGrid, dimBlock>>>(u[m]);
	random<<<dimGrid, dimBlock>>>(u[m]);

	/* Principal */
	for(i=0;i<10;i++)
	{
		max_ant = max;
		max = 0.0;
		multigrid(&u[0],&f[0],&v[0],&d[0],m,&max,def,hdef);
		printf("[Iteracion #%d] nd=%e ratio=%f\n",i,max,max/max_ant);
	}


	/* Liberamos memoria */
	for(i=0;i<N_MALLAS;i++)
	{
		cudaFree(u[i].v);
		cudaFree(f[i].v);
		cudaFree(v[i].v);
		cudaFree(d[i].v);
	}
	cudaFree(def);
	free(hdef);

	return 0;
}

void g_imprime(Grid g, const char *nombre)
{
	FILE * f;
	f=fopen(nombre,"w");
	int i,j;
	float *hg;
	hg=(float*)malloc(g.size*sizeof(float));
	cudaMemcpy(hg,g.v,g.size*sizeof(float),cudaMemcpyDeviceToHost);

	for(i=0;i<g.d;i++)
	{
		for(j=0;j<g.d;j++)
		{
			fprintf(f,"%d %d %f\n",i,j,hg[I(g.d,i,j)]);
		}
		fprintf(f,"\n");
	}
	fclose(f);
}
void g_muestra(Grid g, const char *nombre)
{
	int i,j;
	float *hg;
	hg=(float*)malloc(g.size*sizeof(float));
	cudaMemcpy(hg,g.v,g.size*sizeof(float),cudaMemcpyDeviceToHost);

	printf("%s=\n",nombre);
	for(i=0;i<g.d;i++)
	{
		for(j=0;j<g.d;j++)
		{
			printf("%f",hg[I(g.d,i,j)]);
		}
		printf("\n");
	}


}


void multigrid(Grid *u, Grid *f, Grid *v, Grid *d, int m, float * max,float *def, float * hdef)
{
	int dim;
	int dim_;
	int i;

	/* Primer caso, malla 0, solucion */
	if(m==0)
	{
		exacta<<<1,1>>>(u[m],f[m]);
	}
	else
	{
		/* Definiciones para las llamadas a CUDA */
		dim = (int)pow(2,m+1)+1;
		dim_= (int)pow(2,m)+1;
		dim3 dimBlock(BLOCK_SIZE,BLOCK_SIZE);
		dim3 dimGrid((dim+BLOCK_SIZE-1)/dimBlock.x,(dim+BLOCK_SIZE-1)/dimBlock.y);
		dim3 dimGrid_((dim_+BLOCK_SIZE-1)/dimBlock.x,(dim_+BLOCK_SIZE-1)/dimBlock.y);

		/* Ponemos a 0 las mallas necesarias */
		cero<<<dimGrid,dimBlock>>>(v[m]);
		cero<<<dimGrid,dimBlock>>>(d[m]);
		cero<<<dimGrid_,dimBlock>>>(u[m-1]);
		cero<<<dimGrid_,dimBlock>>>(f[m-1]);

		/* Suavizado R-N */
		suaviza_r<<<dimGrid,dimBlock>>>(u[m],f[m]);
		suaviza_n<<<dimGrid,dimBlock>>>(u[m],f[m]);

		/* Cálculo del defecto */
		defecto<<<dimGrid, dimBlock>>>(u[m],f[m],d[m]);

		/* Restringimos el defecto de d[m] a f[m-1] */
		restringe<<<dimGrid_,dimBlock>>>(d[m],f[m-1]);

		/* Rellamada a multigrid */
		multigrid(&u[0],&f[0],&v[0],&d[0],m-1,max,def,hdef);
		
		/* Interpolamos de u[m-1] a v[m] */
		interpola<<<dimGrid_,dimBlock>>>(u[m-1],v[m]);

		/* Sumamos */
		suma<<<dimGrid,dimBlock>>>(u[m],v[m]);

		/* Suavizado R-N */
		suaviza_r<<<dimGrid,dimBlock>>>(u[m],f[m]);
		suaviza_n<<<dimGrid,dimBlock>>>(u[m],f[m]);

		/* Si estamos en la malla superior, comprobamos el defecto */
		if(m==N_MALLAS-1)
		{
			defecto<<<dimGrid,dimBlock>>>(u[m],f[m],d[m]);
			dim3 dg((dim+BLOCK_SIZE-1)/dimBlock.x,1);
			dim3 db(BLOCK_SIZE,1);

			/* Calculamos el máximo de cada una de las filas */
			maxx<<<dg,db>>>(d[m],def);

			/* Copiamos el vector resultante al host */
			cudaMemcpy(hdef,def,dim*dim*sizeof(float),cudaMemcpyDeviceToHost);

			max[0]=0.0;
			for(i=0;i<dim;i++)
			{
				if(max[0]<hdef[i])
					max[0]=hdef[i];
			}
		}
	}
}

