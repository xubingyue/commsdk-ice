#include <uvssserverwrapper.h>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include <global.h>
#include <uvssserver.h>

UvssServer* uvssServer;

const char* GetUVSSServerVersion()
{
    return Uvss::version.c_str();
}

void SetUVSSServerCallback(UVSSServerCallback uvssServerCallback)
{
    g_connectionCallback = uvssServerCallback;
}

void SetUVSSServerPort(int port)
{
    UvssServer::setPort(port);
}

int InitUVSSServer()
{
    uvssServer = new UvssServer;
    return uvssServer->start();
}

void SendUVSSCheckInfo(const char* uvssImagePathC, const char* plateImagePathC,
                       const char* channelC, const char* plateNumberC,
                       const char* directionC, const char* dateTimeC,
                       const char* extensionC)
{
    const char* const stringsC[] = { channelC, plateNumberC, directionC,
        dateTimeC, extensionC, NULL };
    const char* const filePathsC[] = { uvssImagePathC, plateImagePathC, NULL };
    SendUVSSCheckInfoCore(stringsC, filePathsC);
}

void SendUVSSCheckInfoCore(const char* const* stringsC,
                           const char* const* filePathsC)
{
    int i = 0;
    for (; stringsC[i] != NULL; ++i) {
    }
    int stringsSize = i;
    std::vector<std::string> strings(stringsC, stringsC + stringsSize);

    int j = 0;
    for (; filePathsC[j] != NULL; ++j) {
    }
    int filePathsSize = j;
    std::vector<std::string> filePaths(filePathsC, filePathsC + filePathsSize);

    uvssServer->sendCheckInfo(strings, filePaths);
}

void SendUVSSCheckInfoEx(const char* concatedStringC,
                         const char* concatedFilePathC)
{
    std::vector<std::string> strings;
    strings.push_back(std::string(concatedStringC));

    std::vector<std::string> filePaths;
    boost::split(filePaths, concatedFilePathC, boost::is_any_of("|"),
                 boost::token_compress_on);

    uvssServer->sendCheckInfo(strings, filePaths);
}

void UninitUVSSServer()
{
    uvssServer->shutdown();
    delete uvssServer;
}
