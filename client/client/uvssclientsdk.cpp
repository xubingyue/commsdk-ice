#include "uvssclientsdk.h"

UVSSClientPtr clientSDK = new UVSSClient;

int UVSSInitialize()
{
	return clientSDK->init();
}

void SetUVSSMessageCallback(UVSSMessageCallback callback)
{
	clientSDK->setConnectionInfoCallback(callback);
}

void SetUVSSCheckInfoCallback(UVSSCheckInfoCallback callback)
{
	clientSDK->setCheckInfoCallback(callback);
}

int UVSSConnect(const char* ServerIPAddress, int ServerPort)
{
	return clientSDK->connect(ServerIPAddress, ServerPort);
}

int UVSSDisconnect(int handle)
{
	return clientSDK->disconnect(handle);
}

void UVSSUninitialize()
{
	clientSDK->uninit();
}
