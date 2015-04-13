#include "worldgen.h"

void mainLoopWorldGen() {
	int result = detectMarkers();
	if(result == -1) {
		arUtilSleep(2);
		return;
	} else if(result == 0) {
		argSwapBuffers();
		return;
	}


	int located_markers = inferPosition();

	if(located_markers)
		for(int i = 0; i < marker_num; i++) {
			int id = marker_info[i].id, &measurements = target[id].measurements;
			if(target_set.count(id) && measurements <= SAMPLES) {
				double tmp_mat[TRANS_MAT_ROWS][TRANS_MAT_COLS];
				double tmp_mat2[TRANS_MAT_ROWS][TRANS_MAT_COLS];
				double transformation[TRANS_MAT_ROWS][TRANS_MAT_COLS];
				double p[POS_DIMS], q[QUAT_DIMS];
				memset(p, 0, sizeof p);
				memset(q, 0, sizeof q);

				if(measurements < SAMPLES) {
					glColor3f( 1.0, 1.0, 0.0 );
					
					getResultRaw(&marker_info[i], target[id].marker_trans, target[id].marker_trans_inv);
					int cnt = 0;
					for(auto it = seen.begin(); it != seen.end(); it++) {
						int sid = *it;

						/*
						printf("%d This guy says.. %d\n", sid, id);
						printMat(target[sid].transformation);
						printMat(tmp_mat2);
						*/

						if(!parallelPlanes(target[sid].marker_trans, target[id].marker_trans))
							continue;

						arUtilMatMul(target[sid].marker_trans_inv, target[id].marker_trans, tmp_mat);
						arUtilMatMul(tmp_mat, target[sid].transformation, tmp_mat2);

						if(!lowDepthTransform(tmp_mat2))
							continue;

						cnt += transformAverageAdd(tmp_mat2, p, q);
						printf("T[A][B]:\n");
						printMat(tmp_mat);

					}
					if(!cnt)
						continue;
					transformAverageNormalize(transformation, p, q, cnt);

					transformAverageAdd(transformation, target[id].position, target[id].quaternion_rot);

					printf("Marker %d Transformation Matrix From %d Markers:\n", id, cnt);
					printMat(transformation);

				} else if(measurements == SAMPLES) {
					glColor3f( 0.0, 0.0, 1.0 );

					transformAverageNormalize(target[id].transformation, target[id].position, target[id].quaternion_rot, SAMPLES);

					printf("Marker %d Average Transformation Matrix From Origin:\n", id);
					printMat(target[id].transformation);

					measured_num++;
				}
				measurements++;
				argDrawSquare(marker_info[i].vertex,0,0);
			}
		}

	if(targets == measured_num)
		outputAndDie();

	argSwapBuffers();
}


void printMat(double mat[3][4]) {
	for(int j = 0; j < 3; j++ ) {
		for(int k = 0; k < 4; k++ )
			printf("%15.2f", mat[j][k]);
		printf("\n");
	}
	printf("\n\n");
}


bool transformAverageAdd(	double transformation[TRANS_MAT_ROWS][TRANS_MAT_COLS],
							double position[POS_DIMS], double  quaternion_rot[QUAT_DIMS]) {
	// Transformation Averaging
	double q[4], p[3];
	if(arUtilMat2QuatPos(transformation, q, p) == -1) {
		return false;
	}

	for(int j = 0; j < POS_DIMS; j++)
		position[j] += p[j];

	for(int j = 0; j < QUAT_DIMS; j++)
		quaternion_rot[j] += q[j];

	return true;
}

void transformAverageNormalize(	double transformation[TRANS_MAT_ROWS][TRANS_MAT_COLS],
								double position[POS_DIMS], double quaternion_rot[QUAT_DIMS],
								int count) {
		for(int j = 0; j < POS_DIMS; j++)
			position[j] /= count;

		normalizeQuat(quaternion_rot);

		arUtilQuatPos2Mat(quaternion_rot, position, transformation);
}

void transformSmooth(	double destination[TRANS_MAT_ROWS][TRANS_MAT_COLS],
						double A[TRANS_MAT_ROWS][TRANS_MAT_COLS],
						double B[TRANS_MAT_ROWS][TRANS_MAT_COLS],
						double factor) {
	double	pA[POS_DIMS], qA[QUAT_DIMS],
			pB[POS_DIMS], qB[QUAT_DIMS],
			pT[POS_DIMS], qT[QUAT_DIMS];

	if(arUtilMat2QuatPos(A, qA, pA) == -1 || arUtilMat2QuatPos(B, qB, pB) == -1)
		return;
	
	for(int i = 0; i < POS_DIMS; i++)
		pT[i] = factor*pB[i] + (1-factor)*pA[i];
	for(int i = 0; i < QUAT_DIMS; i++)
		qT[i] = factor*qB[i] + (1-factor)*qA[i];

	normalizeQuat(qT);

	arUtilQuatPos2Mat(qT, pT, destination);
}

void normalizeQuat(double quaternion_rot[QUAT_DIMS]) {
	double norm = 0;
	for(int j = 0; j < QUAT_DIMS; j++)
		norm += quaternion_rot[j]*quaternion_rot[j];
	norm = sqrt(norm);

	for(int j = 0; j < QUAT_DIMS; j++)
		quaternion_rot[j] /= norm;
}


void outputAndDie() {
	ostringstream oss;
	oss << "positioner." << time(nullptr) << ".conf";
	freopen(oss.str().c_str(), "w", stdout);
	cout << cparaname << endl << targets << endl;
	for(int i = 0; i < targets; i++)
		cout << target[i].patternFile << endl << target[i].center[0] << '\t' << target[i].center[1] << '\t' << target[i].width << endl,
		printMat(target[i].transformation);


	exit(0); //die
}

cv::Point3d markerNormal(double M[TRANS_MAT_ROWS][TRANS_MAT_COLS]) {
	double o[3][4], a[3][4], b[3][4];
	arUtilMatMul(M, POINT_O, o);
	arUtilMatMul(M, POINT_A, a);
	arUtilMatMul(M, POINT_B, b);

	double	adx = a[0][3] - o[0][3],
			ady = a[1][3] - o[1][3],
			adz = a[2][3] - o[2][3],
			amg = sqrt(adx*adx + ady*ady + adz*adz);
	double	bdx = b[0][3] - o[0][3],
			bdy = b[1][3] - o[1][3],
			bdz = b[2][3] - o[2][3],
			bmg = sqrt(bdx*bdx + bdy*bdy + bdz*bdz);

	cv::Point3d OA( adx/amg, ady/amg, adz/amg );
	cv::Point3d OB( bdx/bmg, bdy/bmg, bdz/bmg );

	return OA.cross(OB);
}


bool parallelPlanes(double A[TRANS_MAT_ROWS][TRANS_MAT_COLS], double B[TRANS_MAT_ROWS][TRANS_MAT_COLS]) {
	cv::Point3d nA = markerNormal(A), nB = markerNormal(B);
	double dot = nA.dot(nB);
	if(dot < FLAT_MAT_THRESHOLD_DOT)
		return false;
	return true;
}

bool lowDepthTransform(double T[TRANS_MAT_ROWS][TRANS_MAT_COLS]) {
	double dz = fabs(T[2][3]);
	if(dz > FLAT_MAT_THRESHOLD_DZ)
		return false;
	return true;
}