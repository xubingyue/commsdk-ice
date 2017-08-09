#ifndef GLOBAL_H
#define GLOBAL_H

#ifdef _WIN32
#   define SLASH "\\"
#else
#   define SLASH "/"
#endif

#include <uvssserverwrapper.h>

typedef UVSSServerCallback ConnectionCallback;
typedef UVSSCheckInfoCallback CheckInfoCallback;
typedef UVSSCheckInfoExCallback CheckInfoExCallback;

extern ConnectionCallback g_connectionCallback;
extern CheckInfoCallback g_checkInfoCallback;
extern CheckInfoExCallback g_checkInfoExCallback;

#endif
