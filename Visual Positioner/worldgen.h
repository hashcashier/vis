#ifndef TARGETER_H
#define TARGETER_H
/* Visual Positioner */
#include "constants.h"

using namespace std;

//externals from targeter.h
extern	int				targets;
extern	set<int>		target_set;
extern	marker			*target;
extern	set<int>		seen;


extern	ARMarkerInfo    *marker_info;
extern	int             marker_num;
extern	double			pos[3];
extern	double			trans[3][4];

extern	int				inferPosition();
extern	void			getResultRaw( ARMarkerInfo *marker_info, double xyz[3][4] , double mxyz[3][4] );

extern	string			cparaname;

//externals from positioner.h
extern int detectMarkers();

// Local variables
int	measured_num = 1;
int	measured_recognized_num = 1;

double POINT_O[3][4] = {	{1,0,0, 0},
							{0,1,0, 0},
							{0,0,1, 0}	};

double POINT_A[3][4] = {	{1,0,0, 0},
							{0,1,0, 1},
							{0,0,1, 0}	};

double POINT_B[3][4] = {	{1,0,0, 1},
							{0,1,0, 0},
							{0,0,1, 0}	};
// externals from plumber.h
extern	void printMatToStream(double mat[3][4], FILE *stream);

// externals from picasso.h
extern	void draw(ARdouble trans[3][4]);

// Local functions
void mainLoopWorldGen();
void printMat(double mat[3][4]);
bool transformAverageAdd(	double transformation[TRANS_MAT_ROWS][TRANS_MAT_COLS],
							double position[POS_DIMS], double  quaternion_rot[QUAT_DIMS]);
void transformAverageNormalize(	double transformation[TRANS_MAT_ROWS][TRANS_MAT_COLS],
								double position[POS_DIMS], double  quaternion_rot[QUAT_DIMS],
								int count);
void transformSmooth(	double destination[TRANS_MAT_ROWS][TRANS_MAT_COLS],
						double A[TRANS_MAT_ROWS][TRANS_MAT_COLS],
						double B[TRANS_MAT_ROWS][TRANS_MAT_COLS],
						double factor);

void normalizeQuat(double quaternion_rot[QUAT_DIMS]);
void outputAndDie();

cv::Point3d markerNormal(double M[TRANS_MAT_ROWS][TRANS_MAT_COLS]);
bool parallelPlanes(double A[TRANS_MAT_ROWS][TRANS_MAT_COLS], double B[TRANS_MAT_ROWS][TRANS_MAT_COLS]);
bool lowDepthTransform(double T[TRANS_MAT_ROWS][TRANS_MAT_COLS]);

#endif