#ifndef TARGETER_H
#define TARGETER_H
/* Visual Positioner */
#include "constants.h"


using namespace std;

/* camera information */
string			cparaname;// = "Data/camera_para.dat";
#ifdef _WIN32
char			*vconf = "Data\\WDM_camera_flipV.xml";
#else
char			*vconf = "";
#endif

char           *cparam_name    = "Data/camera_para.dat";
ARParam         cparam;

int             xsize;
int             ysize;
int             thresh = 100;
int             outputMode = 0;

int             mouse_ox;
int             mouse_oy;
int             mouse_st = 0;
int             disp_mode = 1;
double          a =   0.0;
double          b = -45.0;
double          r = 500.0;

double			pos[3];// = {0, 0 ,0};
double			trans[3][4];

int				targets;
marker			*target;
set<int>		target_set;
set<int>		seen;


void loadConfig();
void cleanup();
int detectMarkers();
void mainLoopTargeter();
void init();
void getResultRaw( ARMarkerInfo *marker_info, double xyz[3][4] , double mxyz[3][4] );
int inferPosition();

ARMarkerInfo    *marker_info;
int             marker_num;


bool marker_comparison(ARMarkerInfo A, ARMarkerInfo B) {
	return A.cf > B.cf;
}

// externals from positioner.h
extern	int		runMode;
extern	void	updatePosition(double **matrix);

// externals from worldgen.h
extern	void printMat(double mat[3][4]);
extern	bool transformAverageAdd(	double transformation[TRANS_MAT_ROWS][TRANS_MAT_COLS],
									double position[POS_DIMS], double  quaternion_rot[QUAT_DIMS]);
extern	void transformAverageNormalize(	double transformation[TRANS_MAT_ROWS][TRANS_MAT_COLS],
										double position[POS_DIMS], double  quaternion_rot[QUAT_DIMS],
										int count);
extern	void transformSmooth(	double destination[TRANS_MAT_ROWS][TRANS_MAT_COLS],
								double A[TRANS_MAT_ROWS][TRANS_MAT_COLS],
								double B[TRANS_MAT_ROWS][TRANS_MAT_COLS],
								double factor);

// externals from plumber.h
extern	BOOL connected;
#endif