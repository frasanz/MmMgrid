#define I(d,i,j) (i)*(d)+(j) 

typedef struct{
	float *v;
	int d;
	int size;
} Grid;

__global__ void cero(Grid m)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;
	if(i<=m.d && j<=m.d)
		m.v[I(m.d,i,j)]=0.0;
}
__global__ void random(Grid m)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;
	if(i<m.d-1 && j<m.d-1 && i>0 && j>0) // Puntos interiores
		m.v[I(m.d,i,j)]=10.1+sinf(i+cosf(j));
}
__global__ void suaviza_r(Grid u,Grid f)
{
	float h2 = pow(1.0/(u.d-1),2);
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;
	if(i<u.d-1 && j<u.d-1 && i>0 && j>0) // Puntos interiores
	{
		if((i+j)%2==0)
			u.v[I(u.d,i,j)]=0.25*(f.v[I(u.d,i  ,j  )]*h2
			                     +u.v[I(u.d,i-1,j  )]
													 +u.v[I(u.d,i+1,j  )]
													 +u.v[I(u.d,i  ,j-1)]
													 +u.v[I(u.d,i  ,j+1)]);
	}

}
__global__ void suaviza_n(Grid u,Grid f)
{
	float h2 = pow(1.0/(u.d-1),2);
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;
	if(i<u.d-1 && j<u.d-1 && i>0 && j>0) // Puntos interiores
	{
		if((i+j)%2==1)
			u.v[I(u.d,i,j)]=0.25*(f.v[I(u.d,i  ,j  )]*h2
			                     +u.v[I(u.d,i-1,j  )]
													 +u.v[I(u.d,i+1,j  )]
													 +u.v[I(u.d,i  ,j-1)]
													 +u.v[I(u.d,i  ,j+1)]);
	}

}
__global__ void defecto(Grid u, Grid f, Grid d)
{
	float h2 = pow(1.0/(u.d-1),2);
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;
	if(i<u.d-1 && j<u.d-1 && i>0 && j>0) // Puntos interiores
	{
		d.v[I(u.d,i,j)] = f.v[I(u.d,i  ,j  )] 
		              -(4*u.v[I(u.d,i  ,j  )]
									   -u.v[I(u.d,i-1,j  )]
										 -u.v[I(u.d,i+1,j  )]
										 -u.v[I(u.d,i  ,j-1)]
										 -u.v[I(u.d,i  ,j+1)])/h2;
	}
}
__global__ void restringe(Grid sup, Grid in)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;
	if(i<in.d-1 && j<in.d-1 && i>0 && j>0) // Puntos interiores
	{
		in.v[I(in.d,i,j)] = (4* sup.v[I(sup.d,2*i  ,2*j  )]
		                    +2*(sup.v[I(sup.d,2*i-1,2*j  )]
                           +sup.v[I(sup.d,2*i+1,2*j  )]
													 +sup.v[I(sup.d,2*i  ,2*j-1)]
													 +sup.v[I(sup.d,2*i  ,2*j+1)])
											     +sup.v[I(sup.d,2*i-1,2*j-1)]
													 +sup.v[I(sup.d,2*i-1,2*j+1)]
													 +sup.v[I(sup.d,2*i+1,2*j-1)]
													 +sup.v[I(sup.d,2*i+1,2*j+1)])/16;
	}
}
__global__ void exacta(Grid u, Grid f)
{
	u.v[I(u.d,1,1)]=f.v[I(u.d,1,1)]/16;
}
__global__ void interpola(Grid u, Grid v)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;
	if(i<u.d && j<u.d)
	{
		v.v[I(v.d,2*i,2*j)] = u.v[I(u.d,i,j)];
		if(2*i+1<v.d)
			v.v[I(v.d,2*i+1,2*j)]=(u.v[I(u.d,i,j)]+u.v[I(u.d,i+1,j)])/2;
		if(2*j+1<v.d)
			v.v[I(v.d,2*i,2*j+1)]=(u.v[I(u.d,i,j)]+u.v[I(u.d,i,j+1)])/2;
		if(2*i+1<v.d && 2*j+1<v.d)
			v.v[I(v.d,2*i+1,2*j+1)]=(u.v[I(u.d,i,j)]+u.v[I(u.d,i+1,j)]+u.v[I(u.d,i,j+1)]+u.v[I(u.d,i+1,j+1)])/4;
	}
}
__global__ void suma(Grid u, Grid v)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;
	if(i<u.d && j<u.d)
	{
		u.v[I(u.d,i,j)]+=v.v[I(u.d,i,j)];
	}
}
__global__ void maxx(Grid d,float * def)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j;
	def[i]=0.0;
	for(j=0;j<d.d;j++)
	{
		if(abs(d.v[I(d.d,i,j)])>def[i])
		{
			def[i]=abs(d.v[I(d.d,i,j)]);
		}
	}
}


