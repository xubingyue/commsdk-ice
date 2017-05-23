#ifndef UVSSCLIENTSDK_H
#define UVSSCLIENTSDK_H

#ifdef _WIN32
#   ifdef UVSS_COMM_EXPORTS
#       define UVSS_COMM_API _declspec(dllexport)
#   else
#       define UVSS_COMM_API _declspec(dllimport)
#   endif
#else
#   define UVSS_COMM_API
#endif

#include <clienti.h>

typedef ClientConnectionInfoCallback UVSSMessageCallback;
typedef ClientCheckInfoCallback UVSSCheckInfoCallback;

extern "C" {
    UVSS_COMM_API void SetUVSSMessageCallback(UVSSMessageCallback);
    UVSS_COMM_API void SetUVSSCheckInfoCallback(UVSSCheckInfoCallback);
    UVSS_COMM_API int UVSSInitialize();
    UVSS_COMM_API void UVSSUninitialize();
    UVSS_COMM_API int UVSSConnect(const char*, int);
    UVSS_COMM_API int UVSSDisconnect(int);
    UVSS_COMM_API const char* GetUVSSVersion();
}

#endif
