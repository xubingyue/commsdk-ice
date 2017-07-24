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
UVSS_API void UninitUVSSServer();

#ifdef __cplusplus
}
#endif

#endif
