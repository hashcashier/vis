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
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif
/* ARToolkit */
#include <AR/gsub.h>
#include <AR/param.h>
#include <AR/ar.h>
#include <AR/video.h>
/* OpenCV */
#include <opencv2\opencv.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>

using namespace std;

#define SAMPLES 5

#define RUN_MODE_NEW_WORLD		1
#define RUN_MODE_POSITIONER		2
#define	RUN_MODE_OPENCV			3

#define TRANS_MAT_ROWS	3
#define	TRANS_MAT_COLS	4

#define QUAT_DIMS	4
#define	POS_DIMS	3

#define MOVEMENT_THRESHOLD	100
#define MOVEMENT_FACTOR		0.05

#define FLAT_MAT_THRESHOLD_DOT	0.99
#define FLAT_MAT_THRESHOLD_DZ	5

#define PLUMBER_PIPE_NAME	"\\\\.\\pipe\\markergps"

struct marker {
	int				id, idx;
	int				measurements;
	double			transformation[TRANS_MAT_ROWS][TRANS_MAT_COLS];
	double			quaternion_rot[QUAT_DIMS];
	double			position[POS_DIMS];
	double			center[2];
	double			width;
	string			patternFile;
	ARMarkerInfo    *marker_info;
	double			marker_trans[TRANS_MAT_ROWS][TRANS_MAT_COLS];
	double			marker_trans_inv[TRANS_MAT_ROWS][TRANS_MAT_COLS];
};


/*
cv::Mat	cameraMatrix, distortionCoeff;
cv::Mat identityMatrix = cv::Mat::eye(3, 4, CV_64F);
*/

#endif