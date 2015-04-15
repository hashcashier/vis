#include "positioner.h"


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
		cout << "Select run-mode! (P)ositioner/(W)orld Generator/(C)alibration" << endl;
		cin >> mode;
	}

	switch(mode) {
		case 'w':
		case 'W':
			runMode = RUN_MODE_NEW_WORLD;
			break;
		case 'c':
		case 'C':
			runMode = RUN_MODE_CALIBRATE;
			break;
		default:
		case 'p':
		case 'P':
			runMode = RUN_MODE_POSITIONER;
			break;
	}

	if(runMode == RUN_MODE_CALIBRATE) {
		/*
		if(argc) {
			char *argvRecovered[] = {execPath, argv[0]};
			return ocv_main(2, argvRecovered);
		}

		cin >> inputResponse;
		char *argvFake[] = {execPath, (char*) inputResponse.c_str()};
		return ocv_main(2, argvFake);
		*/
	} else {
		// Load configuration
		loadConfig();

		// Initialize GLUT
		glutInit(&argcOriginal, argvOriginal);

		// Initialize Positoner
		init(argc, argv);

		// Start capturing video
		arVideoCapStart();

		if(runMode == RUN_MODE_NEW_WORLD) {
			argSetDispFunc( mainLoopWorldGen, 1 );
		} else {
			// Send a thread off to wait for client
			thread mario(openPipe);
			argSetDispFunc( mainLoopTargeter, 1 );
		}

		argSetKeyFunc( keyFunc );
		count_ar = 0;
		fps[0] = '\0';
		arUtilTimerReset();
		argMainLoop();
	}

	return 0;
}

void keyFunc( unsigned char key, int x, int y )
{
    int   value;

    switch (key) {
		case 0x1b:
			cleanup();
			exit(0);
			break;
		case '1':
		case '-':
        	arGetLabelingThresh( arHandle, &value );
        	value -= 5;
        	if( value < 0 ) value = 0;
        	arSetLabelingThresh( arHandle, value );
        	ARLOG("thresh = %d\n", value);
        	break;
		case '2':
		case '+':
        	arGetLabelingThresh( arHandle, &value );
       		value += 5;
        	if( value > 255 ) value = 255;
        	arSetLabelingThresh( arHandle, value );
        	ARLOG("thresh = %d\n", value);
        	break;
		case 'd':
		case 'D':
        	arGetDebugMode( arHandle, &value );
       		value = 1 - value;
        	arSetDebugMode( arHandle, value );
            break;
        case 'h':
        case 'H':
            if( flipMode & AR_GL_FLIP_H ) flipMode = flipMode & AR_GL_FLIP_V;
            else                         flipMode = flipMode | AR_GL_FLIP_H;
            argViewportSetFlipMode( vp, flipMode );
            break;
        case 'v':
        case 'V':
            if( flipMode & AR_GL_FLIP_V ) flipMode = flipMode & AR_GL_FLIP_H;
            else                         flipMode = flipMode | AR_GL_FLIP_V;
            argViewportSetFlipMode( vp, flipMode );
        	break;
        case ' ':
            distF = 1 - distF;
            if( distF ) {
                argViewportSetDistortionMode( vp, AR_GL_DISTORTION_COMPENSATE_ENABLE );
            } else {
                argViewportSetDistortionMode( vp, AR_GL_DISTORTION_COMPENSATE_DISABLE );
            }
            break;
        case 'c':
            contF = 1 - contF;
            break;
		case '?':
		case '/':
			ARLOG("Keys:\n");
			ARLOG(" [esc]         Quit demo.\n");
			ARLOG(" - and +       Adjust threshhold.\n");
			ARLOG(" d             Activate / deactivate debug mode.\n");
			ARLOG(" h and v       Toggle horizontal / vertical flip mode.\n");
            ARLOG(" [space]       Toggle distortion compensation.\n");
			ARLOG(" ? or /        Show this help.\n");
			ARLOG("\nAdditionally, the ARVideo library supplied the following help text:\n");
			arVideoDispOption();
			break;
		default:
			break;
	}
}

