#include <uvssclientwrapper.h>

#include <global.h>
#include <uvssclient.h>

UvssClient* uvssClient;

const char* GetUVSSVersion()
{
    return Uvss::version.c_str();
}

void SetUVSSMessageCallback(UVSSMessageCallback uvssMessageCallback)
{
    g_initializationCallback = uvssMessageCallback;
    g_connectionCallback = uvssMessageCallback;
}

void SetUVSSCheckInfoCallback(UVSSCheckInfoCallback uvssCheckInfoCallback)
{
    g_checkInfoCallback = uvssCheckInfoCallback;
}

void SetUVSSCheckInfoExCallback(UVSSCheckInfoExCallback uvssCheckInfoExCallback)
{
    g_checkInfoExCallback = uvssCheckInfoExCallback;
}

int UVSSInitialize()
{
    uvssClient = new UvssClient;
    return uvssClient->start();
}

int UVSSConnect(const char* ipAddress, int port)
{
    return uvssClient->connect(ipAddress, port);
}

int UVSSDisconnect(int connectionId)
{
    return uvssClient->disconnect(connectionId);
}

void SendUVSSCheckInfo(const char* uvssImagePath,
                       const char* plateImagePath,
                       const char* channel,
                       const char* plateNumber,
                       const char* direction,
                       const char* dateTime,
                       const char* extension)
{
    uvssClient->sendCheckInfo(std::string(uvssImagePath),
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
    uvssClient->sendCheckInfo(std::string(endpoint),
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
    uvssClient->sendCheckInfo(std::string(concatedString),
                              std::string(concatedFilePath));
}

void SendUVSSCheckInfoExByEndpoint(const char* endpoint,
                                   const char* concatedString,
                                   const char* concatedFilePath)
{
    uvssClient->sendCheckInfo(std::string(endpoint),
                              std::string(concatedString),
                              std::string(concatedFilePath));
}

void UVSSUninitialize()
{
    uvssClient->shutdown();
    delete uvssClient;
}
