/*
 * =====================================================================================
 *
 *       Filename:  smoothers.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/08/11 01:10:27
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef __SMOOTHERS_H_
#define __SMOOTHERS_H_

#include "multigrid.h"

/* Functions */
void defect(Element element, int this_level);
void smooth(Element element, int this_level);
void smooth_rgb(Element element, int this_level);
void restrict_one(Element element, int this_level);
void interpolate_one(Element element, int this_level);
void add_error(Element element, int this_level);
void solve_exactly(Element element);


#endif /* __SMOOTHERS_H_ */

