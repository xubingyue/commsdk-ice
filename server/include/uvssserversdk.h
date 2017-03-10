#ifndef UVSSSERVERSDK_H
#define UVSSSERVERSDK_H

#ifdef _WIN32
    #ifdef UVSS_COMM_EXPORTS
        #define UVSS_COMM_API _declspec(dllexport)
    #else
        #define UVSS_COMM_API _declspec(dllimport)
    #endif
#else
    #define UVSS_COMM_API
#endif

#include <serveri.h>

typedef ServerConnectionInfoCallback UVSSServerCallback;

extern "C" {
    UVSS_COMM_API void SetUVSSServerCallback(UVSSServerCallback);
    UVSS_COMM_API void SetUVSSServerPort(int);
    UVSS_COMM_API int InitUVSSServer();
    UVSS_COMM_API void UninitUVSSServer();
    UVSS_COMM_API void SendUVSSCheckInfo(const char*, const char*,
            const char*, const char*, const char*, const char*, const char*);
    UVSS_COMM_API const char* GetUVSSServerVersion();
}

#endif
