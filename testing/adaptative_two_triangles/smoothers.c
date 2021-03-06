/*
 * =====================================================================================
 *
 *       Filename:  smoothers.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/08/11 01:11:59
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "smoothers.h"
#include "io.h"
#include "ghost.h"

extern int debug;

void defect(Element element, int this_level)
{
	print_debug(3,"\t[WHERE] In function defect\n", "");
	int i,j;
#pragma omp parallel for private(i,j)
	for(i=0;i<element.mesh[this_level].number_nodes_base;i++){
		for(j=0;j<=i;j++){
			if(i>0 && j>0 && j<i && i< element.mesh[this_level].number_nodes_base-1
					&& j< element.mesh[this_level].number_nodes_base-1){ /* Interior points */
				element.mesh[this_level].d[i][j]=element.mesh[this_level].f[i][j]-
					(element.operator[this_level].v[0][0] * element.mesh[this_level].u[i-1][j-1] +
					 element.operator[this_level].v[0][1] * element.mesh[this_level].u[i-1][j  ] +
					 element.operator[this_level].v[1][0] * element.mesh[this_level].u[i  ][j-1] +
					 element.operator[this_level].v[1][1] * element.mesh[this_level].u[i  ][j  ] +
					 element.operator[this_level].v[1][2] * element.mesh[this_level].u[i  ][j+1] +
					 element.operator[this_level].v[2][1] * element.mesh[this_level].u[i+1][j  ] +
					 element.operator[this_level].v[2][2] * element.mesh[this_level].u[i+1][j+1]
					);
			}
			else if(i==element.mesh[this_level].number_nodes_base-1
					&& j> 0
					&& j< element.mesh[this_level].number_nodes_base-1){
				element.mesh[this_level].d[i][j]=element.mesh[this_level].f[i][j]-
					(element.operator[this_level].v[0][0] * element.mesh[this_level].u[i-1][j-1] +
					 element.operator[this_level].v[0][1] * element.mesh[this_level].u[i-1][j  ] +
					 element.operator[this_level].v[1][0] * element.mesh[this_level].u[i  ][j-1] +
					 element.operator[this_level].v[1][1] * element.mesh[this_level].u[i  ][j  ] +
					 element.operator[this_level].v[1][2] * element.mesh[this_level].u[i  ][j+1] +
					 element.operator[this_level].v[2][1] * element.mesh[this_level].u_gh[0][j] +
					 element.operator[this_level].v[2][2] * element.mesh[this_level].u_gh[0][j+1]
					);
			}
		}
	}
}

/* TODO: Do this in a better way*/
void smooth_rgb(Element element, int this_level,int color)
{
	print_debug(3,"\t[WHERE] In function smooth_rgb\n","");
	int i,j;
	{
#pragma omp parallel for private(i,j)
		for(i=0;i<element.mesh[this_level].number_nodes_base;i++){
			for(j=1;j<i;j++){
				if((i+j)%3==color){
					if(i>0 && j>0 && j<i && i< element.mesh[this_level].number_nodes_base-1 
							&& j< element.mesh[this_level].number_nodes_base-1){ /* Interior points */
						element.mesh[this_level].u[i][j]=
							(element.mesh[this_level].f[i][j]-
							 (element.operator[this_level].v[0][0] * element.mesh[this_level].u[i-1][j-1] +
								element.operator[this_level].v[0][1] * element.mesh[this_level].u[i-1][j  ] +
								element.operator[this_level].v[1][0] * element.mesh[this_level].u[i  ][j-1] +
								element.operator[this_level].v[1][2] * element.mesh[this_level].u[i  ][j+1] +
								element.operator[this_level].v[2][1] * element.mesh[this_level].u[i+1][j]   +
								element.operator[this_level].v[2][2] * element.mesh[this_level].u[i+1][j+1])
							)/element.operator[this_level].v[1][1];
					}
					else if(i==element.mesh[this_level].number_nodes_base-1 
							&& j>0 
							&& j<element.mesh[this_level].number_nodes_base-1){  /* This is the edge 0-1 */
						if(element.nei[0][0]>-1){ /* There is a neirghbor */
							element.mesh[this_level].u[i][j]=
								(element.mesh[this_level].f[i][j]-
								 (element.operator[this_level].v[0][0] * element.mesh[this_level].u[i-1][j-1] +
									element.operator[this_level].v[0][1] * element.mesh[this_level].u[i-1][j  ] +
									element.operator[this_level].v[1][0] * element.mesh[this_level].u[i  ][j-1] +
									element.operator[this_level].v[1][2] * element.mesh[this_level].u[i  ][j+1] +
									element.operator[this_level].v[2][1] * element.mesh[this_level].u_gh[0][j]  +
									element.operator[this_level].v[2][2] * element.mesh[this_level].u_gh[0][j+1])
								)/element.operator[this_level].v[1][1];
						}
					}
				}
			}
		}
	}
}
void restrict_one(Element element, int this_level)
{
	int i,j,k,l;
	for(i=1;i<element.mesh[this_level-1].number_nodes_base-1;i++){  /* This is the  */
		for(j=1;j<i;j++){                                             /* interior of the triangle */
			k=2*i;
			l=2*j;
			element.mesh[this_level-1].f[i][j]=(element.mesh[this_level].d[k  ][l  ]+0.5*
			                                   (element.mesh[this_level].d[k-1][l-1]+
																				  element.mesh[this_level].d[k-1][l  ]+
																					element.mesh[this_level].d[k  ][l-1]+
																					element.mesh[this_level].d[k  ][l+1]+
																					element.mesh[this_level].d[k+1][l  ]+
																					element.mesh[this_level].d[k+1][l+1]))/4;
		}
	}
	for(j=1;j<element.mesh[this_level-1].number_nodes_base-1;j++){ 
		k=2*(element.mesh[this_level-1].number_nodes_base-1);
		l=2*j;
		element.mesh[this_level-1].f[i][j]=(element.mesh[this_level].d[k ][l ]+0.5*
																			 (element.mesh[this_level].d[k][l-1]+element.mesh[this_level].d[k][l+1]))/2; /* Edge 0-1 */
		element.mesh[this_level-1].f[j][j]=(element.mesh[this_level].d[l][l]+0.5*
																			 (element.mesh[this_level].d[l-1][l-1]+element.mesh[this_level].d[l+1][l+1]))/2; /* Edge 1-2 */
		element.mesh[this_level-1].f[j][0]=(element.mesh[this_level].d[l][0]+0.5*
																			 (element.mesh[this_level].d[l-1][0]+element.mesh[this_level].d[l+1][0]))/2;  /* Edge 2-0  */
	}
	element.mesh[this_level-1].f[0][0]=element.mesh[this_level].d[0][0]; /* 2= (0,0) */
	element.mesh[this_level-1].f[element.mesh[this_level-1].number_nodes_base-1][element.mesh[this_level-1].number_nodes_base-1]=
		element.mesh[this_level].d[element.mesh[this_level].number_nodes_base-1][element.mesh[this_level].number_nodes_base-1]; /* 1=(max,max)*/
	element.mesh[this_level-1].f[element.mesh[this_level-1].number_nodes_base-1][0]=
		element.mesh[this_level].d[element.mesh[this_level].number_nodes_base-1][0]; /* 0=(max,0) */
}

