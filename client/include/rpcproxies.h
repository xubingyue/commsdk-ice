#ifndef RPCPROXIES_H
#define RPCPROXIES_H

#include <condition_variable>
#include <map>
#include <mutex>
#include <thread>

#include <callback.h>

class RpcProxies {
public:
    RpcProxies();

    void run();
    void start();

    int add(const std::shared_ptr<Uvss::CallbackSenderPrx>&, const std::string&);
    bool remove(int, std::string&, std::shared_ptr<Uvss::CallbackSenderPrx>&);
    bool has(const std::string&);
    int connectionId(const std::string&);

    void destroy();
    void join();

private:
    std::map<std::shared_ptr<Uvss::CallbackSenderPrx>, std::string>
        proxyEndpointMap_;
    std::map<std::string, int> endpointConnectionIdMap_;
    int connectionId_;
    bool destroy_;

    std::mutex mutex_;
    std::condition_variable condition_;
    std::thread heartbeatThread_;
};

#endif // RPCPROXIES_H
