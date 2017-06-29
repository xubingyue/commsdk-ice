#ifndef UVSSSERVER_H
#define UVSSSERVER_H

#include <Ice/Ice.h>

#include <rpcproxies.h>
#include <callbacksenderi.h>

class UvssServer {
public:
    static void setPort(int);

    UvssServer();
    int start();

    void sendCheckInfo(const std::string&, const std::string&,
                       const std::string&, const std::string&,
                       const std::string&, const std::string&,
                       const std::string&);
    void sendCheckInfo(const std::string&, const std::string&);

    void shutdown();
    ~UvssServer();

private:
    Ice::CommunicatorPtr ic_;
    Ice::ObjectAdapterPtr adapter_;
    Ice::Identity ident_;

    // 为兼容旧C接口（init函数中调用C++构造函数和start，init前可以setPort），此处port设为static（比设为全局变量好点）
    static int port_;

#ifdef ICE_CPP11_MAPPING
    std::shared_ptr<RpcProxies> proxies_;
    std::shared_ptr<CallbackSenderI> servant_;
#else
    IceUtil::Handle<RpcProxies> proxies_;
    IceUtil::Handle<CallbackSenderI> servant_;
#endif

    void filePathToFile(const std::string&, std::vector<unsigned char>&);
    void filePathsToFileNamesAndFiles(const std::vector<std::string>&,
                                      std::vector<std::string>&,
                                      std::vector<std::vector<unsigned char>>&);
};

#endif
