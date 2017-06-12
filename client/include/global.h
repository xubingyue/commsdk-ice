#ifndef GLOBAL_H
#define GLOBAL_H

#include <uvssclientwrapper.h>

typedef UVSSMessageCallback InitializationCallback;
typedef UVSSMessageCallback ConnectionCallback;
typedef UVSSCheckInfoCallback CheckInfoCallback;

extern InitializationCallback g_initializationCallback;
extern ConnectionCallback g_connectionCallback;
extern CheckInfoCallback g_checkInfoCallback;

#endif // GLOBAL_H
