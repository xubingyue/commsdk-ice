#include <uvssserverwrapper.h>

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

void SetUVSSCheckInfoCallback(UVSSCheckInfoCallback uvssCheckInfoCallback)
{
    g_checkInfoCallback = uvssCheckInfoCallback;
}

void SetUVSSCheckInfoExCallback(UVSSCheckInfoExCallback uvssCheckInfoExCallback)
{
    g_checkInfoExCallback = uvssCheckInfoExCallback;
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

void SendUVSSCheckInfo(const char* uvssImagePath,
                       const char* plateImagePath,
                       const char* channel,
                       const char* plateNumber,
                       const char* direction,
                       const char* dateTime,
                       const char* extension)
{
    uvssServer->sendCheckInfo(std::string(uvssImagePath),
                              std::string(plateImagePath),
                              std::string(channel),
                              std::string(plateNumber),
                              std::string(direction),
                              std::string(dateTime),
                              std::string(extension));
}

void SendUVSSCheckInfoByEndpoint(const char* endpoint,
                                 const char* uvssImagePath,
                                 const char* plateImagePath,
                                 const char* channel,
                                 const char* plateNumber,
                                 const char* direction,
                                 const char* dateTime,
                                 const char* extension)
{
    uvssServer->sendCheckInfo(std::string(endpoint),
                              std::string(uvssImagePath),
                              std::string(plateImagePath),
                              std::string(channel),
                              std::string(plateNumber),
                              std::string(direction),
                              std::string(dateTime),
                              std::string(extension));
}

void SendUVSSCheckInfoEx(const char* concatedString,
                         const char* concatedFilePath)
{
    uvssServer->sendCheckInfo(std::string(concatedString),
                              std::string(concatedFilePath));
}

void SendUVSSCheckInfoExByEndpoint(const char* endpoint,
                                   const char* concatedString,
                                   const char* concatedFilePath)
{
    uvssServer->sendCheckInfo(std::string(endpoint),
                              std::string(concatedString),
                              std::string(concatedFilePath));
}

void UninitUVSSServer()
{
    uvssServer->shutdown();
    delete uvssServer;
}
