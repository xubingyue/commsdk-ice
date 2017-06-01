#include <uvssclientsdk.h>
#include <uvssclient.h>
#include <version.h>

// auto clientSDK = std::make_shared<UVSSClient>();
UVSSClient* clientSDK;

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
    clientSDK = new UVSSClient;
    return clientSDK->init();
}

void UVSSUninitialize()
{
    clientSDK->uninit();
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
