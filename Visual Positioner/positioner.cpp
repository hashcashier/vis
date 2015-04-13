#include "positioner.h"
#include "ocvCalib3D.h"


int main(int argc, char **argv) {
	int argcOriginal = argc;
	char *execPath = argv[0], **argvOriginal = argv;
	argc -= 1;
	argv += strlen(argv[0]) + 1;

	char mode;
	if(argc) {
		mode = argv[0][0];
		argc -= 1;
		argv += strlen(argv[0]) + 1;
	} else {
		cout << "Select run-mode! (P)ositioner/(W)orld Generator/(O)penCV Calibration" << endl;
		cin >> mode;
	}

	switch(mode) {
		case 'w':
		case 'W':
			runMode = RUN_MODE_NEW_WORLD;
			break;
		case 'o':
		case 'O':
			runMode = RUN_MODE_OPENCV;
			break;
		default:
		case 'p':
		case 'P':
			runMode = RUN_MODE_POSITIONER;
			break;
	}

	if(runMode == RUN_MODE_OPENCV) {
		if(argc) {
			char *argvRecovered[] = {execPath, argv[0]};
			return ocv_main(2, argvRecovered);
		}

		cin >> inputResponse;
		char *argvFake[] = {execPath, (char*) inputResponse.c_str()};
		return ocv_main(2, argvFake);
	} else {
		// Load configuration
		loadConfig();

		// Initialize GLUT
		glutInit(&argcOriginal, argvOriginal);

		// Initialize Positoner
		init();

		// Start capturing video
		arVideoCapStart();

		if(runMode == RUN_MODE_NEW_WORLD) {
			argMainLoop( NULL, NULL, mainLoopWorldGen );
		} else {
			// Send a thread off to wait for client
			thread mario(openPipe);
			argMainLoop( NULL, NULL, mainLoopTargeter );
		}
	}

	return 0;
}

void updatePosition(double x, double y, double z) {
	lock_guard<mutex> lk(loc_mtx);

	loc[0] = x;
	loc[1] = y;
	loc[2] = z;

	ostringstream oss;
	oss << loc[0] << ' ' << loc[1] << ' ' << loc[2] << endl;

	sendMessage(oss.str());
}
