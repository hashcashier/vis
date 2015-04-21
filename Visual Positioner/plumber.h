#ifndef PLUMBER_H
#define PLUMBER_H
/* Visual Positioner */
#include "constants.h"


using namespace std;

mutex	pipe_mtx;
HANDLE	pipe;
BOOL	connected;
BOOL	sent;
DWORD	numBytesWritten;

void openPipe();
void closePipe();
void sendMessage(string message);
void printMatToStream(double mat[3][4], FILE *stream);

// externals from targeter
extern	bool saneMatrix(double mat[3][4]);
#endif