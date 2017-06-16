#ifndef RPCPROXIES_H
#define RPCPROXIES_H

#include <condition_variable>
#include <map>
#include <mutex>
#include <thread>

#include <callback.h>
#include <uvssclientwrapper.h>

class RpcProxies {
public:
    RpcProxies();
    void run();
    void start();
    int add(const std::shared_ptr<Uvss::CallbackSenderPrx>&, const std::string&);
    bool findAndRemove(int, std::string&, std::shared_ptr<Uvss::CallbackSenderPrx>&);
    bool isRepeated(const std::string&);
    int serverConnectionId(const std::string&);

    void destroy();
    void join();

private:
    std::map<std::shared_ptr<Uvss::CallbackSenderPrx>, std::string> serverEndpointMap_;
    std::map<std::string, int> endpointConnectionIdMap_;
    int connectionId_;
    bool destroy_;

    std::mutex mutex_;
    std::condition_variable cv_;
    std::thread senderThread_;
};

#endif // RPCPROXIES_H
