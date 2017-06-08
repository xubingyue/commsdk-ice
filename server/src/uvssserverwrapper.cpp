#include <uvssserverwrapper.h>

#include <uvssserver.h>
#include <version.h>

UvssServer* uvssServer;

void SetUVSSServerCallback(UVSSServerCallback connectionCallback)
{
    uvssServer->setConnectionCallback(connectionCallback);
}

void SetUVSSServerPort(int port)
{
    uvssServer->setPort(port);
}

int InitUVSSServer()
{
    uvssServer = new UvssServer;
    return uvssServer->start();
}

void UninitUVSSServer()
{
    uvssServer->shutdown();
    delete uvssServer;
}

void SendUVSSCheckInfo(const char* const strings[], int stringsSize,
                       const char* const filePaths[], int filePathsSize)
{
    std::vector<std::string> stringsVec(strings, strings + stringsSize);
    std::vector<std::string> filePathsVec(filePaths, filePaths + filePathsSize);
    uvssServer->sendCheckInfo(stringsVec, filePathsVec);
}

const char* GetUVSSServerVersion()
{
    return UVSS_COMM_SDK_VER;
}
