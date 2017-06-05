#include <uvssserverwrapper.h>

#include <uvssserver.h>
#include <version.h>

UVSSServer* serverSDK;

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
    serverSDK = new UVSSServer;
    return serverSDK->init();
}

void UninitUVSSServer()
{
    serverSDK->uninit();
    delete serverSDK;
}

void SendUVSSCheckInfo(const char* const a1[], int sz1,
                       const char* const a[], int sz)
{
    std::vector<std::string> v1(a1, a1+sz1);
    std::vector<std::string> v(a, a+sz);
    serverSDK->sendCheckInfo(v1, v);
}

const char* GetUVSSServerVersion()
{
    return UVSS_COMM_SDK_VER;
}
