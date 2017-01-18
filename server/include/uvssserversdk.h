#ifndef UVSSSERVERSDK_H
#define UVSSSERVERSDK_H

#include "serveri.h"

typedef ServerConnectionInfoCallback UVSSServerCallback;

extern "C" {
    void SetUVSSServerCallback(UVSSServerCallback);
    void SetUVSSServerPort(int);
    int InitUVSSServer();
    void UninitUVSSServer();
    void SendUVSSCheckInfo(const char*, const char*,
            const char*, const char*, const char*, const char*, const char*);
    const char* GetUVSSServerVersion();
}

#endif
