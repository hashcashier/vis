#ifndef POSITIONER_H
#define POSITIONER_H

/* Visual Positioner */
#include "constants.h"


using namespace std;

mutex	loc_mtx;
double	loc[3];

void	updatePosition(double x, double y, double z);

string inputResponse;

int		runMode;


// externals from targeter.h
extern	void	loadConfig();
extern	void	init();
extern	void	mainLoopTargeter();

// externals from worldgen.h
extern	void	mainLoopWorldGen();

// externals from plumber.h
extern	void	openPipe();
extern	void	closePipe();
extern	void	sendMessage(string message);
#endif