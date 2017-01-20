#include <uvssclientsdk.h>
#include <uvssclient.h>
#include <version.h>

UVSSClientPtr clientSDK = new UVSSClient;

void SetUVSSMessageCallback(UVSSMessageCallback connectionInfoCallback)
{
    clientSDK->setConnectionInfoCallback(connectionInfoCallback);
}

void SetUVSSCheckInfoCallback(UVSSCheckInfoCallback checkInfoCallback)
{
    clientSDK->setCheckInfoCallback(checkInfoCallback);
}

int UVSSInitialize()
{
    return clientSDK->init();
}

void UVSSUninitialize()
{
    clientSDK->uninit();
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
