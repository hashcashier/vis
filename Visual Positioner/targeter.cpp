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
}

int detectMarkers() {
    static int      contF2 = 0;
	ARUint8         *dataPtr;
    int             imageProcMode;
    int             debugMode;
    //ARdouble        err;

    /* grab a vide frame */
    if( (dataPtr = (ARUint8 *)arVideoGetImage()) == NULL ) {
        arUtilSleep(2);
        return -1;
    }

	argDrawMode2D(vp);
    arGetDebugMode( arHandle, &debugMode );
    if( debugMode == 0 ) {
        argDrawImage( dataPtr );
    }
    else {
        arGetImageProcMode(arHandle, &imageProcMode);
        if( imageProcMode == AR_IMAGE_PROC_FRAME_IMAGE ) {
            argDrawImage( arHandle->labelInfo.bwImage );
        }
        else {
            argDrawImageHalf( arHandle->labelInfo.bwImage );
        }
    }

    /* detect the markers in the video frame */
	//argDispImage( dataPtr, 0, 0 );

    if( arDetectMarker(arHandle, dataPtr) < 0 ) {
        cleanup();
        exit(0);
    }

    if( count_ar % 60 == 0 ) {
        sprintf(fps, "%f[fps]", 60.0/arUtilTimer());
        arUtilTimerReset();
    }
    count_ar++;
    glColor3f(0.0f, 1.0f, 0.0f);
    argDrawStringsByIdealPos(fps, 10, ysize-30);

    marker_num = arGetMarkerNum( arHandle );
    if( !marker_num ) {
        return 0;
    }

    /* check for object visibility */
    marker_info =  arGetMarker( arHandle ); 
	/*
    int k = -1;
    for(int j = 0; j < marker_num; j++ ) {
        ARLOG("ID=%d, CF = %f\n", marker_info[j].id, marker_info[j].cf);
        if( patt_id == markerInfo[j].id ) {
            if( k == -1 ) {
                if (markerInfo[j].cf >= 0.7) k = j;
            } else if( markerInfo[j].cf > markerInfo[k].cf ) k = j;
        }
    }
    if( k == -1 ) {
        contF2 = 0;
        argSwapBuffers();
        return;
    }

    if( contF && contF2 ) {
        err = arGetTransMatSquareCont(ar3DHandle, &(markerInfo[k]), patt_trans, patt_width, patt_trans);
    }
    else {
        err = arGetTransMatSquare(ar3DHandle, &(markerInfo[k]), patt_width, patt_trans);
    }
    sprintf(errValue, "err = %f", err);
    glColor3f(0.0f, 1.0f, 0.0f);
    argDrawStringsByIdealPos(fps, 10, ysize-30);
    argDrawStringsByIdealPos(errValue, 10, ysize-60);
    //ARLOG("err = %f\n", err);

    contF2 = 1;
    draw(patt_trans);
	*/

    //argSwapBuffers();
	//-----
	glColor3f( 1.0, 0.0, 0.0 );
	glLineWidth(6.0);

	for(int i = 0; i < marker_num; i++ ) {
		//argDrawSquare(marker_info[i].vertex,0,0);
	}

//    arVideoCapNext();
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

	if(cnt && connected) {
		double **tmp = new double*[3];
		for(int i = 0; i < 3; i++)
			tmp[i] = new double[4];
		memcpy(tmp, trans, sizeof trans);
		thread luigi(updatePosition, tmp);
		luigi.detach();
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
			//argDrawSquare(marker_info[i].vertex,0,0);

			getResultRaw(&marker_info[i], target[id].marker_trans, target[id].marker_trans_inv);

			arUtilMatMul(target[id].transformation, target[id].marker_trans_inv, tmp);

			if(transformAverageAdd(tmp, position, quaternion_rot))
				cnt++;
			/*else
				// TODO: research corrective method (smoothing)
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

	arGetTransMatSquare(ar3DHandle, marker_info, target[id].width, xyz);

//	if( arGetTransMat(marker_info, target[id].center, target[id].width, xyz) < 0 ) return;
//	if( arGetTransMatCont(marker_info, xyz, target[id].center, target[id].width, xyz) < 0 ) return;

    if( arUtilMatInv(xyz, mxyz) < 0 ) return;

    return;
}