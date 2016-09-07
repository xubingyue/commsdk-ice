#ifndef UVSSSERVERSDK_H
#define UVSSSERVERSDK_H

#include "uvssserver.h"

typedef ServerConnectionInfoCallback UVSSServerCallback;

extern "C" {
	__declspec(dllexport) void SetUVSSServerPort(int port);
	__declspec(dllexport) int InitUVSSServer();
	__declspec(dllexport) void UninitUVSSServer();
	__declspec(dllexport) void SetUVSSServerCallback(UVSSServerCallback callback);
	__declspec(dllexport) void SendUVSSCheckInfo(const char* UVSSImagePath, const char* PlateImagePath, const char* Channel, const char* PlateNumber, const char* Direction, const char* CheckDateTime, const char* Extension);
}

#endif
