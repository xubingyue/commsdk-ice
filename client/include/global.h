#ifndef GLOBAL_H
#define GLOBAL_H

#include <uvssclientwrapper.h>

typedef UVSSMessageCallback InitializationCallback;
typedef UVSSMessageCallback ConnectionCallback;

typedef UVSSCheckInfoCallback CheckInfoCallback;
typedef UVSSCheckInfoCallbackNormal CheckInfoCallbackNormal;
typedef UVSSCheckInfoCallbackNew CheckInfoCallbackNew;

extern InitializationCallback g_initializationCallback;
extern ConnectionCallback g_connectionCallback;

extern int g_type;
extern CheckInfoCallback g_checkInfoCallback;
extern CheckInfoCallbackNormal g_checkInfoCallbackNormal;
extern CheckInfoCallbackNew g_checkInfoCallbackNew;

#endif // GLOBAL_H