void init(int argc, char *argv[]) {
    ARGViewport     viewport;
    char            vconf[512];
    AR_PIXEL_FORMAT pixFormat;
    ARUint32        id0, id1;
    int             i;

	if(!argc)
		vconf[0] = '\0';
    else {
        strcpy( vconf, argv[1] );
        for( i = 2; i < argc; i++ ) {
			strcat(vconf, " ");
			strcat(vconf,argv[i]);
		}
    }

    
    /* open the video path */
	ARLOG("Using video configuration '%s'.\n", vconf);
    if( arVideoOpen( vconf ) < 0 ) exit(0);
    /* find the size of the window */
    if( arVideoGetSize(&xsize, &ysize) < 0 ) exit(0);
    ARLOG("Image size (x,y) = (%d,%d)\n", xsize, ysize);
    if( (pixFormat=arVideoGetPixelFormat()) < 0 ) exit(0);
    if( arVideoGetId( &id0, &id1 ) == 0 ) {
        ARLOG("Camera ID = (%08x, %08x)\n", id1, id0);
        sprintf(vconf, VPARA_NAME, id1, id0);
        if( arVideoLoadParam(vconf) < 0 ) {
            ARLOGe("No camera setting data!!\n");
        }
    }
    /* set the initial camera parameters */
    if( arParamLoad(CPARA_NAME, 1, &cparam) < 0 ) {
        ARLOGe("Camera parameter load error !!\n");
        exit(0);
    }
    arParamChangeSize( &cparam, xsize, ysize, &cparam );
    ARLOG("*** Camera Parameter ***\n");
    arParamDisp( &cparam );
    if ((gCparamLT = arParamLTCreate(&cparam, AR_PARAM_LT_DEFAULT_OFFSET)) == NULL) {
        ARLOGe("Error: arParamLTCreate.\n");
        exit(-1);
    }
    
    if( (arHandle=arCreateHandle(gCparamLT)) == NULL ) {
        ARLOGe("Error: arCreateHandle.\n");
        exit(0);
    }
    if( arSetPixelFormat(arHandle, pixFormat) < 0 ) {
        ARLOGe("Error: arSetPixelFormat.\n");
        exit(0);
    }

    if( (ar3DHandle=ar3DCreateHandle(&cparam)) == NULL ) {
        ARLOGe("Error: ar3DCreateHandle.\n");
        exit(0);
    }

    /* open the graphics window */
/*
    int winSizeX, winSizeY;
    argCreateFullWindow();
    argGetScreenSize( &winSizeX, &winSizeY );
    viewport.sx = 0;
    viewport.sy = 0;
    viewport.xsize = winSizeX;
    viewport.ysize = winSizeY;
*/
    viewport.sx = 0;
    viewport.sy = 0;
    viewport.xsize = xsize;
    viewport.ysize = ysize;
    if( (vp=argCreateViewport(&viewport)) == NULL ) exit(0);
    argViewportSetCparam( vp, &cparam );
    argViewportSetPixFormat( vp, pixFormat );
    //argViewportSetDispMethod( vp, AR_GL_DISP_METHOD_GL_DRAW_PIXELS );
    argViewportSetDistortionMode( vp, AR_GL_DISTORTION_COMPENSATE_DISABLE );

	if (arVideoCapStart() != 0) {
        ARLOGe("video capture start error !!\n");
        exit(0);
	}

    for(int i = 0; i < targets; i++)
		if( (target[i].arPattHandle = arPattCreateHandle()) == NULL ) {
			ARLOGe("Error: arPattCreateHandle.\n");
			exit(0);
		}
		if( (target[i].id=arPattLoad(target[i].arPattHandle, target[i].patternFile.c_str())) < 0 ) {
			ARLOGe("pattern load error !!\n");
			exit(0);
		}
		arPattAttach( arHandle, target[i].arPattHandle );

    //arDebug = 0;
}

void cleanup()
{
    arVideoCapStop();
    argCleanup();
	arPattDetach(arHandle);
	for(int i = 0; i < targets; i++)
		arPattDeleteHandle(target[i].arPattHandle);
	ar3DDeleteHandle(&ar3DHandle);
	arDeleteHandle(arHandle);
    arParamLTFree(&gCparamLT);
    arVideoClose();
}

void updatePosition(double** matrix) {
	lock_guard<mutex> lk(loc_mtx);

	ostringstream oss;
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 4; j++) {
			if(j) oss << '\t';
			oss << matrix[i][j];
		}
		delete matrix[i];
		oss << endl;
	}
	delete matrix;

	sendMessage(oss.str());
}
