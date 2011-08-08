/*
 * =====================================================================================
 *
 *       Filename:  test1.c
 *
 *    Description:  This is to test the mesh allocation
 *
 *        Version:  1.0
 *        Created:  06/08/11 13:36:31
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <stdio.h>

int main()
{
	int dim=3;
	int total=dim*(dim+1)/2;
	int i,j;
	double ** mesh;
		mesh=(double**)malloc(dim*sizeof(double*));
		for(j=0;j<3;j++){
			mesh[j]=(double*)malloc(j*sizeof(double));
	}



	return 0;
}

