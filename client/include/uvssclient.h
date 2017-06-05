#ifndef UVSSCLIENT_H
#define UVSSCLIENT_H

#include <string>

#include <Ice/Ice.h>

#include <callbackreceiveri.h>
#include <peerproxies.h>
#include <workqueue.h>
#include <uvssclientwrapper.h>

typedef UVSSMessageCallback UVSSInitializeCallback;
typedef UVSSMessageCallback ConnectCallback;

class UVSSClient {
public:
    UVSSClient();
    int init();
    void uninit();
    int connect(const std::string& = "127.0.0.1", int = 20145);
    int disconnect(int);

    static void setInitializeCallback(UVSSInitializeCallback);
    static void setCCB(ConnectCallback);

    static void setConnectionCallback(UVSSMessageCallback);
    static void setCheckInfoCallback(UVSSCheckInfoCallback);

private:
    Ice::CommunicatorPtr ic;
    Ice::ObjectAdapterPtr adapter;
    Ice::Identity id;

    std::shared_ptr<PeerProxies> peerProxies_;
    std::shared_ptr<WorkQueue> _workQueue;
    std::shared_ptr<CallbackReceiverI> client;

    static UVSSInitializeCallback initializeCallback;
    static ConnectCallback ccb_;
};

#endif
