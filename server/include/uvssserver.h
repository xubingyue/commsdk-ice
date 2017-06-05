#ifndef UVSSSERVER_H
#define UVSSSERVER_H

#include <string>

#include <Ice/Ice.h>

#include <serveri.h>

class UVSSServer {
public:
    UVSSServer();

    void setConnectionInfoCallback(UVSSServerCallback);
    static void setPort(int);
    int init();//start
    void uninit();//shutdown

    void sendCheckInfo(const std::vector<std::string>&,
                       const std::vector<std::string>&);

    void filePathToBinary(const std::string&, UVSS::ByteSeq&);
    const std::string createCurrentTime();

private:
    Ice::CommunicatorPtr ic;
//     Ice::CommunicatorHolder ic;
//     adapter?
    static int port;
    std::shared_ptr<ServerI> server;

    std::shared_ptr<RpcExecutor> _workQueue;
};

#endif
