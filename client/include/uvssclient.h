#ifndef UVSSCLIENT_H
#define UVSSCLIENT_H

#include <Ice/Ice.h>

#include <peerproxies.h>
#include <workqueue.h>
#include <callbackreceiveri.h>
#include <uvssclientwrapper.h>

typedef UVSSMessageCallback InitializationCallback;
typedef UVSSMessageCallback ConnectionCallback;
typedef UVSSCheckInfoCallback CheckInfoCallback;

class UvssClient {
public:
    UvssClient();
    ~UvssClient();

    int start();

    int connect(const std::string&, int);
    int disconnect(int);

    void shutdown();

    static void setInitializationCallback(InitializationCallback);
    static void setConnectionCallback(ConnectionCallback);

    static void setHeartbeatCallback(ConnectionCallback);
    static void setCheckInfoCallback(CheckInfoCallback);

private:
    Ice::CommunicatorPtr ic_;
    Ice::ObjectAdapterPtr adapter_;
    Ice::Identity ident_;

    std::shared_ptr<PeerProxies> peerProxies_;
    std::shared_ptr<WorkQueue> workQueue_;
    std::shared_ptr<CallbackReceiverI> client_;

    static InitializationCallback initializationCallback_;
    static ConnectionCallback connectionCallback_;
};

#endif
