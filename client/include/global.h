#ifndef GLOBAL_H
#define GLOBAL_H

#include <uvssclientwrapper.h>

typedef UVSSMessageCallback InitializationCallback;
typedef UVSSMessageCallback ConnectionCallback;

typedef UVSSCheckInfoCallback CheckInfoCallback;
typedef UVSSCheckInfoCallbackCore CheckInfoCallbackCore;
typedef UVSSCheckInfoCallbackEx CheckInfoCallbackEx;

extern InitializationCallback g_initializationCallback;
extern ConnectionCallback g_connectionCallback;

extern int g_type;
extern CheckInfoCallback g_checkInfoCallback;
extern CheckInfoCallbackCore g_checkInfoCallbackCore;
extern CheckInfoCallbackEx g_checkInfoCallbackEx;

#endif // GLOBAL_H
