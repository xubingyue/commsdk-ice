#include <uvssclientwrapper.h>

#include <uvssclient.h>
#include <version.h>

UvssClient* clientSDK;

void SetUVSSMessageCallback(UVSSMessageCallback cb)
{
    UvssClient::setInitializationCallback(cb);
    UvssClient::setConnectionCallback(cb);
    clientSDK->setHeartbeatCallback(cb);
}

void SetUVSSCheckInfoCallback(UVSSCheckInfoCallback checkInfoCallback)
{
    clientSDK->setCheckInfoCallback(checkInfoCallback);
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
    return UVSS_COMM_SDK_VER;
}
