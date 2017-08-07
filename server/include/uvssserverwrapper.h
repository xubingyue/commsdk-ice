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

typedef void(*UVSSServerCallback)(int, const char*);

typedef void(*UVSSCheckInfoCallback)(int connectionId,
                                     const char* uvssImagePath,
                                     const char* plateImagePath,
                                     const char* channel,
                                     const char* plateNumber,
                                     const char* direction,
                                     const char* dateTime,
                                     const char* extension);

typedef void(*UVSSCheckInfoCallbackEx)(int connectionId,
                                       const char* concatedString,
                                       const char* concatedFilePath);

#ifdef __cplusplus
extern "C" {
#endif

UVSS_API const char* GetUVSSServerVersion();
UVSS_API void SetUVSSServerCallback(UVSSServerCallback);
UVSS_API void SetUVSSServerPort(int);

UVSS_API int InitUVSSServer();
UVSS_API void SendUVSSCheckInfo(const char*, const char*, const char*,
                                const char*, const char*, const char*,
                                const char*);
UVSS_API void SendUVSSCheckInfoByEndpoint(const char*,
                                    const char*, const char*, const char*,
                                    const char*, const char*, const char*,
                                    const char*);
UVSS_API void SendUVSSCheckInfoEx(const char*, const char*);
UVSS_API void SendUVSSCheckInfoByEndpointEx(const char*, const char*, const char*);
UVSS_API void UninitUVSSServer();

// SetUVSSCheckInfoCallback: 设置车辆检查信息回调函数
UVSS_API void SetUVSSCheckInfoCallback(UVSSCheckInfoCallback
                                       uvssCheckInfoCallback);

// SetUVSSCheckInfoCallback: 设置车辆检查信息回调Ex函数
UVSS_API void SetUVSSCheckInfoCallbackEx(UVSSCheckInfoCallbackEx
                                         uvssCheckInfoCallbackEx);

#ifdef __cplusplus
}
#endif

#endif
