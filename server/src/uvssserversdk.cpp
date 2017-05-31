#include <uvssserversdk.h>
#include <uvssserver.h>
#include <version.h>

auto serverSDK = std::make_shared<UVSSServer>(); //改为只声明变量

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
    return serverSDK->init(); //改为new + init()
}

void UninitUVSSServer()
{
    serverSDK->uninit(); //改为delete
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
