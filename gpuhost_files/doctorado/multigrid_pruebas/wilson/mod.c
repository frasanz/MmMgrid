/*
 * =====================================================================================
 *
 *       Filename:  mod.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/15/2010 02:52:04 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <stdio.h>
typedef unsigned long long u64;
typedef struct
{
	u64 r1;
	u64 r2;
} representacion;

representacion get_representacion(u64 a, u64 p)
{
	representacion re;
	re.r1=a/p;
	re.r2=a%p;
	return re;
}

representacion mul(representacion a, representacion b, u64 p)
{
	representacion mul;
	mul.r2=(a.r2*b.r2)%p;
	mul.r1=(a.r2*b.r2)/p;
	mul.r1=(mul.r1+a.r1*b.r2+b.r1*a.r2)%p;
	return mul;
}

int main()
{
	u64 primo=50000003;
	u64 max=primo-1;
	u64 min=2;
	u64 i;

	representacion ar;
	representacion br;
	representacion mu;

	mu=get_representacion(1,primo);
	for(i=min;i<=max;i++)
	{
		//printf("%d %d\n",mu.r1,mu.r2);
		mu=mul(mu,get_representacion(i,primo),primo);
	}
	printf("%lld %lld\n",mu.r1,mu.r2);
}
