#include <uvssclientwrapper.h>

#include <uvssclient.h>
#include <global.h>

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
    g_type = 0;
    g_checkInfoCallback = uvssCheckInfoCallback;
}

void SetUVSSCheckInfoCallbackCore(UVSSCheckInfoCallbackCore uvssCheckInfoCallbackCore)
{
    g_type = 1;
    g_checkInfoCallbackCore = uvssCheckInfoCallbackCore;
}

void SetUVSSCheckInfoCallbackEx(UVSSCheckInfoCallbackEx uvssCheckInfoCallbackEx)
{
    g_type = 2;
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
