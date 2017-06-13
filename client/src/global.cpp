#include <global.h>

InitializationCallback g_initializationCallback = 0;
ConnectionCallback g_connectionCallback = 0;

int g_type = 0;
CheckInfoCallback g_checkInfoCallback = 0;
CheckInfoCallbackNormal g_checkInfoCallbackNormal = 0;
CheckInfoCallbackNew g_checkInfoCallbackNew = 0;
