/*
 * =====================================================================================
 *
 *       Filename:  auxiliares.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/09/10 23:44:28
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include "tipos.h"
void malla_cero(malla  *tr, triangulo t);
void malla_solucion(malla  *tr, triangulo t);
void malla_efe(malla  *tr);
void malla_random(malla *tr);
void escala_matriz(matriz * m, double scale);
void suma_matrices(matriz *m, matriz *n);
void copia_matriz(matriz * ori, matriz *dest);
void suma_mallas(malla *m, malla *n);
void malla_cero2(malla *tr);


