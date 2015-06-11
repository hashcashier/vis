#include "socketeer.h"

void initSocketServer() {
	int err = WSAStartup(wVersionRequested, &wsaData);
	if (err) {
		fprintf(stderr, "WSA Startup Failure: %d\n", err);
		return;
	} else if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		fprintf(stderr, "No	usable version of Winsock.dll\n");
		WSACleanup();
		return;
	} else if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
		fprintf(stderr, "Could Not Create Socket\n");
		WSACleanup();
		return;
	}
	memset((void*)&server, '\0', sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(1337);

	server.sin_addr.S_un.S_un_b.s_b1 = (unsigned char)127;
	server.sin_addr.S_un.S_un_b.s_b2 = (unsigned char)0;
	server.sin_addr.S_un.S_un_b.s_b3 = (unsigned char)0;
	server.sin_addr.S_un.S_un_b.s_b4 = (unsigned char)1;

	if (nostd::nostdbind(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_in)) == -1) {
		fprintf(stderr, "Could not bind name to socket.\n");
		closesocket(sock);
		WSACleanup();
		return;
	}

	printf("UDP Socket created.\n");
	socketAvailable = true;
}


void socketServerMainLoop() {
	while (true) {
		if (!socketAvailable) {
			// Busy wait. Don't hog CPU.
			this_thread::sleep_for(chrono::milliseconds(100));
			continue;
		}

		int client_length = (int)sizeof(struct sockaddr_in);

		int bytes_received = recvfrom(sock, inputBuffer, BUFFER_SIZE, 0, (struct sockaddr *)&client, &client_length);

		printf("%d bytes received in UDP Buffer.\n", bytes_received);

		if (bytes_received < 0) {
			fprintf(stderr, "Could not receive datagram.\n");
			closesocket(sock);
			WSACleanup();
			return;
		} else if (inputBuffer[0] == 'U') {
			// Thread safe code
			{
				lock_guard<mutex> lk(loc_mtx);
				sprintf(outputBuffer, "%7.2f    %7.2f    %7.2f", trans[0][3], trans[1][3], trans[2][3]);
			}
			int len = strlen(outputBuffer);
			if (sendto(sock, outputBuffer, len, 0, (struct sockaddr *)&client, client_length) != len) {
				fprintf(stderr, "Error sending datagram.\n");
				closesocket(sock);
				WSACleanup();
				return;
			}
			/*else {
				fprintf(stderr, "Datagram of %d bytes sent: %s\n", len, outputBuffer);
			}*/
		}
		else {
			printf("??\n");
		}
	}
}

void closeSocketServer() {
	closesocket(sock);
	WSACleanup();
}