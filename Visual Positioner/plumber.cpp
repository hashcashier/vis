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