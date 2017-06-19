#include <uvssclientwrapper.h>

#include <global.h>
#include <uvssclient.h>

UvssClient* clientSDK;

const char* GetUVSSVersion()
{
    return Uvss::version.c_str();
}

void SetUVSSMessageCallback(UVSSMessageCallback uvssMessageCallback)
{
    g_initializationCallback = uvssMessageCallback;
    g_connectionCallback = uvssMessageCallback;
}

void SetUVSSCheckInfoCallback(UVSSCheckInfoCallback uvssCheckInfoCallback)
{
    g_checkInfoCallback = uvssCheckInfoCallback;
}

void SetUVSSCheckInfoCallbackEx(UVSSCheckInfoCallbackEx uvssCheckInfoCallbackEx)
{
    g_checkInfoCallbackEx = uvssCheckInfoCallbackEx;
}

int UVSSInitialize()
{
    clientSDK = new UvssClient;
    return clientSDK->start();
}

int UVSSConnect(const char* ipAddress, int port)
{
    return clientSDK->connect(ipAddress, port);
}

int UVSSDisconnect(int connectionId)
{
    return clientSDK->disconnect(connectionId);
}

void UVSSUninitialize()
{
    clientSDK->shutdown();
    delete clientSDK;
}
