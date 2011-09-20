/*
 * =====================================================================================
 *
 *       Filename:  prueba_if.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  16/12/10 12:00:03
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <math.h>
#define NUM 200000000
void haz_if_doble();
void haz_if_modulo();
void haz_if_simple();

int main()
{
	haz_if_doble();
	haz_if_modulo();
	haz_if_simple();
}

void haz_if_modulo()
{
	int i;
	for(i=0;i<NUM;i++)
	{
		if(i%2==0)
		{
			exp(2.3*i);
		}
	}
	for(i=0;i<NUM;i++)
	{
		if(i%2==1)
		{
			exp(2.3*i);
		}
	}
}
void haz_if_doble()
{
	int i;
	for(i=0;i<NUM;i=i+2)
		exp(2.3*i);
	for(i=1;i<NUM;i=i+2)
		exp(2.3*i);
}
void haz_if_simple()
{
	int i;
	for(i=0;i<NUM/2;i++)
		exp(2.3*(i*2+1));
	for(i=1;i<NUM/2;i++)
		exp(2.3*(i*2-1));
}

