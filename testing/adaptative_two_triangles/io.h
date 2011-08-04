/*
 * =====================================================================================
 *
 *       Filename:  aux.h
 *
 *    Description:  Auxiliar functions and definitions
 *
 *        Version:  1.0
 *        Created:  04/08/11 15:21:00
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include "multigrid.h"

void print_debug(int debug, char * texto1, const char * texto2);
int count_lines(const char * file_name);
void scan_elements(Element *, const char *);
void scan_vertex(Vertex *, const char *);
