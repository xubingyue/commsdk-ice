#ifndef UVSSSERVER_H
#define UVSSSERVER_H

#include <string>
#include <Ice/Ice.h>
//#include <IceUtil/IceUtil.h>
#include <serveri.h>

// class UVSSServer;
// typedef IceUtil::Handle<UVSSServer> UVSSServerPtr;

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
//     Ice::CommunicatorPtr ic;
    std::shared_ptr<Ice::Communicator> ic;
    //Ice::CommunicatorHolder ic;
    //adapter?
    std::shared_ptr<ServerI> server;
    int port;
};

#endif
