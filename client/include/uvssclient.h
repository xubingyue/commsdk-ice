#ifndef UVSSCLIENT_H
#define UVSSCLIENT_H

#include <Ice/Ice.h>

#include <rpcproxies.h>
#include <workqueue.h>
#include <callbackreceiveri.h>

#ifdef ICE_CPP11_MAPPING

class UvssClient {
public:
    UvssClient();
    int start();

    int connect(const std::string&, int);
    int disconnect(int);

    void shutdown();
    ~UvssClient();

private:
    Ice::CommunicatorPtr ic_;
    Ice::ObjectAdapterPtr adapter_;
    Ice::Identity ident_;

    std::shared_ptr<WorkQueue> queue_;
    std::shared_ptr<RpcProxies> proxies_;
    std::shared_ptr<CallbackReceiverI> servant_;
};

#else

class UvssClient {
public:
    UvssClient();
    int start();

    int connect(const std::string&, int);
    int disconnect(int);

    void sendCheckInfo(const std::string&, const std::string&,
                       const std::string&, const std::string&,
                       const std::string&, const std::string&,
                       const std::string&);
    void sendCheckInfo(const std::string&,
                       const std::string&, const std::string&,
                       const std::string&, const std::string&,
                       const std::string&, const std::string&,
                       const std::string&);
    void sendCheckInfo(const std::string&, const std::string&);
    void sendCheckInfo(const std::string&,
                       const std::string&, const std::string&);

    void shutdown();
    ~UvssClient();

private:
    Ice::CommunicatorPtr ic_;
    Ice::ObjectAdapterPtr adapter_;
    Ice::Identity ident_;

    IceUtil::Handle<WorkQueue> queue_;
    IceUtil::Handle<RpcProxies> proxies_;
    IceUtil::Handle<CallbackReceiverI> servant_;

    void filePathToFile(const std::string&, std::vector<unsigned char>&);
    void filePathsToFileNamesAndFiles(const std::vector<std::string>&,
                                      std::vector<std::string>&,
                                      std::vector<std::vector<unsigned char> >&,
                                      bool);
    void transformCheckInfo(const std::string&, const std::string&,
                            const std::string&, const std::string&,
                            const std::string&, const std::string&,
                            const std::string&,
                            std::vector<std::string>&,
                            std::vector<std::string>&,
                            std::vector<std::vector<unsigned char> >&);
    void transformCheckInfo(const std::string&, const std::string&,
                            std::vector<std::string>&,
                            std::vector<std::string>&,
                            std::vector<std::vector<unsigned char> >&);
};

#endif

#endif
