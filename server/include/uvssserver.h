#ifndef UVSSSERVER_H
#define UVSSSERVER_H

#include <string>

#include <Ice/Ice.h>

#include <serveri.h>

class UVSSServer {
public:
    UVSSServer();

    void setConnectionInfoCallback(UVSSServerCallback);
    void setPort(int);
    int init();
    void uninit();
    void sendCheckInfo(const std::vector<std::string>&,
            const std::vector<std::string>&);

private:
    Ice::CommunicatorPtr ic;
    //Ice::CommunicatorHolder ic;
    //adapter?
    int port;
    std::shared_ptr<ServerI> server;
};

#endif
