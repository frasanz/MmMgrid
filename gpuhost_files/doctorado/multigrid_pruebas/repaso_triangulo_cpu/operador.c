/*
 * =====================================================================================
 *
 *       Filename:  operador.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/09/10 00:23:28
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include "auxiliares.h"
#include "memoria.h"
#include "tipos.h"
#include "pantalla.h"
void crea_operador(triangulo * tr, matriz * op)
{
	double scale;
	matriz C;
	matriz InvC;
	matriz Cb;
	matriz Sxx;
	matriz Sxy;
	matriz Syy;
	inicializa_matriz_c(&C,2);
	inicializa_matriz_c(&Cb,2);
	inicializa_matriz_c(&InvC,2);
	inicializa_matriz_c(&Sxx,3);
	inicializa_matriz_c(&Sxy,3);
	inicializa_matriz_c(&Syy,3);

	C.v[0][0]=tr->v[1][0]-tr->v[0][0];
	C.v[0][1]=tr->v[2][0]-tr->v[1][0];
	C.v[1][0]=tr->v[1][1]-tr->v[0][1];
	C.v[1][1]=tr->v[2][1]-tr->v[1][1];
	scale=1.0/(C.v[0][0]*C.v[1][1]-C.v[0][1]*C.v[1][0]);
	printf("Matriz afin:\n");
	muestra_matriz(&C);
	printf("\n");

	InvC.v[0][0]= C.v[1][1];
	InvC.v[0][1]=-C.v[0][1];
	InvC.v[1][0]=-C.v[1][0];
	InvC.v[1][1]=C.v[0][0];
	escala_matriz(&InvC,scale);
	printf("Matriz Inversa:\n");
	muestra_matriz(&InvC);
	printf("\n");



	Cb.v[0][0]=InvC.v[0][0]*InvC.v[0][0]+InvC.v[0][1]*InvC.v[0][1];
	Cb.v[0][1]=InvC.v[0][0]*InvC.v[1][0]+InvC.v[0][1]*InvC.v[1][1];
	Cb.v[1][0]=InvC.v[1][0]*InvC.v[0][0]+InvC.v[0][1]*InvC.v[1][1];
	Cb.v[1][1]=InvC.v[1][0]*InvC.v[1][0]+InvC.v[1][1]*InvC.v[1][1];
	muestra_matriz(&Cb);
	
	Sxx.v[1][0]=-1.0;
	Sxx.v[1][1]=2.0;
	Sxx.v[1][2]=-1.0;
	escala_matriz(&Sxx,Cb.v[0][0]);
	muestra_matriz(&Sxx);

	Sxy.v[0][0]=-1.0;
	Sxy.v[0][1]= 1.0;
	Sxy.v[1][0]= 1.0;
	Sxy.v[1][1]=-2.0;
	Sxy.v[1][2]= 1.0;
	Sxy.v[2][1]= 1.0;
	Sxy.v[2][2]=-1.0;
	escala_matriz(&Sxy,0.5*(Cb.v[1][0]+Cb.v[0][1]));
	muestra_matriz(&Sxy);

	Syy.v[0][1]=-1.0;
	Syy.v[1][1]= 2.0;
	Syy.v[2][1]=-1.0;
	escala_matriz(&Syy,Cb.v[1][1]);
	muestra_matriz(&Syy);

	suma_matrices(op,&Sxx);
	suma_matrices(op,&Sxy);
	suma_matrices(op,&Syy);

	libera_matriz(&C);
	libera_matriz(&Cb);
	libera_matriz(&Sxx);
	libera_matriz(&Sxy);
	libera_matriz(&Syy);

}

