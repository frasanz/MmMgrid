/*
 * =====================================================================================
 *
 *       Filename:  operator.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/08/11 17:47:17
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

/* TODO: Check all this */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "operator.h"
#include "io.h"

extern int debug;

void build_operator(int id, Element * element, Vertex * vertex)
{
	if(debug>3)
		printf("\t[WHERE] In function build_operator for node %d\n",id); 
	
	double B[2][2];
	double B_inv[2][2];
	double C[2][2];
	double det;

	int i,j,k;

	/* Matrix */
	double Sxx[3][3] = {{ 0.0, 0.0, 0.0},
	                    {-1.0, 2.0,-1.0},
											{ 0.0, 0.0, 0.0}};
	
	double Syy[3][3] = {{ 0.0,-1.0, 0.0},
	                    { 0.0, 2.0, 0.0},
											{ 0.0,-1.0, 0.0}};

	double Sxy[3][3] = {{ 0.0, 1.0,-1.0},
	                    { 1.0,-2.0, 1.0},
											{-1.0, 1.0, 0.0}}; 

	/* Initialize matrix B*/
	B[0][0]=vertex[element[id].node[1]].c[0] - vertex[element[id].node[0]].c[0];
	B[0][1]=vertex[element[id].node[2]].c[0] - vertex[element[id].node[1]].c[0];
	B[1][0]=vertex[element[id].node[1]].c[1] - vertex[element[id].node[0]].c[1];
	B[1][1]=vertex[element[id].node[2]].c[1] - vertex[element[id].node[1]].c[1];
	if(debug>3)
		printf("\t[DEBUG] B= \t[%f, %f; \n\t\t\t %f, %f]\n",B[0][0],B[0][1],B[1][0],B[1][1]);

	/* Compute the determinant of B */
	det = B[0][0]*B[1][1]-B[0][1]*B[1][0];
	if(debug>3)
		printf("\t[DEBUG] |B|=%f\n",det);

	/* Compute B^-1 */
	B_inv[0][0] = B[1][1]/det;
	B_inv[0][1] =-B[0][1]/det;
	B_inv[1][0] =-B[1][0]/det;
	B_inv[1][1] = B[0][0]/det;
	if(debug>3)
		printf("\t[DEBUG] B_inv= \t[%f, %f; \n\t\t\t %f, %f]\n",B_inv[0][0],B_inv[0][1],B_inv[1][0],B_inv[1][1]);
	
	/* Compute C = B^-1*(B^-1)' */
	C[0][0]=B_inv[0][0]*B_inv[0][0]+B_inv[0][1]*B_inv[0][1];
	C[0][1]=B_inv[0][0]*B_inv[1][0]+B_inv[0][1]*B_inv[1][1];
	C[1][0]=B_inv[1][0]*B_inv[0][0]+B_inv[1][1]*B_inv[0][1];
	C[1][1]=B_inv[1][0]*B_inv[1][0]+B_inv[1][1]*B_inv[1][1];

	if(debug>3){
		printf("\t[DEBUG] C= \t[%f, %f; \n\t\t\t %f, %f]\n",C[0][0],C[0][1],C[1][0],C[1][1]);
		printf("\t[DEBUG] operator[0]=");
	}

	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			element[id].operator[0].v[i][j]=C[0][0]*Sxx[i][j]+(C[0][1]+C[1][0])*Sxy[i][j]+C[1][1]*Syy[i][j];
			if(debug>3)
				printf("%f ",element[id].operator[0].v[i][j]);
		}
		if(debug>3)
			printf("\n\t\t\t");
	}
	if(debug>3)
		printf("\n");

	/* We can now scale the operator to the different levels */
	for(i=1;i<element[id].n_levels;i++){
		if(debug>4)
			printf("\t[DEBUG] operator[%d]=",i);
		for(j=0;j<3;j++){
			for(k=0;k<3;k++){
				element[id].operator[i].v[j][k]=element[id].operator[0].v[j][k]*pow(2,i);
				if(debug>4)
					printf("%f ",element[id].operator[i].v[j][k]);
			}
			if(debug>4)
				printf("\n\t\t\t");
		}
		if(debug>4)
			printf("\n");
	}
}
