/*
 * =====================================================================================
 *
 *       Filename:  suavizador.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/09/10 02:20:14
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include "tipos.h"

void suaviza(malla *u, malla *f, matriz *op);
void suaviza_r(malla *u, malla *f, matriz *op);
void suaviza_g(malla *u, malla *f, matriz *op);
void suaviza_b(malla *u, malla *f, matriz *op);
void defecto(malla *u, malla *f, malla *d, matriz *op);
void restringe(malla *sup, malla *inf);
void interpola(malla *inf, malla *sup);

