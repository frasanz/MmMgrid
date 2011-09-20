/*
 * =====================================================================================
 *
 *       Filename:  aristas.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  24/08/11 00:59:01
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


int main()
{
	int element[9][4];
	FILE *f;
	int i,j,k,l;
	int v1,v2,v3,v4;
	int coincidencia;
	i=0;
	if(!(f=fopen("elementos.txt","r"))){
		exit(-100);
	}
	while(fscanf(f, "%d %d %d %d", &element[i][0],&element[i][1],&element[i][2],&element[i][3])!=EOF){
		i++;
	}
	for(i=0;i<9;i++)
	{
		for(j=i+1;j<9;j++)
		{

			coincidencia=0;
			for(k=1;k<=3;k++)
			{
				for(l=1;l<=3;l++)
				{
					if(element[i][k]==element[j][l]){
						coincidencia++;
						if(coincidencia==1){
							v1=k;
							v2=l;
						}
						else if(coincidencia==2){
							v3=k;
							v4=l;
						}
					}
				}
			}
			if(coincidencia==2){
				printf("%d(%d %d) with %d(%d %d) ",i,v1,v3,j,v2,v4);
				printf("Arista comun\n");
			}
			else if(coincidencia>2){
				printf("Archivo elementos.txt mal formado\n");
				exit(-100);
			}
			else{
				//printf("Sin coincidencia\n");
			}
		}
	}
}
