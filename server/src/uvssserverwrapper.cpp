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

void SendUVSSCheckInfo(const char* uvssImagePath, const char* plateImagePath,
                       const char* channel, const char* plateNumber,
                       const char* direction, const char* dateTime,
                       const char* extension)
{
    std::vector<std::string> strings;
    strings.push_back(channel);
    strings.push_back(plateNumber);
    strings.push_back(direction);
    strings.push_back(dateTime);
    strings.push_back(extension);

    std::vector<std::string> filePaths;
    filePaths.push_back(uvssImagePath);
    filePaths.push_back(plateImagePath);

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
