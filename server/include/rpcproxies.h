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

    void runHeartbeat();
    void startHeartbeat();

    void add(const std::shared_ptr<Uvss::CallbackReceiverPrx>&,
             const std::string&);
    void sendCheckInfo(const std::vector<std::string>&,
                       const std::vector<std::string>&,
                       const std::vector<std::vector<unsigned char>>&);

    void destroyHeartbeat();
    void joinHeartbeat();

private:
    std::map<std::shared_ptr<Uvss::CallbackReceiverPrx>, std::string>
        proxyEndpointMap_;
    bool destroy_;

    std::mutex mutex_;
    std::condition_variable condition_;
    std::thread heartbeatThread_;
};

#endif // RPCPROXIES_H
