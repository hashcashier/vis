#ifndef PICASSO_H
#define PICASSO_H
/* Visual Positioner */
#include "constants.h"

using namespace std;

// externals from positioner.h
extern	ARHandle           *arHandle;
extern	AR3DHandle         *ar3DHandle;
extern	ARGViewportHandle  *vp;

// local functions
void draw(ARdouble trans[3][4]);

#endif