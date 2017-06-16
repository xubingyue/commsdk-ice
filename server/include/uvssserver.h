#ifndef UVSSSERVER_H
#define UVSSSERVER_H

#include <Ice/Ice.h>

#include <rpcproxies.h>
#include <callbacksenderi.h>

class UvssServer {
public:
    UvssServer();
    ~UvssServer();

    int start();

    void sendCheckInfo(const std::string&, const std::string&,
                       const std::string&, const std::string&,
                       const std::string&, const std::string&,
                       const std::string&);
    void sendCheckInfo(const std::string&, const std::string&);

    void shutdown();

    static void setPort(int);

private:
    Ice::CommunicatorHolder ich_;
    Ice::ObjectAdapterPtr adapter_;
    Ice::Identity ident_;

    std::shared_ptr<RpcProxies> rpcProxies_;
    std::shared_ptr<CallbackSenderI> sender_;

    static int port_; // 为兼容旧C接口，设为static

    void filePathToFile(const std::string&, std::vector<unsigned char>&);
};

#endif
