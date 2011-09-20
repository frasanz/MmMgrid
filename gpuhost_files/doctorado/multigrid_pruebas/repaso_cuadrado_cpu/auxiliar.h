/*
 * =====================================================================================
 *
 *       Filename:  auxiliar.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/09/10 01:38:05
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

//////////////////////////////////////////
// Inicializa f, de dimension dimension //
// x dimension													//
void inicializa(double **f,int dimension);
//////////////////////////////////////////
// inicializa_cero, matriz_cuadrada=0   //
void inicializa_cero(double **matriz, int dimension);
//////////////////////////////////////////
// calcula el defecto                   //
void calcula_defecto(double **d, double **f, double **u, int dimension);
void restringe(double **inicial, double **final, int dim_final);
void interpola(double **inicial, double **final, int dim_inicial,int dim_final);
void exacta(double **d, double **v);
void suma_matrices(double ** m1, double **m2, int dim);
void copia_matriz(double **m1, double **m2, int dim);

