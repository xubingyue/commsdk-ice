#include <uvssserverwrapper.h>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

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

void SendUVSSCheckInfoNew(const char* filePathsSrc, const char* stringsSrc)
{
    std::vector<std::string> filePathsVec;
    boost::split(filePathsVec, filePathsSrc, boost::is_any_of("#"), boost::token_compress_on);

    std::vector<std::string> stringsVec;
    stringsVec.push_back(stringsSrc);

    uvssServer->sendCheckInfo(stringsVec, filePathsVec);
}

const char* GetUVSSServerVersion()
{
    return UVSS_VERSION;
}
