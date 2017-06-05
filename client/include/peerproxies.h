#ifndef PEERPROXIES_H
#define PEERPROXIES_H

#include <condition_variable>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

#include <callback.h>
#include <uvssclientwrapper.h>

typedef UVSSMessageCallback UVSSConnectionCallback;

class PeerProxies
{
public:
    PeerProxies();

    void start();
    void join();
    void run();
    void destroy();
    int add(const std::shared_ptr<UVSS::CallbackSenderPrx>&, const std::string&);
    bool findAndRemove(int, std::string&);
    bool isRepeated(const std::string&);
    int serverIndex(const Ice::Current& curr);

    static void setConnectionCallback(UVSSConnectionCallback);

private:
    int index;
    bool isDestroyed;
    std::map<std::shared_ptr<UVSS::CallbackSenderPrx>, std::string> serverProxyToEndpoint;
    std::map<std::string, int> endpointToIndex;

    std::mutex _mutex;
    std::condition_variable _cv;
    std::thread _receiverThread;

    static UVSSConnectionCallback connectionCallback;
};

#endif // RPCEXECUTOR_H