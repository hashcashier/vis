#include "plumber.h"

void openPipe() {
	pipe = CreateNamedPipe(
		"\\\\.\\pipe\\my_pipe",
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
	if(!connected)
		return;
	sent = WriteFile(pipe, msg.c_str(), msg.length() * sizeof(char), &numBytesWritten, NULL);
	if(sent)
		cout << numBytesWritten << " bytes sent out of " << msg.length() * sizeof(char) << '.' << endl;
	else
		cout << "Failed to send " << msg.length() * sizeof(char) << " bytes." << endl;
}

void closePipe() {
	CloseHandle(pipe);
	cout << "Good bye!" << endl;
}