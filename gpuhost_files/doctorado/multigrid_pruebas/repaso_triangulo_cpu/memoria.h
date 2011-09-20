/*
 * =====================================================================================
 *
 *       Filename:  memoria.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/09/10 21:19:19
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#include "tipos.h" 

#ifndef _MEMORIA_H_
#define _MEMORIA_H_
void inicializa_malla(malla  * tr, int nivel);
void inicializa_malla_c(malla  * tr, int nivel);
void libera_malla(malla * tr);
void inicializa_matriz(matriz * m, int dim);
void inicializa_matriz_c(matriz * m, int dim);
void libera_matriz(matriz *m);
#endif
