#include "multigrid.h"

__global__ void cero(Grid m)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;
	if(i<m.dim && j<=i)
		m.v[I(m.dim,i,j)]=0.0;
}
__global__ void random(Grid m)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;
	if(i<m.dim && j<m.dim)
		m.v[I(m.dim,i,j)]=0.0;
	if(i<m.dim-1 && j<i && i>0 && j>0)
		m.v[I(m.dim,i,j)]=10.2*sinf(i+cosf(j));
}
__global__ void suaviza(Grid u, Grid f,int color)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;
	if(i>0 && j> 0  && i < u.dim-1 && j<i)
	{
		if((i+j)%3==color)
		{

			u.v[I(u.dim,i,j)]=(f.v[I(u.dim,i,j)]-(
												 u.op[0]*u.v[I(u.dim,i-1,j-1)]
												+u.op[1]*u.v[I(u.dim,i-1,j)]
												+u.op[3]*u.v[I(u.dim,i,j-1)]
												+u.op[5]*u.v[I(u.dim,i,j+1)]
												+u.op[6]*u.v[I(u.dim,i+1,j)]
												+u.op[7]*u.v[I(u.dim,i+1,j+1)]))/u.op[4];
		}
	}
}
__global__ void defecto(Grid u, Grid f, Grid d)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;
	if(i>0 && j> 0  && i < u.dim-1 && j<i)
	{
		d.v[I(d.dim,i,j)]=(f.v[I(d.dim,i,j)]-(
			 u.op[0]*u.v[I(u.dim,i-1,j-1)]
			+u.op[1]*u.v[I(u.dim,i-1,j  )]
			+u.op[3]*u.v[I(u.dim,i  ,j-1)]
			+u.op[4]*u.v[I(u.dim,i  ,j  )]
			+u.op[5]*u.v[I(u.dim,i  ,j+1)]
			+u.op[7]*u.v[I(u.dim,i+1,j  )]
			+u.op[8]*u.v[I(u.dim,i+1,j+1)]
		));
	}
}
__global__ void restringe(Grid sup, Grid inf)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;
	if(i>0 && j> 0  && i < inf.dim-1 && j<i)
	{
		inf.v[I(inf.dim,i,j)]=(sup.v[I(sup.dim,2*i  ,2*j  )]+
				0.5*(sup.v[I(sup.dim,2*i-1,2*j-1)]+
					sup.v[I(sup.dim,2*i-1,2*j  )]+
					sup.v[I(sup.dim,2*i  ,2*j-1)]+
					sup.v[I(sup.dim,2*i  ,2*j+1)]+
					sup.v[I(sup.dim,2*i+1,2*j  )]+
					sup.v[I(sup.dim,2*i+1,2*j+1)]))/4;
	}
}
__global__ void interpola(Grid inf, Grid sup)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;
	if(i>0 && j> 0  && i < inf.dim-1 && j<i)
	{
		sup.v[I(sup.dim,2*i  ,2*j  )] = inf.v[I(inf.dim,i  ,j  )];
		sup.v[I(sup.dim,2*i-1,2*j-1)] =(inf.v[I(inf.dim,i  ,j  )] + inf.v[I(inf.dim,i-1,j-1)] )/2;
		sup.v[I(sup.dim,2*i-1,2*j  )] =(inf.v[I(inf.dim,i  ,j  )] + inf.v[I(inf.dim,i-1,j  )] )/2;
		sup.v[I(sup.dim,2*i  ,2*j-1)] =(inf.v[I(inf.dim,i  ,j  )] + inf.v[I(inf.dim,i  ,j-1)] )/2;
		sup.v[I(sup.dim,2*i  ,2*j+1)] =(inf.v[I(inf.dim,i  ,j  )] + inf.v[I(inf.dim,i  ,j+1)] )/2;
		sup.v[I(sup.dim,2*i+1,2*j  )] =(inf.v[I(inf.dim,i  ,j  )] + inf.v[I(inf.dim,i+1,j  )] )/2;
		sup.v[I(sup.dim,2*i+1,2*j  )] =(inf.v[I(inf.dim,i  ,j  )] + inf.v[I(inf.dim,i+1,j  )] )/2;
	}
}
__global__ void suma(Grid u, Grid v)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;
	if(i>0 && j> 0 && i< u.dim-1 && j< u.dim-1)
	{
		u.v[I(u.dim,i,j)]=u.v[I(u.dim,i,j)]+v.v[I(v.dim,i,j)];
	}
}
