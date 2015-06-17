#ifndef OCULAR_H
#define OCULAR_H

#include "constants.h"


bool		ocularDrift = false;

ovrHmd		ocularHMD = nullptr;
ovrBool		ocularResult;

void	ocular_error(string err);
int		ocular_init();
void	ocular_report();
void	ocular_exit();

#endif