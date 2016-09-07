#ifndef UVSSCLIENTSDK_H
#define UVSSCLIENTSDK_H

#include "uvssclient.h"

typedef ClientConnectionInfoCallback UVSSMessageCallback;
typedef ClientCheckInfoCallback UVSSCheckInfoCallback;

extern "C" {
	__declspec(dllexport) int UVSSInitialize();
	__declspec(dllexport) void SetUVSSMessageCallback(UVSSMessageCallback callback);
	__declspec(dllexport) void SetUVSSCheckInfoCallback(UVSSCheckInfoCallback callback);
	__declspec(dllexport) int UVSSConnect(const char* ServerIPAddress, int ServerPort);
	__declspec(dllexport) int UVSSDisconnect(int handle);
	__declspec(dllexport) void UVSSUninitialize();
}

#endif
