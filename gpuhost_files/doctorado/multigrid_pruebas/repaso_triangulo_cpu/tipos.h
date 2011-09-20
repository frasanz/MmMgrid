/*
 * =====================================================================================
 *
 *       Filename:  tipos.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/09/10 21:20:32
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef _TIPOS_H_
#define _TIPOS_H_
typedef struct mal_{
	double ** v;
	int dimension;
	int nivel;
} malla;

typedef struct tri_{
	double v[3][2];
} triangulo;
typedef struct mat_{
	double ** v;
	int dimension;
} matriz;

#endif



