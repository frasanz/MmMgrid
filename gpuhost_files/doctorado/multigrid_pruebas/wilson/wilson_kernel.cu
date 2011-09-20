typedef unsigned long long u64;
typedef struct {
	u64 r1;
	u64 r2;
} Representacion;

__device__ Representacion g_get_representacion(u64 a, u64 p)
{
	Representacion re;
	re.r1=a/p;
	re.r2=a%p;
	return re;
}
__device__ Representacion g_mul(Representacion a, Representacion b, u64 p)
{
	Representacion mul;
	mul.r2=(a.r2*b.r2)%p;
	mul.r1=(a.r2*b.r2)/p;
	mul.r1=(mul.r1+a.r1*b.r2+b.r1*a.r2)%p;
	return mul;
}
__global__ void cero(Representacion *num)
{
	num[threadIdx.x].r1=1;
	num[threadIdx.x].r2=1;
}

__global__ void fac_mod_parll(Representacion * num, u64 primo, u64 espacio)
{
	u64 i;
	u64 max;
	__shared__ Representacion mult;
	mult.r1=1;
	mult.r2=1;
	if((threadIdx.x+1)*espacio+1 > primo-1)
		max=primo-1;
	else
		max=(threadIdx.x+1)*espacio+1;
	for(i=threadIdx.x*espacio+2;i<=max;i++)
	{
		mult=g_mul(mult,g_get_representacion(i,primo),primo);
	}
	num[threadIdx.x]=mult;
}
