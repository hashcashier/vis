#ifndef TARGETER_H
#define TARGETER_H
/* Visual Positioner */
#include "constants.h"


using namespace std;

/* camera information */
string			cparaname;// = "Data/camera_para.dat";
string			vparaname;
#ifdef _WIN32
char			*vconf = "Data\\WDM_camera_flipV.xml";
#else
char			*vconf = "";
#endif

double			pos[3];// = {0, 0 ,0};
double			trans[3][4];

int				targets, recognized_targets;
marker			*target;
set<int>		target_set;

int                 count_ar = 0;
char                fps[256];
char                errValue[256];
int                 distF = 0;
int                 contF = 0;



void loadConfig();
int detectMarkers();
void mainLoopTargeter();
void getResultRaw( ARMarkerInfo *marker_info, double xyz[3][4] , double mxyz[3][4] );
int inferPosition();
bool agreeWithMajority(int id);
bool saneMatrix(double mat[3][4]);

ARMarkerInfo    *marker_info;
int             marker_num;


bool marker_comparison(ARMarkerInfo A, ARMarkerInfo B) {
	return A.cf > B.cf;
}

// externals from positioner.h
extern	void	cleanup();
extern	int		runMode;
extern	void	updatePosition(double **matrix);
extern	void	updatePositionS(double matrix[3][4]);
extern	int     xsize;
extern	int		ysize;
extern	ARHandle           *arHandle;
extern	AR3DHandle         *ar3DHandle;
extern	ARGViewportHandle  *vp;


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

// externals from picasso.h
extern	void draw(ARdouble trans[3][4]);

// externals from positioner.h
extern	mutex	loc_mtx;

#endif