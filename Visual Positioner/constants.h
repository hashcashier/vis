#ifndef CONSTANTS_H
#define CONSTANTS_H
/* platform header */
#ifdef _WIN32
#  include <windows.h>
#endif
/* STL */
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <math.h>
#include <set>
/* Multithreading */
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
/* openGL */
#ifndef __APPLE__
#  include <GL/gl.h>
#  include <GL/glut.h>
#else
#  include <OpenGL/gl.h>
#  include <GLUT/glut.h>
#endif
/* ARToolkit */
#include <AR/ar.h>
#include <AR/arFilterTransMat.h>
#include <AR/gsub.h>
#include <AR/video.h>
//#include <AR/param.h>
/* OpenCV */
#include <opencv/cv.h>

using namespace std;

#define SAMPLES 10

#define RUN_MODE_NEW_WORLD		1
#define RUN_MODE_POSITIONER		2
#define	RUN_MODE_CALIBRATE		3

#define TRANS_MAT_ROWS	3
#define	TRANS_MAT_COLS	4

#define QUAT_DIMS	4
#define	POS_DIMS	3

#define MOVEMENT_THRESHOLD	100
#define MOVEMENT_FACTOR		0.05

#define FLAT_MAT_THRESHOLD_DOT	0.99
#define FLAT_MAT_THRESHOLD_DZ	5

#define CLOSE_MAT_THRESHOLD	100

#define PLUMBER_PIPE_NAME	"\\\\.\\pipe\\markergps"

#define	CPARA_NAME	"Data/camera_para.dat"
#define VPARA_NAME  "Data/cameraSetting-%08x%08x.dat"


struct marker {
	bool					valid, validPrev;
	int						id, idx;
	int						measurements;
	double					transformation[TRANS_MAT_ROWS][TRANS_MAT_COLS];
	double					inferred_position[TRANS_MAT_ROWS][TRANS_MAT_COLS];
	double					quaternion_rot[QUAT_DIMS];
	double					position[POS_DIMS];
	double					center[2];
	ARdouble				width;
	ARdouble				height;
	ARdouble				error;
	string					patternFile;
	ARMarkerInfo			*marker_info;
	ARFilterTransMatInfo	*filter;
	double					marker_trans[TRANS_MAT_ROWS][TRANS_MAT_COLS];
	double					marker_trans_inv[TRANS_MAT_ROWS][TRANS_MAT_COLS];
};


/*
cv::Mat	cameraMatrix, distortionCoeff;
cv::Mat identityMatrix = cv::Mat::eye(3, 4, CV_64F);
*/

#endif