#ifndef GLOBAL_H
#define GLOBAL_H

#include <uvssclientwrapper.h>

typedef UVSSMessageCallback InitializationCallback;
typedef UVSSMessageCallback ConnectionCallback;
typedef UVSSCheckInfoCallbackNormal CheckInfoCallbackNormal;

extern InitializationCallback g_initializationCallback;
extern ConnectionCallback g_connectionCallback;
extern CheckInfoCallbackNormal g_checkInfoCallbackNormal;

#endif // GLOBAL_H
