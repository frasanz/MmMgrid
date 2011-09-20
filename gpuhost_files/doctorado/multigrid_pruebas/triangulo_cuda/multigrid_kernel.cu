#define IDT(i,j) (i)*((i)+1)/2+(j)

typedef struct{
	float * v;
	int dim;
	int size;
} Grid;
__global__ void cero(Grid m)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;
	if(i<=m.dim-1 && j<=i)
	{

		if(j==0 || i==m.dim-1 || i==j)
			m.v[IDT(i,j)]=0.0;
		if(j>0 && j<i && i<m.dim-1)
			m.v[IDT(i,j)]=0.0;
	}
}
__global__ void uno(Grid m)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;
	if(i<=m.dim-1 && j<=i)
	{
		if(j==0 || i==m.dim-1 || i==j )
			m.v[IDT(i,j)]=1.0;
		if(j>0 && j<i && i<m.dim-1)
			m.v[IDT(i,j)]=1.0;
	}
}

__global__ void inicializa_f(Grid m)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;
	if(i<=m.dim-1 && j<=i)
	{

		if(j==0 || i==m.dim-1 || i==j)
			m.v[IDT(i,j)]=0.0;
		if(j>0 && j<i && i<m.dim-1)
			m.v[IDT(i,j)]=4.0;
	}
}
__global__ void random(Grid m)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;
	if(i<=m.dim-1 && j<=i)
	{

		if(j==0 || i==m.dim-1 || i==j)
			m.v[IDT(i,j)]=0.0;
		if(j>0 && j<i && i<m.dim-1)
			m.v[IDT(i,j)]=i^2+j^2;
	}
}
__global__ void suaviza_r(Grid u, Grid f, float * op)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;
	if(i<=u.dim-1 && j<=i)
	{

		if(j>0 && j<i && i<u.dim-1)
			if((i+j)%3==0)
			{
				u.v[IDT(i,j)]=(f.v[IDT(i,j)]-op[0]*u.v[IDT(i-1,j-1)]
						-op[1]*u.v[IDT(i-1,j  )]
						-op[2]*u.v[IDT(i-1,j+1)]
						-op[3]*u.v[IDT(i  ,j-1)]
						-op[5]*u.v[IDT(i  ,j+1)]
						-op[6]*u.v[IDT(i+1,j-1)]
						-op[7]*u.v[IDT(i+1,j  )]
						-op[8]*u.v[IDT(i+1,j+1)])/op[4];
			}
	}

}
__global__ void suaviza_g(Grid u, Grid f, float * op)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;
	if(i<=u.dim-1 && j<=i)
	{

		if(j>0 && j<i && i<u.dim-1)
			if((i+j)%3==1)
			{
				u.v[IDT(i,j)]=(f.v[IDT(i,j)]-op[0]*u.v[IDT(i-1,j-1)]
						-op[1]*u.v[IDT(i-1,j  )]
						-op[2]*u.v[IDT(i-1,j+1)]
						-op[3]*u.v[IDT(i  ,j-1)]
						-op[5]*u.v[IDT(i  ,j+1)]
						-op[6]*u.v[IDT(i+1,j-1)]
						-op[7]*u.v[IDT(i+1,j  )]
						-op[8]*u.v[IDT(i+1,j+1)])/op[4];
			}
	}

}
__global__ void suaviza_b(Grid u, Grid f, float * op)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;
	if(i<=u.dim-1 && j<=i)
	{

		if(j>0 && j<i && i<u.dim-1)
			if((i+j)%3==2)
			{
				u.v[IDT(i,j)]=(f.v[IDT(i,j)]-op[0]*u.v[IDT(i-1,j-1)]
						-op[1]*u.v[IDT(i-1,j  )]
						-op[2]*u.v[IDT(i-1,j+1)]
						-op[3]*u.v[IDT(i  ,j-1)]
						-op[5]*u.v[IDT(i  ,j+1)]
						-op[6]*u.v[IDT(i+1,j-1)]
						-op[7]*u.v[IDT(i+1,j  )]
						-op[8]*u.v[IDT(i+1,j+1)])/op[4];
			}
	}
}

__global__ void defecto(Grid u, Grid f, Grid d, float * op)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;

	if(i<=u.dim-1 && j<=i)
	{
		if(j>0 && j<i && i<u.dim-1 && i>0) /* puntos interiores */
		{
			d.v[IDT(i,j)]=f.v[IDT(i,j)]
				-op[1]*u.v[IDT(i-1,j)]
				-op[2]*u.v[IDT(i-1,j+1)]
				-op[3]*u.v[IDT(i,j-1)]
				-op[4]*u.v[IDT(i,j)]
				-op[5]*u.v[IDT(i,j+1)]
				-op[6]*u.v[IDT(i+1,j-1)]
				-op[7]*u.v[IDT(i+1,j)]
				-op[8]*u.v[IDT(i+1,j+1)];
		}
	}
}

__global__ void restringe(Grid sup, Grid inf)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;
	if(j>0 && j<i && i<inf.dim-1 && i>0) /* puntos interiores de la malla inferior*/
	{
		inf.v[IDT(i,j)]=(sup.v[IDT(2*i,2*j)]+0.5*(
					sup.v[IDT(2*i-1,2*j-1)]
					+sup.v[IDT(2*i-1,2*j  )]
					+sup.v[IDT(2*i  ,2*j-1)]
					+sup.v[IDT(2*i  ,2*j+1)]
					+sup.v[IDT(2*i+1,2*j  )]
					+sup.v[IDT(2*i+1,2*j+1)]))/4; 

	}
}
__global__ void interpola(Grid inf, Grid sup)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;
	if(i<=inf.dim-1 && j<=i)
	{

		if(j>0 && j<i && i<inf.dim-1 && i>0) /* puntos interiores de la malla inferior*/
		{
			sup.v[IDT(2*i  ,2*j  )] =  inf.v[IDT(i,j)];
			sup.v[IDT(2*i-1,2*j-1)] = (inf.v[IDT(i,j)]+inf.v[IDT(i-1,j-1)])/2;
			sup.v[IDT(2*i-1,2*j  )] = (inf.v[IDT(i,j)]+inf.v[IDT(i-1,j  )])/2;
			sup.v[IDT(2*i  ,2*j+1)] = (inf.v[IDT(i,j)]+inf.v[IDT(i  ,j+1)])/2;
			sup.v[IDT(2*i+1,2*j+1)] = (inf.v[IDT(i,j)]+inf.v[IDT(i+1,j+1)])/2;
			sup.v[IDT(2*i+1,2*j  )] = (inf.v[IDT(i,j)]+inf.v[IDT(i+1,j  )])/2;
			sup.v[IDT(2*i  ,2*j-1)] = (inf.v[IDT(i,j)]+inf.v[IDT(i-1,j-1)])/2;
		}
	}
}
__global__ void suma(Grid u, Grid v)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;
	if(i<u.dim && j<=i)
	{
		u.v[IDT(i,j)]=u.v[IDT(i,j)]+v.v[IDT(i,j)];
	}
}

