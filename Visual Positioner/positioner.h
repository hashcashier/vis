#ifndef POSITIONER_H
#define POSITIONER_H

/* Visual Positioner */
#include "constants.h"


using namespace std;

mutex	loc_mtx;
double	loc[3];

void	init(int argc, char *argv[]);
void	cleanup();
void	keyFunc( unsigned char key, int x, int y );

string inputResponse;

int				runMode;
int				xsize;
int				ysize;
int             thresh = 100;
int             outputMode = 0;
int             flipMode = 0;
int             mouse_ox;
int             mouse_oy;
int             mouse_st = 0;
int             disp_mode = 1;
double          a =   0.0;
double          b = -45.0;
double          r = 500.0;


ARHandle			*arHandle;
AR3DHandle			*ar3DHandle;
ARPattHandle		*arPattHandle;
ARGViewportHandle	*vp;
ARParamLT			*gCparamLT = NULL;
ARParam				cparam;

// externals from targeter.h
extern	void	loadConfig();
extern	void	init();
extern	void	mainLoopTargeter();
extern	int				targets;
extern	marker			*target;
extern	set<int>		target_set;
extern	set<int>		seen;
extern	int                 count_ar;
extern	char                fps[256];
extern	char                errValue[256];
extern	int                 distF;
extern	int                 contF;




// externals from worldgen.h
extern	void	mainLoopWorldGen();

// externals from plumber.h
extern	void	openPipe();
extern	void	closePipe();
extern	void	sendMessage(string message);
extern	void	printMatToStream(double mat[3][4], FILE *stream);

// externals from socketeer.h
extern	void	initSocketServer();
extern	void	socketServerMainLoop();


// externals from ocular.h
extern	int		ocular_init();
extern	void	ocular_exit();
extern	bool	ocularDrift;
extern	void	ocular_report();

#endif