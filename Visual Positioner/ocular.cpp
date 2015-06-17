#include "ocular.h"

void ocular_error(string err) {
	cerr << "OCULAR ERROR: " << err << endl;
}

int ocular_init() {
	if (!ovr_Initialize(nullptr)) {
		ocular_error("ovr init failed");
		return 0;
	}
	ocularHMD = ovrHmd_Create(0);
	if (!ocularHMD) {
		ocularHMD = nullptr;
		ocular_error("could not find HMD!");
		return 0;
	}
	ovrHmd_ConfigureTracking(ocularHMD, ovrTrackingCap_Orientation|ovrTrackingCap_MagYawCorrection, 0);
	return 1;
}

void ocular_exit() {
	if (ocularHMD != nullptr)
		ovrHmd_Destroy(ocularHMD);
	ovr_Shutdown();
}

void ocular_report() {
	while (true) {
		ovrTrackingState ts = ovrHmd_GetTrackingState(ocularHMD, ovr_GetTimeInSeconds());
		if (ts.StatusFlags && ovrStatus_OrientationTracked) {
			ovrPoseStatef P = ts.HeadPose;

			ovrVector3f A = P.LinearAcceleration;// ts.RawSensorData.Accelerometer;
			//ovrVector3f V = P.LinearVelocity; // Uses oculus camera
			//ovrVector3f G = ts.RawSensorData.Gyro;
			//ovrVector3f M = ts.RawSensorData.Magnetometer;

			double magA = sqrt(A.x*A.x + A.y*A.y + A.z*A.z);
			//double magV = sqrt(V.x*V.x + V.y*V.y + V.z*V.z);
			//cout << "============" << endl;
			cout << "A: " << A.x << ' ' << A.y << ' ' << A.z << '\t' << magA << endl;
			//cout << "V: " << V.x << ' ' << V.y << ' ' << V.z << '\t' << magV << endl;
			this_thread::sleep_for(chrono::milliseconds(200));
		}
	}
}
