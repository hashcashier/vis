#ifndef OCULAR_H
#define OCULAR_H

#include "constants.h"

ovrHmd		ocularHMD;
ovrResult	ocularResult;

void ocular_error(string err);
int ocular_init();
void ocular_exit();

#endif