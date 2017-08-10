#ifndef UVSSSERVERWRAPPER_H
#define UVSSSERVERWRAPPER_H

#ifdef _WIN32
#   ifdef UVSS_EXPORTS
#      define UVSS_API _declspec(dllexport)
#   else
#      define UVSS_API _declspec(dllimport)
#   endif
#else
#   define UVSS_API
#endif

typedef void(*UVSSServerCallback)(int code, const char* message);
typedef void(*UVSSCheckInfoCallback)(int connectionId,
                                     const char* uvssImagePath,
                                     const char* plateImagePath,
                                     const char* channel,
                                     const char* plateNumber,
                                     const char* direction,
                                     const char* dateTime,
                                     const char* extension);
typedef void(*UVSSCheckInfoExCallback)(int connectionId,
                                       const char* concatedString,
                                       const char* concatedFilePath);

#ifdef __cplusplus
extern "C" {
#endif

UVSS_API const char* GetUVSSServerVersion();
UVSS_API void SetUVSSServerCallback(UVSSServerCallback uvssServerCallback);
UVSS_API void SetUVSSCheckInfoCallback(UVSSCheckInfoCallback
                                       uvssCheckInfoCallback);
UVSS_API void SetUVSSCheckInfoExCallback(UVSSCheckInfoExCallback
                                         uvssCheckInfoExCallback);
UVSS_API void SetUVSSServerPort(int port);
UVSS_API int InitUVSSServer();
UVSS_API void SendUVSSCheckInfo(const char* uvssImagePath,
                                const char* plateImagePath,
                                const char* channel,
                                const char* plateNumber,
                                const char* direction,
                                const char* dateTime,
                                const char* extension);
UVSS_API void SendUVSSCheckInfoByEndpoint(const char* endpoint,
                                          const char* uvssImagePath,
                                          const char* plateImagePath,
                                          const char* channel,
                                          const char* plateNumber,
                                          const char* direction,
                                          const char* dateTime,
                                          const char* extension);
UVSS_API void SendUVSSCheckInfoEx(const char* concatedString,
                                  const char* concatedFilePath);
UVSS_API void SendUVSSCheckInfoExByEndpoint(const char* endpoint,
                                            const char* concatedString,
                                            const char* concatedFilePath);
UVSS_API void UninitUVSSServer();

#ifdef __cplusplus
}
#endif

#endif