/* TODO: Do this in a best way  */

void interpolate_one(Element element, int this_level)
{
	print_debug(3,"\t[WHERE] In function interpolate_one\n", "");
	int i,j,k,l;
#pragma omp parallel for private(i,j,k,l)
	for(i=0;i<element.mesh[this_level-1].number_nodes_base;i++){
		for(j=0;j<=i;j++){
			k=2*i;
			l=2*j;
			element.mesh[this_level].v[k  ][l  ]= element.mesh[this_level-1].u[i  ][j  ];
			if(k+1<element.mesh[this_level].number_nodes_base)
				element.mesh[this_level].v[k+1][l  ] = (element.mesh[this_level-1].u[i  ][j  ]+ element.mesh[this_level-1].u[i+1][j ])/2;
			if((l+1)<=k)
				element.mesh[this_level].v[k  ][l+1] = (element.mesh[this_level-1].u[i  ][j  ]+ element.mesh[this_level-1].u[i ][j+1])/2;
			if(l+1<element.mesh[this_level].number_nodes_base && k+1<element.mesh[this_level].number_nodes_base)
				element.mesh[this_level].v[k+1][l+1] = (element.mesh[this_level-1].u[i  ][j  ]+ element.mesh[this_level-1].u[i+1][j+1])/2;
		}
	}
}

void add_error(Element element, int this_level)
{
	print_debug(3,"\t[WHERE] In function add_error\n","");
	int i,j;
#pragma omp parallel for private(i,j)
	for(i=0;i<element.mesh[this_level].number_nodes_base;i++){
		for(j=0;j<=i;j++){
			element.mesh[this_level].u[i][j]=element.mesh[this_level].u[i][j]+
																			 element.mesh[this_level].v[i][j];
		}
	}
}

void solve_exactly(Element element)
{
	int i,j;
	double A[3][3]={{element.operator[2].v[1][1], element.operator[2].v[2][1], element.operator[2].v[2][2]},
									{element.operator[2].v[0][1], element.operator[2].v[1][1], element.operator[2].v[1][2]},
									{element.operator[2].v[0][0], element.operator[2].v[1][0], element.operator[2].v[1][1]}};
	double B[3]={element.mesh[2].f[2][1],element.mesh[2].f[3][1],element.mesh[2].f[3][2]};
	if(debug>0){
		printf("Solution:\n");
		for(i=0;i<3;i++)
		{
			for(j=0;j<3;j++)
			{
				printf("%f\t",A[i][j]);
			}
			printf("\n");
		}

		for(i=0;i<3;i++)
		{
			printf("%f\n",B[i]);
		}
	}


	/* Gaussian elimmination */
	A[1][1]=A[1][1]-A[0][1]*A[1][0]/A[0][0];
	A[1][2]=A[1][2]-A[0][2]*A[1][0]/A[0][0];
	B[1]=B[1]-B[0]*A[1][0]/A[0][0];
	A[2][1]=A[2][1]-A[0][1]*A[2][0]/A[0][0];
	A[2][2]=A[2][2]-A[0][2]*A[2][0]/A[0][0];
	B[2]=B[2]-B[0]*A[2][0]/A[0][0];
	A[2][2]=A[2][2]-A[1][2]*A[2][1]/A[1][1];
	B[2]=B[2]-B[1]*A[2][1]/A[1][1];

	/* And we solve */
	element.mesh[2].u[3][2]=B[2]/A[2][2];
	element.mesh[2].u[3][1]=(B[1]-A[1][2]*element.mesh[2].u[3][2])/A[1][1];
  element.mesh[2].u[2][1]=(B[0]-A[0][2]*element.mesh[2].u[3][2]-A[0][1]*element.mesh[2].u[3][1])/A[0][0];


	if(debug>0){
		printf("Solution:\n");
		printf("%f %f %f\n",element.mesh[2].u[3][2],element.mesh[2].u[3][1],element.mesh[2].u[2][1]);
	}

}
