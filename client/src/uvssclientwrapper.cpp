#include <uvssclientwrapper.h>

#include <uvssclient.h>
#include <version.h>
#include <global.h>

UvssClient* clientSDK;

void SetUVSSMessageCallback(UVSSMessageCallback cb)
{
    g_initializationCallback = cb;
    g_connectionCallback = cb;
}

void SetUVSSCheckInfoCallback(UVSSCheckInfoCallback checkInfoCallback)
{
    g_type = 0;
    g_checkInfoCallback = checkInfoCallback;
}

void SetUVSSCheckInfoCallbackNormal(UVSSCheckInfoCallbackNormal checkInfoCallbackNormal)
{
    g_type = 1;
    g_checkInfoCallbackNormal = checkInfoCallbackNormal;
}

void SetUVSSCheckInfoCallbackNew(UVSSCheckInfoCallbackNew checkInfoCallbackNew)
{
    g_type = 2;
    g_checkInfoCallbackNew = checkInfoCallbackNew;
}

int UVSSInitialize()
{
    clientSDK = new UvssClient;
    return clientSDK->start();
}

void UVSSUninitialize()
{
    clientSDK->shutdown();
    delete clientSDK;

//     g_initializationCallback = 0;
//     g_connectionCallback = 0;
//     g_checkInfoCallback = 0;
}

int UVSSConnect(const char* iPAddress, int port)
{
    return clientSDK->connect(iPAddress, port);
}

int UVSSDisconnect(int index)
{
    return clientSDK->disconnect(index);
}

const char* GetUVSSVersion()
{
    return UVSS_VERSION;
}
