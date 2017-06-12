#ifndef PEERPROXIES_H
#define PEERPROXIES_H

#include <condition_variable>
#include <map>
#include <mutex>
#include <thread>

#include <callback.h>
#include <uvssserverwrapper.h>

class PeerProxies
{
public:
    PeerProxies();
    void run();
    void start();
    void add(const std::shared_ptr<Uvss::CallbackReceiverPrx>&, const std::string&);
    void sendCheckInfo(const std::vector<std::string>&,
                       const std::vector<std::string>&,
                       const std::vector<std::vector<unsigned char>>&);
    void destroy();
    void join();

private:
    std::map<std::shared_ptr<Uvss::CallbackReceiverPrx>, std::string> clientEndpointMap_;
    bool destroy_;

    std::mutex mutex_;
    std::condition_variable cv_;
    std::thread senderThread_;
};

#endif // PEERPROXIES_H
