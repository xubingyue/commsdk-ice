#ifndef GLOBAL_H
#define GLOBAL_H

#ifdef _WIN32
#   define SLASH "\\"
#else
#   define SLASH "/"
#endif

#include <uvssclientwrapper.h>

typedef UVSSMessageCallback InitializationCallback;
typedef UVSSMessageCallback ConnectionCallback;
typedef UVSSCheckInfoCallback CheckInfoCallback;
typedef UVSSCheckInfoExCallback CheckInfoExCallback;

extern InitializationCallback g_initializationCallback;
extern ConnectionCallback g_connectionCallback;
extern CheckInfoCallback g_checkInfoCallback;
extern CheckInfoExCallback g_checkInfoExCallback;

#endif
