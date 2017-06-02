#ifndef UVSSCLIENT_H
#define UVSSCLIENT_H

#include <string>

#include <Ice/Ice.h>

#include <clienti.h>
#include <rpcexecutor.h>
#include <workqueue.h>

#include <uvssclientsdk.h>

typedef UVSSMessageCallback UVSSInitializeCallback;

class UVSSClient {
public:
    UVSSClient();
    int init();
    void uninit();
    int connect(const std::string& = "127.0.0.1", int = 20145);
    int disconnect(int);
    
    static void setInitializeCallback(UVSSInitializeCallback);
    static void setConnectionCallback(UVSSMessageCallback);
    static void setCheckInfoCallback(UVSSCheckInfoCallback);

private:
    Ice::CommunicatorPtr ic;
    Ice::ObjectAdapterPtr adapter;
    Ice::Identity id;
    
    std::shared_ptr<RpcExecutor> _rpcExecutor;
    std::shared_ptr<WorkQueue> _workQueue;
    std::shared_ptr<ClientI> client;
    
    static UVSSInitializeCallback initializeCallback;
};

#endif
