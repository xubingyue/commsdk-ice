#include <uvssserverwrapper.h>

#include <uvssserver.h>
#include <version.h>
#include <global.h>

UvssServer* uvssServer;

void SetUVSSServerCallback(UVSSServerCallback connectionCallback)
{
    g_connectionCallback = connectionCallback;
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

//     g_connectionCallback = 0;
}

// void SendUVSSCheckInfo(const char* const strings[], int stringsSize,
//                        const char* const filePaths[], int filePathsSize)
// {
//     std::vector<std::string> stringsVec(strings, strings + stringsSize);
//     std::vector<std::string> filePathsVec(filePaths, filePaths + filePathsSize);
//     uvssServer->sendCheckInfo(stringsVec, filePathsVec);
// }

void SendUVSSCheckInfo(const char* uvssImagePath, const char* plateImagePath,
        const char* channel, const char* plateNumber, const char* direction,
        const char* time, const char* extension)
{
    const char* const filePaths[] = { uvssImagePath, plateImagePath, NULL };
    const char* const strings[] = { channel, plateNumber, direction, time, extension, NULL };
    SendUVSSCheckInfoNormal(filePaths, strings);
}

void SendUVSSCheckInfoNormal(const char* const filePaths[],
                       const char* const strings[])
{
    int i = 0;
    for (; filePaths[i] != NULL; ++i) {
        
    }
    int filePathsSize = i;
    std::vector<std::string> filePathsVec(filePaths, filePaths + filePathsSize);

    int j = 0;
    for (; strings[j] != NULL; ++j) {
        
    }
    int stringsSize = j;
    std::vector<std::string> stringsVec(strings, strings + stringsSize);

    uvssServer->sendCheckInfo(stringsVec, filePathsVec);
}

const char* GetUVSSServerVersion()
{
    return UVSS_VERSION;
}
