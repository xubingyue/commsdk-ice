#ifndef UVSSCLIENT_H
#define UVSSCLIENT_H

#include <Ice/Ice.h>

#include <rpcproxies.h>
#include <workqueue.h>
#include <callbackreceiveri.h>

class UvssClient {
public:
    UvssClient();
    int start();

    int connect(const std::string&, int);
    int disconnect(int);

    void shutdown();
    ~UvssClient();

private:
    Ice::CommunicatorHolder ich_;
    Ice::ObjectAdapterPtr adapter_;
    Ice::Identity ident_;

    std::shared_ptr<RpcProxies> proxies_;
    std::shared_ptr<WorkQueue> workQueue_;
    std::shared_ptr<CallbackReceiverI> servant_;
};

#endif
