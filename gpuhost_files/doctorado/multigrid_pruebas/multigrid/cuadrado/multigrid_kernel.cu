#include <multigrid.h>
__global__ void cero(Malla m)
{
	int x = blockIdx.x*blockDim.x + threadIdx.x;
	int y = blockIdx.y*blockDim.y + threadIdx.y;
	if(x<m.dim && y < m.dim)
	{
		m.v[x*m.dim+y]=0.0;
	}
}

__global__ void inicializa_f(Malla m)
{
	int x = blockIdx.x*blockDim.x + threadIdx.x;
	int y = blockIdx.y*blockDim.y + threadIdx.y;
	double coord_x=1.0*y/(m.dim-1);
	double coord_y=1.0*x/(m.dim-1);
	if(x==m.dim-1 || y==m.dim-1 || x==0 || y==0)
		m.v[x*m.dim+y]=0.0;
	if(x<m.dim-1 && y<m.dim-1 && x>0 && y>0)
		m.v[x*m.dim+y]=cos(coord_x*coord_y);
}


__global__ void inicializa(Malla m)
{

	int x = blockIdx.x*blockDim.x + threadIdx.x;
	int y = blockIdx.y*blockDim.y + threadIdx.y;
	if(x==m.dim-1 || y==m.dim-1 || x==0 || y==0)
		m.v[x*m.dim+y]=0.0;
	if(x<m.dim-1 && y<m.dim-1 && x>0 && y>0)
		m.v[x*m.dim+y]=1;
}
__global__ void suavizado_n(Malla u, Malla f)
{
	int x = blockIdx.x*blockDim.x + threadIdx.x;
	int y = blockIdx.y*blockDim.y + threadIdx.y;
	double h2=pow(1.0/(u.dim-1),2);
	if(x<u.dim-1 && y < u.dim-1 && x>0 && y>0 && !((x+y)%2))
		u.v[x*u.dim+y]=0.25*(f.v[x*u.dim+y]*h2+
		                     u.v[(x-1)*u.dim+y]+
												 u.v[(x+1)*u.dim+y]+
												 u.v[x*u.dim+y-1]+
												 u.v[x*u.dim+y+1]);
}
__global__ void suavizado_r(Malla u, Malla f)
{
	int x = blockIdx.x*blockDim.x + threadIdx.x;
	int y = blockIdx.y*blockDim.y + threadIdx.y;
	double h2=pow(1.0/(u.dim-1),2);
	if(x<u.dim-1 && y < u.dim-1 && x>0 && y>0 && ((x+y)%2))
		u.v[x*u.dim+y]=0.25*(f.v[x*u.dim+y]*h2+
		                     u.v[(x-1)*u.dim+y]+
												 u.v[(x+1)*u.dim+y]+
												 u.v[x*u.dim+y-1]+
												 u.v[x*u.dim+y+1]);
}
__global__ void defecto(Malla u, Malla f, Malla d)
{
	int x = blockIdx.x*blockDim.x + threadIdx.x;
	int y = blockIdx.y*blockDim.y + threadIdx.y;
	double h2=pow(1.0/(u.dim-1),2);
	if(x==u.dim-1 || y==u.dim-1 || x==0 || y==0)
		d.v[x*u.dim+y]=0.0;
	if(x<u.dim-1 && y < u.dim-1 && x>0 && y>0)
	{
		d.v[x*u.dim+y]=f.v[x*u.dim+y]-
			(4*u.v[x*u.dim+y]-u.v[(x-1)*u.dim+y]-u.v[(x+1)*u.dim+y]-u.v[x*u.dim+(y-1)]-u.v[x*u.dim+(y+1)])/h2;
	}
}


__global__ void restringe(Malla u, Malla u_)
{
	int x = blockIdx.x*blockDim.x + threadIdx.x;
	int y = blockIdx.y*blockDim.y + threadIdx.y;
	if(x<u_.dim-1 && y<u_.dim-1 && x>0 && y>0)
	{
		u_.v[x*u_.dim+y]=(
			4*u.v[(2*x  )*u.dim+2*y  ]+
			2*u.v[(2*x-1)*u.dim+2*y  ]+
			2*u.v[(2*x+1)*u.dim+2*y  ]+
			2*u.v[(2*x  )*u.dim+2*y-1]+
			2*u.v[(2*x  )*u.dim+2*y+1]+
			  u.v[(2*x-1)*u.dim+2*y-1]+
				u.v[(2*x-1)*u.dim+2*y+1]+
				u.v[(2*x+1)*u.dim+2*y-1]+
				u.v[(2*x+1)*u.dim+2*y+1])/16;
	}

}

__global__ void interpola(Malla u_, Malla u)
{
	int x = blockIdx.x*blockDim.x + threadIdx.x;
	int y = blockIdx.y*blockDim.y + threadIdx.y;
	if(x<u_.dim && y< u_.dim )
	{
		u.v[2*x*u.dim+2*y]=u_.v[x*u_.dim+y];
		if(2*x+1<u.dim)
			u.v[(2*x+1)*u.dim+2*y]=(u_.v[x*u_.dim+y]+u_.v[(x+1)*u_.dim+y])/2;
		if(2*y+1<u.dim)
			u.v[2*x*u.dim+2*y+1]=(u_.v[x*u_.dim+y]+u_.v[x*u_.dim+y+1])/2;
		if(2*x+1<u.dim && 2*y+1<u.dim)
			u.v[(2*x+1)*u.dim+2*y+1]=(u_.v[x*u_.dim+y]  +u_.v[(x+1)*u_.dim+y]+
			                          u_.v[x*u_.dim+y+1]+u_.v[(x+1)*u_.dim+y+1])/4;


	}
}
__global__ void suma(Malla u, Malla v)
{
	int x = blockIdx.x*blockDim.x + threadIdx.x;
	int y = blockIdx.y*blockDim.y + threadIdx.y;
	if(x<u.dim && y< u.dim)
	{
		u.v[x*u.dim+y]=u.v[x*u.dim+y]+v.v[x*u.dim+y];
	}
}
__global__ void soluciona(Malla u, Malla f)
{
	u.v[4]=f.v[4]/16;
}

__global__ void calcula_max(Malla m, double * max)
{
	int x = blockIdx.x*blockDim.x + threadIdx.x;
	int j;
	max[x]=0.0;
	if(x<m.dim)
	{
		for(j=1;j<m.dim-1;j++)
		{
			max[x]=x;
			if(abs(m.v[x*m.dim+j])>max[x])
				max[x]=x;
		}
	}
}
__global__ void calcula_max2(double * max)
{
	int x = blockIdx.x*blockDim.x + threadIdx.x;
	if(x<257)
	{
			max[x]=x;
	}
}


