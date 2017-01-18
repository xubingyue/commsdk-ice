#include "uvssserversdk.h"
#include "uvssserver.h"
#include <version.h>

UVSSServerPtr serverSDK = new UVSSServer;

void SetUVSSServerCallback(UVSSServerCallback connectionInfoCallback)
{
    serverSDK->setConnectionInfoCallback(connectionInfoCallback);
}

void SetUVSSServerPort(int port)
{
    serverSDK->setPort(port);
}

int InitUVSSServer()
{
    return serverSDK->init();
}

void UninitUVSSServer()
{
    serverSDK->uninit();
}

void SendUVSSCheckInfo(const char* uVSSImagePath, const char* plateImagePath,
        const char* channel, const char* plateNumber, const char* direction,
        const char* time, const char* extension)
{
    serverSDK->sendCheckInfo(uVSSImagePath, plateImagePath,
            channel, plateNumber, direction, time, extension);
}

const char* GetUVSSServerVersion()
{
    return UVSS_COMM_SDK_VER;
}
