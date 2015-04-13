#include "targeter.h"

void loadConfig() {
	freopen("Data/positioner.conf", "r", stdin);
	cin >> cparaname >> targets;
	target = new marker[targets];
	for(int i = 0; i < targets; i++) {
		cin >> target[i].patternFile;
		cin >> target[i].center[0] >> target[i].center[1] >> target[i].width;
		for(int j = 0; j < TRANS_MAT_ROWS; j++)
			for(int k = 0; k < TRANS_MAT_COLS; k++)
				cin >> target[i].transformation[j][k];
		target[i].measurements = 0;

		for(int j = 0; j < QUAT_DIMS; j++)
			target[i].quaternion_rot[j] = 0;
		for(int j = 0; j < POS_DIMS; j++)
			target[i].position[j] = 0;
	}
	target[0].measurements = SAMPLES + 1;

	/*
	cv::FileStorage fs("out_camera_data.xml", cv::FileStorage::READ);
	if (!fs.isOpened()) {
		cout << "Could not open out_camera_data.xml. Using identity matrices for undistortion process." << endl;
		cameraMatrix = distortionCoeff = cv::Mat(identityMatrix);
	} else {
		fs["Camera_Matrix"] >> cameraMatrix;
		fs["Distortion_Coefficients"] >> distortionCoeff;
	}
	*/
}

void init() {
    ARParam  wparam;
    
    /* open the video path */
    if( arVideoOpen( vconf ) < 0 ) exit(0);
    /* find the size of the window */
    if( arVideoInqSize(&xsize, &ysize) < 0 ) exit(0);
    printf("Image size (x,y) = (%d,%d)\n", xsize, ysize);

    /* set the initial camera parameters */
    if( arParamLoad(cparaname.c_str(), 1, &wparam) < 0 ) {
       printf("Camera parameter load error !!\n");
        exit(0);
    }
    arParamChangeSize( &wparam, xsize, ysize, &cparam );
    arInitCparam( &cparam );
    printf("*** Camera Parameter ***\n");
    arParamDisp( &cparam );

    /* open the graphics window */
    argInit( &cparam, 1.0, 0, 0, 0, 0 );

    for(int i = 0; i < targets; i++)
		if( (target[i].id = arLoadPatt(target[i].patternFile.c_str())) < 0 ) {
			printf("Target pattern %d load error!!\n", i);
			exit(0);
		} else {
			target_set.insert(target[i].id);
			printf("Target %d loaded.\n", target[i].id);
		}

    arDebug = 0;
}

void cleanup()
{
	arVideoCapStop();
    arVideoClose();
    argCleanup();
}

int detectMarkers() {
	ARUint8         *dataPtr;
    /* grab a vide frame */
    if( (dataPtr = (ARUint8 *)arVideoGetImage()) == NULL ) {
        arUtilSleep(2);
        return -1;
    }

    glClearColor( 0.0, 0.0, 0.0, 0.0 );
    glClearDepth( 1.0 );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    argDrawMode2D();
	/*//============================================
	*	OpenCV Should be implemented below this line
	*/
	// Create OpenCV image with 4 channels.
    IplImage* image;
	image = cvCreateImage(cvSize(xsize, ysize), IPL_DEPTH_8U, 4);	

    // Get video frame from ARToolkit
	image->imageData = (char *)dataPtr;
	
    // Send above image frame to perform OpenCV function
	//IplImage* colorHSVImage = colorHSV->processFrame(imgTest,hue,hueMax,saturation,saturationMax,value,valueMax,rmin,rmax,gmin,gmax,bmin,bmax);

	// UNDISTORT!!!!!!!!!!!!!!!!!!!!
	/*
	IplImage *r = cvCreateImage(cvGetSize(image),8,1);
	IplImage *g = cvCreateImage(cvGetSize(image),8,1);
	IplImage *b = cvCreateImage(cvGetSize(image),8,1);
	IplImage *a = cvCreateImage(cvGetSize(image),8,1);
	cvSplit(image, r, g, b, a);
	*/

        
    // Display image on screen
    argDispImage( (unsigned char *)image->imageData, 0, 0 );

	/*
	*	OpenCV Should be implemented before this line
	*///==============================================
    /* detect the markers in the video frame */
	//argDispImage( dataPtr, 0, 0 );

    if( arDetectMarker((unsigned char *)image->imageData, thresh, &marker_info, &marker_num) < 0 ) {
        cleanup();
        exit(0);
    }

	glColor3f( 1.0, 0.0, 0.0 );
	glLineWidth(6.0);

	for(int i = 0; i < marker_num; i++ ) {
		argDrawSquare(marker_info[i].vertex,0,0);
	}

	cvReleaseImage(&image);
    arVideoCapNext();
	return marker_num;
}

