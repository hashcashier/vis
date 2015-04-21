#include "plumber.h"

void openPipe() {
	pipe = CreateNamedPipe(
		PLUMBER_PIPE_NAME,
		PIPE_ACCESS_OUTBOUND,
		PIPE_TYPE_MESSAGE,
		1,
		1024,
		0,
		0,
		NULL
	);

	if(pipe == NULL || pipe == INVALID_HANDLE_VALUE) {
		cout << "Failed to open pipe." << endl;
		return;
	}

	cout << "Waiting for client.." << endl;

	connected = ConnectNamedPipe(pipe, NULL);
	if(!connected) {
		cout << "Could not establish connection" << endl;
		CloseHandle(pipe);
		return;
	} else {
		cout << "Client connected!" << endl;
	}
}


void sendMessage(string msg) {
	if (!connected) return;
	lock_guard<mutex> lk(pipe_mtx);
	sent = WriteFile(pipe, msg.c_str(), msg.length() * sizeof(char), &numBytesWritten, NULL);
	if(sent)
		cout << numBytesWritten << " bytes sent out of " << msg.length() * sizeof(char) << '.' << endl;
	else
		cout << "Failed to send " << msg.length() * sizeof(char) << " bytes." << endl;
}

void closePipe() {
	if (!connected) return;
	lock_guard<mutex> lk(pipe_mtx);
	CloseHandle(pipe);
	cout << "Good bye!" << endl;
}

void printMatToStream(double mat[3][4], FILE *stream) {
	if (!saneMatrix(mat))
		fprintf(stderr, "INSANE MATRIX OUTPUT!\n");

	for (int j = 0; j < 3; j++) {
		for (int k = 0; k < 4; k++)
			fprintf(stream, "%7.2f\t", mat[j][k]);
		fprintf(stream, "\n");
	}
	fprintf(stream, "\n\n");
}