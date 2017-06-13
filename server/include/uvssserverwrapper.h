#ifndef UVSSSERVERWRAPPER_H
#define UVSSSERVERWRAPPER_H

#ifdef _WIN32
#   ifdef UVSS_EXPORTS
#       define UVSS_API _declspec(dllexport)
#   else
#       define UVSS_API _declspec(dllimport)
#   endif
#else
#   define UVSS_API
#endif

typedef void(*UVSSServerCallback)(int messageType, const char* message);

#ifdef __cplusplus
extern "C" {
#endif

UVSS_API void SetUVSSServerCallback(UVSSServerCallback);
UVSS_API void SetUVSSServerPort(int);
UVSS_API const char* GetUVSSServerVersion();
UVSS_API int InitUVSSServer();



UVSS_API void SendUVSSCheckInfo(const char *UVSSImagePath, const char *PlateImagePath,
        const char *Channel, const char *PlateNumber, const char *Direction,
        const char *CheckDateTime, const char *Extension);

// UVSS_API void SendUVSSCheckInfo(const char* const [], int, const char* const [], int);
UVSS_API void SendUVSSCheckInfoNormal(const char* const filePaths[], const char* const strings[]);

UVSS_API void SendUVSSCheckInfoNew(const char* filePathsSrc, const char* stringsSrc);

UVSS_API void UninitUVSSServer();

#ifdef __cplusplus
}
#endif

#endif
