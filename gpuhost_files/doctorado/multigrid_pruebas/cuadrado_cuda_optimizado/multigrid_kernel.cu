#define I(d,i,j) (i)*(d)+(j) 
#define B(i) (i+1)
#define BLOCK_DIM 16

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
  __shared__ float bf[BLOCK_DIM][BLOCK_DIM];
  __shared__ float buo[BLOCK_DIM+2][BLOCK_DIM+2];

  float h2 = pow(1.0/(u.d-1),2);
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  int j = blockIdx.y * blockDim.y + threadIdx.y;

  int n=threadIdx.x;
  int m=threadIdx.y;
  if(i<=u.d-1 && j<=u.d-1 && i>=0 && j>=0) // Carga chapucera en shared
  {
    bf[threadIdx.x][threadIdx.y] = f.v[I(u.d,i  ,j  )];
    buo[threadIdx.x+1][threadIdx.y+1] = u.v[I(u.d,i,j  )];
    if(threadIdx.x==0 && i-1>=0) //Si el limite sup del bloque está, lo cargamos
      buo[threadIdx.x][threadIdx.y+1] = u.v[I(u.d,i-1,j  )];
    if(threadIdx.x==(u.d-2)%BLOCK_DIM && i+1<=u.d-1)
      buo[threadIdx.x+2][threadIdx.y+1] = u.v[I(u.d,i+1,j  )];
    if(threadIdx.y==0 && j-1>=0) //Si el limite izq del bloque está, lo cargamos
      buo[threadIdx.x+1][threadIdx.y] = u.v[I(u.d,i,j-1  )];
    if(threadIdx.y==(u.d-2)%BLOCK_DIM && j+1<=u.d-1)
      buo[threadIdx.x+1][threadIdx.y+2] = u.v[I(u.d,i,j+1  )];
  }
  __syncthreads();

  if(i<u.d-1 && j<u.d-1 && i>0 && j>0) // Puntos interiores
  {
    if((i+j)%2==0)
      u.v[I(u.d,i,j)]=0.25*(bf[n][m]*h2
                           +buo[B(n-1)][B(m)]
                           +buo[B(n+1)][B(m)]
                           +buo[B(n)][B(m-1)]
                           +buo[B(n)][B(m+1)]);
  }
}
__global__ void suaviza_n(Grid u,Grid f)
{
  __shared__ float bf[BLOCK_DIM][BLOCK_DIM];
  __shared__ float buo[BLOCK_DIM+2][BLOCK_DIM+2];

  float h2 = pow(1.0/(u.d-1),2);
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  int j = blockIdx.y * blockDim.y + threadIdx.y;

  int n=threadIdx.x;
  int m=threadIdx.y;
  if(i<=u.d-1 && j<=u.d-1 && i>=0 && j>=0) // Carga chapucera en shared
  {
    bf[threadIdx.x][threadIdx.y] = f.v[I(u.d,i  ,j  )];
    buo[threadIdx.x+1][threadIdx.y+1] = u.v[I(u.d,i,j  )];
    if(threadIdx.x==0 && i-1>=0) //Si el limite sup del bloque está, lo cargamos
      buo[threadIdx.x][threadIdx.y+1] = u.v[I(u.d,i-1,j  )];
    if(threadIdx.x==(u.d-2)%BLOCK_DIM && i+1<=u.d-1)
      buo[threadIdx.x+2][threadIdx.y+1] = u.v[I(u.d,i+1,j  )];
    if(threadIdx.y==0 && j-1>=0) //Si el limite izq del bloque está, lo cargamos
      buo[threadIdx.x+1][threadIdx.y] = u.v[I(u.d,i,j-1  )];
    if(threadIdx.y==(u.d-2)%BLOCK_DIM && j+1<=u.d-1)
      buo[threadIdx.x+1][threadIdx.y+2] = u.v[I(u.d,i,j+1  )];
  }
  __syncthreads();

  if(i<u.d-1 && j<u.d-1 && i>0 && j>0) // Puntos interiores
  {
    if((i+j)%2==1)
      u.v[I(u.d,i,j)]=0.25*(bf[n][m]*h2
                           +buo[B(n-1)][B(m)]
                           +buo[B(n+1)][B(m)]
                           +buo[B(n)][B(m-1)]
                           +buo[B(n)][B(m+1)]);
  }

}
__global__ void defecto(Grid u, Grid f, Grid d)
{
  float h2 = pow(1.0/(u.d-1),2);

  __shared__ float bu[BLOCK_DIM+2][BLOCK_DIM+2];
  __shared__ float bf[BLOCK_DIM][BLOCK_DIM];

  int i = blockIdx.x * blockDim.x + threadIdx.x;
  int j = blockIdx.y * blockDim.y + threadIdx.y;
  int n=threadIdx.x;
  int m=threadIdx.y;
  if(i<=u.d-1 && j<=u.d-1 && i>=0 && j>=0) // Carga chapucera en shared
  {
    bf[threadIdx.x][threadIdx.y] = f.v[I(u.d,i  ,j  )];
    bu[threadIdx.x+1][threadIdx.y+1] = u.v[I(u.d,i,j  )];
    if(threadIdx.x==0 && i-1>=0) //Si el limite sup del bloque está, lo cargamos
      bu[threadIdx.x][threadIdx.y+1] = u.v[I(u.d,i-1,j  )];
    if(threadIdx.x==(u.d-2)%BLOCK_DIM && i+1<=u.d-1)
      bu[threadIdx.x+2][threadIdx.y+1] = u.v[I(u.d,i+1,j  )];
    if(threadIdx.y==0 && j-1>=0) //Si el limite izq del bloque está, lo cargamos
      bu[threadIdx.x+1][threadIdx.y] = u.v[I(u.d,i,j-1  )];
    if(threadIdx.y==(u.d-2)%BLOCK_DIM && j+1<=u.d-1)
      bu[threadIdx.x+1][threadIdx.y+2] = u.v[I(u.d,i,j+1  )];
  }
  __syncthreads();

  if(i<u.d-1 && j<u.d-1 && i>0 && j>0) // Puntos interiores
  {
    d.v[I(u.d,i,j)]= bf[threadIdx.x][threadIdx.y]
                  -(4*bu[B(n)][B(m)]
                     -bu[B(n-1)][B(m)]
                     -bu[B(n+1)][B(m)]
                     -bu[B(n)][B(m-1)]
                     -bu[B(n)][B(m+1)])/h2;
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
  __shared__ float bv[BLOCK_DIM][BLOCK_DIM];
  if(i<u.d && j<u.d)
  {
     bv[threadIdx.x][threadIdx.y]=v.v[I(u.d,i,j)];
  }
  __syncthreads();
  if(i<u.d && j<u.d)
  {
    u.v[I(u.d,i,j)]+=bv[threadIdx.x][threadIdx.y];
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


