#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <multigrid_kernel.cu>

#define BLOCK_SIZE 16
#define MALLAS 2
void multigrid(Malla, Malla, int,double *);
void imprime_g(Malla);
void imprime_g2(Malla);

double anterior;

int main(int argc, char **argv)
{
	int m = MALLAS-1; /* m representa la malla en la que estamos */
	double dim  = pow(2.0,m+1)+1;
	double dim_ = pow(2.0,m)+1;
	double nd; //Norma del defecto
	double nda;
	dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
	dim3 dimGrid(((int)dim+BLOCK_SIZE-1)/dimBlock.x,((int)dim+BLOCK_SIZE-1)/dimBlock.y);
	dim3 dimGrid_(((int)dim_+BLOCK_SIZE-1)/dimBlock.x,((int)dim_+BLOCK_SIZE-1)/dimBlock.y);
	printf("%d %d\n",dimGrid.x,dimGrid.y);
	//Definimos la malla de la función en la GPU
	Malla f_g;
	f_g.dim = (int)dim;
	cudaMalloc((void**)&f_g.v,dim*dim*sizeof(double));
	inicializa_f<<<dimGrid, dimBlock>>>(f_g);
	//imprime_g(f_g);


	//Definimos la malla de la solución en la GPU
	Malla u_g;
	u_g.dim = (int)dim;
	cudaMalloc((void**)&u_g.v,dim*dim*sizeof(double));
	cero<<<dimGrid,dimBlock>>>(u_g);
	//imprime_g(u_g);

	//Llamamos a multigrid
	for(int i=0;i<1;i++)
	{
		nda=nd;
		nd=0.0;
		printf("[Iteración #%d]\n",i);
		multigrid(u_g,f_g,MALLAS,&nd);
		printf("\t max(defecto)=%f\tratio=%0.10f\n",nd,nd/nda);
	}
	//Limpieza de la GPU
	cudaFree(u_g.v);
	cudaFree(f_g.v);

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
void imprime_g2(Malla m) /* Imprime un elemento de una malla allocada en GPU */
{
	Malla m_h;	/* Definimos una malla, en el host */
	m_h.v= (double *)malloc(m.dim*m.dim*sizeof(double));
	cudaMemcpy(m_h.v,m.v,m.dim*m.dim*sizeof(double),cudaMemcpyDeviceToHost);
	printf("%.10f ",m_h.v[10*m.dim+10]);
	free(m_h.v);
	printf("\n");
}


void multigrid(Malla u, Malla f, int mallas,double * nd)
{
	double * norma_defecto_g;
	double * norma_defecto;
	int i;

	int m = mallas-1; /* m representa la malla en la que estamos */
	double dim  = pow(2.0,m+1)+1;
	double dim_ = pow(2.0,m)+1;

	if(m==0) /* Nº total de mallas=1, luego estamos en la malla 0 */
	{
		soluciona<<<1,1>>>(u,f);
	}
	else
	{

			/* Datos para llamar a la GPU */
		dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
		dim3 dimGrid(((int)dim+BLOCK_SIZE-1)/dimBlock.x,((int)dim+BLOCK_SIZE-1)/dimBlock.y);
		dim3 dimGrid_(((int)dim_+BLOCK_SIZE-1)/dimBlock.x,((int)dim_+BLOCK_SIZE-1)/dimBlock.y);

		/* Cramos las mallas necesarias */
		Malla d_g; // Para almacenar el defecto 
		d_g.dim = (int)dim;
		cudaMalloc((void**)&d_g.v,dim*dim*sizeof(double));
		cero<<<dimGrid,dimBlock>>>(d_g);
		Malla d__g; // Para almacenar el defecto restringido
		d__g.dim=(int)dim_;
		cudaMalloc((void **)&d__g.v,dim_*dim_*sizeof(double));
		cero<<<dimGrid_,dimBlock>>>(d__g);
		Malla v_g;	// Para almacenar las v
		v_g.dim = (int) dim;
		cudaMalloc((void**)&v_g.v,dim*dim*sizeof(double));
		cero<<<dimGrid,dimBlock>>>(v_g);
		Malla v__g; // Para almacenar la v restringida
		v__g.dim = (int) dim_;
		cudaMalloc((void**)&v__g.v,dim_*dim_*sizeof(double));
		cero<<<dimGrid_,dimBlock>>>(v__g);

		/* Presuavizado */
		suavizado_n<<<dimGrid, dimBlock>>>(u,f); // Suavizado negro
		suavizado_r<<<dimGrid, dimBlock>>>(u,f); // Suavizado rojo
		imprime_g(u);
		//printf("Malla %d\n",m); imprime_g2(u);


		/* Defecto */
		defecto<<<dimGrid,dimBlock>>>(u,f,d_g);
		imprime_g(d_g);
		
		/* Restringimos el defecto */
		restringe<<<dimGrid_,dimBlock>>>(d_g,d__g);
		imprime_g(d__g);

		/* Rellamada a multigrid */
		multigrid(v__g,d__g,mallas-1,NULL);
		imprime_g(v__g);

		/* Interpolamos la v */
		interpola<<<dimGrid_,dimBlock>>>(v__g,v_g);
		imprime_g(v_g);

		/* Sumamos */
		suma<<<dimGrid,dimBlock>>>(u,v_g);
		imprime_g(u);

		/* Volvemos a suavizar */
		suavizado_n<<<dimGrid, dimBlock>>>(u,f);	//Suavizado negro
		suavizado_r<<<dimGrid, dimBlock>>>(u,f);	//Suavizado rojo
		imprime_g(u);
		printf("Mallas %d\n",m); imprime_g2(u);


		/* En la malla mas fina, calculamos el defecto y comprobamos cómo va la cosa */
		if(mallas==MALLAS)
		{
			defecto<<<dimGrid,dimBlock>>>(u,f,d_g);
			imprime_g(d_g);

			cudaMalloc((void**)&norma_defecto_g,u.dim*sizeof(double)); //Los primeros para la norma de cada fila 
			norma_defecto=(double *)malloc(u.dim*sizeof(double));

			printf("%d %d",((int)dim+BLOCK_SIZE-1)/dimBlock.x,BLOCK_SIZE);
			calcula_max2<<<((int)dim+BLOCK_SIZE-1)/dimBlock.x,BLOCK_SIZE>>>(norma_defecto_g);

			cudaMemcpy(norma_defecto,norma_defecto_g,u.dim*sizeof(double),cudaMemcpyDeviceToHost);
			for(i=0;i<u.dim;i++)
			{
				printf("%d %f\n",i,norma_defecto[i]);
				if(abs(norma_defecto[i])>nd[0])
					nd[0]=abs(norma_defecto[i]);
			}
			printf("%f\n",nd[0]);


			free(norma_defecto);
			cudaFree(norma_defecto_g);
		}


		/* Liberamos recursos */
		cudaFree(d_g.v);
		cudaFree(d__g.v);
		cudaFree(v_g.v);
		cudaFree(v__g.v);
	}
}
