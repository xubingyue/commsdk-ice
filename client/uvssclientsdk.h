#ifndef UVSSCLIENTSDK_H
#define UVSSCLIENTSDK_H

#include "clienti.h"

typedef ClientConnectionInfoCallback UVSSMessageCallback;
typedef ClientCheckInfoCallback UVSSCheckInfoCallback;

extern "C" {
    __declspec(dllexport) void SetUVSSMessageCallback(UVSSMessageCallback);
    __declspec(dllexport) void SetUVSSCheckInfoCallback(UVSSCheckInfoCallback);
    __declspec(dllexport) int UVSSInitialize();
    __declspec(dllexport) void UVSSUninitialize();
    __declspec(dllexport) int UVSSConnect(const char*, int);
    __declspec(dllexport) int UVSSDisconnect(int);
    __declspec(dllexport) const char* GetUVSSVersion();
}

#endif
