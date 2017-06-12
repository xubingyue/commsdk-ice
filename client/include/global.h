#ifndef GLOBAL_H
#define GLOBAL_H

#include <uvssclientwrapper.h>

typedef UVSSMessageCallback InitializationCallback;
typedef UVSSMessageCallback ConnectionCallback;

typedef UVSSCheckInfoCallback CheckInfoCallback;
typedef UVSSCheckInfoCallbackNormal CheckInfoCallbackNormal;

extern InitializationCallback g_initializationCallback;
extern ConnectionCallback g_connectionCallback;

extern int g_type;
extern CheckInfoCallback g_checkInfoCallback;
extern CheckInfoCallbackNormal g_checkInfoCallbackNormal;

#endif // GLOBAL_H
