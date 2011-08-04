/*
 * =====================================================================================
 *
 *       Filename:  multigrid.h
 *
 *    Description:  main multrigrid algorithm
 *
 *        Version:  1.0
 *        Created:  04/08/11 15:34:40
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef __MULTIGRID_H_
#define __MULTIGRID_H_

/* Functions */
void multigrid(const char *, const char * );

/* Structures */
typedef struct{
	int id;
	int node[3];
} Element;

typedef struct{
	int id;
	double c[2];
} Vertex;

#endif /* __MULTIGRID_H_ */



