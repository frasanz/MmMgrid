/*
 * =====================================================================================
 *
 *       Filename:  ghost.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  28/08/11 00:20:37
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
#include <math.h>
#include "ghost.h"
#include "multigrid.h"
extern int debug;

void u_to_ghost(Element * element, int level,int number_elements)
{
	int i,j;
	int neighbor;
	int sent;
	int nnb;
	for(i=0;i<number_elements;i++){ /* For each element */
		nnb=element[i].mesh[level].number_nodes_base;
		if((neighbor=element[i].nei[0][0])>=0){ /* Edge 0-1 has neighbor */
			sent=element[i].nei[0][2]*element[neighbor].nei[element[i].nei[0][1]][2]; /* TODO: check that this line is ok */
			if(element[i].nei[0][1]==0){         /* the edge of the neigbor is 0-1 */
				if(sent==1){                       /* Direct copy from 0-1 to ghost 0-1 */
					for(j=0;j<=nnb-2;j++){
						element[i].mesh[level].u_gh[0][j]=element[neighbor].mesh[level].u[nnb-2][j];
					}
				}else{                             /* Inverse copy from 0-1 to ghost 0-1*/
					for(j=0;j<=nnb-2;j++){
						element[i].mesh[level].u_gh[0][j]=element[neighbor].mesh[level].u[nnb-2][nnb-2-j];
					}
				}

			}else if(element[i].nei[0][1]==1){   /* the edge of the neigbor is 1-2 */
				sent=element[i].nei[0][2]*element[neighbor].nei[element[i].nei[0][1]][2]; /* TODO: check that this line is ok */
				if(sent==1){                       /* Direct copy from 1-2 to 0-1  */
					for(j=0;j<=nnb-2;j++)
						element[i].mesh[level].u_gh[0][j]=element[neighbor].mesh[level].u[nnb-1-j][nnb-2-j];
				}else{                             /* Inverse copy from  1-2 to 0-1 */
					for(j=0;j<=nnb-2;j++){
						element[i].mesh[level].u_gh[0][j]=element[neighbor].mesh[level].u[(j+1)][j];
					}
				}

			}else if(element[i].nei[0][1]==2){   /* the edge of the neigbor is 2-0 */
				sent=element[i].nei[0][2]*element[neighbor].nei[element[i].nei[0][1]][2]; /* TODO: check that this line is ok */
				if(sent==1){                       /* Direct copy from 2-0 to 0-1  */
					for(j=0;j<=nnb-2;j++){
						element[i].mesh[level].u_gh[0][j]=element[neighbor].mesh[level].u[j+1][1];
					}
				}else{
					for(j=0;j<=nnb-2;j++)/* Inverse copy from 2-0 to 0-1 */
						element[i].mesh[level].u_gh[0][j]=element[neighbor].mesh[level].u[nnb-1-j][1];
				}
			}
		}
		if((neighbor=element[i].nei[1][0])>0){ /* Edge 1-2 has neighbor */

		}
		if((neighbor=element[i].nei[2][0])>0){ /* Edge 2-0 has neighbor */

		}
	}
}

void print_ghost(Element * element, int number_elements, int level)
{
	int i,j,k;
	printf("Ghost nodes\n");
	for(i=0;i<number_elements;i++){
		printf("Element %d\n",i);
		for(j=0;j<3;j++){
			for(k=0;k<element[i].mesh[level].number_nodes_base-1;k++)
				printf("%f ",element[i].mesh[level].u_gh[j][k]);
		printf("--\n");
		}
	}
}
