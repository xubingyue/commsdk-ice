#ifndef RPCEXECUTOR_H
#define RPCEXECUTOR_H

#include <condition_variable>
#include <mutex>
#include <thread>
#include <map>

#include <callback.h>
#include <uvssserverwrapper.h>

class RpcExecutor
{
public:
    RpcExecutor();
    void start();
    void join();

    void run();
    void add(Ice::Identity id, const Ice::Current& curr);
    void sendCheckInfo(const std::vector<std::string>&, const UVSS::ByteSeqSeq&, const std::vector<std::string>&);
    void destroy();

    static void setConnectionInfoCallback(UVSSServerCallback);

private:
    std::map<std::shared_ptr<UVSS::ClientPrx>, std::string> clientProxyToEndpoint;
    bool _destroy;

    std::mutex _mutex;
    std::condition_variable _cv;
    std::thread _senderThread;

    static UVSSServerCallback connectionInfoCallback;
};

#endif // RPCEXECUTOR_H
