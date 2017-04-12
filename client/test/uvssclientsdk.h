#ifndef UVSSCLIENTSDK_H
#define UVSSCLIENTSDK_H

#ifdef _WIN32
#ifdef UVSS_COMM_EXPORTS
#define UVSS_COMM_API _declspec(dllexport)
#else
#define UVSS_COMM_API _declspec(dllimport)
#endif

#define STDCALL __stdcall
#else
#define UVSS_COMM_API
#define STDCALL
#endif

//#include <clienti.h>

//typedef ClientConnectionInfoCallback UVSSMessageCallback;
//typedef ClientCheckInfoCallback UVSSCheckInfoCallback;

typedef void(STDCALL *UVSSMessageCallback)(int handle, int code,
    const char *message);
typedef void(STDCALL *UVSSCheckInfoCallback)(int handle,
    const char *UVSSImagePath, const char *PlateImagePath,
    const char *Channel, const char *PlateNumber, const char *Direction,
    const char *CheckDateTime, const char *Extension);

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
