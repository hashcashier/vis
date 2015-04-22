#ifndef SOCKETEER_H
#define SOCKETEER_H
/* Visual Positioner */
#include "constants.h"


bool		socketAvailable;
WORD		wVersionRequested = MAKEWORD(2, 2);
WSADATA		wsaData;
SOCKET		sock;
sockaddr_in	server;
sockaddr_in	client;
char		inputBuffer[BUFFER_SIZE];
char		outputBuffer[BUFFER_SIZE];


void initSocketServer();
void closeSocketServer();

// Hacking around using namespace std directive.
namespace nostd {
	int nostdbind(SOCKET s, const struct sockaddr *name, int namelen) {
		return bind(s, name, namelen);
	}
};

// externals from targeter.h
extern	double	trans[3][4];

// externals from positioner.h
extern	mutex	loc_mtx;

#endif