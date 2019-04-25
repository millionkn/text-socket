#include "pch.h"
#include <WinBase.h>
typedef int SOCKET;
HMODULE hModule = LoadLibrary(L"text-socket");
bool (*connectWith)(SOCKET* socketKeyBuf, const char* ip, int port) = (bool(*)(SOCKET*,const char*,int))GetProcAddress(hModule, "connectWith");
void (*sendTo)(SOCKET* socketKey, const char* data, int size) = (void(*)(SOCKET*, const char*, int))GetProcAddress(hModule,"sendTo");
int (*peekMessageSize)(SOCKET* socketKey) = (int(*)(SOCKET*))GetProcAddress(hModule,"peekMessageSize");
extern "C" {
	__declspec(dllimport) void getMessage(SOCKET* socketKey, char* data); 
}
TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  SOCKET socket = { 0 };
  bool result = connectWith(&socket,"127.0.0.1", 8090);
  EXPECT_TRUE(result);
  sendTo(&socket, "≤‚ ‘123", 8);
}