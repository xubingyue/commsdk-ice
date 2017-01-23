#ifndef UVSSSERVER_H
#define UVSSSERVER_H

#include <string>

#include <Ice/Ice.h>

#include <serveri.h>

class UVSSServer : virtual public IceUtil::Shared {
public:
    UVSSServer();

    void setConnectionInfoCallback(ServerConnectionInfoCallback);
    void setPort(int);
    int init();
    void uninit();
    void sendCheckInfo(const std::string&, const std::string&,
            const std::string&, const std::string&, const std::string&,
            const std::string&, const std::string&);

private:
    Ice::CommunicatorPtr ic;
    //Ice::CommunicatorHolder ic;
    //adapter?
    int port;
    std::shared_ptr<ServerI> server;
};

#endif