void mainLoopTargeter() {
	int result = detectMarkers();
	if(result == -1) {
		arUtilSleep(2);
		return;
	} else if(result == 0) {
		argSwapBuffers();
		return;
	}

	int cnt = inferPosition();

	if(cnt) {
		updatePosition(pos[0], pos[1], pos[2]);
	}

    argSwapBuffers();
}

int inferPosition() {
	int cnt = 0;
	double tmp[3][4];
	double quaternion_rot[QUAT_DIMS], position[POS_DIMS];
	memset(quaternion_rot, 0, sizeof quaternion_rot);
	memset(position, 0, sizeof position);

	sort(marker_info, marker_info + marker_num, marker_comparison);

	seen.clear();
	for(int i = 0; i < marker_num; i++) {
		int id = marker_info[i].id;
		target[id].idx = -1;
		if(marker_info[i].cf > 0.7 && !seen.count(id) && target_set.count(id)) {
			target[id].idx = i;
			if(target[id].measurements <= SAMPLES && runMode != RUN_MODE_POSITIONER)
				continue;
			seen.insert(id);
			glColor3f( 0.0, 1.0, 0.0 );
			argDrawSquare(marker_info[i].vertex,0,0);

			getResultRaw(&marker_info[i], target[id].marker_trans, target[id].marker_trans_inv);

			arUtilMatMul(target[id].transformation, target[id].marker_trans_inv, tmp);

			if(transformAverageAdd(tmp, position, quaternion_rot))
				cnt++;
			/*else
				// TODO: research corrective method
				printf("Abnormal Quaternion:\n%4.2f\t%4.2f\t%4.2f\n%4.2f\t%4.2f\t%4.2f\t%4.2f\t\n",
				position[0], position[1], position[2],
				quaternion_rot[0], quaternion_rot[1], quaternion_rot[2], quaternion_rot[3]);
			*/

			if(runMode == RUN_MODE_POSITIONER) {
				printf("%d (%f) Says:\n", id, marker_info[i].cf),
				printMat(target[id].transformation);
				printMat(target[id].marker_trans_inv);
				printMat(tmp);
			}
		}
	}


	if(cnt) {
		
		transformAverageNormalize(tmp, position, quaternion_rot, cnt);

		bool significant = true;
		/*
		for(int i = 0; i < TRANS_MAT_ROWS; i++)
			if(fabs(tmp[i][3] - trans[i][3]) > MOVEMENT_THRESHOLD) {
				printf("%d\t%f\t%f\n", i, tmp[i][3], trans[i][3]);
				significant = true;
				break;
			}
		*/
				
		if(significant) {
			memcpy(trans, tmp, sizeof tmp);
			//transformSmooth(trans, trans, tmp, MOVEMENT_FACTOR);
			if(runMode == RUN_MODE_POSITIONER)
				printf("%d Markers Say:\n", cnt),
				printMat(trans);
		}
	}

	return cnt;
}

void getResultRaw( ARMarkerInfo *marker_info, double xyz[3][4] , double mxyz[3][4] )
{
	int id = marker_info->id;

	if( arGetTransMat(marker_info, target[id].center, target[id].width, xyz) < 0 ) return;
//	if( arGetTransMatCont(marker_info, xyz, target[id].center, target[id].width, xyz) < 0 ) return;

    if( arUtilMatInv(xyz, mxyz) < 0 ) return;

    return;
}