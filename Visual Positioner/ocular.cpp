#include "ocular.h"

void ocular_error(string err) {
	cerr << "OCULAR ERROR: " << err << endl;
}

int ocular_init() {
	if (ovr_Initialize(nullptr) != ovrSuccess) {
		ocular_error("ovr init failed");
		return 0;
	}
	ocularHMD = nullptr;
	ocularResult = ovrHmd_Create(0, &ocularHMD);
	if (ocularResult != ovrSuccess) {
		ocularHMD = nullptr;
		ocular_error("could not find HMD!");
		return 0;
	}
	return 1;
}

void ocular_exit() {
	if (ocularHMD != nullptr)
		ovrHmd_Destroy(ocularHMD);
	ovr_Shutdown();
}
