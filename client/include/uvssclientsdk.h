#ifndef UVSSCLIENTSDK_H
#define UVSSCLIENTSDK_H

#include <clienti.h>

typedef ClientConnectionInfoCallback UVSSMessageCallback;
typedef ClientCheckInfoCallback UVSSCheckInfoCallback;

extern "C" {
    void SetUVSSMessageCallback(UVSSMessageCallback);
    void SetUVSSCheckInfoCallback(UVSSCheckInfoCallback);
    int UVSSInitialize();
    void UVSSUninitialize();
    int UVSSConnect(const char*, int);
    int UVSSDisconnect(int);
    const char* GetUVSSVersion();
}

#endif
