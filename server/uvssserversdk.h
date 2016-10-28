#ifndef UVSSSERVERSDK_H
#define UVSSSERVERSDK_H

#include "serveri.h"

typedef ServerConnectionInfoCallback UVSSServerCallback;

extern "C" {
    __declspec(dllexport) void SetUVSSServerCallback(UVSSServerCallback);
    __declspec(dllexport) void SetUVSSServerPort(int);
    __declspec(dllexport) int InitUVSSServer();
    __declspec(dllexport) void UninitUVSSServer();
    __declspec(dllexport) void SendUVSSCheckInfo(const char*, const char*,
            const char*, const char*, const char*, const char*, const char*);
}

#endif
