#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <multigrid2_kernel.cu>

#define BLOCK_SIZE 16
#define N_MALLAS 12 
void imprime_g(Malla);
void multigrid(Malla *, Malla *, Malla *, Malla *, int, double *, double *,double *);

int main(int argc, char **argv)
{

	int i;
	int dim;
	int nivel;
	Malla * mu;
	Malla * mv;
	Malla * mf;
	Malla * md;
	mu = (Malla *)malloc(N_MALLAS*sizeof(Malla));
	mv = (Malla *)malloc(N_MALLAS*sizeof(Malla));
	mf = (Malla *)malloc(N_MALLAS*sizeof(Malla));
	md = (Malla *)malloc(N_MALLAS*sizeof(Malla));

	/* Para calcular los defectos */
	double * def_fila_g;
	double * def_fila;
	double nd;
	double nda;
	dim  =(int)pow(2.0,N_MALLAS)+1;
	cudaMalloc((void**)&def_fila_g,dim*sizeof(double));
	def_fila=(double *)malloc(dim*sizeof(double));


	printf("Reservando memoria...\n");
	for(i=0;i<N_MALLAS;i++)
	{
		dim = (int)pow(2.0,i+1)+1;
		mu[i].dim=dim;
		mv[i].dim=dim;
		mf[i].dim=dim;
		md[i].dim=dim;
		cudaMalloc((void**)&mu[i].v,dim*dim*sizeof(double));
		cudaMalloc((void**)&mv[i].v,dim*dim*sizeof(double));
		cudaMalloc((void**)&mf[i].v,dim*dim*sizeof(double));
		cudaMalloc((void**)&md[i].v,dim*dim*sizeof(double));
	}

	nivel=N_MALLAS-1;
	dim  =(int)pow(2.0,N_MALLAS)+1;
	/* Parámtros de CUDA */
	dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
	dim3 dimGrid(((int)dim+BLOCK_SIZE-1)/dimBlock.x,((int)dim+BLOCK_SIZE-1)/dimBlock.y);
	printf("%d %d\n",dimGrid.x,dimGrid.y);

	/* Definimos la malla de la funcion en la GPU */
	inicializa_f<<<dimGrid,dimBlock>>>(mf[nivel]);

	/* Inicializamos a 0 la malla de la solución */
	cero<<<dimGrid,dimBlock>>>(mu[nivel]);

	/* Llamamos a multigrid */
	printf("Llamada a multigrid...\n");
	for(i=0;i<10;i++)
	{
		nda=nd;
		nd=0.0;
		multigrid(mu,mf,md,mv,N_MALLAS,def_fila_g,def_fila,&nd);
		printf("Iteracion %d \t max(defecto)=%f\tratio=%0.10f\n",i, nd,nd/nda);
	}


	/* Liberacion de memoria */
	printf("Liberando memoria...\n");
	for(i=0;i<N_MALLAS;i++)
	{
		cudaFree(mu[i].v);
		cudaFree(mv[i].v);
		cudaFree(mf[i].v);
		cudaFree(md[i].v);
	}
	cudaFree(def_fila_g);
	free(def_fila);
	free(mu);
	free(mv);
	free(mf);
	free(md);

}
void multigrid(Malla * u, Malla * f, Malla * v, Malla * d, int n_mallas, double * def_fila_g, double * def_fila, double *nd)
{
	int m = n_mallas -1; // m representa en la malla en la que estamos
	int dim;
	int dim_;
	int i;
	if(m==0) // estamos en la malla 0, buscamos la solución exacta
	{
		soluciona<<<1,1>>>(u[m],f[m]);
	}
	else
	{
		dim = (int)pow(2.0,m+1)+1;
		dim_= (int)pow(2.0,m)+1;

		/* Datos para llamar a la GPU */
		dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
		dim3 dimGrid(((int)dim+BLOCK_SIZE-1)/dimBlock.x,((int)dim+BLOCK_SIZE-1)/dimBlock.y);
		dim3 dimGrid_(((int)dim_+BLOCK_SIZE-1)/dimBlock.x,((int)dim_+BLOCK_SIZE-1)/dimBlock.y);


		/* Ponemos las mallas necesarias a 0 */
		cero<<<dimGrid,dimBlock>>>(d[m]);
		cero<<<dimGrid,dimBlock>>>(v[m]);
		cero<<<dimGrid_,dimBlock>>>(u[m-1]);
		cero<<<dimGrid_,dimBlock>>>(f[m-1]);


		/* Presuavizado */
		suavizado_n<<<dimGrid, dimBlock>>>(u[m],f[m]); //Suavizado negro
		suavizado_r<<<dimGrid, dimBlock>>>(u[m],f[m]); //Suavizado rojo
		suavizado_n<<<dimGrid, dimBlock>>>(u[m],f[m]); //Suavizado negro
		suavizado_r<<<dimGrid, dimBlock>>>(u[m],f[m]); //Suavizado rojo
		suavizado_n<<<dimGrid, dimBlock>>>(u[m],f[m]); //Suavizado negro
		suavizado_r<<<dimGrid, dimBlock>>>(u[m],f[m]); //Suavizado rojo
		suavizado_n<<<dimGrid, dimBlock>>>(u[m],f[m]); //Suavizado negro
		suavizado_r<<<dimGrid, dimBlock>>>(u[m],f[m]); //Suavizado rojo





		/* Defecto */
		defecto<<<dimGrid,dimBlock>>>(u[m],f[m],d[m]);

		/* Restringimos el defecto (de d a la f del nivel inferior)*/
		restringe<<<dimGrid_,dimBlock>>>(d[m],f[m-1]);

		/* Rellamada a multigrid */
		multigrid(u,f,d,v,m,def_fila_g,def_fila,nd);

		/* Imterpolamos (de la u de la malla inf a la v de la actual) */
		interpola<<<dimGrid_,dimBlock>>>(u[m-1],v[m]);

		/* Sumamos */
		suma<<<dimGrid,dimBlock>>>(u[m],v[m]);

		/* Volvemos a suavizar */
		suavizado_n<<<dimGrid, dimBlock>>>(u[m],f[m]); //Suavizado negro
		suavizado_r<<<dimGrid, dimBlock>>>(u[m],f[m]); //Suavizado rojo
		suavizado_n<<<dimGrid, dimBlock>>>(u[m],f[m]); //Suavizado negro
		suavizado_r<<<dimGrid, dimBlock>>>(u[m],f[m]); //Suavizado rojo
		suavizado_n<<<dimGrid, dimBlock>>>(u[m],f[m]); //Suavizado negro
		suavizado_r<<<dimGrid, dimBlock>>>(u[m],f[m]); //Suavizado rojo
		suavizado_n<<<dimGrid, dimBlock>>>(u[m],f[m]); //Suavizado negro
		suavizado_r<<<dimGrid, dimBlock>>>(u[m],f[m]); //Suavizado rojo





		/* En la malla mas fina, calculamos el defecto y comprobamos cómo va la cosa */
		if(m==N_MALLAS-1)
		{
			defecto<<<dimGrid,dimBlock>>>(u[m],f[m],d[m]);
			calcula_max<<<((int)dim+BLOCK_SIZE-1)/dimBlock.x,BLOCK_SIZE>>>(d[m],def_fila_g);
			cudaMemcpy(def_fila,def_fila_g,dim*sizeof(double),cudaMemcpyDeviceToHost);
			for(i=0;i<dim;i++)
			{
				if(abs(def_fila[i])>nd[0])
					nd[0]=abs(def_fila[i]);
			}
		}
	}
}

void imprime_g(Malla m) /* Imprime una malla allocada en la GPU */
{
	Malla m_h;	/* Definimos una malla, en el host */
	int i,j;
	m_h.v= (double *)malloc(m.dim*m.dim*sizeof(double));
	cudaMemcpy(m_h.v,m.v,m.dim*m.dim*sizeof(double),cudaMemcpyDeviceToHost);
	for(i=0;i<m.dim;i++)
	{
		for(j=0;j<m.dim;j++)
		{
			printf("%.4f ",m_h.v[i*m.dim+j]);
		}
		printf("\n");
	}
	free(m_h.v);
	printf("\n");
}

