/*
 * =====================================================================================
 *
 *       Filename:  suavizadores.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/09/10 02:16:39
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

void suaviza(double **u, double **f,int iteraciones,const char*tipo,int dim);
void suaviza_gsrb(double **u, double **f,int iteraciones,int dim);

