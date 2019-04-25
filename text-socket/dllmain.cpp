// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		WSADATA wsaData;
		return !WSAStartup(MAKEWORD(2, 2), &wsaData);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
		break;
    case DLL_PROCESS_DETACH:
		WSACleanup();
        break;
    }
    return TRUE;
}

void inline sendAll(SOCKET* socket, const char* data, int size) {
	int offset = 0;
	while (offset < size) {
		offset += send(*socket, data + offset, size - offset, 0);
	}
}
void inline recvAll(SOCKET* socket, char* data, int size) {
	int offset = 0;
	while (offset < size) {
		offset += recv(*socket, data + offset, size - offset, MSG_WAITALL);
	}
}
extern "C" {
	__declspec(dllexport) bool connectWith(SOCKET* socketKeyBuf,char* ip,int port) {
		SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		sockaddr_in addr = {0};
		addr.sin_family = PF_INET;
		inet_pton(PF_INET,ip,&addr.sin_addr);
		addr.sin_port = htons(port);
		if (0== connect(sock, (SOCKADDR*)& addr, sizeof(SOCKADDR))) {
			memcpy(socketKeyBuf, &sock, sizeof(SOCKET));
			return true;
		}
		return false;
	}
	__declspec(dllexport) void sendTo(SOCKET* socketKey,const char* data, int size) {
		sendAll(socketKey, (const char*)& size, sizeof(int));
		sendAll(socketKey, data, size);
	}
	__declspec(dllexport) int peekMessageSize(SOCKET* socketKey) {
		int size = 0;
		if (sizeof(int) > recv(*socketKey, (char*)& size, sizeof(int), MSG_PEEK)) {
			return 0;
		}else {
			return size;
		}
	}
	__declspec(dllexport) void getMessage(SOCKET* socketKey, char* data) {
		int size = 0;
		recvAll(socketKey, (char*)& size, sizeof(int));
		recvAll(socketKey, data, size);
	}
	_declspec(dllexport) void disconnect(SOCKET* socketKey) {
		closesocket(*socketKey);
	}
}
